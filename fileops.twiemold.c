/*
 * Thomas Wiemold
 * CS201
 * 4/24/2022
 * Programming Assignment #5: File Operations
 * C File
 */
#include "fileops.twiemold.h"

int insertWord(FILE *fp, char *word) {
    // validate input
    int wordError = 1;
    wordError = checkWord(word);
    if (wordError) {
        printf("CheckWord failed. Please insert alphabetic word");
        return 1;
    }

    char *lowerWord = malloc(sizeof word);
    long *wordLocation = malloc(sizeof(long));
    Record *recordInfo = malloc(sizeof(Record));
    Record *toWrite = malloc(sizeof(Record));
    long filesize = getFilesize(fp);

    // sanitize input
    convertToLower(word, lowerWord);

    // match starting letter
    long startingLocation = matchStartingLetter(lowerWord[0]);

    fseek(fp, startingLocation, SEEK_SET);
    fread(wordLocation, sizeof(long), 1, fp);
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

int countWords(FILE *fp, char letter, int *count) {
    // validate input
    if ( ! isalpha(letter) ){
        printf("CountWord Alpha Check failed. Please input a alphabetic letter");
        return 1;
    }
    // sanitize input
    letter = tolower(letter);

    // match starting letter
    long startingLocation = matchStartingLetter(letter);

    long *wordLocation = malloc(sizeof(long));
    Record *recordInfo = malloc(sizeof(Record));

    fseek(fp, startingLocation, SEEK_SET);
    fread(wordLocation, sizeof(long), 1, fp);

    if (*wordLocation == 0) {
        // no words with this letter
        return 0;
    } else {
        // there are words with this letter
        *count += 1;
        fseek(fp, *wordLocation, SEEK_SET);
        fread(recordInfo, sizeof(Record), 1, fp);
        while (recordInfo->nextpos != 0) {
            *count += 1;
            *wordLocation = recordInfo->nextpos;
            fseek(fp, recordInfo->nextpos, SEEK_SET);
            fread(recordInfo, sizeof(Record), 1, fp);
        }
    }

    return 0;
}

long getFilesize(FILE *fp) {
    int rc = fseek(fp, 0, SEEK_END);
    if (rc != 0) {
        printf("fseek() failed\n");
        return rc;
    }
    long filesize = ftell(fp);
    return filesize;
}

long matchStartingLetter(char letter) {
    long startingLocation = -1;
    for (int i = 0; i < strlen(ALPHABET); ++i) {
        if (letter == ALPHABET[i]) {
            startingLocation = i * 8;
            break;
        }
    }
    return startingLocation;
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

int testMainFunctions(FILE *fp) {
    char *word = malloc(MAXWORDLEN);
    int *count = malloc(sizeof(count));

    strcpy(word, "nargles");
    insertWord(fp, word);
    countWords(fp,'n', count);
    assert(*count == 1);
    strcpy(word, "hermione");
    insertWord(fp, word);

    free(word);
    free(count);
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
    // this initializes first 208 bytes
    // need to put in an if for when file is empty
    long *ptr = malloc(sizeof(long));
    *ptr = 0;
    for (int i = 0; i < 26; ++i) {
        fseek(fp, i * 8, SEEK_SET);
        fwrite(ptr, sizeof(long), 1, fp);
    }
    free(ptr);

    testMainFunctions(fp);

    fclose(fp);
    free(filename);
    return 0;

}
