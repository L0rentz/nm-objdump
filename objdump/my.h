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
#include <dirent.h>

extern jmp_buf s_jumpBuffer;

#define HAS_RELOC (0x01)
#define EXEC_P (0x02)
#define HAS_LINENO (0x04)
#define HAS_DEBUG (0x08)
#define HAS_SYMS (0x10)
#define HAS_LOCALS (0x20)
#define DYNAMIC (0x40)
#define WP_TEXT (0x80)
#define D_PAGED (0x100)
#define PRINTABLE(x) (x >= 32 && x <= 126)

enum {
    SPACER,
    NO_FILE,
    NOT_RECOGNIZED,
    TRUNCATED,
    MALFORMED_AR,
    IS_A_DIR
};

typedef enum {
    QUOTES,
    NO_QUOTES
} FORMAT;

typedef struct s_errors {
    char *filename;
    char *binary;
    char *obj_name;
    size_t next_obj;
} errors_t;

int check_file(char const *filename, int *type);
int check_archive(char const *filename);
void parse_archive_x86(errors_t *s_errors);
void parse_archive_x64(errors_t *s_errors);
void errors_prefix(char const *binary, char const *filename, FORMAT quotes);
void error_no_file(char const *binary, char const *filename);
void error_not_recognized(char const *binary, char const *filename);
void error_truncated(char const *binary, char const *filename);
void error_malformed(char const *binary, char const *filename);
void error_no_symbols(char const *binary, char const *filename);
void error_directory(char const *binary, char const *filename);
void objdump_x64(void *data, errors_t *s_errors, int archive);
void objdump_x86(void *data, errors_t *s_errors, int archive);
void shift_array_left(char *arr, int len);
void get_obj_name(char *obj_name, int save, int k, char *data);
int find_archive_type(char *data, off_t fd_size);
void flags_writer(unsigned int flags);
void hexa_display(unsigned char *data, int size);

#endif /* !MY_H_ */