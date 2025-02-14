/** \file dab_space_alg_ntree.h
 */

#ifndef _dab_space_alg_ntree_h_
#define _dab_space_alg_ntree_h_

#include <Eigen/Dense>
#include "dab_space_alg.h"
#include "dab_space_ntree.h"
#include "dab_space_ntree_visitor.h"

namespace dab
{

namespace space
{

class NTreeNode;

class NTreeAlg : public SpaceAlg
{
    friend class NTreeVisitor;
    
public:
    NTreeAlg(unsigned int pDim);
    NTreeAlg(const Eigen::VectorXf& pMinPos, const Eigen::VectorXf& pMaxPos) throw (Exception);
    ~NTreeAlg();
    
    void resize(const Eigen::VectorXf& pMinPos, const Eigen::VectorXf& pMaxPos) throw (Exception);
    void updateStructure( std::vector< SpaceProxyObject* >& pObjects ) throw (Exception);
    void updateNeighbors( std::vector< SpaceProxyObject* >& pObjects ) throw (Exception);
    
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
    friend std::ostream& operator<< (std::ostream & pOstream, const NTreeAlg& pAlg)
    {
        pOstream << std::string(pAlg);
        
        return pOstream;
    }
    
protected:
    NTreeAlg();
    
    /**
     \brief NTree space partitioning instance
     */
    NTree mTree;
    
    /**
     \brief NTreeVisitor visitor for Ntree
     */
    NTreeVisitor mTreeVisitor;
};

};

};

#endif