#include "coordinates.h"
#include "constants.h"
#include "parse.h"
#include "neighbors.h"

#include <stdio.h>
#include <stdbool.h>
#include <time.h>

/*Include the MPI header.*/

char names[CSV_LENGTH][MAX_NAME_LENGTH];
Coordinates latlongs[CSV_LENGTH];
bool neighbors[CSV_LENGTH];

const double time_till_bed = 5;

const int RANK_USA = 0;
const int RANK_JPN = 1;

int main(int argc, char **argv) {
    /*Initialize MPI.*/
    int rank;
    int size;
    /*Get this process' rank.*/
    /*Get the number of processes.*/

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

    /*Write the rest of the program however you like!*/

    /*Finalize MPI.*/
    return 0;
}
