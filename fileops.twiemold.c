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
        return 8;
    }

    char *lowerWord = malloc(sizeof word);
    long *wordPos = malloc(sizeof(long));
    Record *recordInfo = malloc(sizeof(Record));
    Record *toWrite = malloc(sizeof(Record));
    long filesize = getFilesize(fp);
    int num;

    // sanitize input
    convertToLower(word, lowerWord);

    // match starting letter
    long startPos = matchStartingLetter(lowerWord[0]);

    num = fseek(fp, startPos, SEEK_SET);
    if (num > 0) {
        printf("ERROR during seek in insertWord\n");
        return 8;
    }
    num = fread(wordPos, sizeof(long), 1, fp);
    if (num != 1) {
        printf("ERROR during read in insertWord\n");
        return 8;
    }
    if (*wordPos == 0) {
        // no words with this letter
        num = fseek(fp, startPos, SEEK_SET);
        if (num > 0) {
            printf("ERROR during seek in insertWord\n");
            return 8;
        }
        long *filesizePtr = &filesize;
        num = fwrite(filesizePtr, sizeof(long), 1, fp);
        if (num != 1) {
            printf("ERROR during write in insertWord\n");
            return 8;
        }
        strcpy(toWrite->word, word);
        toWrite->nextpos = 0;
        num = fseek(fp, 0, SEEK_END);
        if (num > 0) {
            printf("ERROR during seek in insertWord\n");
            return 8;
        }
        num = fwrite(toWrite, sizeof(Record), 1, fp);
        if (num != 1) {
            printf("ERROR during write in insertWord\n");
            return 8;
        }
    } else {
        // there are words with this letter
        num = fseek(fp, *wordPos, SEEK_SET);
        if (num > 0) {
            printf("ERROR during seek in insertWord\n");
            return 8;
        }
        num = fread(recordInfo, sizeof(Record), 1, fp);
        if (num != 1) {
            printf("ERROR during read in insertWord\n");
            return 8;
        }
        while (recordInfo->nextpos != 0) {
            *wordPos = recordInfo->nextpos;
            num = fseek(fp, recordInfo->nextpos, SEEK_SET);
            if (num > 0) {
                printf("ERROR during seek in insertWord\n");
                return 8;
            }
            num = fread(recordInfo, sizeof(Record), 1, fp);
            if (num != 1) {
                printf("ERROR during read in insertWord\n");
                return 8;
            }
        }
        num = fseek(fp, *wordPos, SEEK_SET);
        if (num > 0) {
            printf("ERROR during seek in insertWord\n");
            return 8;
        }
        strcpy(toWrite->word, recordInfo->word);
        toWrite->nextpos = filesize;
        num = fwrite(toWrite, sizeof(Record), 1, fp);
        if (num != 1) {
            printf("ERROR during write in insertWord\n");
            return 8;
        }
        num = fseek(fp, 0, SEEK_END);
        if (num > 0) {
            printf("ERROR during seek in insertWord\n");
            return 8;
        }
        strcpy(toWrite->word, word);
        toWrite->nextpos = 0;
        num = fwrite(toWrite, sizeof(Record), 1, fp);
        if (num != 1) {
            printf("ERROR during write in insertWord\n");
            return 8;
        }
    }

    free(toWrite);
    free(recordInfo);
    free(wordPos);
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
    long startPos = matchStartingLetter(letter);

    long *wordPos = malloc(sizeof(long));
    Record *recordInfo = malloc(sizeof(Record));

    fseek(fp, startPos, SEEK_SET);
    fread(wordPos, sizeof(long), 1, fp);

    if (*wordPos == 0) {
        // no words with this letter
        free(wordPos);
        free(recordInfo);
        return 0;
    } else {
        // there are words with this letter
        *count += 1;
        fseek(fp, *wordPos, SEEK_SET);
        fread(recordInfo, sizeof(Record), 1, fp);
        while (recordInfo->nextpos != 0) {
            *count += 1;
            *wordPos = recordInfo->nextpos;
            fseek(fp, recordInfo->nextpos, SEEK_SET);
            fread(recordInfo, sizeof(Record), 1, fp);
        }
    }

    free(wordPos);
    free(recordInfo);
    return 0;
}

char **getWords(FILE *fp, char letter) {
    char **wordList;
    // validate input
    if ( ! isalpha(letter) ){
        printf("GetWord Alpha Check failed. Please input a alphabetic letter");
        return NULL;
    } else if (getFilesize(fp) == 0) {
        // no words with this letter
        wordList = malloc(sizeof(char *));
        wordList[0] = NULL;
        return wordList;
    }
    // sanitize input
    letter = tolower(letter);

    // match starting letter
    long startPos = matchStartingLetter(letter);

    long *wordPos = malloc(sizeof(long));
    Record *recordInfo = malloc(sizeof(Record));

    fseek(fp, startPos, SEEK_SET);
    fread(wordPos, sizeof(long), 1, fp);

    if (*wordPos == 0) {
        // no words with this letter
        wordList = malloc(sizeof(char *));
        wordList[0] = NULL;
        free(wordPos);
        free(recordInfo);
    } else {
        // there are words with this letter
        int *wordCount = malloc(sizeof(int));
        char *word;
        int wordLen;
        int wordIdx = 0;
        countWords(fp, letter, wordCount);
        wordList =  malloc((*wordCount + 1) * sizeof(char *));
        fseek(fp, *wordPos, SEEK_SET);
        fread(recordInfo, sizeof(Record), 1, fp);
        while (recordInfo->nextpos != 0) {
            wordLen = strlen(recordInfo->word);
            wordList[wordIdx] = (char *) malloc((wordLen + 1) * sizeof(char));
            word = malloc((wordLen + 1) * sizeof(char));
            strcpy(word, recordInfo->word);
            strcpy(wordList[wordIdx], word);
            wordIdx++;
            *wordPos = recordInfo->nextpos;
            fseek(fp, recordInfo->nextpos, SEEK_SET);
            fread(recordInfo, sizeof(Record), 1, fp);
        }
        wordLen = strlen(recordInfo->word);
        wordList[wordIdx] = malloc((wordLen + 1) * sizeof(char));
        word = malloc((wordLen + 1) * sizeof(char));
        strcpy(word, recordInfo->word);
        strcpy(wordList[wordIdx], word);
        wordIdx++;
        wordList[wordIdx] = NULL;
        free(word);
        free(wordPos);
        free(wordCount);
    }
    return wordList;
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

int testMainFunctions(char *filename) {
    // test suite will fail if run on same file multiple times
    int fileExists = 0;
    FILE *fp;

    fp = fopen(filename, "r+");
    if (fp != NULL) {
        fileExists = 1;
    }
    if ( ! fileExists ) {
        fp = (FILE *) fopen(filename, "w+");
        if (fp == NULL) {
            printf("cannot open file '%s'\n", filename);
            return 8;
        }
        // this initializes first 208 bytes when file is empty
        long *ptr = malloc(sizeof(long));
        *ptr = 0;
        for (int i = 0; i < 26; ++i) {
            fseek(fp, i * 8, SEEK_SET);
            fwrite(ptr, sizeof(long), 1, fp);
        }
        free(ptr);
    }
    char *word = malloc(MAXWORDLEN);
    int *count = malloc(sizeof(count));
    char **wordList;
    int i = 0;
    int badReturn = 0;
    *count = 0;

    countWords(fp, 'k', count);
    assert(*count == 0);

    strcpy(word, "nargles");
    badReturn = insertWord(fp, word);
    if (badReturn) {
        printf("InsertWord test failed on word %s", word);
        return 1;
    }
    countWords(fp,'n', count);
    assert(*count == 1);
    strcpy(word, "niffler");
    badReturn = insertWord(fp, word);
    if (badReturn) {
        printf("InsertWord test failed on word %s", word);
        return 1;
    }
    *count = 0;
    countWords(fp,'n', count);
    assert(*count == 2);
    wordList = getWords(fp, 'n');
    printf("getWord Testing:\n");
    while (wordList[i] != NULL) {
        printf("%s\n", wordList[i]);
        i++;
    }
    strcpy(word, "hermione");
    badReturn = insertWord(fp, word);
    if (badReturn) {
        printf("InsertWord test failed on word %s", word);
        return 1;
    }
    *count = 0;
    countWords(fp,'h', count);
    assert(*count == 1);

    fclose(fp);
    free(word);
    free(count);
    free(wordList);
    return 0;
}

//-------------------------------------

int main() {
    char *filename = malloc(MAXWORDLEN);
    strcpy(filename, "../test_file.dat");

    testMainFunctions(filename);

    free(filename);
    return 0;
}
