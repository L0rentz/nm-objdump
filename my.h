/*
** EPITECH PROJECT, 2021
** B-PSU-400-NCE-4-1-nmobjdump-lorentz-andrea.romeo
** File description:
** my
*/

#ifndef MY_H_
#define MY_H_

#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <setjmp.h>
#include <unistd.h>
#include <elf.h>
#include <sys/mman.h>

extern jmp_buf s_jumpBuffer;

enum {
    SPACER,
    NO_FILE,
    NOT_RECOGNIZED,
    TRUNCATED
};

enum {
    QUOTES,
    NO_QUOTES
};

void check_file(char const *filename);
void error_no_file(char const *binary, char const *filename);
void error_not_recognized(char const *binary, char const *filename);
void error_truncated(char const *binary, char const *filename);

#endif /* !MY_H_ */