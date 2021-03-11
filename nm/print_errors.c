/*
** EPITECH PROJECT, 2021
** B-PSU-400-NCE-4-1-nmobjdump-lorentz-andrea.romeo
** File description:
** print_errors
*/

#include "my.h"

void errors_prefix(char const *binary, char const *filename, int select)
{
    struct stat stats;
    fprintf(stderr, binary + 2);
    if (select == QUOTES) {
        fprintf(stderr, ": '");
        fprintf(stderr, filename);
        fprintf(stderr, "': ");
    }
    else {
        fprintf(stderr, ": ");
        fprintf(stderr, filename);
        fprintf(stderr, ": ");
    }
}

void error_no_file(char const *binary, char const *filename)
{
    errors_prefix(binary, filename, QUOTES);
    fprintf(stderr, "No such file\n");
}

void error_not_recognized(char const *binary, char const *filename)
{
    errors_prefix(binary, filename, NO_QUOTES);
    fprintf(stderr, "file format not recognized\n");
}

void error_truncated(char const *binary, char const *filename)
{
    errors_prefix(binary, filename, NO_QUOTES);
    fprintf(stderr, "file truncated\n");
}

void error_malformed(char const *binary, char const *filename)
{
    errors_prefix(binary, filename, NO_QUOTES);
    fprintf(stderr, "malformed archive\n");
    exit(84);
}