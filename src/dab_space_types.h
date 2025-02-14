/** \file dab_space_types.h
 */

#ifndef _dab_space_types_h_
#define _dab_space_types_h_

namespace dab
{
    
namespace space
{
    
enum SpaceAlgType
{
    PermanentNeighborsAlgType,
    NTreeAlgType,
    KDTreeAlgType,
    ANNAlgType,
    RTreeAlgType,
    GridAlgType
};
    
enum ClosestShapePointType
{
    ClosestPointAABB,
    ClosestPointShape
};
    
}
    
}

#endif