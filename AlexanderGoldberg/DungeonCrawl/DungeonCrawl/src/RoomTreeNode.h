#pragma once
#include<memory>
#include <string>
#include <vector>
class RoomTreeNode
{

protected:	
	RoomTreeNode* m_parent = nullptr;	
	std::vector<RoomTreeNode*> m_branches;
	bool m_locked = false;
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
	bool ShouldStopLocking();
	bool m_ChildAndParent = false;
	bool m_Checked = false;
	int m_Index = -1;

	
};

