// rpg.c
// Random Password Generator (RPG)
// This function provides capability for generating a stong random password.
// Copyright 2018 Viper Science LLC under the MIT license.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "rngs.h"

#define MAX_PW_LENGTH 512

struct Flags {
   bool  digits;
   bool  lower;
   bool  upper;
   bool  special;
   bool  more;
   bool  reduce_confusion;
};

// Generate a random integer in the range min to max by binning the result from
// a 0-1 RNG similar to a histogram.
int RandomInt(int min, int max) {
    double delta = 1.0/(max - min);
    double rnum = Random(); // Random value between 0 and 1.
    int randInt = 1;
    while (randInt*delta <= rnum)
        randInt++;
    return (randInt - 1) + min;
}

// Generate a random string of characters for use as a strong password.
void rpg(int n, char* buff, struct Flags flags, char* forbidden) {

    // Get a list of characters that the password will be generated from.
    char ascii_chars[128];
    ascii_chars[0] = '\0';
    int ascii_chars_len = 0;

    if (flags.digits) {
        if (flags.reduce_confusion) {
            strcpy(ascii_chars, "023456789");
            ascii_chars_len += 9;
        }
        else {
            strcpy(ascii_chars, "0123456789");
            ascii_chars_len += 10;
        }
    }

    if (flags.upper) {
        if (flags.reduce_confusion) {
            strcat(ascii_chars, "ABCDEFGHIJKLMNPQRSTUVWXYZ");
            ascii_chars_len += 25;
        }
        else {
            strcat(ascii_chars, "ABCDEFGHIJKLMNOPQRSTUVWXYZ");
            ascii_chars_len += 26;
        }
    }

    if (flags.lower) {
        if (flags.reduce_confusion) {
            strcat(ascii_chars, "abcdefghijkmnopqrstuvwxyz");
            ascii_chars_len += 25;
        }
        else {
            strcat(ascii_chars, "abcdefghijklmnopqrstuvwxyz");
            ascii_chars_len += 26;
        }
    }

    if (flags.special) {
        strcat(ascii_chars, "_=+!@#$\%&*?-");
        ascii_chars_len += 12;
    }

    if (flags.more) {
        if (flags.reduce_confusion) {
            strcat(ascii_chars, "`^\\/~<>'\",.(){}[];:");
            ascii_chars_len += 19;
        }
        else {
            strcat(ascii_chars, "`|^\\/~<>'\",.(){}[];:");
            ascii_chars_len += 20;
        }
    }

    // Remove any characters specified as forbidden by the user.
    // Got some help here: https://stackoverflow.com/questions/9895216/how-to-remove-all-occurrences-of-a-given-character-from-string-in-c
    char *fchar = forbidden;
    while(*fchar) {
        // Remove forbidden char from list.
        char *pread = ascii_chars, *pwrite = ascii_chars;
        while (*pread) {
            *pwrite = *pread++; // Copy read to write (post increment read).
            if (*pwrite != *fchar) pwrite++; // Increment write if allowed char.
        }
        *pwrite = '\0'; // End NULL char to end string
        fchar++; // Increment the pointer to the next char
    }
    
    // Generate the random password and ensure it meets the criteria (contains
    // characters from each specified group).
    int i;
    for (i = 0; i < n; i++) {
        int idx = RandomInt(0, ascii_chars_len);
        buff[i] = ascii_chars[idx];
    }
    buff[i] = '\0'; // Terminate the string

}

int main(int argc, char *argv[]) {

    int n = 12; // Default password length
    char f[128]; f[0] = '\0'; // String of forbidden characters

    // Flag structure (default values)
    struct Flags flags;
    flags.digits = false;
    flags.lower = false;
    flags.upper = false;
    flags.special = false;
    flags.more = false;
    flags.reduce_confusion = false;

    // Parse command line arguments
    for (int count = 1; count < argc; count++) {
        if (strcmp(argv[count], "-n") == 0) {
            if (argc > (count + 1)) {
                count++;
                n = atoi(argv[count]);
            }
        }
        else if (strcmp(argv[count], "--forbidden") == 0 ||
            strcmp(argv[count], "-f") == 0) {
            if (argc > (count + 1)) {
                count++;
                strcpy(f, argv[count]);
            }
        }
        else if (strcmp(argv[count], "--digits") == 0 ||
            strcmp(argv[count], "-d") == 0) {
            flags.digits = true;
        }
        else if (strcmp(argv[count], "--lower") == 0 ||
            strcmp(argv[count], "-l") == 0) {
            flags.lower = true;
        }
        else if (strcmp(argv[count], "--upper") == 0 ||
            strcmp(argv[count], "-u") == 0) {
            flags.upper = true;
        }
        else if (strcmp(argv[count], "--special") == 0 ||
            strcmp(argv[count], "-s") == 0) {
            flags.special = true;
        }
        else if (strcmp(argv[count], "--more") == 0 ||
            strcmp(argv[count], "-m") == 0) {
            flags.more = true;
        }
        else if (strcmp(argv[count], "--reduce-confusion") == 0 ||
            strcmp(argv[count], "-r") == 0) {
            flags.reduce_confusion = true;
        }
        else {
            printf("Unknown input %s", argv[count]);
        }
    }

    // If no characters were specified, use all of them.
    if (!(flags.digits || flags.lower || flags.upper || flags.special ||
          flags.more)) {
        flags.digits = true;
        flags.lower = true;
        flags.upper = true;
        flags.special = true;
        flags.more = true;
    }

    // Note on RNG:
    // For this application we could have simply used the built in srand() and
    // rand() functions since the randomness we are trying to emulate (or
    // improve upon) is a user randomly selecting characters on their keyboard.
    // However, since the built in functions have such a horrible reputation we
    // will use a better quality implementation i.e., Park and Miller.

    // Seed the RNG
    PlantSeeds(-1); // Negative seed uses system time as the seed.
    Random(); // Call the RNG once to initialize (Important!).

    char buff[MAX_PW_LENGTH]; // Buffer for the created password

    // Create the password
    rpg(n, buff, flags, f);

    // Display the password to the user
    printf("%s", buff);
    printf("%s", "\n");

    return 0;
}
