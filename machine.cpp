#include "machine.h"
#include "tree.h"
#include <iostream>
#include <sys/mman.h>
#include <unistd.h>

using namespace std;

extern "C" void print_int(int v)
{
        cout << v;
}

extern "C" void print_string(const char* s)
{
        cout << s;
}

extern "C" int read_int()
{
    int x;
    cin >> x;
    return x;
}

extern char stringTable[10000];
extern char variableTable[4000];

static int offset = 0;

void emit_node(unsigned char* prog, const std::shared_ptr<TreeNode>& node)
{
    if (!node)
    {
            return;
    }

    if (node->nodeKind == NODE_STATEMENT_BLOCK)
    {
        for (auto cur = node->child; cur; cur = cur->sibling)
        {
            emit_node(prog, cur);
        }
    }
    else if (node->nodeKind == NODE_DECLARATION)
    {
        prog[offset++] = 0xb8; //MOV EAX, 0
        prog[offset++] = 0x00;
        prog[offset++] = 0x00;
        prog[offset++] = 0x00;
        prog[offset++] = 0x00;

        prog[offset++] = 0x48; //MOV RDI, var_address
        prog[offset++] = 0xbf;

        const char* address = variableTable + node->value;
        long addr = (long)address;

        for (int i = 0; i < 8; ++i)
        {
                prog[offset++] = addr & 0xff;
                addr >>= 8;
        }

        prog[offset++] = 0x89; //MOV [RDI], EAX
        prog[offset++] = 0x07;
    }
    else if (node->nodeKind == NODE_READ)
    {
        prog[offset++] = 0x48; //MOV RAX, read_int
        prog[offset++] = 0xb8;

        long fn = (long)read_int;
        for (int i = 0; i < 8; ++i)
        {
                prog[offset++] = fn & 0xff;
                fn >>= 8;
        }

        prog[offset++] = 0xff; //CALL RAX
        prog[offset++] = 0xd0;

        prog[offset++] = 0x48; //MOV RDI, var_address
        prog[offset++] = 0xbf;
        const char* address = variableTable + node->value;
        long addr = (long)address;
        for (int i = 0; i < 8; ++i)
        {
                prog[offset++] = addr & 0xff;
                addr >>= 8;
        }

        prog[offset++] = 0x89; //MOV [RDI], EAX
        prog[offset++] = 0x07;
    }
    else if (node->nodeKind == NODE_ASSIGN)
    {
        emit_node(prog, node->child->sibling); //Eval RHS into EAX

        prog[offset++] = 0x48; //MOV RDI, var_address
        prog[offset++] = 0xbf;
        const char* address = variableTable + node->child->value;
        long addr = (long)address;
        for (int i = 0; i < 8; ++i)
        {
                prog[offset++] = addr & 0xff;
                addr >>= 8;
        }

        prog[offset++] = 0x89; //MOV [RDI], EAX
        prog[offset++] = 0x07;
    }
    else if (node->nodeKind == NODE_PRINT)
    {
        auto cur = node->child;
        while (cur)
        {
            emit_node(prog, cur);

            if (cur->valueType == VALUE_STRING)
            {
                const char* addr = stringTable + cur->value;
                long strPtr = (long)addr;
                long fnPtr = (long)print_string;

                prog[offset++] = 0x48; //MOV RDI, strPtr
                prog[offset++] = 0xbf;
                for (int i = 0; i < 8; ++i)
                {
                        prog[offset++] = strPtr & 0xff;
                        strPtr >>= 8;
                }

                prog[offset++] = 0x48; //MOV RSI, print_string
                prog[offset++] = 0xbe;
                for (int i = 0; i < 8; ++i)
                {
                        prog[offset++] = fnPtr & 0xff;
                        fnPtr >>= 8;
                }

                prog[offset++] = 0xff; //CALL RSI
                prog[offset++] = 0xd6;

            }
            else if (cur->valueType == VALUE_INT)
            {
                prog[offset++] = 0x89; //MOV EDI, EAX
                prog[offset++] = 0xc7;
                long fn = (long)print_int;

                prog[offset++] = 0x48; //MOV RSI, print_int
                prog[offset++] = 0xbe;
                for (int i = 0; i < 8; ++i)
                {
                        prog[offset++] = fn & 0xff;
                        fn >>= 8;
                }

                prog[offset++] = 0xff; //CALL RSI
                prog[offset++] = 0xd6;
            }

            cur = cur->sibling;
        }
    }
    else if (node->type == TOKEN_INTEGER)
    {
        prog[offset++] = 0xb8; //MOV EAX, imm32
        prog[offset++] = node->value & 0xff;
        prog[offset++] = (node->value >> 8) & 0xff;
        prog[offset++] = (node->value >> 16) & 0xff;
        prog[offset++] = (node->value >> 24) & 0xff;
    }
    else if (node->type == TOKEN_STRING)
    {
        return;
    }
    else if (node->type == TOKEN_IDENT && node->value >= 0)
    {
        prog[offset++] = 0x48; //MOV RDI, var_address
        prog[offset++] = 0xbf;
        const char* address = variableTable + node->value;
        long addr = (long)address;
        for (int i = 0; i < 8; ++i)
        {
                prog[offset++] = addr & 0xff;
                addr >>= 8;
        }

        prog[offset++] = 0x8b;
        prog[offset++] = 0x07;
    }
    else if (node->type == TOKEN_EXP)
    {
        emit_node(prog, node->child);
        prog[offset++] = 0x50; //PUSH EAX

        emit_node(prog, node->child->sibling);

        prog[offset++] = 0x41; //MOV R10D, EAX
        prog[offset++] = 0x89;
        prog[offset++] = 0xc2;
        prog[offset++] = 0x41; //POP R9D
        prog[offset++] = 0x8f;
        prog[offset++] = 0xc1;
        prog[offset++] = 0x45; //XOR R8D, R8D
        prog[offset++] = 0x31;
        prog[offset++] = 0xc0;
        prog[offset++] = 0x45; //TEST R10D, R10D
        prog[offset++] = 0x85;
        prog[offset++] = 0xd2;
        prog[offset++] = 0x7c; //JL
        prog[offset++] = 0x1e;
        prog[offset++] = 0x41; //INC R8D
        prog[offset++] = 0xff;
        prog[offset++] = 0xc0;
        prog[offset++] = 0x45; //TEST R10D, R10D
        prog[offset++] = 0x85;
        prog[offset++] = 0xd2;
        prog[offset++] = 0x74; //JE END
        prog[offset++] = 0x16;
        prog[offset++] = 0x41; //TEST R10D, 0x01
        prog[offset++] = 0xf7;
        prog[offset++] = 0xc2;
        prog[offset++] = 0x01;
        prog[offset++] = 0x00;
        prog[offset++] = 0x00;
        prog[offset++] = 0x00;
        prog[offset++] = 0x74; //JE SKIP
        prog[offset++] = 0x04;
        prog[offset++] = 0x45; //IMUL R8D, R9D
        prog[offset++] = 0x0f;
        prog[offset++] = 0xaf;
        prog[offset++] = 0xc1;
        prog[offset++] = 0x45; //IMUL R9D, R9D
        prog[offset++] = 0x0f;
        prog[offset++] = 0xaf;
        prog[offset++] = 0xc9;
        prog[offset++] = 0x41; //SAR R10D, 1
        prog[offset++] = 0xd1;
        prog[offset++] = 0xfa;
        prog[offset++] = 0xeb; //JMP -27
        prog[offset++] = 0xe5;
        prog[offset++] = 0x44; //MOV EAX, R8D
        prog[offset++] = 0x89;
        prog[offset++] = 0xc0;

    }
    else
    {
        emit_node(prog, node->child);
        prog[offset++] = 0x50; //PUSH EAX

        emit_node(prog, node->child->sibling);
        prog[offset++] = 0x89; //MOV ECX, EAX
        prog[offset++] = 0xc1;
        prog[offset++] = 0x58; //POP EAX


    	if ((node->type == TOKEN_PLUS || node->type == TOKEN_MINUS) && node->isUnary)
	{
    		emit_node(prog, node->child); // value goes into EAX

    		if (node->type == TOKEN_MINUS)
    		{
        		// NEG EAX
        		prog[offset++] = 0xf7;
        		prog[offset++] = 0xd8;
    		}
    	// else for unary plus, do nothing — EAX already holds the value
	}
	else if (node->type == TOKEN_PLUS)
        {
            prog[offset++] = 0x01; //ADD EAX, ECX
            prog[offset++] = 0xc8;
        }
        else if (node->type == TOKEN_MINUS)
        {
            prog[offset++] = 0x29; //SUB EAX, ECX
            prog[offset++] = 0xc8;
        }
        else if (node->type == TOKEN_MULT)
        {
            prog[offset++] = 0x0f; //IMUL EAX, ECX
            prog[offset++] = 0xaf;
            prog[offset++] = 0xc1;
        }
        else if (node->type == TOKEN_DIV)
        {
            prog[offset++] = 0x99; //CDQ
            prog[offset++] = 0xf7; //IDIV ECX
            prog[offset++] = 0xf9;
        }
        else if (node->type == TOKEN_MOD)
        {
            prog[offset++] = 0x99; //CDQ
            prog[offset++] = 0xf7; //IDIV ECX
            prog[offset++] = 0xf9;
            prog[offset++] = 0x89; //MOV EAX, EDX
            prog[offset++] = 0xd0;
        }
    }
}

int machine_generate(unsigned char* prog, const std::shared_ptr<TreeNode>& root, int&)
{
    offset = 0;
    emit_node(prog, root);
    prog[offset++] = 0xc3; //RET

    cout << "Code size: " << offset << " bytes.\n";
    cout << "Code execution:\n";
    ((void(*)())prog)();
    cout << endl;

    return offset;
}


