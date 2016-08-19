#include "arpabet.h"
#include "mutate.h"
#include "parse.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <mpi.h>

char words[DICTIONARY_LENGTH][MAX_WORD_LENGTH];
enum phoneme phonemes[DICTIONARY_LENGTH][MAX_NUM_PHONEMES];
accent accents[DICTIONARY_LENGTH][MAX_NUM_PHONEMES];
size_t Ns[DICTIONARY_LENGTH];

int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "Need to give me one or more words.\n");
        return 1;
    }

    FILE *dictionary = fopen(FILENAME_DICTIONARY, "r");
    if (dictionary == NULL) {
        fprintf(stderr, "Could not open dictionary file %s.\n",
                FILENAME_DICTIONARY);
        return 1;
    }
    parse_dictionary(dictionary, words, phonemes, accents, Ns);
    fclose(dictionary);

    MPI_Init(&argc, &argv);
    int rank;
    int size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    srand(time(NULL) + rank * rank);

    if (size == 1) {
        fprintf(stdout, "You can't play this game with only one player!\n");
        return 1;
    }

    char whisper[argc - 1][MAX_WORD_LENGTH];

    if (rank == 0) {
        for (size_t i = 1; i < argc; ++i) {
            if (strlen(argv[i]) > MAX_WORD_LENGTH) {
                fprintf(
                        stderr,
                        "Please use shorter words! No more than %d chars.",
                        MAX_WORD_LENGTH
                        );
            } else {
                strcpy(whisper[i - 1], argv[i]);
            }
        }
    }

    bool sent = false;
    bool received = false;
    while (!(sent && received)) {
        if ((rank == 0 && !sent) || received) {
            fprintf(stdout, "Rank %d whispers: ", rank);
            print(whisper, argc - 1);
            MPI_Send(whisper[0], (argc - 1) * MAX_WORD_LENGTH, MPI_CHAR,
                    (rank + 1) % size, 0, MPI_COMM_WORLD);
            sent = true;
        } else {
            MPI_Status status;
            MPI_Recv(whisper[0], (argc - 1) * MAX_WORD_LENGTH, MPI_CHAR,
                    (size + rank - 1) % size, 0, MPI_COMM_WORLD, &status);
            mutate(whisper, argc - 1, words, phonemes, accents, Ns);
            received = true;
        }
    }

    MPI_Finalize();
    return 0;
}
