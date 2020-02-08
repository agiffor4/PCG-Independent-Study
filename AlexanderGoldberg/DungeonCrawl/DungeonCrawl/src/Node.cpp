#include "Node.h"

Node::Node(){}
Node::~Node(){}



std::string Node::GetXY(int _width) {
    int x = m_IndexOfTile % _width;
    int y = (m_IndexOfTile - x) / _width;
    return "(" + std::to_string(x) + ", " + std::to_string(y) + ")";
}


