#pragma once
#include<memory>
#include <string>
#include <vector>
class RoomTreeNode
{

protected:	
	RoomTreeNode* m_parent = nullptr;	
	std::vector<RoomTreeNode*> m_branches = std::vector<RoomTreeNode*>();
	bool m_locked = false;
	int m_depth = 1;
public:	
	RoomTreeNode(RoomTreeNode* _parent);
	~RoomTreeNode();
	void SetParent(RoomTreeNode* _parent);
	RoomTreeNode* GetParent();
	void AddChild(RoomTreeNode* _child);
	bool IsRoot();
	int ChildCount();
	RoomTreeNode* GetChild(int _index);
	void Lock(bool _val);
	bool IsLocked();
	void SetDepth(int _depth);
	int GetDepth();
	bool ShouldStopLocking();
	bool m_ChildAndParent = false;
	bool m_Checked = false;
	int m_Index = -1;
	const std::vector<RoomTreeNode*>& GetChildren();

	
};

