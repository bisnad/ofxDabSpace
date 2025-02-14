/**	\file dab_space_ntree_visitor.h
 */

#ifndef _dab_space_ntree_visitor_h_
#define _dab_space_ntree_visitor_h_

#include <Eigen/Dense>
#include "dab_space_ntree.h"
#include "dab_space_ntree_node.h"

namespace dab
{

namespace space
{

class NeighborPool;
class SpaceProxyObject;

class NTreeVisitor
{
public:
    NTreeVisitor(unsigned int pDimension);
    ~NTreeVisitor();
    
    void createNodePool();
    
    void buildTree(NTree& pTree, std::vector<SpaceProxyObject*>& pObjects);
    void buildTree(NTree& pTree, NTreeNode* pNode);
    void updateTree(NTree& pTree, std::vector<SpaceProxyObject*>& pObjects);
    void updateTree(NTree& pTree, NTreeNode* pNode);
    
    void calcNeighbors(NTree& pTree, std::vector<SpaceProxyObject*>& pObjects);
    void calcNeighbors(NTreeNode*, std::vector<SpaceProxyObject*>& pObjects);
    
    void calcNeighbors(NTreeNode* pNode, SpaceProxyObject* pObject);
    void clearTree(NTree& pTree);
    void clearTree(NTreeNode* pNode);
    
    /**
     \brief print ntree information
     \param pTree ntree
     */
    std::string info(const NTree& pTree) const;
    
    /**
     \brief print ntree node information
     \param pNode ntree node
     */
    std::string info(const NTreeNode* pNode) const;
    
protected:
    NTreeVisitor();
    
    /**
     \brief dimension
     */
    unsigned int mDim;
    
    /**
     \brief temporaey node center position
     */
    Eigen::VectorXf mCenterPos;
    
    /**
     \brief temporary node minimum position
     */
    Eigen::VectorXf mMinPos;
    
    /**
     \brief temporary node maximum position
     */
    Eigen::VectorXf mMaxPos;
    
    /**
     \brief pool of nodes for building an ntree
     */
    NTreeNodePool* mNodePool;
    
};

};

};

#endif


