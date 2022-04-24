/*
 * Thomas Wiemold
 * CS201
 * 4/24/2022
 * Programming Assignment #5: File Operations
 * C File
 */
#include "fileops.twiemold.h"

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

int main() {
  testUtils();
}
