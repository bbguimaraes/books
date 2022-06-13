#include <assert.h>
#include <ctype.h>
#include <stdalign.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

#include <immintrin.h>

#ifndef USE_SIMD
#define USE_SIMD 1
#endif

#define ITER_SIZE 8
#define BUFFER_SIZE (sizeof(__m256i) << 12)
static_assert(__builtin_popcount(ITER_SIZE) == 1);
static_assert(__builtin_popcount(BUFFER_SIZE) == 1);
static_assert(!(BUFFER_SIZE % ITER_SIZE));

typedef uint64_t u64;
typedef __m256i i256;

static_assert('\b' + 1 == '\t');
static_assert('\t' + 1 == '\n');
static_assert('\n' + 1 == '\v');
static_assert('\v' + 1 == '\f');
static_assert('\f' + 1 == '\r');

static bool is_word(int n_trans) { return n_trans & 1; }
static int bool_mask(bool b) { return ~b + 1; }
static int inv_bool_mask(bool b) { return b - 1; }

static i256 wc_avx2_lines(i256 dst, i256 src);
static int wc_avx2_lines_end(i256 src);
static i256 wc_avx2_is_space(i256 src);
static int wc_avx2_trans(i256 src, i256 first);
static i256 wc_avx2_edges(i256 src, i256 first);
static int wc_avx2_reduce(i256 src[static ITER_SIZE]);

static void wc_avx2(const char *bc, const char *ec, int *lines, int *trans) {
    enum { N = ITER_SIZE, M = N - 1 };
    const i256 *b = (const i256*)bc;
    const i256 *e = (const i256*)ec;
    i256 lines_acc[N] = {0}, is_space[N + 1];
    is_space[0] = _mm256_set1_epi8(inv_bool_mask(is_word(*trans)));
    int n_trans[N] = {0};
    for(; b != e; b += N) {
        for(int i = 0; i != N; ++i) {
            const int sp = (i + 1) & M;
            is_space[sp] = wc_avx2_is_space(b[i]);
            lines_acc[i] = wc_avx2_lines(lines_acc[i], b[i]);
            n_trans[i] += wc_avx2_trans(is_space[sp], is_space[i]);
        }
    }
    for(int i = 1; i != N; ++i)
        *n_trans += n_trans[i];
    *lines += wc_avx2_reduce(lines_acc);
    *trans += *n_trans;
}

static i256 wc_avx2_lines(i256 dst, i256 src) {
    const i256 cmp = _mm256_cmpeq_epi8(src, _mm256_set1_epi8('\n'));
    const i256 add = _mm256_abs_epi8(cmp);
    const i256 sad = _mm256_sad_epu8(add, _mm256_setzero_si256());
    return _mm256_add_epi64(dst, sad);
}

static int wc_avx2_lines_end(i256 src) {
    alignas(i256) u64 v[4];
    _mm256_store_si256((i256*)v, src);
    return v[0] + v[1] + v[2] + v[3];
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

static int wc_avx2_trans(i256 src, i256 first) {
    const i256 trans = wc_avx2_edges(src, first);
    return __builtin_popcount(_mm256_movemask_epi8(trans));
}

static i256 wc_avx2_edges(i256 src, i256 first) {
    const i256 first_high = _mm256_permute2x128_si256(first, first, 1);
    const i256 first31 = _mm256_bsrli_epi128(first_high, 15);
    const i256 low = _mm256_permute2x128_si256(src, src, 8);
    const i256 shl = _mm256_alignr_epi8(src, low, 15);
    const i256 mask = _mm256_setr_epi64x(0xff, 0, 0, 0);
    const i256 prev = _mm256_blendv_epi8(shl, first31, mask);
    return _mm256_xor_si256(prev, src);
}

static int wc_avx2_reduce(i256 src[static ITER_SIZE]) {
    for(int i = 1; i != ITER_SIZE; ++i)
        *src = _mm256_add_epi64(*src, src[i]);
    return wc_avx2_lines_end(*src);
}

static int lines = 0, trans = 0, chars = 0;

static bool is_space(char c);

static void wc(const char *b, const char *e) {
    for(const char *p = b; p != e; ++p)
        lines += *p == '\n';
    bool prev = is_word(trans);
    for(const char *p = b; p != e; ++p) {
        const bool next = !is_space(*p);
        trans += prev != next;
        prev = next;
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

int wc_file(FILE *f) {
    alignas(i256) unsigned char buffer[BUFFER_SIZE];
    while(!feof(f)) {
        const size_t n = fread(buffer, 1, sizeof(buffer), f);
        if(ferror(f))
            return perror("fread"), 1;
#if USE_SIMD
        if(n == sizeof(buffer))
            wc_avx2(buffer, buffer + n, &lines, &trans);
        else
#endif
            wc(buffer, buffer + n);
        chars += n;
    }
    return 0;
}

int wc_mmap(const char *f) {
    int fd = open(f, O_RDONLY);
    if(fd == -1)
        return perror("open"), 1;
    struct stat st;
    if(fstat(fd, &st) == -1)
        return perror("fstat"), 1;
    size_t size = st.st_size;
    const char *p = mmap(NULL, size, PROT_READ, MAP_PRIVATE, fd, 0);
    if(p == MAP_FAILED)
        return perror("mmap"), 1;
    enum { N = BUFFER_SIZE };
    for(; N < size; chars += N, size -= N, p += N) {
#if USE_SIMD
        wc_avx2(p, p + N, &lines, &trans);
#else
        wc(p, p + N);
#endif
    }
    wc(p, p + size);
    chars += size;
    return 0;
}

int main(int argc, char **argv) {
    int ret;
    if(argc == 1)
        ret = wc_file(stdin);
    else
        ret = wc_mmap(argv[1]);
    const int words = (trans + 1) >> 1;
    printf("%d %d %d\n", lines, words, chars);
    return ret;
}
