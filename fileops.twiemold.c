/*
 * Thomas Wiemold
 * CS201
 * 4/24/2022
 * Programming Assignment #5: File Operations
 * C File
 */
#include "fileops.twiemold.h"

int insertWord(FILE *fp, char *word) {
    char *lowerWord = malloc(sizeof word);
    long *wordLocation = malloc(sizeof(long));
    Record *recordInfo = malloc(sizeof(Record));
    Record *toWrite = malloc(sizeof(Record));

    int rc = fseek(fp, 0, SEEK_END);
    if (rc != 0) {
        printf("fseek() failed\n");
        return rc;
    }
    long filesize = ftell(fp);

    convertToLower(word, lowerWord);
    long startingLocation = -1;
    for (int i = 0; i < strlen(ALPHABET); ++i) {
        if (lowerWord[0] == ALPHABET[i]) {
            startingLocation = i * 8;
            break;
        }
    }
    fseek(fp, startingLocation, SEEK_SET);
    fread(wordLocation, sizeof(long), 1, fp);
    // word location is sometimes a large negative
    if (*wordLocation == 0) {
        // no words with this letter
        fseek(fp, startingLocation, SEEK_SET);
        long *filesizePtr = &filesize;
        fwrite(filesizePtr, sizeof(long), 1, fp);
        strcpy(toWrite->word, word);
        toWrite->nextpos = 0;
        fseek(fp, 0, SEEK_END);
        fwrite(toWrite, sizeof(Record), 1, fp);
    } else {
        // there are words with this letter
        fseek(fp, *wordLocation, SEEK_SET);
        fread(recordInfo, sizeof(Record), 1, fp);
        while (recordInfo->nextpos != 0) {
            *wordLocation = recordInfo->nextpos;
            fseek(fp, recordInfo->nextpos, SEEK_SET);
            fread(recordInfo, sizeof(Record), 1, fp);
        }
        fseek(fp, *wordLocation, SEEK_SET);
        strcpy(toWrite->word, recordInfo->word);
        toWrite->nextpos = filesize;
        fwrite(toWrite, sizeof(Record), 1, fp);
        fseek(fp, 0, SEEK_END);
        strcpy(toWrite->word, word);
        toWrite->nextpos = 0;
        fwrite(toWrite, sizeof(Record), 1, fp);
    }

    free(toWrite);
    free(recordInfo);
    free(wordLocation);
    free(lowerWord);

    return 0;
}

//--------------------------------------------------------
// return 1 if any chararacters are non-alphabetic;
// otherwise return 0

int checkWord(char *word) {
    int i, len;

    len = strlen(word);
    for (i=0; i<len; ++i) {
        if ( ! isalpha(word[i]) )
            return 1;
    }

    return 0;
}

//--------------------------------------------------------
// convert all characters to their lower-case equivalents;
// leaves characters that are already lower case unchanged
// returns zero always

int convertToLower(char *word, char *convertedWord) {
    int i, len;

    strcpy(convertedWord, word);

    len = strlen(word);
    for (i=0; i<len; ++i)
        convertedWord[i] = tolower(word[i]);

    return 0;
}

//--------------------------------------------------------

int test_tolower(char *word) {
    int len, i, val;
    char newword[MAXWORDLEN];

    len = strlen(word);
    strcpy(newword, word);
    for (i=0; i<len; ++i) {
        newword[i] = tolower(word[i]);
    }
    printf("newword is |%s|\n", newword);
    return 0;
}

//--------------------------------------------------------

int testUtils() {
    char buffer[MAXWORDLEN];
    char convertedBuffer[MAXWORDLEN];
    int rc;

    test_tolower("Nargles");

//strcpy(buffer, "nargles");
//test_tolower(buffer);

    strcpy(buffer, "John's");
    rc = checkWord(buffer);
    printf("%s: %d\n", buffer, rc);

    strcpy(buffer, "maryRose");
    rc = checkWord(buffer);
    printf("%s: %d\n", buffer, rc);
    if (rc) {
        convertToLower(buffer, convertedBuffer);
        printf("converted: |%s|\n", convertedBuffer);
    }

    strcpy(buffer, "mary Rose");
    rc = checkWord(buffer);
    printf("%s: %d\n", buffer, rc);

    return 0;
}

//-------------------------------------
int testInsertWord(FILE *fp) {
    char *word = malloc(MAXWORDLEN);

    strcpy(word, "nargles");
    insertWord(fp, word);
    strcpy(word, "hermione");
    insertWord(fp, word);

    free(word);
    return 0;
}

//-------------------------------------

int main() {
    int fileExists = 0;
    char *filename = malloc(MAXWORDLEN);
    strcpy(filename, "test_file");
    FILE *fp;

    fp = fopen(filename, "r+");
    if (fp != NULL)
        fileExists = 1;
    if ( ! fileExists ) {
        fp = (FILE *) fopen(filename, "w+");
        if (fp == NULL) {
            printf("cannot open file '%s'\n", filename);
            return 8;
        }
    }

    for (int i = 0; i < 26; ++i) {
        long *ptr = malloc(sizeof(long));
        fseek(fp, i * 8, SEEK_SET);
        fwrite(ptr, sizeof(long), 1, fp);
    }

    testInsertWord(fp);

    fclose(fp);
    free(filename);
    return 0;

}
