/**	\file dab_space_ntree_node_pool.h
 */

#ifndef _dab_space_ntree_node_pool_h_
#define _dab_space_ntree_node_pool_h_

#include <ostream>
#include <deque>
#include <sstream>

namespace dab
{

namespace space
{

class NTreeNode;

class NTreeNodePool
{
public:
    /**
     \brief create pool of ntree nodes
     \param pDim dimension of nodes
     */
    NTreeNodePool(unsigned int pDim);
    
    /**
     \brief create pool of ntree nodes
     \param pDim dimension of nodes
     \param pStartPoolSize number of nodes that are initially in the pool
     \param pPoolSizeIncrement size by which the pool increases whenever it runs out of nodes
     */
    NTreeNodePool(unsigned int pDim, unsigned int pStartPoolSize, unsigned int pPoolSizeIncrement = sPoolSizeIncrement);
    
    /**
     \brief destructor
     */
    virtual ~NTreeNodePool();
    
    /**
     \brief retrieve node from pool
     */
    NTreeNode* retrieve();
    
    /**
     \brief release node into pool
     \param pNode node to be released
     */
    void release(NTreeNode* pNode);
    
    /**
     \brief obtain textual node pool information
     \return String containing node pool information
     */
    operator std::string() const;
    
    /**
     \brief obtain textual node pool information
     \return String containing node pool information
     */
    std::string info() const;
    
    /**
     \brief retrieve textual ntree node pool information
     \param pOstream output stream
     \param pPool ntree node pool
     */
    friend std::ostream& operator<< (std::ostream & pOstream, const NTreeNodePool& pPool)
    {
        pOstream << std::string(pPool);
        
        return pOstream;
    }
    
protected:
    /**
     \brief default constructor
     */
    NTreeNodePool();
    
    /**
     \brief default initial pool size
     */
    static unsigned int sStartPoolSize;
    
    /**
     \brief default pool size increment
     */
    static unsigned int sPoolSizeIncrement;
    
    /**
     \brief node dimension
     */
    unsigned int mDim;
    
    /**
     \brief size by which the pool increases whenever it runs out of events
     */
    unsigned int mPoolSizeIncrement;
    
    /**
     \brief vector of unused ntree nodes
     */
    std::deque< NTreeNode* > mNodes;
};

};

};

#endif