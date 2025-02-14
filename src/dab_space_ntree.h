/**	\file dab_space_ntree.h
 */

#ifndef _dab_space_ntree_h_
#define _dab_space_ntree_h_

#include <Eigen/Dense>
#include "dab_exception.h"
#include "dab_space_ntree_node_pool.h"

namespace dab
{

namespace space
{

class NTreeNode;
    class NTreeAlg;

class NTree
{
    friend class NTreeVisitor;
    
public:
    /**
     \brief create ntree
     \param pMinPos minimum position in space
     \param pMaxPos maximum position in space
     */
    NTree(const Eigen::VectorXf& pMinPos, const Eigen::VectorXf& pMaxPos);
    
    /**
     \brief destructor
     */
    ~NTree();
    
    /**
     \brief change space size
     \param pMinPos minimum position in space
     \param pMaxPos maximum position in space
     
     doing this requires that the tree must be cleared and rebuilt from scratch.\n
     this is fairly dangerous because there is no mechanism that ensures that this happens, the user must take care if it\n
     */
    void resize(const Eigen::VectorXf& pMinPos, const Eigen::VectorXf& pMaxPos);
    
    unsigned int dim() const;
    
    /**
     \brief return root node
     \return root node
     */
    NTreeNode* rootNode();
    
    /**
     \brief obtain textual ntree information
     \return String containing textual ntree information
     */
    operator std::string() const;
    
    /**
     \brief obtain textual ntree information
     \return String containing textual ntree information
     */
    std::string info() const;
    
    /**
     \brief retrieve textual ntree information
     \param pOstream output stream
     \param pTree ntree
     */
    friend std::ostream& operator<< (std::ostream & pOstream, const NTree& pTree)
    {
        pOstream << std::string(pTree);
        
        return pOstream;
    }
    
protected:
    /**
     /brief default constructor
     */
    NTree();
    
    /**
     \brief maximum ntree depth
     
     > 0 : enforce depth limit
     < 0 : no depth limit 
     */
    int mMaxDepth;
    
    /**
     \brief minimum space object count
     
     minimum number of space objects in a node for recursion to continue creating node children
     
     > 0 : enforce space object count limit
     < 0 : no space object count limit
     */
    int mMinObjectCount;
    
    /**
     \brief ntree minimum space position
     */
    Eigen::VectorXf mMinPos;
    
    /**
     \brief ntree maximum space position
     */
    Eigen::VectorXf mMaxPos;
    
    /**
     \brief root node
     */
    NTreeNode* mRootNode;
};

};

};

#endif