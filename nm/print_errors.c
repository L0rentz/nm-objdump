/*
** EPITECH PROJECT, 2021
** B-PSU-400-NCE-4-1-nmobjdump-lorentz-andrea.romeo
** File description:
** print_errors
*/

#include "my.h"

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

void error_no_symbols(char const *binary, char const *filename)
{
    errors_prefix(binary, filename, NO_QUOTES);
    fprintf(stderr, "no symbols\n");
}