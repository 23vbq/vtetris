#ifndef BLOCK_H
#define BLOCK_H

#include <vector>
#include "vector2.h"

class Block{
    std::vector<Vector2> childs;
    int* maxOffsets; // Left, Down, Right, Up

public:
    Block();
    Block(std::vector<int>);
    std::vector<Vector2> GetChilds();
    int* GetMaxOffsets();
    void CalculateMaxOffsets();
};

#endif