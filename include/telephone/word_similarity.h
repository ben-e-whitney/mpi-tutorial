#ifndef WORD_SIMILARITY_H
#define WORD_SIMILARITY_H

#include "arpabet.h"

#include <stdlib.h>
#include <stdbool.h>

unsigned char similarity_words(
        enum phoneme [], accent [], size_t,
        enum phoneme [], accent [], size_t
        );

#endif
