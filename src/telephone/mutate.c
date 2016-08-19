#include "arpabet.h"
#include "word_similarity.h"
#include "parse.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

size_t find_index_inner(char word[],
        char words[DICTIONARY_LENGTH][MAX_WORD_LENGTH], size_t left,
        size_t right) {
    size_t middle = (left + right) / 2;
    if (middle == left || middle == right) {
        return DICTIONARY_LENGTH;
    }
    int comparison = strcmp(word, words[middle]);
    if (comparison < 0) {
        return find_index_inner(word, words, left, middle);
    } else if (comparison > 0) {
        return find_index_inner(word, words, middle, right);
    } else {
        return middle;
    }
}

size_t find_index(char word[], char words[DICTIONARY_LENGTH][MAX_WORD_LENGTH]) {
    return find_index_inner(word, words, 0, DICTIONARY_LENGTH - 1);
}

size_t mutate_word(
        char word[],
        char words[DICTIONARY_LENGTH][MAX_WORD_LENGTH],
        enum phoneme phonemes[DICTIONARY_LENGTH][MAX_NUM_PHONEMES],
        accent accents[DICTIONARY_LENGTH][MAX_NUM_PHONEMES],
        size_t Ns[DICTIONARY_LENGTH]
        ) {
    size_t index;
    {
        size_t N = strlen(word) + 1;
        char *capitalized;
        capitalized = calloc(N, sizeof(char));
        for (size_t i = 0; i < N; ++i) {
            capitalized[i] = toupper(word[i]);
        }
        capitalized[N] = '\0';
        index = find_index(capitalized, words);
        free(capitalized);
    }
    if (index == DICTIONARY_LENGTH) {
        return DICTIONARY_LENGTH;
    }
    unsigned char self_score = similarity_words(
            phonemes[index], accents[index], Ns[index],
            phonemes[index], accents[index], Ns[index]
            );
    unsigned char worthy_scores[DICTIONARY_LENGTH];
    unsigned int worthy_score_total = 0;
    for (size_t i = 0; i < DICTIONARY_LENGTH; ++i) {
        if (i == index) {
            continue;
        }
        unsigned char score = similarity_words(
                phonemes[index], accents[index], Ns[index],
                phonemes[i], accents[i], Ns[i]);
        if (score > 3 * self_score / 4) {
            worthy_scores[i] = score;
            worthy_score_total += score;
        } else {
            worthy_scores[i] = 0;
        }
    }
    if (worthy_score_total == 0) {
        return index;
    }
    int choice = rand() % worthy_score_total;
    int cumulative_score = 0;
    for (size_t i = 0; i < DICTIONARY_LENGTH; ++i) {
        cumulative_score += worthy_scores[i];
        if (cumulative_score >= choice) {
            return i;
        }
    }
    return index;
}

void mutate(
        char whisper[][MAX_WORD_LENGTH],
        size_t N,
        char words[DICTIONARY_LENGTH][MAX_WORD_LENGTH],
        enum phoneme phonemes[DICTIONARY_LENGTH][MAX_NUM_PHONEMES],
        accent accents[DICTIONARY_LENGTH][MAX_NUM_PHONEMES],
        size_t Ns[DICTIONARY_LENGTH]
        ) {
    int choice = rand() % N;
    size_t index = mutate_word(whisper[choice], words, phonemes, accents, Ns);
    if (index != DICTIONARY_LENGTH) {
        strcpy(whisper[choice], words[index]);
    }
}

void print(char whisper[][MAX_WORD_LENGTH], size_t N) {
    for (size_t i = 0; i < N; ++i) {
        for (size_t j = 0; j < MAX_WORD_LENGTH; ++j) {
            if (whisper[i][j] == '\0') {
                break;
            } else {
                fprintf(stdout, "%c", tolower(whisper[i][j]));
            }
        }
        fprintf(stdout, " ");
    }
    fprintf(stdout, "\n");
}
