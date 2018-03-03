#!/usr/bin/env python3

# Random Password Generator (RPG)
# This module provides capability for generating a stong random password.
# Copyright 2018 Viper Science LLC under the MIT license.
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.
#


import argparse
import random


def gen(n=12, flags={}, forbidden=""):
    """Generate a random string of characters for use as a secure password.

    Inputs
    n: Length of the string to generate.
    flags:
       digits: Include at least one digit in the string.
       lower: Include at least one lowercase letter in the string.
       upper: Include at least one uppercase letter in the string.
       special: Include at least one common special character in the string.
       more: Include at least one of other special characters in the string.
       ReduceConfusion: Remove "01l|O" characters.
    forbidden: Any characters that should not be in the final string.

    Outputs
    A string of random ACSII characters.
    """

    # Create the random number generator
    rng = random.SystemRandom()

    # This variable will hold a string of potential characters from which to
    # make the password.
    ascii_chars = "";

    # Digits
    ascii_digits = '0123456789'
    if flags['digits']:
        ascii_chars += ascii_digits

    # Uppercase Letters
    ascii_uppers = "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    if flags['upper']:
        ascii_chars += ascii_uppers

    # Lowercase Letters
    ascii_lowers = "abcdefghijklmnopqrstuvwxyz"
    if flags['lower']:
        ascii_chars += ascii_lowers

    # Special Characters (Common)
    ascii_special = "_=+!@#$%&*?-"
    if flags['special']:
        ascii_chars += ascii_special

    # More special Characters (Less Common)
    ascii_more = r"""`|^\/~<>'",.(){}[];:"""
    if flags['more']:
        ascii_chars += ascii_more

    # Remove Confusion (no 1's, 0's, l's, |'s or O's)
    if flags['ReduceConfusion']:
        ascii_chars = ascii_chars.replace("l", "")
        ascii_chars = ascii_chars.replace("O", "")
        ascii_chars = ascii_chars.replace("1", "")
        ascii_chars = ascii_chars.replace("0", "")
        ascii_chars = ascii_chars.replace("|", "")

    # Remove any characters specified as forbidden by the user
    ascii_chars = [c for c in ascii_chars if c not in forbidden]

    def meetsCriteria(candidate):
        """Ensure the candidate password contains characters from each desired
        set of ASCII characters."""
        met = True
        if flags['digits']:
            if len(set(candidate) & set(ascii_digits)) < 1:
                met = False
        if flags['upper']:
            if len(set(candidate) & set(ascii_uppers)) < 1:
                met = False
        if flags['lower']:
            if len(set(candidate) & set(ascii_lowers)) < 1:
                met = False
        if flags['special']:
            if len(set(candidate) & set(ascii_special)) < 1:
                met = False
        if flags['more']:
            if len(set(candidate) & set(ascii_more)) < 1:
                met = False
        return met

    # Generate the random password and ensure it meets the criteria (contains
    # characters from each specified group).
    pw = ""
    while not meetsCriteria(pw):
        pw = ""
        for count in range(0,n):
            # Randomly select a character from the list
            ridx = rng.randint(0, len(ascii_chars) - 1)
            pw += ascii_chars[ridx]

    return pw  # Return the newly create password


if __name__ == "__main__":

    # Get/Parse Inputs
    parser = argparse.ArgumentParser(description="Generate a random password.")
    parser.add_argument('-n', '--length',
        help='Password length (number of characters)',
        default=12,
        type=int)
    parser.add_argument('-d', '--digits',
        help='Include characters: 0123456789',
        action='store_true')
    parser.add_argument('-l', '--lower',
        help='Include characters: abcdefghijklmnopqrstuvwxyz',
        action='store_true')
    parser.add_argument('-u', '--upper',
        help='Include characters: ABCDEFGHIJKLMNOPQRSTUVWXYZ',
        action='store_true')
    parser.add_argument('-s', '--special',
        help='Include characters: =+-!@#$%%&*?_',
        action='store_true')
    parser.add_argument('-m', '--more',
        help=r"""Include characters: `|^\/~<>'",.(){}[];:""",
        action='store_true')
    parser.add_argument('-r', '--reduce-confusion',
        help="Do not use confusing characters: 01l|O",
        action='store_true')
    parser.add_argument('-f', '--forbidden',
        help="Characters not to be used.",
        default="")
    args = parser.parse_args()

    # Create the flags dict to pass to the generator
    flags = {}
    if args.digits or args.lower or args.upper or args.special or args.more:
        flags['digits'] = args.digits
        flags['lower'] = args.lower
        flags['upper'] = args.upper
        flags['special'] = args.special
        flags['more'] = args.more
    else:
        # If no arguments set, use all characters.
        flags['digits'] = True
        flags['lower'] = True
        flags['upper'] = True
        flags['special'] = True
        flags['more'] = True
    flags['ReduceConfusion'] = args.reduce_confusion

    # Create the password and print it to the screen
    print(gen(args.length, flags, args.forbidden))
