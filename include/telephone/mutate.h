#ifndef MUTATE_H
#define MUTATE_H

#include "arpabet.h"

#include <stdlib.h>

void mutate(
        char [][MAX_WORD_LENGTH],
        size_t,
        char [DICTIONARY_LENGTH][MAX_WORD_LENGTH],
        enum phoneme [DICTIONARY_LENGTH][MAX_NUM_PHONEMES],
        accent [DICTIONARY_LENGTH][MAX_NUM_PHONEMES],
        size_t [DICTIONARY_LENGTH]
        );

void print(char [][MAX_WORD_LENGTH], size_t);

#endif
