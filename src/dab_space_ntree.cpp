/**	\file dab_space_ntree.cpp
 */

#include "dab_space_ntree.h"
#include "dab_space_ntree_node.h"

using namespace dab;
using namespace dab::space;

NTree::NTree()
: mMinPos(1)
, mMaxPos(1)
, mMaxDepth(3)
, mMinObjectCount(-1)
, mRootNode(nullptr)
{}

NTree::NTree(const Eigen::Matrix<float, Eigen::Dynamic, 1>& pMinPos, const Eigen::Matrix<float, Eigen::Dynamic, 1>& pMaxPos)
: mMinPos(pMinPos)
, mMaxPos(pMaxPos)
, mMaxDepth(3)
, mMinObjectCount(-1)
, mRootNode(nullptr)
{}

NTree::~NTree()
{}

void
NTree::resize(const Eigen::Matrix<float, Eigen::Dynamic, 1>& pMinPos, const Eigen::Matrix<float, Eigen::Dynamic, 1>& pMaxPos)
{
	mMinPos = pMinPos;
	mMaxPos = pMaxPos;
}

unsigned int
NTree::dim() const
{
    return mMinPos.rows();
}

NTreeNode*
NTree::rootNode()
{
	return mRootNode;
}

NTree::operator std::string() const
{
    return info();
}

std::string
NTree::info() const
{
    std::stringstream stream;
    
    stream << "NTree:\n";

	return stream.str();
}