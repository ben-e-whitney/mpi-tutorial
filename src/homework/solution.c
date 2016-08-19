#include <stdlib.h>
#include <stdio.h>

#include <stdbool.h>

void get_partner(const int rank, const int size, const int turn, int * partner,
        bool * send_first) {
    *partner = (turn + size - rank) % size;
    *send_first = rank > *partner;
}

#include "times_table.h"

#include <mpi.h>

const int ROOT = 0;

#define SEND_RECV    0
#define GATHER_BCAST 1
#define ALLGATHER    2
/*Valid values are `SEND_RECV`, `GATHER_BCAST`, and `ALLGATHER`.*/
#define METHOD SEND_RECV

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);
    int rank;
    int size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    const int factor = rank + 1;

    unsigned char *p = calloc(size * size, sizeof(unsigned char));
    unsigned char **times_table = calloc(size, sizeof(unsigned char *));
    for (size_t i = 0; i < size; ++i) {
        times_table[i] = p + i * size * sizeof(unsigned char);
    }

    for (unsigned char i = 0; i < size; ++i) {
        times_table[rank][i] = factor * (i + 1);
    }

#if METHOD == SEND_RECV
    /*Using `MPI_Send` and `MPI_Recv`.*/
    const int ROW_TAG = 0;
    for (int turn = 0; turn < size; ++turn) {
        int partner;
        bool send_first;
        get_partner(rank, size, turn, &partner, &send_first);
        if (partner == rank) {
            continue;
        }
        if (send_first) {
            MPI_Send(times_table[rank], size, MPI_UNSIGNED_CHAR, partner,
                    ROW_TAG, MPI_COMM_WORLD);
        }
        MPI_Status status;
        MPI_Recv(times_table[partner], size, MPI_UNSIGNED_CHAR, partner,
                ROW_TAG, MPI_COMM_WORLD, &status);
        if (!send_first) {
            MPI_Send(times_table[rank], size, MPI_UNSIGNED_CHAR, partner,
                    ROW_TAG, MPI_COMM_WORLD);
        }
    }
#elif METHOD == GATHER_BCAST
    /*Using `MPI_Gather` and `MPI_Bcast`.*/
    MPI_Gather(times_table[rank], size, MPI_UNSIGNED_CHAR,
            p, size, MPI_UNSIGNED_CHAR, ROOT, MPI_COMM_WORLD);
    MPI_Bcast(p, size * size, MPI_UNSIGNED_CHAR, ROOT, MPI_COMM_WORLD);
#elif METHOD == ALLGATHER
    /*Using `MPI_Allgather`.*/
    MPI_Allgather(times_table[rank], size, MPI_UNSIGNED_CHAR,
            p, size, MPI_UNSIGNED_CHAR, MPI_COMM_WORLD);
#else
#error METHOD must be `SEND_RECV`, `GATHER_BCAST`, or `ALLGATHER`.
#endif

    const unsigned char grade = grade_times_table(times_table, size);
    unsigned char *grades = NULL;
    if (rank == ROOT) {
        grades = calloc(size, sizeof(unsigned char));
    }
    MPI_Gather(&grade, 1, MPI_UNSIGNED_CHAR,
            grades, 1, MPI_UNSIGNED_CHAR, ROOT, MPI_COMM_WORLD);
    if (rank == ROOT) {
        print_grades(grades, size);
        free(grades);
    }

    if (rank == ROOT) {
        printf("Rank 0's table:\n");
        print_times_table(times_table, size);
    }

    free(times_table);
    free(p);
    MPI_Finalize();
    return 0;
}
