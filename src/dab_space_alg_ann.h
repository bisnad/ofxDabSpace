/** \file dab_space_alg_ann.h
 */

#ifndef _dab_space_alg_ann_h_
#define _dab_space_alg_ann_h_

#include "dab_space_alg.h"
#include <vector>
#include <Eigen/Dense>
#include <ANN/ANN.h>
#include <ANN/ANNx.h>
#include "dab_exception.h"

namespace dab
{

namespace space
{

class ANNAlg : public SpaceAlg
{
public:
    ANNAlg(unsigned int pDim);
    ANNAlg(const Eigen::VectorXf& pMinPos, const Eigen::VectorXf& pMaxPos);
    ~ANNAlg();
    
    void updateStructure( std::vector< SpaceProxyObject* >& pObjects ) throw (Exception);
    void updateNeighbors( std::vector< SpaceProxyObject* >& pObjects ) throw (Exception);
    
    /**
     \brief obtain textual ntree information
     \return String containing textual ann information
     */
    operator std::string() const;
    
    /**
     \brief obtain textual ntree information
     \return String containing textual ann information
     */
    std::string info() const;
    
    /**
     \brief retrieve textual ntree information
     \param pOstream output stream
     \param pTree ntree
     */
    friend std::ostream& operator<< (std::ostream & pOstream, const ANNAlg& pAlg)
    {
        pOstream << std::string(pAlg);
        
        return pOstream;
    }
    
protected:
    ANNAlg();
    
    ANNkd_tree* mTree; // search structure
    ANNpointArray mDataPts; // data points
    std::vector< SpaceProxyObject* >* mNeighborObjects; //  helper data structure (all space objects whose position is stored in the tree stucture)
};

};

};

#endif