/** \file dab_space_alg_kdtree.h
 */

#ifndef _dab_space_alg_kdtree_h_
#define _dab_space_alg_kdtree_h_

#include <Eigen/Dense>
#include "dab_space_alg.h"
#include <kdtree.h>

// TODO: untested

namespace dab
{
    
namespace space
{
    
class KDTreeAlg : public SpaceAlg
{
public:
    KDTreeAlg(unsigned int pDim);
    KDTreeAlg(const Eigen::VectorXf& pMinPos, const Eigen::VectorXf& pMaxPos);
    ~KDTreeAlg();
    
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
    friend std::ostream& operator<< (std::ostream & pOstream, const KDTreeAlg& pAlg)
    {
        pOstream << std::string(pAlg);
        
        return pOstream;
    }
    
protected:
    KDTreeAlg();
    
    kdtree* mTree; ///\brief KD Tree
    kdres* mSearchResult; ///\brief Search Result
};
    
};
    
};

#endif