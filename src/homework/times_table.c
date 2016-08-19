#include "times_table.h"

#include <stdio.h>
#include <stdlib.h>

size_t get_max_width(const unsigned char * p, const size_t N) {
    size_t max_width = 0;
    for (size_t i = 0; i < N; ++i) {
        const size_t width = snprintf(NULL, 0, "%d", p[i]);
        if (width > max_width) {
            max_width = width;
        }
    }
    return max_width;
}

void print_times_table(unsigned char ** const table, const size_t N) {
    unsigned char *factors = calloc(N, sizeof(size_t));
    for (size_t i = 0; i < N; ++i) {
        factors[i] = i + 1;
    }
    size_t *max_widths = calloc(N + 1, sizeof(size_t));
    max_widths[0] = get_max_width(factors, N);
    /*Using commutativity of multiplication: find the maximum width of the
    column by finding the maximum width of the corresponding row.*/
    for (size_t i = 0; i < N; ++i) {
        max_widths[i + 1] = get_max_width(table[i], N);
    }

    size_t dash_count = 0;
    for (size_t i = 0; i < 1 + max_widths[0]; ++i) {
       fprintf(stdout, " ");
        ++dash_count;
    }
   fprintf(stdout, "|");
    ++dash_count;
    for (size_t i = 0; i < N; ++i) {
       fprintf(stdout, " %*d", (int) max_widths[i + 1], factors[i]);
        dash_count += 1 + max_widths[i + 1];
    }
   fprintf(stdout, "\n");

    for (size_t i = 0; i < dash_count; ++i) {
       fprintf(stdout, "-");
    }
   fprintf(stdout, "\n");

    for (size_t i = 0; i < N; ++i) {
       fprintf(stdout, "%*d |", (int) max_widths[0], factors[i]);
        for (size_t j = 0; j < N; ++j) {
           fprintf(stdout, " %*d", (int) max_widths[j + 1], table[i][j]);
        }
       fprintf(stdout, "\n");
    }
    free(max_widths);
    free(factors);
}

unsigned short grade_times_table(unsigned char ** const table, const size_t N) {
    unsigned short correct = 0;
    for (size_t i = 0; i < N; ++i) {
        const unsigned char i_factor = i + 1;
        for (size_t j = 0; j < N; ++j) {
            const unsigned char j_factor = j + 1;
            if (table[i][j] == i_factor * j_factor) {
                ++correct;
            }
        }
    }
    return correct;
}

void print_grades(unsigned char * const grades, const size_t N) {
    const unsigned short perfect_score = N * N;
    unsigned char *bad_ranks  = calloc(N, sizeof(unsigned char));
    unsigned char *bad_grades = calloc(N, sizeof(unsigned char));
    size_t j = 0;
    for (size_t i = 0; i < N; ++i) {
        if (grades[i] != perfect_score) {
            bad_ranks[j] = i;
            bad_grades[j] = grades[i];
            ++j;
        }
    }
    const size_t max_rank_width  = get_max_width(bad_ranks, j);
    const size_t max_score_width = get_max_width(bad_grades, j);
    for (size_t i = 0; i < j; ++i) {
       fprintf(stdout, "Rank %*d made a mistake! %*d of %d correct.\n",
                (int) max_rank_width, bad_ranks[i],
                (int) max_score_width, bad_grades[i], perfect_score);
    }
}
