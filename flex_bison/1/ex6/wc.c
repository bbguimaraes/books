#include <assert.h>
#include <ctype.h>
#include <stdalign.h>
#include <stdbool.h>
#include <stdio.h>

#include <immintrin.h>

#ifndef USE_SIMD
#define USE_SIMD 1
#endif

typedef __m256i i256;

static_assert('\b' + 1 == '\t');
static_assert('\t' + 1 == '\n');
static_assert('\n' + 1 == '\v');
static_assert('\v' + 1 == '\f');
static_assert('\f' + 1 == '\r');

static int lines = 0, words = 0, chars = 0;
static bool is_word = false;

static i256 wc_avx2_lines(i256 dst, i256 src);
static int wc_avx2_lines_end(i256 lines_v);
static void wc_avx2_words(i256 src);
static i256 shift_left(i256 src);
static i256 wc_avx2_is_space(i256 src);

static void wc_avx2(const char *bc, const char *ec) {
    const i256 *b = (const i256*)bc;
    const i256 *e = (const i256*)ec;
    i256 lines_v = {0};
    for(; b != e; ++b) {
        const i256 s = _mm256_load_si256(b);
        lines_v = wc_avx2_lines(lines_v, s);
        wc_avx2_words(s);
    }
    lines += wc_avx2_lines_end(lines_v);
}

static i256 wc_avx2_lines(i256 dst, i256 src) {
    const i256 cmp = _mm256_cmpeq_epi8(src, _mm256_set1_epi8('\n'));
    const i256 add = _mm256_and_si256(cmp, _mm256_set1_epi8(1));
    return _mm256_add_epi8(add, dst);
}

static int wc_avx2_lines_end(i256 lines_v) {
    alignas(i256) unsigned char v[sizeof(i256)] = {0};
    _mm256_store_si256((i256*)v, lines_v);
    int ret = 0;
    for(int i = 0; i != sizeof(i256); ++i)
        ret += v[i];
    return ret;
}

static void wc_avx2_words(i256 src) {
    const i256 is_space = wc_avx2_is_space(src);
    const i256 prev = _mm256_insert_epi8(shift_left(is_space), is_word - 1, 0);
    const i256 is_trans = _mm256_xor_si256(prev, is_space);
    const int n = __builtin_popcount(_mm256_movemask_epi8(is_trans));
    words += (n + is_word) >> 1;
    is_word ^= n & 1;
}

static i256 shift_left(i256 src) {
    /* alternatively
    return _mm256_insert_epi8(
        _mm256_slli_si256(src, 1),
        _mm256_extract_epi8(src, 15), 16); */
    const i256 shl = _mm256_slli_si256(src, 1);
    const i256 cross = _mm256_permute4x64_epi64(src, 0x10);
    const i256 e16 = _mm256_srli_si256(cross, 7);
    return _mm256_blendv_epi8(shl, e16, _mm256_setr_epi64x(0, 0, 0xff, 0));
}

static i256 wc_avx2_is_space(i256 src) {
    const i256 min = _mm256_set1_epi8('\b' - 1);
    const i256 max = _mm256_set1_epi8('\r' + 1);
    return _mm256_or_si256(
        _mm256_cmpeq_epi8(src, _mm256_set1_epi8(' ')),
        _mm256_and_si256(
            _mm256_cmpgt_epi8(src, min),
            _mm256_cmpgt_epi8(max, src)));
}

static bool is_space(char c);

static void wc(const char *b, const char *e) {
    for(const char *p = b; p != e; ++p)
        lines += *p == '\n';
    for(const char *p = b; p != e; ++p) {
        const bool is_trans = is_word == is_space(*p);
        words += is_word & is_trans;
        is_word ^= is_trans;
    }
}

static bool is_space(char c) {
    switch(c) {
    case '\t':
    case '\n':
    case '\v':
    case '\f':
    case '\r':
    case ' ': return true;
    default: return false;
    }
}

int main(void) {
    alignas(i256) unsigned char buffer[sizeof(i256) << 12];
    while(!feof(stdin)) {
        const size_t n = fread(buffer, 1, sizeof(buffer), stdin);
        if(ferror(stdin))
            return perror("fread"), 1;
#if USE_SIMD
        if(n == sizeof(buffer))
            wc_avx2(buffer, buffer + n);
        else
#endif
            wc(buffer, buffer + n);
        chars += n;
    }
    words += is_word;
    printf("%d %d %d\n", lines, words, chars);
}
