#include <stdlib.h>
#include <stdio.h>

#include <stdbool.h>

#include "times_table.h"

/*Include the MPI header.*/

int main(int argc, char **argv) {
    /*Initialize MPI.*/
    int rank;
    int size;
    /*Get this process' rank.*/
    /*Get the number of processes.*/
    const int factor = rank + 1;

    unsigned char *p = calloc(size * size, sizeof(unsigned char));
    unsigned char **times_table = calloc(size, sizeof(unsigned char *));
    for (size_t i = 0; i < size; ++i) {
        times_table[i] = p + i * size * sizeof(unsigned char);
    }

    for (unsigned char i = 0; i < size; ++i) {
        times_table[rank][i] = factor * (i + 1);
    }

    /*Fill in the times table by communicating with the other processes.*/

    const unsigned char grade = grade_times_table(times_table, size);
    if (grade != size * size) {
        fprintf(stdout, "Rank %*d made a mistake! %d of %d correct.\n", rank,
                grade, size * size);
    }
    MPI_Barrier(MPI_COMM_WORLD);

    if (rank == 0) {
        fprintf(stdout, "Rank 0's table:\n");
        print_times_table(times_table, size);
    }

    free(times_table);
    free(p);
    /*Finalize MPI.*/
    return 0;
}
