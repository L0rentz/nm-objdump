/*
** EPITECH PROJECT, 2021
** B-PSU-400-NCE-4-1-nmobjdump-lorentz-andrea.romeo
** File description:
** symbols_list
*/

#include "my.h"

void swap(symbolsInfos_t *a, symbolsInfos_t *b)
{
    symbolsInfos_t tmp = *a;
    *a = *b;
    *b = tmp;
}

int is_sorted(char *a, char *b)
{
    int i = 0, j = 0;
    for (; a[i] != '\0' && b[j] != '\0'; i++, j++) {
        for (; !isalnum(a[i]); i++);
        for (; !isalnum(b[j]); j++);
        if (toupper(a[i]) > toupper(b[j])) return (0);
        else if (toupper(a[i]) == toupper(b[j])) continue;
        else return (1);
    }
    if (a[i] == '\0' && b[j] == '\0' && j > i) return (0);
    else if (a[i] != '\0' && b[j] == '\0') return (0);
    return (1);
}

void sort_symbol_list(symbolsInfos_t (*list)[], size_t len)
{
    for (int i = 0; i < len - 1; i++) {
        for (int j = 0; j < len - i - 1; j++) {
            if (!is_sorted((*list)[j].name, (*list)[j + 1].name)) {
                swap((*list) + j, (*list) + j + 1);
            }
        }
    }
}

void print_symbol_list(symbolsInfos_t *list, size_t len, int type)
{
    char adr_tmp[len][24];
    int max_adr_len = 16;
    if (type == 1) max_adr_len = 8;
    for (size_t i = 0; i < len; i++)
        sprintf(adr_tmp[i], "%x", list[i].adr);
    for (size_t i = 0, spacer = 0; i < len; i++) {
        spacer = max_adr_len - strlen(adr_tmp[i]);
        if (strlen(adr_tmp[i]) > 0 && adr_tmp[i][0] != '0')
            printf("%0*d%s %c %s\n",
                spacer, 0, adr_tmp[i], list[i].type, list[i].name);
        else if (list[i].type == 'T' || list[i].type == 'n'
        || list[i].type == 'B')
            printf("%0*d%s %c %s\n",
                spacer, 0, adr_tmp[i], list[i].type, list[i].name);
        else
            printf("%*s %c %s\n", spacer + 1, "", list[i].type, list[i].name);
    }
}