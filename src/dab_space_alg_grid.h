/** \file dab_space_alg_grid.h
 *
 *  Created by Daniel Bisig on 6/9/09.
 *  Ported to OpenFrameworks by Daniel Bisig on 10/08/17.
 */

#ifndef _iso_space_alg_grid_h_
#define _iso_space_alg_grid_h_

#include "dab_space_alg.h"
#include "dab_space_grid.h"
#include <ANN/ANN.h>
#include <ANN/ANNx.h>

// WARNING: Untested!!

namespace dab
{

namespace space
{
    
class SpaceObject;

class GridAlg : public SpaceAlg
{
public:
    enum GridUpdateMode // write back into grid mode
    {
        NoUpdateMode, // do not write back into grid
        NearestReplaceMode, // overwrite values in nearest grid cells
        NearestAddMode, // add to values in nearest grid cells
        AvgReplaceMode, // overwrite values via linear interpolation
        AvgAddMode // add to values via linear interpolation
    };
    
    enum GridNeighborMode // neighbor calculation mode
    {
        CellLocationMode, // add value of the cell at position of space object to space object
        GridLocationMode, // add values at all those grid points to space object that are neighboring to the space objects position
        AvgLocationMode, // add single linearly interpolated value at the position of space object to space object
        AvgRegionMode, // calculate single averaged location based on the weighting of grid values
        PeakSearchMode, // calculate multiple locations representing peaks in grid values within a search radius
        CentroidSearchMode // calculate centroid location based on grid value distribution
    };
    
    /**
     \brief create grid alg
     \param pValueDim dimension of values stored in grid
     \param pSubdivisionCount number of grid subdivisions
     \param pMinPos minimum position in grid space
     \param pMaxPos maximum position in grid space
     \param pNeighborMode neighbor calculation mode
     \param pUpdateMode grid update mode
     \exception Exception failed to create grid alg
     */
    GridAlg(unsigned int pValueDim, const dab::Array<unsigned int>& pSubdivisionCount, const Eigen::VectorXf& pMinPos, const Eigen::VectorXf& pMaxPos, GridNeighborMode pNeighborMode, GridUpdateMode pUpdateMode = NoUpdateMode) throw (Exception);
    
    /**
     \brief create grid alg
     \param pName space name
     \param pSpaceGrid space grid
     \param pNeighborMode neighbor calculation mode
     \param pUpdateMode grid update mode
     */
    GridAlg(SpaceGrid* pSpaceGrid, GridNeighborMode pNeighborMode, GridUpdateMode pUpdateMode = NoUpdateMode);
    
    /**
     \brief destructor
     */
    ~GridAlg();
    
    bool gridOwner() const;
    
    GridNeighborMode neighborMode() const;
    
    GridUpdateMode updateMode() const;
    
    /**
     \brief return space grid
     \return space grid
     \
     */
    SpaceGrid& grid();
    
    /**
     \brief set space grid
     \param pSpaceGrid space grid
     \exception Exception grid mismatch
     */
    void setGrid( SpaceGrid* pSpaceGrid ) throw (Exception);
    
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
    friend std::ostream& operator<< (std::ostream & pOstream, const GridAlg& pAlg)
    {
        pOstream << pAlg.info();
        
        return pOstream;
    }
    
protected:
    GridAlg();
    
    SpaceGrid* mGrid; ///\brief grid	
    bool mGridOwner; ///\brief flag indicating whether we're owner of the grid or not
    GridNeighborMode mNeighborMode; ///\brief mode of grid space neighbor calculation
    GridUpdateMode mUpdateMode; ///\brief mode of grid updating
    std::vector<SpaceObject*> mTmpSpaceObjects; ///\brief temporarily created space objects
};

};

};

#endif