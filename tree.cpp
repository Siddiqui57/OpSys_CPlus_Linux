#include "tree.h"
#include "lex.h"
#include "error.h"
#include "buffio.h"
#include <iostream>

using namespace std;

// ---------- String Table ----------
char stringTable[10000];
int stringTableOffset = 0;

int addStringToTable(const std::string& str)
{
    int start = stringTableOffset;
    for (char ch : str) stringTable[stringTableOffset++] = ch;
    stringTable[stringTableOffset++] = '\0';
    return start;
}

// ---------- Variable Table ----------
char variableTable[4000];
int variableTableOffset = 0;

int lookup_variable(const string& name)
{
    for (int i = 0; i < variableTableOffset;) {
        string existing(&variableTable[i]);
        if (existing == name) return i;
        i += existing.length() + 1;
    }
    return -1;
}

int insert_variable(const string& name)
{
    int offset = lookup_variable(name);
    if (offset != -1) return offset;
    int start = variableTableOffset;
    for (char ch : name) variableTable[variableTableOffset++] = ch;
    variableTable[variableTableOffset++] = '\0';
    return start;
}

// ---------- Tree Functions ----------
void delete_tree(shared_ptr<TreeNode>& node)
{
    if (!node) return;
    delete_tree(node->child);
    delete_tree(node->sibling);
    node.reset();
}

void print_tree(const shared_ptr<TreeNode>& node, int depth)
{
    if (!node) {
        cout << "Code tree:\n\n";
        return;
    }

    if (depth == 0)
        cout << "Code tree:\n";

    for (int i = 0; i < depth; ++i) cout << "  ";

    if (node->nodeKind == NODE_STATEMENT_BLOCK) cout << "statement block\n";
    else if (node->nodeKind == NODE_PRINT) cout << "print\n";
    else if (node->nodeKind == NODE_READ) cout << "read\n";
    else if (node->nodeKind == NODE_ASSIGN) cout << "<-\n";
    else if (node->nodeKind == NODE_DECLARATION) cout << "variable: " << &variableTable[node->value] << "\n";
    else if (node->type == TOKEN_INTEGER) cout << node->value << "\n";
    else if (node->type == TOKEN_STRING) cout << node->stringValue << "\n";
    else if (node->type == TOKEN_IDENT && node->value >= 0) cout << "variable: " << &variableTable[node->value] << "\n";
    else if (node->type == TOKEN_PLUS && node->isUnary) cout << "+ (pos)\n";
    else if (node->type == TOKEN_MINUS && node->isUnary) cout << "- (neg)\n";
    else if (node->type == TOKEN_PLUS) cout << "+ (add)\n";
    else if (node->type == TOKEN_MINUS) cout << "- (sub)\n";
    else if (node->type == TOKEN_MULT) cout << "* (mult)\n";
    else if (node->type == TOKEN_DIV) cout << "/ (div)\n";
    else if (node->type == TOKEN_EXP) cout << "^ (pow)\n";
    else if (node->type == TOKEN_MOD) cout << "mod\n";
    else cout << "?\n";

    if (node->child) print_tree(node->child, depth + 1);
    if (node->sibling) print_tree(node->sibling, depth);

    if (depth == 0) cout << "\n";
}

// ---------- Expression Parser ----------
void parseE(shared_ptr<TreeNode>& c, Token& curToken);
void parseT(shared_ptr<TreeNode>& c, Token& curToken);
void parseP(shared_ptr<TreeNode>& c, Token& curToken);
void parseU(shared_ptr<TreeNode>& c, Token& curToken);
void parseV(shared_ptr<TreeNode>& c, Token& curToken);

void parseE(shared_ptr<TreeNode>& c, Token& curToken)
{
    parseT(c, curToken);
    while (curToken.id == TOKEN_PLUS || curToken.id == TOKEN_MINUS)
    {
        int opType = curToken.id;
        get_token(curToken);
        shared_ptr<TreeNode> right;
        parseT(right, curToken);
        auto opNode = make_shared<TreeNode>(opType);
        opNode->child = c;
        c->sibling = right;
        opNode->valueType = VALUE_INT;
        c = opNode;
    }
}

void parseT(shared_ptr<TreeNode>& c, Token& curToken)
{
    parseP(c, curToken);
    while (curToken.id == TOKEN_MULT || curToken.id == TOKEN_DIV ||
           (curToken.id == TOKEN_MOD))
    {
        int opType = curToken.id;
        string text = curToken.textValue;
        get_token(curToken);
        shared_ptr<TreeNode> right;
        parseP(right, curToken);
        auto opNode = make_shared<TreeNode>(opType);
        opNode->textValue = text;
        opNode->child = c;
        c->sibling = right;
        opNode->valueType = VALUE_INT;
        c = opNode;
    }
}



void parseP(shared_ptr<TreeNode>& c, Token& curToken)
{
    parseU(c, curToken);
    while (curToken.id == TOKEN_EXP)
    {
        get_token(curToken);
        shared_ptr<TreeNode> right;
        parseP(right, curToken);
        auto opNode = make_shared<TreeNode>(TOKEN_EXP);
        opNode->child = c;
        c->sibling = right;
        opNode->valueType = VALUE_INT;
        c = opNode;
    }
}

void parseU(shared_ptr<TreeNode>& c, Token& curToken)
{
    if (curToken.id == TOKEN_PLUS || curToken.id == TOKEN_MINUS)
    {
        int opType = curToken.id;
        get_token(curToken);
        parseU(c, curToken);
        auto unaryNode = make_shared<TreeNode>(opType);
        unaryNode->isUnary = true;
        unaryNode->child = c;
        unaryNode->valueType = VALUE_INT;
        c = unaryNode;
        return;
    }
    parseV(c, curToken);
}


void parseV(shared_ptr<TreeNode>& c, Token& curToken)
{
    if (curToken.id == TOKEN_INTEGER)
    {
        c = make_shared<TreeNode>(TOKEN_INTEGER);
        c->value = curToken.integerValue;
        c->valueType = VALUE_INT;
        get_token(curToken);
    }
    else if (curToken.id == TOKEN_STRING)
    {
        int offset = addStringToTable(curToken.textValue);
        c = make_shared<TreeNode>(TOKEN_STRING);
        c->value = offset;
        c->valueType = VALUE_STRING;
        c->stringValue = curToken.textValue;
        get_token(curToken);
    }
    else if (curToken.id == TOKEN_LPAREN)
    {
        get_token(curToken);
        parseE(c, curToken);
        if (curToken.id == TOKEN_RPAREN) get_token(curToken);
    }
    else if (curToken.id == TOKEN_IDENT)
    {

        int offset = lookup_variable(curToken.textValue);
        auto varNode = make_shared<TreeNode>(TOKEN_IDENT);
        varNode->textValue = curToken.textValue;
        varNode->value = offset;
        varNode->valueType = VALUE_INT;
        get_token(curToken);
        c = varNode;
    }
    else
    {
        get_token(curToken); // skip invalid token
    }
}


// ---------- Statement Parser ----------
void parseStatementSequence(shared_ptr<TreeNode>& root, Token& curToken)
{
    auto seqNode = make_shared<TreeNode>(TOKEN_NULL);
    seqNode->nodeKind = NODE_STATEMENT_BLOCK;
    shared_ptr<TreeNode>* tail = &seqNode->child;

    while (curToken.id != TOKEN_EOF)
    {

        if (curToken.id == TOKEN_IDENT && curToken.textValue == "print") 
	{
            get_token(curToken);
            auto printNode = make_shared<TreeNode>(TOKEN_NULL);
            printNode->nodeKind = NODE_PRINT;
            bool validPrint = true;

            if (curToken.id != TOKEN_LPAREN) 
	    {
                cerr << "Expected '(' at " << (curToken.line + 1) << ":" << (curToken.col + 1) << endl;
                string lineStr;
                if (get_src_line(curToken.line, lineStr) == 0) 
		{
                    cerr << lineStr << endl;
                    for (int i = 0; i < curToken.col; ++i) cerr << "-";
                    cerr << "^\n";
                }
                validPrint = false;
            } 
	    else 
	    {
                get_token(curToken); 
            }

            if (validPrint)
	    {
                shared_ptr<TreeNode> expr;
                parseE(expr, curToken);
                printNode->child = expr;

                while (curToken.id == TOKEN_COMMA) 
		{
                    get_token(curToken);
                    shared_ptr<TreeNode> nextExpr;
                    parseE(nextExpr, curToken);
                    shared_ptr<TreeNode> last = printNode->child;
                    while (last->sibling) last = last->sibling;
                    last->sibling = nextExpr;
                }

                if (curToken.id == TOKEN_RPAREN) 
		{
                    get_token(curToken);

                } 
		else 
		{
                    validPrint = false;
                }

            }

            while (curToken.id != TOKEN_SEMICOLON && curToken.id != TOKEN_EOF) 
	    {
                cerr << "Unexpected symbol at " << (curToken.line + 1) << ":" << (curToken.col + 1) << endl;
                string lineStr;
                if (get_src_line(curToken.line, lineStr) == 0) 
		{
                    cerr << lineStr << endl;
                    for (int i = 0; i < curToken.col; ++i) cerr << "-";
                    cerr << "^\n";
                }
                get_token(curToken);
            }

            if (curToken.id == TOKEN_SEMICOLON) 
	    {
		    get_token(curToken);
	    }

            if (validPrint) 
	    {
                *tail = printNode;
                tail = &printNode->sibling;
            }

        }
        else if (curToken.id == TOKEN_IDENT && curToken.textValue == "read") 
	{
            get_token(curToken);
            auto readNode = make_shared<TreeNode>(TOKEN_NULL);
            readNode->nodeKind = NODE_READ;

            if (curToken.id == TOKEN_LPAREN) 
	    {
		    get_token(curToken);
	    }

            if (curToken.id == TOKEN_IDENT) 
	    {
                int offset = lookup_variable(curToken.textValue);
                readNode->value = offset;
                get_token(curToken);            
	    }

            if (curToken.id == TOKEN_RPAREN) 
	    {
		    get_token(curToken);
	    }

            if (curToken.id == TOKEN_SEMICOLON) 
	    {
		    get_token(curToken);
	    }

            *tail = readNode;
            tail = &readNode->sibling;
        }
        else if (curToken.id == TOKEN_IDENT && curToken.textValue == "int4") 
	{
            get_token(curToken);

            if (curToken.id != TOKEN_IDENT)
            {
                cerr << "Expected identifier after 'int4'\n";
                continue;
            }

            string varName = curToken.textValue;
            int varLine = curToken.line;
            int varCol  = curToken.col;

            int exist = lookup_variable(varName);
            if (exist != -1)
            {
                // Duplicate symbol error
                cerr << "Duplicate symbol: " << varName
                     << " at " << (varLine + 1) << ":" << (varCol + 1) << endl;
                string lineStr;
                if (get_src_line(varLine, lineStr) == 0)
                {
                    cerr << lineStr << endl;
                    for (int i = 0; i < varCol; ++i) cerr << "-";
                    cerr << "^\n";
                }

                // Unexpected symbol error
                cerr << "Unexpected symbol at " << (varLine + 1) << ":" << (varCol + 2) << endl;
                if (get_src_line(varLine, lineStr) == 0)
                {
                    cerr << lineStr << endl;
                    for (int i = 0; i < varCol + 1; ++i) cerr << "-";
                    cerr << "^\n";
                }

                // Skip until semicolon
                while (curToken.id != TOKEN_SEMICOLON && curToken.id != TOKEN_EOF)
                    get_token(curToken);
                if (curToken.id == TOKEN_SEMICOLON) get_token(curToken);
                continue; // do NOT add declaration
            }

            get_token(curToken);

            if (curToken.id != TOKEN_SEMICOLON)
            {
                cerr << "expected ';' at " << (varLine + 1) << ":" << (varCol + 1 + varName.length()) << endl;
                string lineStr;
                if (get_src_line(varLine, lineStr) == 0)
                {
                    cerr << lineStr << endl;
                    for (int i = 0; i < varCol + (int)varName.length(); ++i)
                        cerr << "-";
                    cerr << "^\n";
                }

                while (curToken.id != TOKEN_SEMICOLON && curToken.id != TOKEN_EOF)
                {
                    if (curToken.id == TOKEN_IDENT)
                    {
                        cerr << "Undefined symbol: " << curToken.textValue
                             << " at " << (curToken.line + 1) << ":" << (curToken.col + 1) << endl;
                        if (get_src_line(curToken.line, lineStr) == 0)
                        {
                            cerr << lineStr << endl;
                            for (int i = 0; i < curToken.col; ++i) cerr << "-";
                            cerr << "^\n";
                        }
                    }
                    else if (curToken.id != TOKEN_COMMA)
                    {
                        cerr << "Unexpected symbol at " << (curToken.line + 1) << ":" << (curToken.col + 1) << endl;
                        if (get_src_line(curToken.line, lineStr) == 0)
                        {
                            cerr << lineStr << endl;
                            for (int i = 0; i < curToken.col; ++i) cerr << "-";
                            cerr << "^\n";
                        }
                    }
                    get_token(curToken);
                }

                if (curToken.id == TOKEN_SEMICOLON)
                    get_token(curToken);
                continue; // do NOT add declaration
            }

            int offset = insert_variable(varName);
            auto declNode = make_shared<TreeNode>(TOKEN_NULL);
            declNode->nodeKind = NODE_DECLARATION;
            declNode->value = offset;
            *tail = declNode;
            tail = &declNode->sibling;

            get_token(curToken);
        
        }
        else if (curToken.id == TOKEN_IDENT) 
	{
            string varName = curToken.textValue;
            int offset = lookup_variable(varName);
	    get_token(curToken);

            if (curToken.id == TOKEN_ASSIGN) 
	    {
                get_token(curToken);
                auto assignNode = make_shared<TreeNode>(TOKEN_NULL);
                assignNode->nodeKind = NODE_ASSIGN;

                auto lhs = make_shared<TreeNode>(TOKEN_IDENT);
                lhs->textValue = varName;
                lhs->value = offset;
                lhs->valueType = VALUE_INT;

                assignNode->child = lhs;

                shared_ptr<TreeNode> rhs;
                parseE(rhs, curToken);
                lhs->sibling = rhs;

                if (curToken.id == TOKEN_SEMICOLON) 
		{
			get_token(curToken);
		}

                *tail = assignNode;
                tail = &assignNode->sibling;
            }
        }
        else 
	{
            cerr << "Unexpected symbol at " << (curToken.line + 1) << ":" << (curToken.col + 1) << endl;
            string lineStr;
            if (get_src_line(curToken.line, lineStr) == 0) 
	    {
                cerr << lineStr << endl;
                for (int i = 0; i < curToken.col; ++i) cerr << "-";
                cerr << "^\n";
            }
            get_token(curToken); // skip the unknown token
        }
    }

    root = seqNode;
}

