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
#include <iomanip>
#include <string>
#include "error.h"

using namespace std;


void print_error(const Error &e) 
{
	//error code output
    	cout << "Error Code: " << e.error << " at Line: " << e.line 
	     << ", Column: " << e.col << endl;
    
    	if (e.error == NCC_OK) 
	{
        	cout << "No error." << endl;
    
	} 
	else if (e.error == NCC_FILE_NOT_FOUND) 
	{
        	cout << "Error: File not found." << endl;
    
	} 
	else if (e.error == NCC_EOF) 
	{
        	cout << "Error: End of file reached unexpectedly." << endl;
    
	}
	else if (e.error == NCC_NOT_EQ)
	{
		cout << "Error: Improper NOT EQUAL (~=)" << endl;

	}
	else if (e.error == NCC_INT_START_ZERO)
	{
		cout << "Error: Integer cannot start with 0" << endl;

	}
	else if (e.error == NCC_FLOAT)
	{
		cout << "Error: Improper REAL number" << endl;

	}
	else if (e.error == NCC_BLOCK)
	{
		cout << "Error: Improper Block Comment Syntax" << endl;

	}
	else if (e.error == NCC_UNICODE)
	{
		cout << "Error: Improper Unicode Syntax" << endl;

	}
	else 
	{
        	cout << "Error: Unknown error code." << endl;
    	}

}


