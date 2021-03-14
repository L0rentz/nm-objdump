/*
** EPITECH PROJECT, 2021
** B-PSU-400-NCE-4-1-nmobjdump-lorentz-andrea.romeo
** File description:
** print_error_prefix
*/

#include "my.h"

void errors_prefix(char const *binary, char const *filename, FORMAT quotes)
{
    struct stat stats;
    fprintf(stderr, binary + 2);
    if (quotes == QUOTES) {
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

void error_directory(char const *binary, char const *filename)
{
    fprintf(stderr, "%s: Warning: '%s' is a directory\n",
        binary + 2, filename);
}