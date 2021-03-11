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
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

extern jmp_buf s_jumpBuffer;

enum {
    SPACER,
    NO_FILE,
    NOT_RECOGNIZED,
    TRUNCATED,
    MALFORMED_AR
};

typedef enum {
    QUOTES,
    NO_QUOTES
} FORMAT;

typedef struct s_symbolsInfos {
    char *name;
    char type;
    size_t adr;
} symbolsInfos_t;

typedef struct s_errors {
    char *filename;
    char *binary;
    char *obj_name;
    size_t next_obj;
} errors_t;

int check_file(char const *filename);
int check_archive(char const *filename);
void parse_archive(errors_t *s_errors);
void errors_prefix(char const *binary, char const *filename, FORMAT quotes);
void error_no_file(char const *binary, char const *filename);
void error_not_recognized(char const *binary, char const *filename);
void error_truncated(char const *binary, char const *filename);
void error_malformed(char const *binary, char const *filename);
void error_no_symbols(char const *binary, char const *filename);
void nm_x64(void *data, errors_t *s_errors);
void sort_symbol_list(symbolsInfos_t (*list)[], size_t len);
void print_symbol_list(symbolsInfos_t *list, size_t len);
char get_symbol_type(Elf64_Sym sym, Elf64_Shdr *shdr);

#endif /* !MY_H_ */