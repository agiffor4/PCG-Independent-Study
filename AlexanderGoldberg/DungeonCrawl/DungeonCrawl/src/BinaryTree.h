#pragma once
#include<vector>
class TreeNode;

class BinaryTree
{
private:
	int m_size = 0;
	TreeNode* m_root = nullptr;
	std::vector<TreeNode*> m_leaves;
public:
	BinaryTree();
	~BinaryTree();
	int size();
	void Add();
	enum class DTS // DirectionToSplit
	{
		HORIZONTAL,
		VERTICAL
	};

	void Split(DTS _splitDirection);

};

