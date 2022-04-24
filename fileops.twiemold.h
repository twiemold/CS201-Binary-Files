/*
 * Thomas Wiemold
 * CS201
 * 4/24/2022
 * Programming Assignment #5: File Operations
 * Header File
 */

#ifndef CS201BINARYFILES_FILEOPS_TWIEMOLD_H
#define CS201BINARYFILES_FILEOPS_TWIEMOLD_H

#define MAXWORDLEN 31

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct {
    char word[1+MAXWORDLEN];
    long nextpos;
} Record;

// Main functions
int insertWord(FILE *fp, char *word);
int countWords(FILE *fp, char letter, int *count);
char **getWords(FILE *fp, char letter);

// Utility Functions
int checkWord(char *word);
int convertToLower(char *word, char *convertedWord);
int test_tolower(char *word);

// Testing functions
int testUtils();

#endif //CS201BINARYFILES_FILEOPS_TWIEMOLD_H
