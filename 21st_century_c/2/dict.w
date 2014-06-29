@ Here is some text to test cweb. There would be a lot more text here if it was
a serious example, but I'm only testing it and don't want to put code from the
next chapters here yet. The comments on each section will be short and
pointless to keep this file simple enough. The code used is the dictionary
example from chapter eleven, already added because it was need for the unit
test example. I think this is enough useless text to test cweb's comments.
Signing off.

@c

@<includes@>
@<global declarations@>
@<the code@>

@ Including some files.

@<includes@>=
#include <stdio.h>
#include <stdlib.h>
#include "dict.h"

@ Here is a global declaration. This is the definition for the declaration in
|dict.h| (look, a different font!) and is used by |dictionary_find| as a return
value when the key is not found on the dictionary.

@<global declarations@>=
void * dictionary_not_found;

@ Alright, now that I tested enough, the next comments will be void of useful
content. This is the dictionary\_new function.

@<the code@>=
dictionary * dictionary_new() {
    static int dnf;
    if(!dictionary_not_found) dictionary_not_found = &dnf;
    dictionary * out = malloc(sizeof(dictionary));
    *out = (dictionary) {};
    return out;
}

@ This is the dictionary\_add\_keyval (static) function.

@c

static void dictionary_add_keyval(dictionary * in, keyval *kv) {
    in->length++;
    in->pairs = realloc(in->pairs, sizeof(keyval *) * in->length);
    in->pairs[in->length - 1] = kv;
}

@ This is the dictionary\_add function.

@c

void dictionary_add(dictionary * in, char * key, void * value) {
    if(!key) {
        fprintf(stderr, "NULL is not a valid key.\n");
        abort();
    }
    dictionary_add_keyval(in, keyval_new(key, value));
}

@ This is the dictionary\_find function.

@c

void * dictionary_find(dictionary const * in, char const * key) {
    for(int i = 0; i < in->length; ++i)
        if(keyval_matches(in->pairs[i], key))
            return in->pairs[i]->value;
    return dictionary_not_found;
}

@ This is the dictionary\_copy function.

@c

dictionary * dictionary_copy(dictionary * in) {
    dictionary * out = dictionary_new();
    for(int i = 0; i < in->length; ++i)
        dictionary_add_keyval(out, keyval_copy(in->pairs[i]));
    return out;
}

@ This is the dictionary\_free function.

@c

void dictionary_free(dictionary * in) {
    for(int i = 0; i < in->length; ++i)
        keyval_free(in->pairs[i]);
    free(in);
}

@ Thanks. This was a fun test. Unfortunatelly, I am not able to tell, from this
simple test, if |cweb| would be useful for a larger project. I definetly don't
like the formatting of the code (after years of writing Egyptian curly braces,
suddenly I can't stand them anymore). But maybe it is just misconfiguration (it
would be good if the author of the book included some images of the final
product when presenting |cweb|).

I'm just being pedantic here, Ben, if you ever read this. Your book is awesome.

Paragraphs. That's nice.

There are some understandable errors, like not recognizing more modern struct
initialization (|(dictionary) {};|), but there are more serious errors like
failing to recognize some parts of the code. Maybe I'll give it a try on a
future project. For now, I'm more worried about the ever-growing pile of books
in my bedroom. To finish, here is a comment showing the end of the code file.
It seems |cweb| adds some kind of index after the contents of the file, which
is nice (if it wasn't in the wrong page). Well, I just discovered there is a
``cweb'' syntax file for vim that is installed by default, so I guess that is a
plus.

And now I'm just going to add text until the index below is placed on the next
page. That's right, I'll repeat this phrase until then. And now I'm just going
to add text until the index below is placed on the next page. That's right,
I'll repeat this phrase until then. Ok, now I'm done.

@c
// end
