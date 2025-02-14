/** \file dab_space_grid_tools.cpp
*/

#include "dab_space_grid_tools.h"

using namespace dab;
using namespace dab::space;

SpaceGridTools::SpaceGridTools()
{}

SpaceGridTools::~SpaceGridTools()
{}

SpaceGrid*
SpaceGridTools::createDistanceGrid( const std::array<unsigned int, 3>& pGridSubdivisionCount, const glm::vec3& pGridMinPos, const glm::vec3& pGridMaxPos, SpaceShape* pSpaceShape  )
{
    SpaceGrid* spaceGrid = new SpaceGrid(3, dab::Array<unsigned int>( { pGridSubdivisionCount[0], pGridSubdivisionCount[1], pGridSubdivisionCount[2] } ), Eigen::Vector3f( pGridMinPos.x, pGridMinPos.y, pGridMinPos.z ), Eigen::Vector3f( pGridMaxPos.x, pGridMaxPos.y, pGridMaxPos.z ) );
    
    try
    {
        createDistanceGrid( spaceGrid, pSpaceShape );
        return spaceGrid;
    }
    catch (Exception& e)
    {
        std::cout << e << "\n";
    }
}

//SpaceGrid
//SpaceGridTools::createDistanceGrid( const std::array<unsigned int, 3>& pGridSubdivisionCount, const ofVec3f& pGridMinPos, const ofVec3f& pGridMaxPos, SpaceShape* pSpaceShape  )
//{
//    SpaceGrid spaceGrid(3, dab::Array<unsigned int>( { pGridSubdivisionCount[0], pGridSubdivisionCount[1], pGridSubdivisionCount[2] } ), Eigen::Vector3f( pGridMinPos.x, pGridMinPos.y, pGridMinPos.z ), Eigen::Vector3f( pGridMaxPos.x, pGridMaxPos.y, pGridMaxPos.z ) );
//    
//    unsigned int gridDim = 3;
//	unsigned int vectorDim = 3;
//
//    const std::array<unsigned int, 3>& fieldSize = pGridSubdivisionCount;
//	ofVec3f wGridPointStep; // position increment for grid point in world coordinates
//    
//	// calculate grid point position increment
//	for(unsigned int i=0; i<gridDim; ++i)
//	{
//		if(fieldSize[i] > 1) wGridPointStep[i] = ( pGridMaxPos[i] - pGridMinPos[i] ) / static_cast<float>(fieldSize[i] - 1);
//		else wGridPointStep[i] = 0;
//	}
//    
//	//std::cout << "wGridPointStep " << wGridPointStep << "\n";
//	
//	ofVec3f wGridPoint = pGridMinPos; // grid point in world coordinates
//	ofVec3f oGridPoint; // grid point in mesh shape object coordinates
//	ofVec3f wClosestPoint; // closest point in world coordinates
//	ofVec3f oClosestPoint; // closest point in mesh shape object coordinates
//    
//    std::vector< Eigen::Matrix<float, Eigen::Dynamic, 1> >& fieldVectors = spaceGrid.vectorField().vectors();
//    
//	for(unsigned int z=0, fieldVectorIndex=0; z<fieldSize[2]; ++z)
//	{
//		wGridPoint.z = pGridMinPos[2] + wGridPointStep[2] * static_cast<float>(z);
//		
//		for(unsigned int y=0; y<fieldSize[1]; ++y)
//		{
//			wGridPoint.y = pGridMinPos[1] + wGridPointStep[1] * static_cast<float>(y);
//			
//			for(unsigned int x=0; x<fieldSize[0]; ++x, ++fieldVectorIndex)
//			{
//				wGridPoint.x = pGridMinPos[0] + wGridPointStep[0] * static_cast<float>(x);
//                
//                pSpaceShape->world2object(wGridPoint, oGridPoint);
//                pSpaceShape->closestPoint(oGridPoint, oClosestPoint);
//                pSpaceShape->object2world(oClosestPoint, wClosestPoint);
//                
//                Eigen::Matrix<float, Eigen::Dynamic, 1>& fieldVector = fieldVectors[fieldVectorIndex];
//                fieldVector[0] = wClosestPoint.x - wGridPoint.x;
//                fieldVector[1] = wClosestPoint.y - wGridPoint.y;
//                fieldVector[2] = wClosestPoint.z - wGridPoint.z;
//                
//                //std::cout << "x " << x << " y " << y << " z " << z << " wGridPoint " << wGridPoint << " oGridPoint " << oGridPoint << " oClosestPoint " << oClosestPoint << " wClosestPoint " << wClosestPoint << " fieldVec " << fieldVector.transpose() << "\n";
//			}
//		}
//	}
//	
//	return spaceGrid;
//}

void
SpaceGridTools::createDistanceGrid( SpaceGrid* pSpaceGrid, SpaceShape* pSpaceShape ) throw (Exception)
{
    if( pSpaceGrid->gridDim() != 3 ) throw Exception("SPACE ERROR: space grid dimension must be 3D", __FILE__, __FUNCTION__, __LINE__);
    if( pSpaceGrid->valueDim() != 3 ) throw Exception("SPACE ERROR: space grid value dimension must be 3D", __FILE__, __FUNCTION__, __LINE__);
    
    unsigned int gridDim = 3;
	unsigned int vectorDim = 3;
    
    const dab::Array<unsigned int>& fieldSize = pSpaceGrid->subdivisionCount();
	ofVec3f wGridPointStep; // position increment for grid point in world coordinates
    const Eigen::Matrix<float, Eigen::Dynamic, 1>& gridMinPos = pSpaceGrid->minPos();
    const Eigen::Matrix<float, Eigen::Dynamic, 1>& gridMaxPos = pSpaceGrid->maxPos();
    
	// calculate grid point position increment
	for(unsigned int i=0; i<gridDim; ++i)
	{
		if(fieldSize[i] > 1) wGridPointStep[i] = ( gridMaxPos[i] - gridMinPos[i] ) / static_cast<float>(fieldSize[i] - 1);
		else wGridPointStep[i] = 0;
	}
    
	//std::cout << "wGridPointStep " << wGridPointStep << "\n";
	
	glm::vec3 wGridPoint(gridMinPos[0], gridMinPos[1], gridMinPos[2]); // grid point in world coordinates
	glm::vec3 oGridPoint; // grid point in mesh shape object coordinates
	glm::vec3 wClosestPoint; // closest point in world coordinates
	glm::vec3 oClosestPoint; // closest point in mesh shape object coordinates
    
    std::vector< Eigen::Matrix<float, Eigen::Dynamic, 1> >& fieldVectors = pSpaceGrid->vectorField().vectors();
    
	for(unsigned int z=0, fieldVectorIndex=0; z<fieldSize[2]; ++z)
	{
		wGridPoint.z = gridMinPos[2] + wGridPointStep[2] * static_cast<float>(z);
		
		for(unsigned int y=0; y<fieldSize[1]; ++y)
		{
			wGridPoint.y = gridMinPos[1] + wGridPointStep[1] * static_cast<float>(y);
			
			for(unsigned int x=0; x<fieldSize[0]; ++x, ++fieldVectorIndex)
			{
				wGridPoint.x = gridMinPos[0] + wGridPointStep[0] * static_cast<float>(x);
                
                pSpaceShape->world2object(wGridPoint, oGridPoint);
                pSpaceShape->closestPoint(oGridPoint, oClosestPoint);
                pSpaceShape->object2world(oClosestPoint, wClosestPoint);
                
                Eigen::Matrix<float, Eigen::Dynamic, 1>& fieldVector = fieldVectors[fieldVectorIndex];
                fieldVector[0] = wClosestPoint.x - wGridPoint.x;
                fieldVector[1] = wClosestPoint.y - wGridPoint.y;
                fieldVector[2] = wClosestPoint.z - wGridPoint.z;
                
                //std::cout << "x " << x << " y " << y << " z " << z << " wGridPoint " << wGridPoint << " oGridPoint " << oGridPoint << " oClosestPoint " << oClosestPoint << " wClosestPoint " << wClosestPoint << " fieldVec " << fieldVector.transpose() << "\n";
			}
		}
	}
}

void
SpaceGridTools::createDistanceGrid( SpaceGrid* pSpaceGrid, SpaceShape* pSpaceShape, math::VectorField<float>* pScaleField ) throw (Exception)
{
    if( pSpaceGrid->gridDim() != 3 ) throw Exception("SPACE ERROR: space grid dimension must be 3D", __FILE__, __FUNCTION__, __LINE__);
    if( pSpaceGrid->valueDim() != 3 ) throw Exception("SPACE ERROR: space grid value dimension must be 3D", __FILE__, __FUNCTION__, __LINE__);
    if( pScaleField->fieldDim() != 3 ) throw Exception("SPACE ERROR: scale field dimension must be 3D", __FILE__, __FUNCTION__, __LINE__);
    if( pScaleField->vectorDim() != 3 ) throw Exception("SPACE ERROR: scale field vector dimension must be 3D", __FILE__, __FUNCTION__, __LINE__);
    
    const dab::Array<unsigned int>& gridSize = pSpaceGrid->subdivisionCount();
    const dab::Array<unsigned int>& fieldSize = pScaleField->size();
    
    if(gridSize != fieldSize)
    {
        std::string gridSizeString = gridSize;
        std::string fieldSizeString = fieldSize;
        throw Exception("SPACE ERROR: size of grid " + gridSizeString + " doesn't match size of field " + fieldSizeString, __FILE__, __FUNCTION__, __LINE__ );
    }
    
    int gridDim = 3;
    
	ofVec3f wGridPointStep; // position increment for grid point in world coordinates
    const Eigen::Matrix<float, Eigen::Dynamic, 1>& gridMinPos = pSpaceGrid->minPos();
    const Eigen::Matrix<float, Eigen::Dynamic, 1>& gridMaxPos = pSpaceGrid->maxPos();
    
	// calculate grid point position increment
	for(unsigned int i=0; i<gridDim; ++i)
	{
		if(fieldSize[i] > 1) wGridPointStep[i] = ( gridMaxPos[i] - gridMinPos[i] ) / static_cast<float>(fieldSize[i] - 1);
		else wGridPointStep[i] = 0;
	}
    
	//std::cout << "wGridPointStep " << wGridPointStep << "\n";
	
	glm::vec3 wGridPoint(gridMinPos[0], gridMinPos[1], gridMinPos[2]); // grid point in world coordinates
	glm::vec3 oGridPoint; // grid point in mesh shape object coordinates
	glm::vec3 wClosestPoint; // closest point in world coordinates
	glm::vec3 oClosestPoint; // closest point in mesh shape object coordinates
    
    std::vector< Eigen::Matrix<float, Eigen::Dynamic, 1> >& fieldVectors = pSpaceGrid->vectorField().vectors();
     std::vector< Eigen::Matrix<float, Eigen::Dynamic, 1> >& scaleVectors = pScaleField->vectors();
    
	for(unsigned int z=0, fieldVectorIndex=0; z<fieldSize[2]; ++z)
	{
		wGridPoint.z = gridMinPos[2] + wGridPointStep[2] * static_cast<float>(z);
		
		for(unsigned int y=0; y<fieldSize[1]; ++y)
		{
			wGridPoint.y = gridMinPos[1] + wGridPointStep[1] * static_cast<float>(y);
			
			for(unsigned int x=0; x<fieldSize[0]; ++x, ++fieldVectorIndex)
			{
				wGridPoint.x = gridMinPos[0] + wGridPointStep[0] * static_cast<float>(x);
                
                pSpaceShape->world2object(wGridPoint, oGridPoint);
                pSpaceShape->closestPoint(oGridPoint, oClosestPoint);
                pSpaceShape->object2world(oClosestPoint, wClosestPoint);
                
                Eigen::Matrix<float, Eigen::Dynamic, 1>& fieldVector = fieldVectors[fieldVectorIndex];
                Eigen::Matrix<float, Eigen::Dynamic, 1>& scaleVector = scaleVectors[fieldVectorIndex];
                fieldVector[0] = (wClosestPoint.x - wGridPoint.x) * scaleVector[0];
                fieldVector[1] = (wClosestPoint.y - wGridPoint.y) * scaleVector[1];
                fieldVector[2] = (wClosestPoint.z - wGridPoint.z) * scaleVector[2];
                
                //std::cout << "x " << x << " y " << y << " z " << z << " wGridPoint " << wGridPoint << " oGridPoint " << oGridPoint << " oClosestPoint " << oClosestPoint << " wClosestPoint " << wClosestPoint << " fieldVec " << fieldVector.transpose() << "\n";
			}
		}
	}
}