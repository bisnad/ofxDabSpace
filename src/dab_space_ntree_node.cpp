/**	\file dab_space_ntree_node.cpp
 */

#include "dab_space_ntree_node.h"

using namespace dab;
using namespace dab::space;

NTreeNode::NTreeNode()
: mParent(nullptr)
, mChildren(nullptr)
, mChildrenCount(pow(2.0, 1.0))
, mLastCheckedObject(nullptr)
, mLevel(0)
, mMinPos(1)
, mMaxPos(1)
{
	mChildren = new NTreeNode*[mChildrenCount];
	for(unsigned int i=0; i<mChildrenCount; ++i) mChildren[i] = nullptr;
}

NTreeNode::NTreeNode(unsigned int pDimension)
: mParent(nullptr)
, mChildren(nullptr)
, mChildrenCount(pow(2.0, static_cast<double>(pDimension)))
, mLastCheckedObject(nullptr)
, mLevel(0)
, mMinPos(pDimension)
, mMaxPos(pDimension)
{
	mChildren = new NTreeNode*[mChildrenCount];
	for(unsigned int i=0; i<mChildrenCount; ++i) mChildren[i] = nullptr;
}

NTreeNode::~NTreeNode()
{
	clear();
    
	delete [] mChildren;
	mChildren = nullptr;
}

unsigned int
NTreeNode::dim() const
{
	return mMinPos.rows();
}

unsigned int
NTreeNode::childrenCount() const
{
	return mChildrenCount;
}

unsigned int
NTreeNode::level() const
{
	return mLevel;
}

unsigned int
NTreeNode::objectCount() const
{
	return mObjects.size();
}

void
NTreeNode::clear()
{
	mParent = nullptr;
	mLastCheckedObject = nullptr;
	for(unsigned int i=0; i<mChildrenCount; ++i) mChildren[i] = nullptr;
	
	mObjects.clear();
}

NTreeNode::operator std::string() const
{
    return info();
}

std::string
NTreeNode::info() const
{
    std::stringstream stream;
    
    stream << "NTreeNode:\n    level " << mLevel << "\n";
    
	unsigned int dim = mMinPos.rows();
	
	stream << "    minPos ";
	for(unsigned int i=0; i<dim; ++i) stream << mMinPos[i] << " ";
	stream << "\n";
	stream << "    maxPos ";
	for(unsigned int i=0; i<dim; ++i) stream << mMaxPos[i] << " ";
	stream << "\n";
	stream << "    objectCount " << mObjects.size();
	stream << "\n";
    
    SpaceProxyObject* object;
	unsigned int objectCount = mObjects.size();
	
	for(unsigned int i=0; i<objectCount; ++i)
	{
		object = mObjects[i];
        
        stream << object->info() << "\n";
	}
	
	return stream.str();
}