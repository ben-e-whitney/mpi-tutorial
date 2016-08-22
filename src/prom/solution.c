#include "coordinates.h"
#include "constants.h"
#include "parse.h"
#include "neighbors.h"

#include <stdio.h>
#include <stdbool.h>
#include <time.h>

#include <mpi.h>

char names[CSV_LENGTH][MAX_NAME_LENGTH];
Coordinates latlongs[CSV_LENGTH];
bool neighbors[CSV_LENGTH];

const double time_till_bed = 5;

const int RANK_USA = 0;
const int RANK_JPN = 1;

int main(int argc, char **argv) {
    MPI_Init(&argc, &argv);
    int rank;
    int size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    if (size < 2) {
        fprintf(stderr, "Please use at least two nodes.\n");
    }

    FILE *csv = fopen(FILENAME_CSV, "r");
    if (csv == NULL) {
        fprintf(stderr, "Could not open csv file %s.\n", FILENAME_CSV);
        return 1;
    }
    parse_csv(csv, names, latlongs);
    fclose(csv);

    get_neighbors(rank, size, latlongs, neighbors);

    time_t start = time(NULL);
    int pair[2];

    if (rank == RANK_USA) {
        MPI_Request request;
        pair[0] = RANK_USA;
        pair[1] = RANK_JPN;
        for (size_t i = 0; i < size; ++i) {
            if (neighbors[i]) {
                MPI_Isend(pair, 2, MPI_INT, i, 0, MPI_COMM_WORLD, &request);
            }
        }
        bool JPN_responded = false;
        int JPN_said_yes;
        MPI_Irecv(&JPN_said_yes, 1, MPI_INT, RANK_JPN, 0, MPI_COMM_WORLD,
                &request);
        while (difftime(time(NULL), start) < time_till_bed) {
            int flag;
            MPI_Test(&request, &flag, MPI_STATUS_IGNORE);
            if (flag) {
                JPN_responded = true;
                fprintf(stdout, "JPN said %s!\n", JPN_said_yes ? "yes" : "no");
                break;
            }
        }
        if (!JPN_responded) {
            fprintf(stdout, "No response from JPN.\n");
        }
    } else {
        MPI_Request request;
        MPI_Irecv(pair, 2, MPI_INT, MPI_ANY_SOURCE, 0, MPI_COMM_WORLD,
                &request);
        while (difftime(time(NULL), start) < time_till_bed) {
            int flag;
            MPI_Test(&request, &flag, MPI_STATUS_IGNORE);
            if (flag) {
                if (pair[1] == rank) {
                    int I_say_yes = ((float) rand()) / RAND_MAX < 0.75;
                    MPI_Isend(&I_say_yes, 1, MPI_INT, pair[0], 0,
                            MPI_COMM_WORLD, &request);
                } else {
                    for (size_t i = 0; i < size; ++i) {
                        if (neighbors[i]) {
                            MPI_Isend(pair, 2, MPI_INT, i, 0, MPI_COMM_WORLD,
                                    &request);
                        }
                    }
                }
                break;
            }
        }
    }

    MPI_Finalize();
    return 0;
}
