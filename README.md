# rpg
Random Password Generator - A simple utility for generating strong random passwords.

```
./rpg -n 16 --reduce-confusion
```

> 7p!%A\9EjS4p!U4^

## Usage

For the Python or C versions, the utility can be called from the command line with the following options.

| Option | Meaning|
| ------ | ------ |
| -n N   | Number of characters N to be used in the password |
| -\-digits, -d | Use digits in the password |
| -\-lower, -l | Use lowercase characters in the password |
| -\-upper, -u | Use uppercase characters in the password |
| -\-special, -s | Use common special characters in the password |
| -\-more, -m | Use less common special characters in the password |
| -\-reduce-confusion, -r | Remove similar characters to reduce confusion |
| -\-forbidden chars, -f chars | Disallowed characters |

### Examples
The compiled C version of the utility may be used as follows:

```
./rpg -n 16 --reduce-confusion
```

This will produce a 16 character password using all the character options. The following example will generate a 12 character password using only digits and upper and lower case letters.

```
./rpg -n 12 --reduce-confusion --digits --upper --lower
```

For the python version, the inputs are the same but the script needs to be run in the python interpreter (designed for python3).

```
python3 rpg.py --reduce-confusion --digits --upper --lower --special
```

## Installation
First clone the repository on your local system.

### Python
Just run the python file (rpg.py) in the python3 interpreter. You can also place the file in a bin file contained in your path and call it from anywhere.

### C
Compile program using `make` and run the program as `./rpg <options>`. You can also put the executable in a bin folder referenced by your path which allows for `rpg` to be called in the terminal regardless of the working directory.

### Javascript

You can use the code for your own application (rpg.js) or use it [here]() where it is running in the browser.

###### Copyright 2018 Viper Science LLC
