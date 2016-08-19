#include "arpabet.h"
#include "mutate.h"
#include "parse.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/*Include the MPI header.*/

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

    /*Initialize MPI.*/
    int rank;
    int size;
    /*Get this process' rank.*/
    /*Get the number of processes.*/

    srand(time(NULL) + rank * rank);

    if (size == 1) {
        fprintf(stdout, "You can't play this game with only one player!\n");
        return 1;
    }

    char whisper[argc - 1][MAX_WORD_LENGTH];

    /*This block copies the message given in the command line arguments to the
     * `whisper` buffer. It should only be executed by the process that whispers
     * first. The other processes will get the message from their neighbor.*/
    /*if () {*/
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
    /*}*/

    /*Each process should whisper once and listen once (not necessarily in that
     * order!) during the game. Write code that accomplishes that here.*/

    /*Each process should execute these two lines when they whisper.*/
    fprintf(stdout, "Rank %d whispers: ", rank);
    print(whisper, argc - 1);

    /*Each process should execute this line after they hear the message.*/
    mutate(whisper, argc - 1, words, phonemes, accents, Ns);

    /*Once each process has heard and passed on the message, the game is over
     * and you can exit.*/

    /*Finalize MPI.*/
    return 0;
}
