#include "word_similarity.h"

#include <stdlib.h>
#include <stdbool.h>

bool is_vowel(enum phoneme a) {
    return VOWEL == classify_family(classify_genus(a));
}

unsigned char max(unsigned char a, unsigned char b) {
    return a > b ? a : b;
}

size_t min(size_t a, size_t b) {
    return a < b ? a : b;
}

size_t abs_diff(size_t a, size_t b) {
    return a < b ? b - a : a - b;
}

unsigned char similarity_accents(accent a, accent b) {
    if (b > a) {
        return similarity_accents(b, a);
    }
    /*Replace this with a switch statement if you want to tweak the values.*/
    return 2 - (a - b);
}

unsigned char similarity_phonemes(enum phoneme a, enum phoneme b) {
    if (a == b) {
        return 3;
    } else if (classify_genus(a) == classify_genus(b)) {
        return 1;
    } else {
        return 0;
    }
}

unsigned char similarity_phoneme_groups(
        enum phoneme phonemes_a[], size_t size_a,
        enum phoneme phonemes_b[], size_t size_b) {
    unsigned char score = 0;
    for (size_t i = 0; i < min(size_a, size_b); ++i) {
        score += similarity_phonemes(phonemes_a[i], phonemes_b[i]);
    }
    return score;
}

unsigned char similarity_accent_groups(accent accents_a[],
        size_t size_a, accent accents_b[], size_t size_b) {
    unsigned char score = 0;
    for (size_t i = 0; i < min(size_a, size_b); ++i) {
        score += similarity_accents(accents_a[i], accents_b[i]);
    }
    return score;
}

unsigned char similarity_phoneme_accent_groups(
        enum phoneme phonemes_a[], accent accents_a[], size_t size_a,
        enum phoneme phonemes_b[], accent accents_b[], size_t size_b) {
    unsigned char score = similarity_phoneme_groups(
            phonemes_a, size_a, phonemes_b, size_b
            );
    if ((size_a > 0 && size_b > 0) &&
            (is_vowel(phonemes_a[0]) && is_vowel(phonemes_b[0]))) {
        score += similarity_accent_groups(accents_a, size_a, accents_b, size_b);
    }
    return score;
}

size_t next_block_size(enum phoneme phonemes[], size_t N) {
    if (N == 0) {
        return 0;
    }
    size_t block_size = 1;
    enum phoneme_family family = classify_family(classify_genus(phonemes[0]));
    for (;
            block_size < N &&
            classify_family(classify_genus(phonemes[block_size])) == family;
            ++block_size) {
    }
    return block_size;
}

unsigned char similarity_by_block(
        enum phoneme phonemes_a[], accent accents_a[], size_t size_a,
        enum phoneme phonemes_b[], accent accents_b[], size_t size_b) {
    unsigned char score = 0;
    size_t i = 0;
    size_t j = 0;
    while (i < size_a && j < size_b) {
        size_t block_size_a = next_block_size(phonemes_a + i, size_a - i);
        size_t block_size_b = next_block_size(phonemes_b + j, size_b - j);
        score += similarity_phoneme_accent_groups(
                phonemes_a + i, accents_a + i, block_size_a,
                phonemes_b + j, accents_b + j, block_size_b
                );
        i += block_size_a;
        j += block_size_b;
    }
    return score;
}

unsigned char similarity_words(
        enum phoneme phonemes_a[], accent accents_a[], size_t size_a,
        enum phoneme phonemes_b[], accent accents_b[], size_t size_b) {
    unsigned char max_score = 0;
    for (size_t i = 0; i < size_a; ++i) {
        for (size_t j = 0; j < size_b; ++j) {
            unsigned char score = similarity_by_block(
                    phonemes_a + i, accents_a + i, size_a - i,
                    phonemes_b + j, accents_b + j, size_b - j
                    );
            /*Apply a penalty for mismatched lengths.*/
            size_t overhang = abs_diff(size_a - i, size_b - j);
            unsigned char penalty = (i + j + overhang) * 2;
            max_score = max(max_score, penalty > score ? 0 : score - penalty);
        }
    }
    return max_score;
}
