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

#ifndef NCC_ERROR_H
#define NCC_ERROR_H

#define  NCC_OK                     0
#define  NCC_FILE_NOT_FOUND        -1
#define  NCC_EOF                   -2
#define  NCC_NOT_EQ                -3
#define  NCC_INT_START_ZERO        -4
#define  NCC_FLOAT                 -5
#define  NCC_BLOCK                 -6
#define  NCC_UNICODE               -7


// etc.  etc. ....


struct Error
{
  int error;
  int line, col;

  // Add additional fields as needed
};

void print_error(const Error & e);

#endif /* NCC_ERROR_H */
