/* current program version */
const char* program_version = "GCI v0.3";

/* description of software */
const char* program_doc = "\nGCI is an automated tool for compiling, allocating, linking, and injecting C code into games for the Wii/Gamecube. The main use of this program is with the \"--inject\" flag, which tells the program to do the following 4 steps: 1) compile code in the directory provided 2) find an arrangement of the code that will fit inside the memory regions provided 3) link the code 4) inject the code into the iso file provided. In one command the user can take their raw C files and produce a runnable iso file that has their _main() function injected at a specified address";


