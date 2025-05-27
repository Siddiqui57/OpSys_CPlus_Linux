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
#include "token.h"


using namespace std;

void print_token(const Token& t) 
{
	//token output
	cout << "Token ID: " << t.id << " at Line: " << t.line + 1 
	     << ", Column: " << t.col + 1 << " - ";


	
    
    	if (t.id == TOKEN_NULL) 
	{
        	cout << "Null Token" << endl;
    	} 
	else if (t.id == TOKEN_EOF) 
	{
        	cout << "End of File" << endl;
    	} 
	else if (t.id == TOKEN_PLUS) 
	{
        	cout << "Plus Symbol (+)" << endl;
    	} 
	else if (t.id == TOKEN_MINUS) 
	{
        	cout << "Minus Symbol (-)" << endl;
    	} 
	else if (t.id == TOKEN_MULT) 
	{
        	cout << "Multiplication Symbol (*)" << endl;
    	} 
	else if (t.id == TOKEN_DIV) 
	{
        	cout << "Division Symbol (/)" << endl;
    	}
	else if (t.id == TOKEN_EXP) 
	{
        	cout << "Exponentiation Symbol (^)" << endl;
    	} 
	else if (t.id == TOKEN_LESS) 
	{
        	cout << "Less Than Symbol (<)" << endl;
    	} 
	else if (t.id == TOKEN_LESS_EQ) 
	{
        	cout << "Less Than Or Equal Symbol (<=)" << endl;
    	} 
	else if (t.id == TOKEN_GREATER) 
	{
        	cout << "Greater Than Symbol (>)" << endl;
    	} 
	else if (t.id == TOKEN_GREATER_EQ) 
	{
        	cout << "Greater Than Or Equal Symbol (>=)" << endl;
    	} 
	else if (t.id == TOKEN_EQUAL) 
	{
        	cout << "Equal Symbol (=)" << endl;
    	} 
	else if (t.id == TOKEN_NOT_EQUAL) 
	{
        	cout << "Not Equal Symbol (~=)" << endl;
    	} 
	else if (t.id == TOKEN_ASSIGN) 
	{
        	cout << "Assign Symbol (<-)" << endl;
    	} 
	else if (t.id == TOKEN_NOT) 
	{
        	cout << "Not Symbol (!)" << endl;
    	} 
	else if (t.id == TOKEN_LPAREN) 
	{
        	cout << "Left Parenthesis Symbol ('(')" << endl;
    	}
	else if (t.id == TOKEN_RPAREN) 
	{
        	cout << "Right Parenthesis Symbol (')')" << endl;
    	} 
	else if (t.id == TOKEN_LBRACE) 
	{
        	cout << "Left Brace Symbol ('{')" << endl;
    	} 
	else if (t.id == TOKEN_RBRACE) 
	{
        	cout << "Right Brace Symbol ('}')" << endl;
    	} 
	else if (t.id == TOKEN_LBRACKET) 
	{
        	cout << "Left Bracket Symbol ('[')" << endl;
    	} 
	else if (t.id == TOKEN_RBRACKET) 
	{
        	cout << "Right Bracket Symbol (']')" << endl;
    	} 
	else if (t.id == TOKEN_AND) 
	{
        	cout << "And Symbol (&)" << endl;
    	} 
	else if (t.id == TOKEN_OR) 
	{
        	cout << "Or Symbol ('|')" << endl;
    	} 
	else if (t.id == TOKEN_DOT) 
	{
        	cout << "Dot Symbol (.)" << endl;
    	} 
	else if (t.id == TOKEN_AT) 
	{
        	cout << "At Symbol (@)" << endl;
    	} 
	else if (t.id == TOKEN_COLON) 
	{
        	cout << "Colon Symbol (:)" << endl;
    	} 
	else if (t.id == TOKEN_SEMICOLON) 
	{
        	cout << "Semicolon Symbol (;)" << endl;
    	} 
	else if (t.id == TOKEN_COMMA) 
	{
        	cout << "Comma Symbol (,)" << endl;
    	}
	else if (t.id == TOKEN_IDENT) 
	{
        	cout << "Identifier String: " << t.textValue << endl;
    	}
	else if (t.id == TOKEN_INTEGER) 
	{
        	cout << "Integer Number: " << t.integerValue << endl;
    	}
	else if (t.id == TOKEN_REAL) 
	{
        	cout << "Real Number: " << t.floatValue << endl;
    	}
	else if (t.id == TOKEN_STRING) 
	{
        	cout << "Normal String: " << t.textValue << endl;
    	}
	else if (t.id == TOKEN_UNKNOWN) 
	{
        	cout << "Unknown Symbol: (" << t.textValue << ")" <<endl;
    	}

	else
	{
        	cout << "Unknown Token" << endl;
    	}
	


}
