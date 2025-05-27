#ifndef TREE_H
#define TREE_H

#include "token.h"
#include <memory>
#include <string>

// ---------- Tree Node Kinds ----------
enum NodeType {
    NODE_EXPR,
    NODE_STATEMENT_BLOCK,
    NODE_PRINT,
    NODE_READ,
    NODE_DECLARATION,
    NODE_ASSIGN
};

// ---------- Value Types ----------
enum ValueType {
    VALUE_NULL,
    VALUE_INT,
    VALUE_STRING
};

// ---------- Tree Node Structure ----------
struct TreeNode {
    int type = TOKEN_NULL;
    NodeType nodeKind = NODE_EXPR;
    ValueType valueType = VALUE_NULL;

    int value = 0;                         // For integers or offset into stringTable/variableTable
    std::string stringValue;              // Only used for string literals
    std::string textValue;                // Optional text (e.g., variable name)
    bool isUnary = false;

    std::shared_ptr<TreeNode> child;
    std::shared_ptr<TreeNode> sibling;

    TreeNode(int t) : type(t) {}
};

// ---------- External Tables ----------
extern char stringTable[10000];
extern int stringTableOffset;

extern char variableTable[4000];
extern int variableTableOffset;

// ---------- Tree Function Prototypes ----------
void delete_tree(std::shared_ptr<TreeNode>& node);
void print_tree(const std::shared_ptr<TreeNode>& node, int depth = 0);
void parseE(std::shared_ptr<TreeNode>& c, Token& curToken);
void parseStatementSequence(std::shared_ptr<TreeNode>& root, Token& curToken);

// ---------- Variable Table Access ----------
int lookup_variable(const std::string& name);
int insert_variable(const std::string& name);

#endif


