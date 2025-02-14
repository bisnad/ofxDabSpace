/** \file dab_space_ntree_node_pool.cpp
 */

#include "dab_space_ntree_node_pool.h"
#include "dab_space_ntree_node.h"

using namespace dab;
using namespace dab::space;

unsigned int NTreeNodePool::sStartPoolSize = 1000;
unsigned int NTreeNodePool::sPoolSizeIncrement = 100;

NTreeNodePool::NTreeNodePool()
: mPoolSizeIncrement(0)
, mDim(0)
{}

NTreeNodePool::NTreeNodePool(unsigned int pDim)
: mPoolSizeIncrement(sPoolSizeIncrement)
, mDim(pDim)
{
	for(unsigned int i=0; i<sStartPoolSize; ++i)
	{
		mNodes.push_back(new NTreeNode(mDim));
	}
}

NTreeNodePool::NTreeNodePool(unsigned int pDim, unsigned int pStartPoolSize, unsigned int pPoolSizeIncrement)
: mPoolSizeIncrement(pPoolSizeIncrement)
, mDim(pDim)
{
	for(unsigned int i=0; i<pStartPoolSize; ++i)
	{
		mNodes.push_back(new NTreeNode(mDim));
	}
}

NTreeNodePool::~NTreeNodePool()
{
	unsigned int poolSize = mNodes.size();
	
	for(unsigned int i=0; i<poolSize; ++i)
	{
		NTreeNode* node = mNodes[i];
		delete node;
	}
	
	mNodes.clear();
}

NTreeNode*
NTreeNodePool::retrieve()
{
	if(mNodes.size() == 0) // create more nodes
	{
		for(unsigned int i=0; i<mPoolSizeIncrement; ++i) mNodes.push_back(new NTreeNode(mDim));
	}
    
    NTreeNode* node = mNodes[0];
    mNodes.pop_front();
	return node;
}

void
NTreeNodePool::release(NTreeNode* pNode)
{
	mNodes.push_back(pNode);
}

NTreeNodePool::operator std::string() const
{
    return info();
}

std::string
NTreeNodePool::info() const
{
    std::stringstream stream;
    
    stream << "NTreeNodePool:\n" << "    poolSize: " << mNodes.size() << "\n" << "    poolSizeIncrement: " << mPoolSizeIncrement << "\n";

	return stream.str();
}