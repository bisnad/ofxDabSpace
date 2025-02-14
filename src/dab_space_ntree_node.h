/**	\file dab_space_ntree_node.h
 */

#ifndef _dab_space_ntree_node_h_
#define _dab_space_ntree_node_h_

#include "dab_space_proxy_object.h"
#include <Eigen/Dense>
#include <vector>

namespace dab
{

namespace space
{

class NTreeNode
{
    friend class NTreeVisitor;
    
public:
    /**
     \brief create ntree node
     \param pDimension dimension of node
     */
    NTreeNode(unsigned int pDimension);
    
    /**
     \brief destructor
     */
    ~NTreeNode();
    
    /**
     \brief return dimension
     */
    unsigned int dim() const;
    
    /**
     \brief return number of children nodes
     */
    unsigned int childrenCount() const;
    
    /**
     \brief return node level within ntree
     */
    unsigned int level() const;
    
    /**
     \brief return number of parameters stored in node
     */
    unsigned int objectCount() const;
    
    /**
     \brief clear node
     
     removes all node children and parameters
     */
    void clear();
    
    /**
     \brief obtain textual node information
     \return String containing node information
     */
    operator std::string() const;
    
    /**
     \brief obtain textual node information
     \return String containing node information
     */
    std::string info() const;
    
    /**
     \brief retrieve textual node pool information
     \param pOstream output stream
     \param pNode ntree node
     */
    friend std::ostream& operator<< (std::ostream & pOstream, const NTreeNode& pNode)
    {
        pOstream << std::string(pNode);
        
        return pOstream;
    }
    
protected:
    /**
     \brief default constructor
     */
    NTreeNode();
    
    /**
     \brief parent node
     */
    NTreeNode* mParent;
    
    /**
     \brief array of children nodes
     */
    NTreeNode** mChildren;
    
    /**
     \brief number of children nodes
     */
    unsigned int mChildrenCount;
    
    /**
     \brief stored parameters
     */
    std::vector<SpaceProxyObject*> mObjects;
    
    /**
     \brief last checked parameter
     
     for internal use only\n
     required when building space opject neighbor lists
     */
    SpaceProxyObject* mLastCheckedObject;
    
    /**
     \brief node level within ntree
     */
    unsigned int mLevel;
    
    /**
     \brief minimum corner of node hypercube
     */
    Eigen::VectorXf mMinPos;
    
    /**
     \brief maximum corner of node hypercube
     */
    Eigen::VectorXf mMaxPos;
};

};

};

#endif