#include "parse.h"

#include "constants.h"
#include "coordinates.h"

#include <stdlib.h>
#include <string.h>

void parse_line(char line[MAX_LINE_LENGTH + 1], char name[MAX_NAME_LENGTH],
        Coordinates *coord) {
    char * begin = line;
    char * end = NULL;
    /*Advance past country code.*/
    for (; *begin != ','; ++begin) {
    }
    ++begin;
    (*coord).latitude = strtof(begin, &end);

    if (*end == '\0') {
        return;
    }
    begin = end;
    ++begin;
    (*coord).longitude = strtof(begin, &end);
    if (*end == '\0') {
        return;
    }

    begin = ++end;
    strcpy(name, begin);
}

void parse_csv(
        FILE *csv,
        char names[CSV_LENGTH][MAX_NAME_LENGTH],
        Coordinates coords[CSV_LENGTH]
        ) {
    size_t i = 0;
    char line[MAX_NAME_LENGTH + 1];
    line[MAX_NAME_LENGTH] = '\0';
    while (fgets(line, MAX_LINE_LENGTH + 1, csv) != NULL) {
        /*Skip comments.*/
        if (strlen(line) > 0 && line[0] == '#') {
            /*If we haven't already read in the whole line, read more.*/
            if (strlen(line) == MAX_LINE_LENGTH &&
                    line[MAX_LINE_LENGTH - 1] != '\n') {
                while (getc(csv) != '\n') {
                }
            }
            continue;
        }
        for (size_t j = 0; j < MAX_LINE_LENGTH; ++j) {
            if (line[j] == '\n') {
                line[j] = '\0';
                break;
            }
        }
        parse_line(line, names[i], &(coords[i]));
        ++i;
    }
}
