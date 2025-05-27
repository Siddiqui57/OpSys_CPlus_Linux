# OpSys_CPlus_Linux
Operating system made using C++ on a Linux server. The OS uses intel architecture and contains Lexical Analysis, a Parse Tree, and Machine Code Generation.

Instructions included in the system contain arithmetic expressions, strings, and variables with stored memory.

input.txt: Includes different input tests.

Makefile: Used to create the executable on the Linux server.

buffio: The system to read in input, which is done character by character.

error: Creates error messages based on certain states.

lex: The lexical analyzer that takes in characters and converts them into tokens.

token: Used to create identities for the tokens in this project.

tree: The code that formulates the parse tree and puts nodes in proper position. Maintains proper associativity.

machine: The code for machine generation based on instructions in the parse tree. Follows intel architecture.

main: Runs the code using all of the files.
