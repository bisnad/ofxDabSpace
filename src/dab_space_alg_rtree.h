/** \file dab_space_alg_rtree.h
 */

#ifndef _dab_space_alg_rtree_h_
#define _dab_space_alg_rtree_h_

#include "dab_space_types.h"
#include "dab_space_alg.h"
#include "dab_space_rtree.h"

// TODO: untested

namespace dab
{

namespace space
{

class RTreeAlg : public SpaceAlg
{
public:
    RTreeAlg(const Eigen::Vector3f& pMinPos, const Eigen::Vector3f& pMaxPos);
    ~RTreeAlg();
    
    ClosestShapePointType closestShapePointType() const;
    
    
    /**
     \brief set closest point type
     \param pClosestPointType closest point type
     */
    void setClosestPointType(ClosestShapePointType pClosestPointType);
    
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
    friend std::ostream& operator<< (std::ostream & pOstream, const RTreeAlg& pAlg)
    {
        pOstream << std::string(pAlg);
        
        return pOstream;
    }
    
protected:
    RTreeAlg();
    
    /**
     \brief RTree space partitioning instance
     */
    RTree<SpaceProxyObject*, float, float> mTree;
    
    ClosestShapePointType mClosestPointType;	
};

};

};

#endif