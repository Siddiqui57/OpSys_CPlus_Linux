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
#include "lex.h"
#include "buffio.h"

using namespace std;

static char current_char;

Error lex_init(const char *src_code) 
{
	//use buffer initialization function
	if (buffer_init(src_code) != 0) 
	{
        	return {NCC_FILE_NOT_FOUND, 0, 0};
    	}

    	return {NCC_OK, 0, 0};

}

Error get_token(Token &t) 
{
	while (!buffer_eof()) 
	{
		//no content in file
        	if (buffer_get_cur_char(current_char) != 0) 
		{
			t.id = TOKEN_EOF;
            		t.line = src_line_no;
            		t.col = src_col_no;
            		return {NCC_EOF, src_line_no, src_col_no};
        	}

		//set token values for line and column
        	t.line = src_line_no;
        	t.col = src_col_no;

		//Token definitions
        	if (current_char == '+') 
		{
            		t.id = TOKEN_PLUS;
            		buffer_next_char();
            		return {NCC_OK, src_line_no, src_col_no};
		} 
		else if (current_char == '-') 
		{
            		t.id = TOKEN_MINUS;
            		buffer_next_char();
            		return {NCC_OK, src_line_no, src_col_no};
		} 
		else if (current_char == '*') 
		{
            		t.id = TOKEN_MULT;
            		buffer_next_char();
            		return {NCC_OK, src_line_no, src_col_no};
		} 
		else if (current_char == '/') 
		{
            		t.id = TOKEN_DIV;
            		buffer_next_char();
            		return {NCC_OK, src_line_no, src_col_no};
		} 
		else if (current_char == '^') 
		{
            		t.id = TOKEN_EXP;
            		buffer_next_char();
            		return {NCC_OK, src_line_no, src_col_no};
		} 
		else if (current_char == '<') 
		{

			//first increment to see char after '<'
			buffer_get_next_char(current_char);

			if (current_char == '=')
			{
				t.id = TOKEN_LESS_EQ;
				buffer_next_char();
				return {NCC_OK, src_line_no, src_col_no};
			}
			else if (current_char == '-') // for assign operator
			{
				t.id = TOKEN_ASSIGN;
				buffer_next_char();
				return {NCC_OK, src_line_no, src_col_no};
			}
			else if (current_char == '<') //for block comments
			{

					
				buffer_get_next_char(current_char); //check for -

				if (current_char != '-')
				{
					return {NCC_BLOCK, src_line_no, src_col_no};
				}
				else
				{
					buffer_get_next_char(current_char);

					while(current_char != '-')
					{

						if(current_char == '\0' || current_char == EOF)
						{
							return {NCC_BLOCK, src_line_no, src_col_no};
						}


						buffer_get_next_char(current_char); 
					}



					if (current_char == '\0' || current_char == EOF)
					{
						return {NCC_BLOCK, src_line_no, src_col_no};
					}
					else
					{
						buffer_get_next_char(current_char);

						if (current_char == '>')
						{
							buffer_get_next_char(current_char);

							if (current_char == '>')
							{
								buffer_next_char();
							}
							else
							{
								return {NCC_BLOCK, src_line_no, src_col_no};
							}
						}
						else
						{
							return {NCC_BLOCK, src_line_no, src_col_no};
						}
					}


				}

			}
			
			else
			{
				//move back a character if just '<' token
				//could optimize, but makes code clearer
				buffer_back_char();

            			t.id = TOKEN_LESS;
            			buffer_next_char();
            			return {NCC_OK, src_line_no, src_col_no};
			}


		} 
		else if (current_char == '>') 
		{
			//first increment to see char after '>'
			buffer_get_next_char(current_char);

			if (current_char == '=')
			{
				t.id = TOKEN_GREATER_EQ;
				buffer_next_char();
				return {NCC_OK, src_line_no, src_col_no};
			}
			else
			{
				//move back a character if just '>' token
				buffer_back_char();

            			t.id = TOKEN_GREATER;
            			buffer_next_char();
            			return {NCC_OK, src_line_no, src_col_no};
			}

		}
		else if (current_char == '=') 
		{
            		t.id = TOKEN_EQUAL;
            		buffer_next_char();
            		return {NCC_OK, src_line_no, src_col_no};
		} 
		else if (current_char == '~') 
		{
			//check if next char is '='
			buffer_get_next_char(current_char);

			if (current_char == '=')
			{
				t.id = TOKEN_NOT_EQUAL;
				buffer_next_char();
				return {NCC_OK, src_line_no, src_col_no};

			}
			else
			{

				//Unknown Token
            			t.id = TOKEN_UNKNOWN;
				t.textValue = '~';
            			buffer_next_char();
            			return {NCC_OK, src_line_no, src_col_no};

			}
					

		} 
		else if (current_char == '!') 
		{
            		t.id = TOKEN_NOT;
            		buffer_next_char();
            		return {NCC_OK, src_line_no, src_col_no};
		} 
		else if (current_char == '(') 
		{
            		t.id = TOKEN_LPAREN;
            		buffer_next_char();
            		return {NCC_OK, src_line_no, src_col_no};
		} 
		else if (current_char == ')') 
		{
            		t.id = TOKEN_RPAREN;
            		buffer_next_char();
            		return {NCC_OK, src_line_no, src_col_no};
		} 
		else if (current_char == '{') 
		{
            		t.id = TOKEN_LBRACE;
            		buffer_next_char();
            		return {NCC_OK, src_line_no, src_col_no};
		} 
		else if (current_char == '}') 
		{
            		t.id = TOKEN_RBRACE;
            		buffer_next_char();
            		return {NCC_OK, src_line_no, src_col_no};
		} 
		else if (current_char == '[') 
		{
            		t.id = TOKEN_LBRACKET;
            		buffer_next_char();
            		return {NCC_OK, src_line_no, src_col_no};
		} 
		else if (current_char == ']') 
		{
            		t.id = TOKEN_RBRACKET;
            		buffer_next_char();
            		return {NCC_OK, src_line_no, src_col_no};
		} 
		else if (current_char == '&') 
		{
            		t.id = TOKEN_AND;
            		buffer_next_char();
            		return {NCC_OK, src_line_no, src_col_no};
		} 
		else if (current_char == '|') 
		{
            		t.id = TOKEN_OR;
            		buffer_next_char();
            		return {NCC_OK, src_line_no, src_col_no};
		} 
		else if (current_char == '.') 
		{

			t.textValue = "";
			t.textValue += current_char; //stored '.' for if it is a real number

			buffer_get_next_char(current_char);

			if (current_char >= '0' && current_char <= '9')
			{
				t.id = TOKEN_REAL;

				while ((current_char >= '0' && current_char <= '9') || (current_char == 'E') || (current_char == 'e') || (current_char == '+') || (current_char == '-'))
				{
					t.textValue += current_char;

					if (current_char == '+' || current_char == '-')
					{
						buffer_get_next_char(current_char);

						//if after + or -
						//the character is not a number
						//return an error
						if (!(current_char >= '0' && current_char <= '9'))
						{
							//improper val after sign
							return {NCC_FLOAT, src_line_no, src_col_no};
						}

						//go back a character, to original location
						buffer_back_char();

						//check value before sign, should be E, or it is error
						buffer_back_char();
						buffer_get_cur_char(current_char);

						if (current_char != 'e' && current_char != 'E')
						{
							return {NCC_FLOAT, src_line_no, src_col_no};

						}

						//move back to original position
						buffer_get_next_char(current_char);


					}


					if (current_char == 'e' || current_char == 'E')
					{
						buffer_get_next_char(current_char);

						//if after E
						//the character is not a number or sign
						//return an error
						if (!(current_char >= '0' && current_char <= '9') && (current_char != '+') && (current_char != '-'))
						{
							//improper val after e
							return {NCC_FLOAT, src_line_no, src_col_no};

						}
						

						//go back a character, to keep loop simple
						buffer_back_char();

						//check value before e, should be number, or it is error
						buffer_back_char();
						buffer_get_cur_char(current_char);

						if (!(current_char >= '0' && current_char <= '9'))
						{
							return {NCC_FLOAT, src_line_no, src_col_no};

						}

						//move back to original position
						buffer_get_next_char(current_char);

					}
				



					buffer_get_next_char(current_char);


					
				}

				t.floatValue = stof(t.textValue);
				return {NCC_OK, src_line_no, src_col_no};

			}
		        else
			{
				//not a real number
				buffer_back_char();
			}


            		t.id = TOKEN_DOT;
            		buffer_next_char();
            		return {NCC_OK, src_line_no, src_col_no};
		} 
		else if (current_char == '@') 
		{
            		t.id = TOKEN_AT;
            		buffer_next_char();
            		return {NCC_OK, src_line_no, src_col_no};
		} 
		else if (current_char == ':') 
		{
            		t.id = TOKEN_COLON;
            		buffer_next_char();
            		return {NCC_OK, src_line_no, src_col_no};
		}
		else if (current_char == ';') 
		{
            		t.id = TOKEN_SEMICOLON;
            		buffer_next_char();
            		return {NCC_OK, src_line_no, src_col_no};
		}
		else if (current_char == ',') 
		{
            		t.id = TOKEN_COMMA;
            		buffer_next_char();
            		return {NCC_OK, src_line_no, src_col_no};
		}
		//test all possible starts for identifiers
		else if ((current_char >= 'a' && current_char <= 'z') || (current_char >= 'A' && current_char <= 'Z') || (current_char == '_'))
		{
			//set ID
			t.id = TOKEN_IDENT;

			//reset string
			t.textValue = "";

			//after start, can include numbers
			while((current_char >= 'a' && current_char <= 'z') || (current_char >= 'A' && current_char <= 'Z') || (current_char == '_') || (current_char >= '0' && current_char <= '9'))
			{
				t.textValue += current_char;
				buffer_get_next_char(current_char);
			}

			if (t.textValue == "mod")
			{
				t.id = TOKEN_MOD;
			}

			//can skip buffer_next_char b/c already at ' '
			return {NCC_OK, src_line_no, src_col_no};
		}
		else if (current_char >= '0' && current_char <= '9') 
		{

			//reset string
			t.textValue = "";

			//Error test for integer starting with zero
			if (current_char == '0')
			{
				//save the zero in string
				t.textValue += current_char;

				buffer_get_next_char(current_char);

				if (current_char == ' ' || current_char == '\n')
				{
					t.id = TOKEN_INTEGER;
					t.integerValue = 0;
					return {NCC_OK, src_line_no, src_col_no};

				}
				else if (current_char == '.') //is real number
				{
					//set ID to real
					t.id = TOKEN_REAL;



					//add decimal point to string
					t.textValue += current_char;

					buffer_get_next_char(current_char);

					
					//test if real number ends here
					if (current_char == ' ' || current_char == '\n')
					{
						t.floatValue = stof(t.textValue);
						return {NCC_OK, src_line_no, src_col_no};
						
					}





					//if after decimal point
					//the character is not a number
					//return an error
					if (!(current_char >= '0' && current_char <= '9'))
					{
						//improper decimal
						return {NCC_FLOAT, src_line_no, src_col_no};


					}


					while ((current_char >= '0' && current_char <= '9') || (current_char == 'E') || (current_char == 'e') || (current_char == '+') || (current_char == '-'))
					{
						t.textValue += current_char;

						if (current_char == '+' || current_char == '-')
						{
							buffer_get_next_char(current_char);

							//if after + or -
							//the character is not a number
							//return an error
							if (!(current_char >= '0' && current_char <= '9'))
							{
								//improper val after sign
								return {NCC_FLOAT, src_line_no, src_col_no};

							}

							//go back a character, to original location
							buffer_back_char();

							//check value before sign, should be E, or it is error
							buffer_back_char();
							buffer_get_cur_char(current_char);

							if (current_char != 'e' && current_char != 'E')
							{
								return {NCC_FLOAT, src_line_no, src_col_no};

							}

							//move back to original position
							buffer_get_next_char(current_char);


						}


						if (current_char == 'e' || current_char == 'E')
						{
							buffer_get_next_char(current_char);

							//if after E
							//the character is not a number or sign
							//return an error
							if (!(current_char >= '0' && current_char <= '9') && (current_char != '+') && (current_char != '-'))
							{
								//improper val after e
								return {NCC_FLOAT, src_line_no, src_col_no};

							}
						

							//go back a character, to keep loop simple
							buffer_back_char();

							//check value before e, should be number, or it is error
							buffer_back_char();
							buffer_get_cur_char(current_char);

							if (!(current_char >= '0' && current_char <= '9'))
							{
								return {NCC_FLOAT, src_line_no, src_col_no};

							}

							//move back to original position
							buffer_get_next_char(current_char);




						}



						buffer_get_next_char(current_char);


					}


					t.floatValue = stof(t.textValue);
					return {NCC_OK, src_line_no, src_col_no};


				}
			        else //means int starts with zero
				{
					t.id = TOKEN_INTEGER;
					t.integerValue = 0;
					return {NCC_OK, src_line_no, src_col_no};

			        }


			}


			while((current_char >= '0' && current_char <= '9') || (current_char == '.'))
			{
				t.textValue += current_char;

				if (current_char == '.') //is a real number
				{

					t.id = TOKEN_REAL;


					buffer_get_next_char(current_char);

					
					//test if real number ends here
					if (current_char == ' ' || current_char == '\n')
					{
						t.floatValue = stof(t.textValue);
						return {NCC_OK, src_line_no, src_col_no};
						
					}





					//if after decimal point
					//the character is not a number
					//return an error
					if (!(current_char >= '0' && current_char <= '9'))
					{
						//improper decimal
						return {NCC_FLOAT, src_line_no, src_col_no};

					}


					while ((current_char >= '0' && current_char <= '9') || (current_char == 'E') || (current_char == 'e') || (current_char == '+') || (current_char == '-'))
					{
						t.textValue += current_char;

						if (current_char == '+' || current_char == '-')
						{
							buffer_get_next_char(current_char);

							//if after + or -
							//the character is not a number
							//return an error
							if (!(current_char >= '0' && current_char <= '9'))
							{
								//improper val after sign
								return {NCC_FLOAT, src_line_no, src_col_no};
							}

							//go back a character, to original location
							buffer_back_char();

							//check value before sign, should be E, or it is error
							buffer_back_char();
							buffer_get_cur_char(current_char);

							if (current_char != 'e' && current_char != 'E')
							{
								return {NCC_FLOAT, src_line_no, src_col_no};

							}

							//move back to original position
							buffer_get_next_char(current_char);


						}


						if (current_char == 'e' || current_char == 'E')
						{
							buffer_get_next_char(current_char);

							//if after E
							//the character is not a number or sign
							//return an error
							if (!(current_char >= '0' && current_char <= '9') && (current_char != '+') && (current_char != '-'))
							{
								//improper val after e
								return {NCC_FLOAT, src_line_no, src_col_no};

							}
						

							//go back a character, to keep loop simple
							buffer_back_char();

							//check value before e, should be number, or it is error
							buffer_back_char();
							buffer_get_cur_char(current_char);

							if (!(current_char >= '0' && current_char <= '9'))
							{
								return {NCC_FLOAT, src_line_no, src_col_no};

							}

							//move back to original position
							buffer_get_next_char(current_char);

						}
				



						buffer_get_next_char(current_char);


					}


					t.floatValue = stof(t.textValue);
					return {NCC_OK, src_line_no, src_col_no};
				}




				//increment for next loop
				buffer_get_next_char(current_char);

			}

			//passes all checks, just an integer number
			t.id = TOKEN_INTEGER;
			t.integerValue = stoll(t.textValue);
			return {NCC_OK, src_line_no, src_col_no};


		}
		else if (current_char == '"') 
		{
            		t.id = TOKEN_STRING;

			t.textValue = "";
	

			buffer_get_next_char(current_char);

			while (current_char != '"') //loop until end quote
			{

				if (current_char == '\\') //escape sequences
				{
					buffer_get_next_char(current_char);

					if (current_char == '"')
					{
						t.textValue += current_char;
						//skip over so loop doesn't end early
						buffer_get_next_char(current_char);

					}
					else if (current_char == 'n')
					{
						t.textValue += '\n';
						buffer_get_next_char(current_char);

					}
					else if (current_char == 't')
					{
						t.textValue += '\t';
						buffer_get_next_char(current_char);

					}
					else if (current_char == 'r')
					{
						t.textValue += '\r';
						buffer_get_next_char(current_char);

					}
					else if (current_char == '\\')
					{
						t.textValue += '\\';

						//accounts for \n after '\\'
						buffer_get_next_char(current_char);
						

					}
					else if (current_char == 'a')
					{
						t.textValue += '\a';
						buffer_get_next_char(current_char);

					}
					else if (current_char == 'b')
					{
						t.textValue += '\b';
						buffer_get_next_char(current_char);

					}
					else if (current_char == 'u')
					{
						//reset unicode value
						t.unicodeValue = 0;
						
						
						buffer_get_next_char(current_char);

						for (int count = 0; count < 6; count++)
						{
							

							if ((current_char >= '0' && current_char <= '9') || (current_char >= 'a' && current_char <= 'f') || (current_char >= 'A' && current_char <= 'F'))
							{

								//convert UTF to unicode value
								if (current_char >= '0' && current_char <= '9')
								{
									t.unicodeValue = t.unicodeValue * 16 + (current_char - '0');
								}
								else if (current_char >= 'a' && current_char <= 'f')
								{
									t.unicodeValue = t.unicodeValue * 16 + (current_char - 'a' + 10);

								}
								else if (current_char >= 'A' && current_char <= 'F')
								{
									t.unicodeValue = t.unicodeValue * 16 + (current_char - 'A' + 10);

								}

								buffer_get_next_char(current_char);


							}
							else
							{
								//improper UTF Format
								return {NCC_UNICODE, src_line_no, src_col_no};
							}

						}




						//convert unicode value to text
						if (t.unicodeValue >= 0x0 && t.unicodeValue <= 0x7F)
						{

							t.textValue.push_back(t.unicodeValue); 

						}
						else if (t.unicodeValue > 0x7F && t.unicodeValue <= 0x7FF)
						{

							t.textValue.push_back(0xC0 | (t.unicodeValue >> 6));


							t.textValue.push_back(0x80 | (t.unicodeValue & 0x3F));	

						}
						else if (t.unicodeValue > 0x7FF && t.unicodeValue <= 0xFFFF)
						{

							t.textValue.push_back(0xE0 | (t.unicodeValue >> 12));


							t.textValue.push_back(0x80 | ((t.unicodeValue >> 6) & 0x3F));


							t.textValue.push_back(0x80 | (t.unicodeValue & 0x3F));		

						}
						else if (t.unicodeValue > 0x0FFFF && t.unicodeValue <= 0x10FFFF)
						{

							t.textValue.push_back(0xF0 | (t.unicodeValue >> 18));

							
							t.textValue.push_back(0x80 | ((t.unicodeValue >> 12) & 0x3F));

							
							t.textValue.push_back(0x80 | ((t.unicodeValue >> 6) & 0x3F));

							
							t.textValue.push_back(0x80 | (t.unicodeValue & 0x3F));		

						}
						else
						{
							//outside of range
							return {NCC_UNICODE, src_line_no, src_col_no};
						}
						
						
					}
					else
					{
						buffer_get_next_char(current_char);
					}



				}
				else
				{
					//not escape sequence
					t.textValue += current_char;
					buffer_get_next_char(current_char);

				}


			}

			//increment past end quote
			buffer_next_char();

			return {NCC_OK, src_line_no, src_col_no};

            				
		}
		else if (current_char == '#') 
		{

			while(current_char != '\n')
			{
				//increment to end of line
				buffer_get_next_char(current_char);
			}

            		buffer_next_char();

		}
		else if (current_char == ' ') //skip white space 
		{
            		buffer_next_char();
		}
		else if (current_char == '\n')
		{
            		buffer_next_char();
		}
		else if (current_char == '\0') 
		{
            		t.id = TOKEN_EOF;
            		return {NCC_EOF, src_line_no, src_col_no};
		}
	       	else 
		{
			t.id = TOKEN_NULL;
			buffer_next_char();
			return {NCC_OK, src_line_no, src_col_no};
            		buffer_next_char();
        	}
    	}

	//catch case
    	t.id = TOKEN_EOF;
    	t.line = src_line_no;
    	t.col = src_col_no;
    	return {NCC_EOF, src_line_no, src_col_no};
}

bool lex_eof() 
{
	return buffer_eof();
}

void lex_cleanup() 
{
	buffer_cleanup();
}

