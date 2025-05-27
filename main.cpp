/**************************************************************************
 * CSCI 490D - Assignment 1 - Spring 2025
 *
 * Programmer:  Muhammad Siddiqui Z1922019
 * Date Due:    02/07/2025
 * Section:     1
 * Professor:   Kirk Duffin
 *
 *
 *************************************************************************/

#include <iostream>
#include <sys/mman.h>
#include "lex.h"
#include "error.h"
#include "token.h"
#include "tree.h"
#include "buffio.h"
#include "machine.h"

using namespace std;

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        cerr << "Usage: " << argv[0] << " <source_file>\n";
        return 1;
    }

    Error err = lex_init(argv[1]);
    if (err.error != NCC_OK)
    {
        print_error(err);
        return 1;
    }

    // Allocate executable memory for the program
    unsigned char* prog = (unsigned char*) mmap(0, 50000,
        PROT_EXEC | PROT_READ | PROT_WRITE,
        MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);

    if (prog == MAP_FAILED)
    {
        cerr << "Memory allocation failed.\n";
        return 1;
    }

    Token curToken;
    get_token(curToken);

    shared_ptr<TreeNode> root;
    parseStatementSequence(root, curToken);

    if (root)
    {
        print_tree(root);

        int dummy = 0;
        machine_generate(prog, root, dummy);
    }

    lex_cleanup();
    munmap(prog, 50000);
    return 0;
}




