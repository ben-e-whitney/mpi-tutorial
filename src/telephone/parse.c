#include "parse.h"

#include "arpabet.h"

#include "word_similarity.h"

#include <assert.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

void parse_line(char line[], size_t N_line, size_t *N_word,
        enum phoneme phonemes[], accent accents[],
        size_t M_available, size_t *M_used) {
    size_t i = 0;
    for (; i < N_line && !isspace(line[i]); ++i) {
    }
    *N_word = i;
    /*If this word is an alternate pronunciation, `line` will look like*/
    /*'WORD(123) ...'. We only want 'WORD'.*/
    if (i > 0 && line[i - 1] == ')') {
        size_t j = 2;
        for (; j <= i && isdigit(line[i - j]); ++j) {
        }
        assert(line[i - j] == '(');
        *N_word -= j;
    }

    *M_used = 0;
    while (i < N_line && *M_used <= M_available) {
        /*Advance to the start of the next phoneme.*/
        for (; i < N_line && !isalpha(line[i]); ++i) {
        }
        if (!isalpha(line[i])) {
            break;
        }

        {
            size_t j = 0;
            for (; i + j < N_line && isalpha(line[i + j]); ++j) {
            }
            {
                /*Copy the chars into a null-terminated string.*/
                char *phoneme = calloc(j + 1, sizeof(char));
                for (size_t k = 0; k < j; ++k) {
                    phoneme[k] = line[i + k];
                }
                phoneme[j] = '\0';
                phonemes[*M_used] = classify_phoneme(phoneme);
                free(phoneme);
            }
            /*Apparently this converts a character to an integer. See*/
            /*<http://stackoverflow.com/a/868508>.*/
            accents[*M_used] = i + j < N_line && isdigit(line[i + j]) ?
                line[i + (j++)] - '0' : 0;
            i += j;
        }
        ++(*M_used);
    }
}

void parse_dictionary(
        FILE *dictionary,
        char words[DICTIONARY_LENGTH][MAX_WORD_LENGTH],
        enum phoneme phonemes[DICTIONARY_LENGTH][MAX_NUM_PHONEMES],
        accent accents[DICTIONARY_LENGTH][MAX_NUM_PHONEMES],
        size_t Ns[DICTIONARY_LENGTH]
        ) {
    size_t i = 0;
    char line[MAX_LINE_LENGTH + 1];
    while (fgets(line, MAX_LINE_LENGTH + 1, dictionary) != NULL) {
        if (strlen(line) >= 3) {
            bool comment = true;
            for (size_t j = 0; j < 3; ++j) {
                if (line[j] != ';') {
                    comment = false;
                    break;
                }
            }
            if (comment) {
                while (getc(dictionary) != '\n') {
                }
                continue;
            }
        }
        size_t N_word;
        size_t M_used;
        parse_line(line, strlen(line), &N_word, phonemes[i], accents[i],
                MAX_NUM_PHONEMES, &M_used);
        for (size_t j = 0; j < N_word; ++j) {
            words[i][j] = line[j];
        }
        words[i][N_word] = '\0';
        Ns[i] = M_used;
        ++i;
    }
}

