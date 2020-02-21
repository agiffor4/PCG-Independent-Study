#pragma once
#include<float.h>
#include <string>
class Node
{
protected:
   
    
public:
    int m_IndexOfTile = -1; 
    float gCost = FLT_MAX;
    float hCost = FLT_MAX;
    float fCost = FLT_MAX;
    Node* m_Parent = nullptr;
    Node();
    ~Node();
    
    std::string GetXY(int _width);
    
};



inline bool operator< (const Node& lhs, const Node& rhs) { return lhs.fCost < rhs.fCost; }
