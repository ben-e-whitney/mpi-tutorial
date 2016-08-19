#ifndef PARSE_H
#define PARSE_H

#include "arpabet.h"

#include <stdio.h>
#include <stdlib.h>

void parse_dictionary(
        FILE *,
        char [DICTIONARY_LENGTH][MAX_WORD_LENGTH],
        enum phoneme [DICTIONARY_LENGTH][MAX_NUM_PHONEMES],
        accent [DICTIONARY_LENGTH][MAX_NUM_PHONEMES],
        size_t [DICTIONARY_LENGTH]
        );

#endif
