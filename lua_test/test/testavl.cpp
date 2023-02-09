/*
AVL树的概念

二叉搜索树虽可以缩短查找的效率,但如果数据有序或接近有序二叉搜索树将退化为单支树,查找元素相当于在顺序表中搜索元素,效率低下。 
因此,两位俄罗斯的数学家 G.M.Adelson-Velskii 和 E.M.Landis 在1962年发明了一种解决上述问题的方法： 
当向二叉搜索树中插入新结点后,如果能保证每个结点的左右子树高度之差的绝对值不超过1(需要对树中的结点进行调整), 即可降低树的高度,从而减少平均搜索长度。

AVL树是具有以下性质的二叉搜索树：
	它的左右子树都是AVL树
	左右子树平衡因子差的绝对值不超过1(-1/0/1),也就是说每个节点的平衡因子只有三种取值 (-1/0/1)

平衡因子(平衡状态)的计算为右子树的高度减去左子树的高度
	= -1, 说明左子树比右子树高1,
	=  0, 说明左右子树一样高,
	=  1, 说明右子树比左子树高1。

当平衡因子更新为2或-2时,说明左右子树的高度差为2,此时AVL树不再平衡,那么如何控制它平衡呢？
有以下四种旋转
	1.左旋(右右)		parent._bf == 2 && cur._bf == 1		//当前父节点只有一个右子节点(当前节点),在当前节点上添加了一个右子节点, 更新后父节点因子为2	
	2.右左双旋(右左)	parent._bf == 2 && cur._bf == -1	//当前父节点只有一个右子节点(当前节点),在当前节点上添加了一个左子节点, 更新后父节点因子为2	
	3.右旋(左左)		parent._bf == -2 && cur._bf == -1	//当前父节点只有一个左子节点(当前节点),在当前节点上添加了一个右子节点, 更新后父节点因子为-2	
	4.左右双旋(左右)	parent._bf == -2 && cur._bf == 1	//当前父节点只有一个左子节点(当前节点),在当前节点上添加了一个左子节点, 更新后父节点因子为-2	
 
 
二叉树的高度: root层为0, 每向下一层，高度加1, AVL里不使用高度

*/


#pragma once
#include<iostream>
#include<assert.h>
using namespace std;
template<class K,class V>
struct AVLTreeNode
{
	AVLTreeNode<K, V>* _left;
	AVLTreeNode<K, V>* _right;
	AVLTreeNode<K, V>* _parent;
	K _key;
	V _value;
	int _bf; //平衡因子
	AVLTreeNode(const K& key,const V& value)
		: _left(nullptr)
		, _right(nullptr)
		, _parent(nullptr)
		, _key(key)
		, _value(value)
		, _bf(0)
	{}
};


//todo  delete/find node 
template<class K,class V>
class AVLTree
{
	typedef AVLTreeNode<K,V> Node;
public:
	AVLTree()
		: _root(nullptr)
	{}
	
	~AVLTree()
	{
		Destroy(_root);
		_root = nullptr;
	}
 
	bool Insert(const K& key,const V& value)
	{
		//如果树为空，插入新结点
		if (_root == nullptr)
		{
			_root = new Node(key,value);
			return true;
		}
		//按照二叉搜索树的性质找到要插入的位置
		Node* cur = _root;
		Node* parent = nullptr;
		while (cur)
		{
			if (key < cur->_key)
			{
				parent = cur;
				cur = cur->_left;
			}
			else if (key > cur->_key)
			{
				parent = cur;
				cur = cur->_right;
			}
			else
			{
				return false;
			}
		}
		//插入新结点
		cur = new Node(key,value);
		if (key < parent->_key)
		{
			parent->_left = cur;
		}
		else
		{
			parent->_right = cur;
		}
 
		//更新cur的父亲结点
		cur->_parent = parent;
 
		//插入新结点后，AVL树的平衡性可能遭到破坏，此时需要更新平衡因子，并检查是否破坏了AVL树的平衡性
		
		while (parent)
		{
			//更新父亲的平衡因子
			if (cur == parent->_left)
			{
				parent->_bf--;
			}
			else
			{
				parent->_bf++;
			}
			//更新后检查父亲的平衡
			if (parent->_bf == 0)
			{
				break;
			}
			
			if (abs(parent->_bf) == 1)
			{
				//向上更新
				cur = parent;
				parent = parent->_parent;
				continue;
			}
			
			if (abs(parent->_bf) == 2)
			{
				//插入新结点后，AVL数的平衡性遭到破坏，需要对以parent为根的树进行旋转处理
				if (parent->_bf == -2 && cur->_bf == -1)
				{
					RotateR(parent);
				}
				else if (parent->_bf == 2 && cur->_bf == 1)
				{
					RotateL(parent);
				}
				else if (parent->_bf == -2 && cur->_bf == 1)//左右双旋
				{
					RotateLR(parent);
				}
				else if (parent->_bf == 2 && cur->_bf == -1)//右左双旋
				{
					RotateRL(parent);
				}
				break;
			}
			else
			{
				assert(false);
			}
		}
		return true;
	}
	void RotateLR(Node* parent)
	{
		Node* subL = parent->_left;
		Node* subLR = subL->_right;
		int bf = subLR->_bf;
 
		RotateL(parent->_left);
		RotateR(parent);
		if (bf == -1)
		{
			parent->_bf = 1;
			subL->_bf = 0;
		}
		else if (bf == 1)
		{
			parent->_bf = 0;
			subL->_bf = -1;
		}
		else
		{
			parent->_bf = subL->_bf = 0;
		}
		subLR->_bf = 0;
	}
	void RotateRL(Node* parent)
	{
		Node* subR = parent->_right;
		Node* subRL = subR->_left;
		int bf = subRL->_bf;
 
		RotateR(parent->_right);
		RotateL(parent);
		if (bf == -1)
		{
			parent->_bf = 0;
			subR->_bf = 1;
		}
		else if (bf == 1)
		{
			parent->_bf = -1;
			subR->_bf = 0;
		}
		else
		{
			parent->_bf = subR->_bf = 0;
		}
		subRL->_bf = 0;
	}
	void RotateL(Node* parent)//parent 节点左移
	{
		//把右子树节点 subR  提到父节点位置
		//父节点放左子树 和 subR->_left 位置冲突需要处理, 把 subR->_left 放父节点的右子树上正好
		
		Node* subR = parent->_right; //变量保存 右子树节点 subR 
	
		Node* subRL = subR->_left; //变量保存 右子树节点的左子树节点 subRL,  parent 将放到  subR->left, 这里要先处理原有的 left
		parent->_right = subRL;    //把 subR->_left 放父节点的右子树上
		if (subRL)
		{
			subRL->_parent = parent; //更改 subR->_left 父节点指针指向
		}

		subR->_left = parent; //把 父节点放到 subR->_left

		Node* pparent = parent->_parent;//变量保存 父节点的父节点 pparent 
		parent->_parent = subR; //更改 父节点 指针指向
		if (parent == _root)
		{
			_root = subR; //如果是 root 节点左移,更新 _root 变量
		}
		else
		{
			// 更改 父节点的父节点 的 子树指针 指向
			if (pparent->_left == parent) 
			{
				pparent->_left = subR;
			}
			else
			{
				pparent->_right = subR;
			}
		}

		subR->_parent = pparent; // 更改 右子树节点 subR 父节点指针 指向
 
		subR->_bf = parent->_bf = 0; //更改 平衡因子  --- 这里为什么都是 0 ?  
 
	}
	void RotateR(Node* parent) //左左
	{
		//把 subL  提到父节点位置
		Node* subL = parent->_left;
		Node* subLR = subL->_right;
		parent->_left = subLR;
		if (subLR)
		{
			subLR->_parent = parent;
		}
		subL->_right = parent;
		Node* pparent = parent->_parent;
		parent->_parent = subL;
		if (parent == _root)
		{
			_root = subL;
		}
		else
		{
			if (pparent->_left = parent)
			{
				pparent->_left = subL;
			}
			else
			{
				pparent->_right = subL;
			}
		}
		subL->_parent = pparent;
 
		subL->_bf = parent->_bf = 0;
	}
	void Inorder()
	{
		_Inorder(_root);
		cout << endl;
	}
	void Height()
	{
		cout << _Height(_root) << endl;
	}
	bool IsBalanceTree()
	{
		return _IsBalanceTree(_root);
	}
	int _Height(Node* root)
	{
		if (root == nullptr)
			return 0;
		int leftHeight = _Height(root->_left);
		int rightHeight = _Height(root->_right);
		return leftHeight > rightHeight ? leftHeight + 1 : rightHeight + 1;
	}
	bool _IsBalanceTree(Node* root)
	{
		if (_root == nullptr)
			return true;
 		int leftHeight = _Height(root->_left);
		int rightHeight = _Height(root->_right);
		if (abs(leftHeight - rightHeight) > 2 || (leftHeight - rightHeight) != root->_bf)
		{
			cout << " 平衡因子错误" << endl;
			return false;
		}
		return  _IsBalanceTree(root->_left) && _IsBalanceTree(root->_right);
	}
private:
	void Destroy(Node* root)
	{
		if (root)
		{
			Destroy(root->_left);
			Destroy(root->_right);
			delete root;
		}
	}
	void _Inorder(Node* root)
	{
		if (root == nullptr)
			return;
		_Inorder(root->_left);
		cout << root->_key << " ";
		_Inorder(root->_right);
	}
	
	Node* _root;
};
void TestAVLTree()
{
	AVLTree<int,int> tree;
	int arr[] = { 26, 18, 14, 5, 3 };
	for (auto& e : arr)
	{
		tree.Insert(e,e);
	}
	tree.Inorder();
	tree.Height();
	tree.IsBalanceTree();
}



void testAVL()
{
    AVLTree<int,int> t;
    t.Insert(1,1);
    t.Insert(2,2);
    //t.Insert(2,2); //todo insert the same? 插入一大堆相同的 key 平衡不了? 
    //t.Insert(2,2);
    t.Insert(3,3);
    t.Insert(4,4);
    t.Insert(5,5);
    t.Insert(6,6);

}

