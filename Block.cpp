#include "Block.h"

/*
 * Constructors
*/
Block::Block(){
    childs = std::vector<Vector2>();
    maxOffsets = new int[4];
}
Block::Block(std::vector<int> positions){
    // Init
    childs = std::vector<Vector2>();
    maxOffsets = new int[4];
    // Prepare
    int n = positions.size();
    if (n % 2 != 0)
        throw "Invalid length of loaded block.";
    for (int i = 0; i < n; i += 2){
        childs.push_back(Vector2(positions[i], positions[i + 1]));
    }
    CalculateMaxOffsets();
}
std::vector<Vector2> Block::GetChilds(){
    return childs;
}
int* Block::GetMaxOffsets(){
    return maxOffsets;
}
void Block::CalculateMaxOffsets(){
    maxOffsets[0] = 0;
    maxOffsets[1] = 0;
    maxOffsets[2] = 0;
    maxOffsets[3] = 0;
    for(Vector2 &child : childs){
        if (child.x < maxOffsets[0])
            maxOffsets[0] = (int)child.x;
        else if (child.x > maxOffsets[2])
            maxOffsets[2] = (int)child.x;
        if (child.y < maxOffsets[1])
            maxOffsets[1] = (int)child.y;
        else if (child.y > maxOffsets[3])
            maxOffsets[3] = (int)child.y;
    }
}