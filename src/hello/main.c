#include <stdio.h>

/*Include the MPI header.*/

int main(int argc, char **argv) {
    /*Initialize MPI.*/
    int rank;
    int size;
    /*Get this process' rank.*/
    /*Get the number of processes.*/
    printf("Hello world! I am rank %d of %d.\n", rank, size);
    /*Finalize MPI.*/
    return 0;
}
