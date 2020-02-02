#pragma once
#include<memory>
#include <string>
enum class DTS;
class TreeNode
{

protected:	
	
	struct RectA
	{
		int x1 = 0;
		int y1 = 0;
		int x2 = 0;
		int y2 = 0;
	};
	TreeNode* m_parent = nullptr;
	TreeNode* m_LTreeNode = nullptr;
	TreeNode* m_RTreeNode = nullptr;
	std::string m_name = "";
	bool m_checked = false;
public:	
	TreeNode(TreeNode* _parent, std::string _name);
	~TreeNode();	
	void Add(TreeNode* _parent, int _number);
	TreeNode* GetLeft();
	TreeNode* GetRight();
	
};

