#ifndef MACHINE_H
#define MACHINE_H

#include "tree.h"
#include <memory>

int machine_generate(unsigned char* prog, const std::shared_ptr<TreeNode>& root, int& execValueOut);

#endif








