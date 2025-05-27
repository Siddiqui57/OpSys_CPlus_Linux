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
#include <fstream>
#include <sstream>
#include <vector>
#include "buffio.h"

using namespace std;

//used to open and close the file
static ifstream fileStream;

//holds the lines of file
static vector<string> sourceLines;

//line and column number
int src_line_no = 0;
int src_col_no = 0;


int buffer_init(const char * filename)
{
	//open the file in binary
	fileStream.open(filename, ios::binary);

	if (!fileStream.is_open())
	{
		cerr << "File did not open correctly" << endl;
		return -1;
	}

	string line;

	//read the whole file into the buffer
	while (getline(fileStream, line))
	{
		sourceLines.push_back(line);

	}


	//clear the file stream
	fileStream.clear();

	//reset line and column numbers to 0
	src_line_no = 0;
	src_col_no = 0;

	return 0;


}


int buffer_get_cur_char(char & c)
{
	if (src_line_no > (int) sourceLines.size())
	{
		cerr << "Going beyond end of file" << endl;
		return -1;

	}	


	//get line position
	const string & line = sourceLines[src_line_no];

	if (src_col_no >= (int) line.size())
	{
	
		//if end of file, do not do a newline, instead set c to EOF
		if(src_line_no == (int) sourceLines.size())
		{
			c = '\0';
		}
		else
		{
			//if end of line, set c to \n
			c = '\n';
		}
		

	}
	else
	{
		//get character using the column number
		c = line[src_col_no];
	}

	return 0;


}


int buffer_next_char() 
{
	if (src_line_no > (int) sourceLines.size()) 
	{
		cerr << "Going beyond end of file" << endl;
		return -1;
    	}

	//get line position
    	const string & line = sourceLines[src_line_no];

	//if not at the end of the line
    	if (src_col_no <= (int) line.size()) 
	{
		//increment column number
        	src_col_no++;
    	}


    	if (src_col_no > (int) line.size()) 
	{
        	//increment line number
        	src_line_no++;

		//reset column number
        	src_col_no = 0;
   	}


	return 0;
}


int buffer_get_next_char(char & c)
{
	//advance to next position with next character function
	int advance = buffer_next_char();

	if (advance != 0)
	{
		cerr << "Going beyond end of file" << endl;
		return -1;
	}

	//use get current character function after advancing
	return buffer_get_cur_char(c);

}


bool buffer_eof()
{
	if (src_line_no >= (int) sourceLines.size())
	{
		return true;
	}
	else
	{
		return false;
	}



}


int buffer_back_char() 
{
    	if (src_line_no == 0 && src_col_no == 0) 
	{
		cerr << "Already at beginning of buffer" << endl;
        	return -1; 
    	}

    	if (src_col_no > 0) //NOT at beginning of line
	{
        	src_col_no--;
    	} 
	else
	{
		//go back a line
        	src_line_no--;

		//column num = end of previous line
        	src_col_no  = (int) sourceLines[src_line_no].size();
    	}

   	return 0;
}

int buffer_cleanup() 
{
    	if (fileStream.is_open()) 
	{
		//close file
        	fileStream.close();
    	}

	//clears out the vector for lines
    	sourceLines.clear();

	//reset line and column number
    	src_line_no = 0;
    	src_col_no = 0;

    	return 0;
}


int get_src_line(int line_no, string& line) 
{
    	if (line_no < 0 || line_no  > (int) sourceLines.size()) 
	{
		cerr << "invalid line number" << endl;
        	return -1;
    	}

    	line = sourceLines[line_no];

    	return 0; 
}









