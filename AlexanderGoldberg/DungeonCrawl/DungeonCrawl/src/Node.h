#pragma once
#include<float.h>
#include <string>
class Node
{
protected:
   
    
public:
    Node* m_Parent = nullptr;
    float gCost = FLT_MAX;
    float hCost = FLT_MAX;
    float fCost = FLT_MAX;
    Node();
    ~Node();
    int m_IndexOfTile = -1;
    std::string GetXY(int _width);
    
};



inline bool operator< (const Node& lhs, const Node& rhs) { return lhs.fCost < rhs.fCost; }