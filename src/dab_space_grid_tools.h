/** \file dab_space_grid_tools.h
 * tools can only be used in 3D space
*/

#ifndef _dab_space_grid_tools_h_
#define _dab_space_grid_tools_h_

#include <iostream>
#include <array>
#include <Eigen/Dense>
#include "dab_singleton.h"
#include "dab_geom_mesh.h"
#include "dab_space_grid.h"
#include "dab_space_shape.h"

namespace dab
{
    
namespace space
{
    
enum GridDistanceMode
{
    ShapeDistance,
    ContourDistance,
    ShapeTangent,
    ContourTangent
};

class SpaceGridTools : public Singleton<SpaceGridTools>
{
friend class Singleton<SpaceGridTools>;
    
public:
    /**
     \brief create distance space grid from mesh shape
     \param pGridSubdivisionCount number of space grid subdivisions
     \param pGridMinPos minimum space grid position
     \param pGridMaxPos maximum space grid position
     \param SpaceShape space shape
     \return space grid
     
     calculates distance field in world coordinates.\n
     the distance vectors point from individual grid positions towards to closest position on the mesh shape's triangulated surface
     */
    SpaceGrid* createDistanceGrid( const std::array<unsigned int, 3>& pGridSubdivisionCount, const glm::vec3& pGridMinPos, const glm::vec3& pGridMaxPos, SpaceShape* pSpaceShape );
    
    /**
     \brief create distance space grid for space shape
     \param pSpaceGrid space grid
     \param pShape shape
     \exception Exception fails if grid dimension is not 3D
     
     calculates distance field in world coordinates.\n
     the distance vectors point from individual grid positions towards to closest position to the space shape object
     */
    void createDistanceGrid( SpaceGrid* pSpaceGrid, SpaceShape* pSpaceShape ) throw (Exception);
    
    /**
     \brief create distance space grid for space shape
     \param pSpaceGrid space grid
     \param pShape shape
     \param pScaleField distance scale
     \exception Exception fails if grid or field dimension is not 3D
     
     calculates distance field in world coordinates.\n
     the distance vectors point from individual grid positions towards to closest position to the space shape object
     */
    void createDistanceGrid( SpaceGrid* pSpaceGrid, SpaceShape* pShape, math::VectorField<float>* pScaleField ) throw (Exception);
    
protected:
    /**
     \brief default constructor
     */
    SpaceGridTools();
    
    /**
     \brief destructor
     */
    ~SpaceGridTools();
};
    
};
    
};


#endif
