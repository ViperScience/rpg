// main.rs
// Random Password Generator (RPG) Main Function
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

extern crate rand;

//use std::io;

use std::env;
use rand::Rng;

// Structure for storing the program controls.
struct Controls {
    pwlen: usize,
    digits: bool,
    upper: bool,
    lower: bool,
    special: bool,
    more: bool,
    reduce: bool,
    forbidden: Vec<char>,
}

// Check the generated password to ensure it meets the criteria.
fn check(password: &String, controls: &Controls) -> bool {
    if password.len() != controls.pwlen {
        return false;
    }
    if controls.digits {
        if !password.chars().any(|c| "0123456789".contains(c)) {
            return false;
        }
    }
    if controls.upper {
        if !password.chars().any(|c| "ABCDEFGHIJKLMNOPQRSTUVWXYZ".contains(c)) {
            return false;
        }
    }
    if controls.lower {
        if !password.chars().any(|c| "abcdefghijklmnopqrstuvwxyz".contains(c)) {
            return false;
        }
    }
    if controls.special {
        if !password.chars().any(|c| "_=+!@#$%&*?-".contains(c)) {
            return false;
        }
    }
    if controls.more {
        if !password.chars().any(|c| r#"`|^\/~<>'",.(){}[];"#.contains(c)) {
            return false;
        }
    }
    true
}

// Generate a random string of characters for use as a strong password.
fn rpg(controls: Controls) -> String {

    // This string will contain the characters from which to generate the pw.
    let mut chars: String = String::new();

    // Digits
    if controls.digits {
        chars.push_str("0123456789");
    }

    // Uppercase Letters
    if controls.upper {
        chars.push_str("ABCDEFGHIJKLMNOPQRSTUVWXYZ");
    }

    // Lowercase Letters
    if controls.lower {
        chars.push_str("abcdefghijklmnopqrstuvwxyz");
    }

    // Special Characters (Common)
    if controls.special {
        chars.push_str("_=+!@#$%&*?-");
    }

    // More special Characters (Less Common)
    if controls.more {
        chars.push_str(r#"`|^\/~<>'",.(){}[];"#);
    }

    // Remove Confusion (no 1's, 0's, l's, |'s or O's)
    if controls.reduce {
        chars = chars.replace("l", "");
        chars = chars.replace("O", "");
        chars = chars.replace("1", "");
        chars = chars.replace("0", "");
        chars = chars.replace("|", "");
    }

    // Remove any characters specified as forbidden by the user.
    for c in &controls.forbidden {
        chars = chars.replace(&(c.to_string())[..], "");
    }
    
    // Generate the random password and ensure it meets the criteria (contains
    // characters from each specified group).
    let mut pw = String::new();
    while !check(&pw, &controls) {
        pw = String::new();
        for _ in 0..controls.pwlen {
            let rand_idx = rand::thread_rng().gen_range(0, chars.len());
            pw.push(chars.chars().nth(rand_idx).unwrap());
        }
    }

    pw
}

// Parse the command line arguments.
fn parse_args(args: Vec<String>) -> Controls {
    // Takes ownership of the input "args".

    // Create a controls structure and initialize to default values.
    let mut controls = Controls {
        pwlen: 12,
        digits: false,
        upper: false,
        lower: false,
        special: false,
        more: false,
        reduce: true,
        forbidden: Vec::new(),
    };

    // If no control arguments are defined, return defaults. Otherwise, process 
    // the input args and set the control values accordingly. 
    if args.len() > 1 {

        // Loop through the arguments and parse them appropriately. We use a 
        // while loop and index here because we may need to consume multiple 
        // args on a single iteration (e.g., parsing length input).
        let mut index = 1;
        while index < args.len() {

            // Clone the String value and convert it to lower case only.
            let arg: String = args[index].clone().to_lowercase(); 
            
            match &arg[..] { // match on string slices
                "-n" | "--length" => {
                    index += 1;
                    controls.pwlen = args[index].parse()
                        .expect("Positive integer must follow length flag.");
                }
                "-d" | "--digits" => {
                    controls.digits = true;
                }
                "-l" | "--lower" => {
                    controls.lower = true;
                }
                "-u" | "--upper" => {
                    controls.upper = true;
                }
                "-s" | "--special" => {
                    controls.special = true;
                }
                "-m" | "--more" => {
                    controls.more = true;
                }
                "-r" | "--reduce-confusion" => {
                    controls.reduce = true;
                }
                "-f" | "--forbidden" => {
                    index += 1;
                    let char_vec: Vec<char> = args[index].chars().collect();
                    for c in char_vec {
                        controls.forbidden.push(c);
                    }
                }
                _ => {
                    println!("Skipping unknown input: {}", arg);
                }
            }
            index += 1;
        }
    } else {
        // If no inputs are specified, use the default length but turn on all 
        // possible characters (no forbidden characters). Additionally, reduce 
        // confusion will be off.
        controls.digits = true;
        controls.upper = true;
        controls.lower = true;
        controls.special = true;
        controls.more = true;
    }
    
    controls // Return the controls structure
}

fn main() {

    // Parse the input args.
    let args: Vec<String> = env::args().collect();
    let ctrl = parse_args(args);

    // Create the password and print it to the terminal.
    println!("{}", rpg(ctrl));

}
