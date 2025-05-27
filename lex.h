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

#ifndef  LEXICAL_ANALYZER_H
#define  LEXICAL_ANALYZER_H

#include "error.h"
#include "token.h"

Error lex_init(const char *  src_code);

Error get_token(Token & t);

bool lex_eof(void);  // Read past end of input stream?

void lex_cleanup();


#endif /* LEXICAL_ANALYZER_H */
