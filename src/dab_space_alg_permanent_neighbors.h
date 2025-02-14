/** \file dab_space_alg_permanent_neighbors.h
*/

#ifndef _dab_space_alg_permanent_neighbors_h_
#define _dab_space_alg_permanent_neighbors_h_

#include <iostream>
#include "dab_exception.h"
#include "dab_space_alg.h"

namespace dab
{
    
namespace space
{
    
class SpaceProxyObject;
    
class PermanentNeighborsAlg : public SpaceAlg
{
friend class NTreeVisitor;
    
public:
    PermanentNeighborsAlg(unsigned int pDim);
    PermanentNeighborsAlg(const Eigen::Vector3f& pMinPos, const Eigen::Vector3f& pMaxPos);
    ~PermanentNeighborsAlg();
    
    void updateNeighbors( std::vector< SpaceProxyObject* >& pObjects ) throw (Exception);
    
    /**
     \brief obtain textual ntree information
     \return String containing textual ntree information
     */
    operator std::string() const;
    
    /**
     \brief retrieve textual ntree information
     \param pOstream output stream
     \param pTree ntree
     */
    friend std::ostream& operator<< (std::ostream & pOstream, const PermanentNeighborsAlg& pAlg)
    {
        pOstream << std::string(pAlg);
        
        return pOstream;
    }
    
protected:
    PermanentNeighborsAlg();
};
    
};
    
};

#endif
