/** \file dab_space_alg_grid.cpp
 */

#include "dab_space_alg_grid.h"
#include "dab_space_neighbor_relation.h"
#include "dab_space_proxy_object.h"

using namespace dab;
using namespace dab::space;

GridAlg::GridAlg()
: SpaceAlg(1)
, mGrid(nullptr)
{
	mGrid = new SpaceGrid( 1, dab::Array<unsigned int>( {1} ), Eigen::VectorXf(1), Eigen::VectorXf(1) );
    
    Eigen::VectorXf minPos(1);
    Eigen::VectorXf maxPos(1);
    
    minPos[0] = 0.0;
    maxPos[0] = 1.0;
    
    mGrid->setMinPos(minPos);
    mGrid->setMaxPos(maxPos);
    
	mGridOwner = true;
}

GridAlg::GridAlg(unsigned int pValueDim, const dab::Array<unsigned int>& pSubdivisionCount, const Eigen::VectorXf& pMinPos, const  Eigen::VectorXf& pMaxPos, GridNeighborMode pNeighborMode, GridUpdateMode pUpdateMode) throw (Exception)
: SpaceAlg(pMinPos, pMaxPos)
, mGrid(nullptr)
, mNeighborMode(pNeighborMode)
, mUpdateMode(pUpdateMode)
{
    try
    {
        mGrid = new SpaceGrid( pValueDim, pSubdivisionCount, pMinPos, pMaxPos );
        mGridOwner = true;
    }
    catch (Exception& e)
    {
        e += Exception("SPACE ERROR: failed to create grid alg", __FILE__, __FUNCTION__, __LINE__);
        throw e;
    }
}

GridAlg::GridAlg(SpaceGrid* pSpaceGrid, GridNeighborMode pNeighborMode, GridUpdateMode pUpdateMode)
: SpaceAlg(pSpaceGrid->minPos(), pSpaceGrid->maxPos())
, mNeighborMode(pNeighborMode)
, mUpdateMode(pUpdateMode)
{
	mGrid = pSpaceGrid;
	mGridOwner = false;
}

GridAlg::~GridAlg()
{
	if(mGridOwner == true) delete mGrid;
}

bool
GridAlg::gridOwner() const
{
	return mGridOwner;
}

GridAlg::GridNeighborMode
GridAlg::neighborMode() const
{
	return mNeighborMode;
}

GridAlg::GridUpdateMode
GridAlg::updateMode() const
{
	return mUpdateMode;
}

SpaceGrid&
GridAlg::grid()
{
	return *mGrid;
}

void
GridAlg::setGrid( SpaceGrid* pSpaceGrid ) throw (Exception)
{
    if( pSpaceGrid->gridDim() != mGrid->gridDim() ) throw Exception("SPACE ERROR: grid dimension mismatch", __FILE__, __FUNCTION__, __LINE__);
    if( pSpaceGrid->valueDim() != mGrid->valueDim() ) throw Exception("SPACE ERROR: value dimension mismatch", __FILE__, __FUNCTION__, __LINE__);
    
	if(mGridOwner == true) delete mGrid;
	
	mGrid = pSpaceGrid;
	mGridOwner = false;
}

void
GridAlg::updateStructure( std::vector< SpaceProxyObject* >& pObjects ) throw (Exception)
{
	try
	{
		if(mUpdateMode == NoUpdateMode) return;
        
		int dim = mGrid->dim();
		unsigned int objectCount = pObjects.size();
		SpaceProxyObject* object;
		unsigned int neighborCount;
        
		if(mUpdateMode == NearestReplaceMode)
		{
			math::VectorField<float>& vectorField = mGrid->vectorField();
			unsigned int gridIndex;
            
            // clear grid
            Eigen::VectorXf clearValue(dim);
            clearValue.setConstant(0.0);
			mGrid->setValues(clearValue);
			
			for(unsigned int objectNr = 0; objectNr < objectCount; ++objectNr)
			{
				object = pObjects[objectNr];
				
				if(object->canHaveNeighbors() == false) continue;
				
                std::vector<SpaceNeighborRelation*>& neighbors = object->neighborGroup()->neighborRelations();
				neighborCount = neighbors.size();
				
				for(unsigned int neighborNr = 0; neighborNr < neighborCount; ++neighborNr )
				{
					SpaceNeighborRelation* neighborRel = neighbors[neighborNr];
					const Eigen::VectorXf& neigborPos = neighborRel->neighbor()->position();
					const Eigen::VectorXf& neigborVal = neighborRel->value();
					mGrid->position2index(neigborPos, gridIndex);
					vectorField.set(gridIndex, neigborVal);
				}
			}
		}
		else if(mUpdateMode == NearestAddMode)
		{
			math::VectorField<float>& vectorField = mGrid->vectorField();
			unsigned int gridIndex;
			
			for(unsigned int objectNr = 0; objectNr < objectCount; ++objectNr)
			{
				object = pObjects[objectNr];
				
				if(object->canHaveNeighbors() == false) continue;
				
                std::vector<SpaceNeighborRelation*>& neighbors = object->neighborGroup()->neighborRelations();
				neighborCount = neighbors.size();
				
				for(unsigned int neighborNr = 0; neighborNr < neighborCount; ++neighborNr )
				{
					SpaceNeighborRelation* neighborRel = neighbors[neighborNr];
					const Eigen::VectorXf& neigborPos = neighborRel->neighbor()->position();
					const Eigen::VectorXf& neigborVal = neighborRel->value();
					mGrid->position2index(neigborPos, gridIndex);
					vectorField.add(gridIndex, neigborVal);
				}
			}
		}
		else if(mUpdateMode == AvgReplaceMode)
		{
            // clear grid
            Eigen::VectorXf clearValue(dim);
            clearValue.setConstant(0.0);
			mGrid->setValues(clearValue);
			
			for(unsigned int objectNr = 0; objectNr < objectCount; ++objectNr)
			{
				object = pObjects[objectNr];
				
				if(object->canHaveNeighbors() == false) continue;
				
                std::vector<SpaceNeighborRelation*>& neighbors = object->neighborGroup()->neighborRelations();
				neighborCount = neighbors.size();
				
				for(unsigned int neighborNr = 0; neighborNr < neighborCount; ++neighborNr )
				{
					SpaceNeighborRelation* neighborRel = neighbors[neighborNr];
					const Eigen::VectorXf& neigborPos = neighborRel->neighbor()->position();
					const Eigen::VectorXf& neigborVal = neighborRel->value();
					mGrid->setValue(neigborPos, neigborVal);
				}
			}
		}
		else if(mUpdateMode == AvgAddMode)
		{
			for(unsigned int objectNr = 0; objectNr < objectCount; ++objectNr)
			{
				object = pObjects[objectNr];
				
				if(object->canHaveNeighbors() == false) continue;
				
                std::vector<SpaceNeighborRelation*>& neighbors = object->neighborGroup()->neighborRelations();
				neighborCount = neighbors.size();
				
				for(unsigned int neighborNr = 0; neighborNr < neighborCount; ++neighborNr )
				{
					SpaceNeighborRelation* neighborRel = neighbors[neighborNr];
					const Eigen::VectorXf& neigborPos = neighborRel->neighbor()->position();
					const Eigen::VectorXf& neigborVal = neighborRel->value();
					mGrid->changeValue(neigborPos, neigborVal);
				}
			}
		}
	}
	catch(Exception& e)
	{
        e += Exception("SPACE ERROR: failed to update grid structure", __FILE__, __FUNCTION__, __LINE__);
		throw e;
	}
}

void
GridAlg::updateNeighbors( std::vector< SpaceProxyObject* >& pObjects ) throw (Exception)
{
	try
	{
		//std::cout << "GridAlg updateNeighbors() begin\n";
		
		unsigned int dim = mMinPos.rows();
        
		// remove all neighbors
		unsigned int objectCount = pObjects.size();
		for(unsigned int i=0; i<objectCount; ++i) if(pObjects[i]->canHaveNeighbors() == true) pObjects[i]->removeNeighbors();
        
		// remove temporarily created space objects
		unsigned int tmpSpaceObjectCount = mTmpSpaceObjects.size();
		for(unsigned int i=0; i<tmpSpaceObjectCount; ++i) delete mTmpSpaceObjects[i];
		mTmpSpaceObjects.clear();
        
		// create new neighbors
		SpaceProxyObject* proxyObject;
		SpaceObject* spaceObject;
		const dab::Array<unsigned int>& subdivisionCount = mGrid->subdivisionCount();
		unsigned int gridPosCount = static_cast<unsigned int>( pow( 2.0f, static_cast<float>( dim ) ) );
		
		const math::VectorField<float>& vectorField = mGrid->vectorField();
		const dab::Array<unsigned int>& indexOffset = vectorField.indexOffset();
        const std::vector< Eigen::Matrix<float, Eigen::Dynamic, 1> >& fieldVectors = vectorField.vectors();
		Eigen::VectorXf value(mGrid->valueDim());
		Eigen::VectorXf direction(mGrid->gridDim());
		float distance;
		int maxNeighborCount;
		float neighborRadius;
		
		/*
         std::cout << "subdivision count " <<  subdivisionCount << "\n";
         std::cout << "minPos " <<  mMinPos << " maxPos " << mMaxPos << "\n";
         std::cout << "world2gridPosScale " << world2gridPosScale << "\n";
         */
		
		for(unsigned int i=0; i<objectCount; ++i)
		{
			//std::cout << "update neighbors for " << i << "\n";
            
			proxyObject = pObjects[i];
			
			const Eigen::VectorXf& objectPosition = proxyObject->position();
			
			maxNeighborCount = proxyObject->maxNeighborCount();
			neighborRadius = proxyObject->neighborRadius();
			
			//std::cout << "pos " << objectPosition << " minPos " << mMinPos << " maxPos " << mMaxPos << "\n";
            
			if(proxyObject->canHaveNeighbors() == true)
			{
				// skip objects outside of this space
				bool skipObject = false;
				for(unsigned int j=0; j<dim; ++j)
				{
					if(objectPosition[j] < mMinPos[j] || objectPosition[j] > mMaxPos[j])
					{
						skipObject = true;
						break;
					}
				}
				if(skipObject == true) continue;
				
				if(mNeighborMode ==CellLocationMode)
				{
                    //					std::cout << "cell loc mode begin\n";
                    
					// neighbor relation contains value of cell at position of space object
					unsigned int gridIndex;
					Eigen::VectorXf& objectPosition = proxyObject->position();
					mGrid->position2index( objectPosition, gridIndex );
					value = mGrid->value( gridIndex );
					direction.setConstant(0.0);
					distance = 0.0;
					
                    //					std::cout << "pos " << objectPosition << " index " << gridIndex << " value " << value << "\n";
					
					spaceObject = new SpaceObject( objectPosition );
					mTmpSpaceObjects.push_back(spaceObject);
					proxyObject->addNeighbor(spaceObject, value, direction, distance);
					
                    //					std::cout << "cell loc mode end\n";
				}
				else if(mNeighborMode == AvgLocationMode)
				{
					// linear interpolation of grid values at grid nodes for space object position
					// 1 neighbor relation: position at space object position, value is interpolated grid value
					// example: grid values represent motion vectors from camera
					// determine motion vector at position of space object (e.g. agent position parameter) -> results in force acting on agent
                    
					//std::cout << "AvgLocationMode\n";
					
					//spaceObject = new SpaceObject(mGrid->value(proxyObject->position()));
					spaceObject = new SpaceObject( proxyObject->position() );
					mGrid->value( proxyObject->position(), value );
					
					direction.setConstant(0.0);
					distance = 0.0;
					
					mTmpSpaceObjects.push_back(spaceObject);
					proxyObject->addNeighbor(spaceObject, value, direction, distance);
				}
				else if(mNeighborMode == AvgRegionMode)
				{
					// create single neighbor relation whose position is the weighted (by value at grid nodes) position of each grid node within the space object's search radius and whose value is the average of all grid node values within search radius.
					
					//std::cout << "AvgRegionMode\n";
                    
					std::vector< Eigen::Matrix<float, Eigen::Dynamic, 1> >& fieldVectors = mGrid->vectorField().vectors();
                    
					float neighborRadius = proxyObject->neighborRadius();
                    Eigen::VectorXf neighborRadiusVec(dim);
                    neighborRadiusVec.setConstant(neighborRadius);
                    
					dab::Array<unsigned int> startIndexPos = mGrid->position2index( objectPosition - neighborRadiusVec );
					dab::Array<unsigned int> endIndexPos = mGrid->position2index( objectPosition + neighborRadiusVec );
                    Eigen::VectorXf indexPos( mGrid->gridDim() );
					unsigned int index;
					unsigned int valueCount = 0;
					float totalSumValue = 0;
					float sumValue;
                    dab::Array<float> totalPosValue( mGrid->gridDim() );
					unsigned int gridDim = mGrid->gridDim();
					const dab::Array<unsigned int>& gridElementCount = mGrid->subdivisionCount();
					
                    //					std::cout << "objectPos " << objectPosition << " neighborRadius " << neighborRadius << "\n";
                    //					std::cout << "objectIndexPox " << mGrid->position2index( objectPosition ) << "\n";
                    //					std::cout << "startIndexPos " << startIndexPos << " endIndexPos " << endIndexPos << "\n";
                    //					std::cout << "fieldVectorCount " << mGrid->vectorField().vectorCount() << "\n";
					
					unsigned int yIndex, zIndex;
					
					if(gridElementCount.size() == 3)
					{
						for(unsigned int z=startIndexPos[2]; z<=endIndexPos[2]; ++z)
						{
							indexPos[2] = static_cast<float>( z );
							zIndex = z * gridElementCount[0] * gridElementCount[1];
                            
							for(unsigned int y=startIndexPos[1]; y<=endIndexPos[1]; ++y)
							{
								indexPos[1] = static_cast<float>( y );
								yIndex = y * gridElementCount[0];
                                
								for(unsigned int x=startIndexPos[0]; x<=endIndexPos[0]; ++x, ++valueCount)
								{
									indexPos[0] = static_cast<float>( x );
									
									index = x + yIndex + zIndex;
									value += fieldVectors[index];
									sumValue = fieldVectors[index].sum();
									totalSumValue += sumValue;
									
									for(unsigned int d=0; d<gridDim; ++d)
									{
										totalPosValue[d] += indexPos[d] * sumValue;
									}
									
									//totalPosValue += indexPos * value;
									//std::cout << "x " << x << " y " << y << " z " << z << " indexPos " << indexPos << " index " << index << " value " << value << " totalValue " << totalValue << " totalPosValue " << totalPosValue << "\n";
								}
							}
						}
						
						//std::cout << "totalValue " << totalValue << "\n";
						
						if(totalSumValue > 0.0)
						{
                            for(unsigned int d=0; d<gridDim; ++d) totalPosValue[d] /= totalSumValue;
							value /= static_cast<float>( valueCount );
							
							//std::cout << "totalPosValue " << totalPosValue << "\n";
							
							Eigen::VectorXf resultPosition = mGrid->index2position( totalPosValue );
							
							//std::cout << "resultPosition " << resultPosition << "\n";
							
							spaceObject = new SpaceObject( resultPosition );
							direction = resultPosition - objectPosition;
							distance = direction.norm();
							
							mTmpSpaceObjects.push_back(spaceObject);
							proxyObject->addNeighbor(spaceObject, value, direction, distance);
							
							//std::cout << "added neighbor to " << i << " : nCount " << proxyObject->neighborGroup()->neighborCount() << "\n";
						}
					}
					else if(gridElementCount.size() == 2)
					{
						//std::cout << "agentPos " << objectPosition << " index " << mGrid->position2index(objectPosition)  << " startIndex " << startIndexPos << " endIndex " << endIndexPos << "\n";
						
						for(unsigned int y=startIndexPos[1]; y<endIndexPos[1]; ++y)
						{
							indexPos[1] = static_cast<float>( y );
							yIndex = y * gridElementCount[0];
							
							for(unsigned int x=startIndexPos[0]; x<endIndexPos[0]; ++x, ++valueCount)
							{
								indexPos[0] = static_cast<float>( x );
								
								index = x + yIndex;
								//value += *( fieldVectors[index] );
								sumValue = fieldVectors[index].sum();
								totalSumValue += sumValue;
								
								for(unsigned int d=0; d<gridDim; ++d)
								{
									totalPosValue[d] += indexPos[d] * sumValue;
								}
								
								//std::cout << "indexPos " << indexPos << " index " << index << " checkIndex " << mGrid->vectorField().calcIndex( indexPos ) << " checkIndexPos " << mGrid->vectorField().calcIndex( index ) << "\n";
								//std::cout << "pos " << mGrid->index2position( indexPos ) << "\n";
								//std::cout << "pos2 " << mGrid->index2position( math::Vector2<unsigned int>( static_cast<unsigned int>( indexPos.c[0] ), static_cast<unsigned int>( indexPos.c[1] ) ) ) << "\n";
								
								//totalPosValue += indexPos * value;
								//std::cout << "x " << x << " y " << y << " indexPos " << indexPos << " index " << index << " value " << *( fieldVectors[index] ) << " totalSumValue " << totalSumValue << " totalPosValue " << totalPosValue << "\n";
							}
						}
						
						//std::cout << "totalValue " << totalValue << "\n";
						
						if(totalSumValue > 0.0)
						{
							for(unsigned int d=0; d<gridDim; ++d) totalPosValue[d] /= totalSumValue;
							value /= static_cast<float>( valueCount );
							
							//std::cout << "totalPosValue " << totalPosValue << "\n";
							
							Eigen::VectorXf resultPosition = mGrid->index2position( totalPosValue );
							
							//std::cout << "resultPosition " << resultPosition << "\n";
							
							spaceObject = new SpaceObject( resultPosition );
							direction = resultPosition - objectPosition;
							distance = direction.norm();
							
							
							mTmpSpaceObjects.push_back(spaceObject);
							proxyObject->addNeighbor(spaceObject, value, direction, distance);
							
							//std::cout << "added neighbor to " << i << " : nCount " << proxyObject->neighborGroup()->neighborCount() << "\n";
						}
					}
					else
					{
                        throw Exception("SPACE ERROR: grid dimensions other than 2D or 3D not yet supported", __FILE__, __FUNCTION__, __LINE__);
						// TODO: create a more generic algorithms for dealing with lower or higher dimensional grids
					}
                    
				}
				else if(mNeighborMode == GridLocationMode)
				{
					// space object searches (within search radius) for grid node possessing the highest value
					// 1 neighbor relation: position is at the grid node with highest value, direction towards this grid node, value : highest value
					// example: agents are attracted towards high grayscale pixel values from camera
					
					//std::cout << "GridLocationMode\n";
                    
					Eigen::VectorXf gridWorldPosition(dim);
                    dab::Array<unsigned int> gridGridPosition(dim);
                    
					Eigen::VectorXf objectGridPosition(dim);
                    
					Eigen::VectorXf world2gridPosScale(dim);
					for(unsigned int i=0; i<dim; ++i) world2gridPosScale[i] = static_cast<float>(subdivisionCount[i]) / (mMaxPos[i] - mMinPos[i]);
                    
					Eigen::VectorXf grid2worldPosScale(dim);
					for(unsigned int i=0; i<dim; ++i) grid2worldPosScale[i] = 1.0 / world2gridPosScale[i];
                    
					// transform object world position into object grid position
					const Eigen::VectorXf& objectWorldPosition = objectPosition;
					objectGridPosition = (objectWorldPosition - mMinPos) * world2gridPosScale;
					
					//std::cout << i << " objectWorldPos " << objectWorldPosition << " objectGridPos " << objectGridPosition << "\n";
                    
					unsigned int index;
					unsigned int maskBits;
                    
					// calculate neighboring grid positions
					for(unsigned int j=0; j<gridPosCount; ++j)
					{
						// calculate index into vector array
						maskBits = 1;
						
						for(int z=0; z<dim; ++z)
						{
							gridGridPosition[z] = j & maskBits ? ceil(objectGridPosition[z]) : floor(objectGridPosition[z]);
							gridWorldPosition[z] = static_cast<float>( gridGridPosition[z] ) * grid2worldPosScale[z] + mMinPos[z];
							
							maskBits = maskBits << 1;
						}
						
						//std::cout << j << " gridGridPosition " << gridGridPosition << " gridWorldPosition " << gridWorldPosition << "\n";
                        
						//spaceObject = new SpaceObject(mGrid->gridValue(gridGridPosition));
                        
						spaceObject = new SpaceObject(gridWorldPosition);
						value = mGrid->gridValue(gridGridPosition);
						direction = gridWorldPosition - objectWorldPosition;
						distance = direction.norm();
						
						mTmpSpaceObjects.push_back(spaceObject);
						
						//std::cout << "direction " << direction << " distance " << distance << "\n";
						
						//proxyObject->addNeighbor(spaceObject, distance, direction);
						proxyObject->addNeighbor(spaceObject, value, direction, distance);
					}
				}
				else if( mNeighborMode == PeakSearchMode )
				{
					// space object searches (within search radius) for grid node possessing the highest value
					// 1 neighbor relation: position is at the grid node with highest value, direction towards this grid node, value : highest value
					// example: agents are attracted towards high grayscale pixel values from camera
					
					dab::Array<unsigned int> objectGridPos(dim);
					mGrid->position2index( objectPosition, objectGridPos );
                    
					//std::cout << "objectPos " << objectPosition << " objectGridPos " << objectGridPos << "\n";
					
                    Eigen::VectorXf cellSize(dim);
					for(unsigned int i=0; i<dim; ++i) cellSize[i] = (mMaxPos[i] - mMinPos[i]) / static_cast<float>(subdivisionCount[i]);
					
					//std::cout << "cellSize " << cellSize << " minPos " << mMinPos << " maxPos " << mMaxPos << " subdivisions " << subdivisionCount << "\n";
					
					dab::Array<int> cellSearchRange(dim);
					for(unsigned int i=0; i<dim; ++i) cellSearchRange[i] = ceil( ( neighborRadius - 0.5 * cellSize[i] ) / cellSize[i] );
					
					//std::cout << "cellSearchRange " << cellSearchRange << " neighborRadius " << neighborRadius << "\n";
					
					dab::Array<int> searchStartPos(dim);
					dab::Array<int> searchEndPos(dim);
					for(unsigned int i=0; i<dim; ++i)
					{
						searchStartPos[i] = objectGridPos[i] - cellSearchRange[i];
						searchEndPos[i] = objectGridPos[i] + cellSearchRange[i];
						
						if( searchStartPos[i] < 0 ) searchStartPos[i] = 0;
						if( searchEndPos[i] >= subdivisionCount[i] ) searchEndPos[i] = subdivisionCount[i] - 1;
					}
					
					//std::cout << "searchStartPos " << searchStartPos << " searchEndPos " << searchEndPos << "\n";
					
					int searchPosCount = 1;
					for(unsigned int i=0; i<dim; ++i) searchPosCount *= searchEndPos[i] - searchStartPos[i];
					
					//std::cout << "searchPosCount " << searchPosCount << "\n";
					
					int neighborCount = maxNeighborCount;
					if( neighborCount > searchPosCount ) neighborCount = searchPosCount;
					
                    std::vector< unsigned int > neighborAbsIndices( neighborCount, 0 );
					std::vector< float > neighborGridValues( neighborCount, 0.0 );
					int smallestNeighborIndex = 0;
					int tmpSmallestNeighborIndex;
					float tmpSmallestGridValue;
                    
					// go through all grid points from searchStartPos to searchEndPos and look for heighest values
					int absStartIndex = vectorField.calcIndex( searchStartPos );
					int absEndIndex = vectorField.calcIndex( searchEndPos );
					dab::Array<int> curRelIndex( searchStartPos );
					int absIndex = absStartIndex;
					//const Eigen::VectorXf* valueVec;
					
					while( absIndex < absEndIndex )
					{
						// first dimension
						for(curRelIndex[0] = searchStartPos[0]; curRelIndex[0] <= searchEndPos[0]; curRelIndex[0] += 1, absIndex++)
						{
							//std::cout << "absIndex " << absIndex << " absStartIndex " << absStartIndex << " absEndIndex " << absEndIndex << " relIndex " << curRelIndex << " searchStartPos " << searchStartPos << " searchEndPos " << searchEndPos << "\n";
							
							
							const Eigen::VectorXf& valueVec = fieldVectors[absIndex];
							if( valueVec.norm() >= neighborGridValues[smallestNeighborIndex] )
							{
								//std::cout << "val " << valueVec->length() << " old curMinVal " << neighborGridValues[smallestNeighborIndex]  << " curMinIndex " << smallestNeighborIndex << "\n";
								
								neighborAbsIndices[smallestNeighborIndex] = absIndex;
								neighborGridValues[smallestNeighborIndex] = valueVec.norm();
								
								// find new smallest neighor index
								tmpSmallestGridValue = FLT_MAX;
								tmpSmallestNeighborIndex = 0;
								for(int i=0; i<neighborCount; ++i)
								{
									if( tmpSmallestGridValue > neighborGridValues[i] )
									{
										tmpSmallestGridValue = neighborGridValues[i];
										tmpSmallestNeighborIndex = i;
									}
								}
								
								smallestNeighborIndex = tmpSmallestNeighborIndex;
								
								//std::cout << "val " << valueVec->length() << " new curMinVal " << neighborGridValues[smallestNeighborIndex]  << " curMinIndex " << smallestNeighborIndex << "\n";
							}
						}
						
						if(absIndex >= absEndIndex) break;
						
						// successive dimensions
						for(unsigned int d = 0; d < dim, curRelIndex[d] > searchEndPos[d]; ++d)
						{
							curRelIndex[d] = searchStartPos[d];
							curRelIndex[d + 1] += 1;
							absIndex -= indexOffset[d] * ( searchEndPos[d] - searchStartPos[d] + 1);
							absIndex += indexOffset[d + 1];
						}
					}
					
                    //					std::cout << "before sorting\n";
                    //					for(int i=0; i<neighborCount; ++i)
                    //					{
                    //						std::cout << "( " << neighborAbsIndices[i] << " : " << neighborGridValues[i] << ")";
                    //					}
                    //					std::cout << "\n";
					
					// sort index and value vectors according to values (bubblesort)
					bool sorted = false;
					float tmpGridValue;
					unsigned int tmpGridIndex;
					while( sorted == false )
					{
						sorted = true;
						
						for(int i=1; i<neighborCount; ++i)
						{
							if( neighborGridValues[i-1] < neighborGridValues[i] )
							{
								tmpGridValue = neighborGridValues[i-1];
								tmpGridIndex = neighborAbsIndices[i-1];
								
								neighborGridValues[i-1] = neighborGridValues[i];
								neighborAbsIndices[i-1] = neighborAbsIndices[i];
								
								neighborGridValues[i] = tmpGridValue;
								neighborAbsIndices[i] = tmpGridIndex;
								
								sorted = false;
							}
						}
					}
					
                    //					std::cout << "after sorting\n";
                    //					for(int i=0; i<neighborCount; ++i)
                    //					{
                    //						std::cout << "( " << neighborAbsIndices[i] << " : " << neighborGridValues[i] << ")";
                    //					}
                    //					std::cout << "\n";
					
					// transform indices and highest grid values into neighbor relationships
					dab::Array<unsigned int> cellIndex( dim );
                    Eigen::VectorXf cellPosition(dim);
					for(int i=0; i<neighborCount; ++i)
					{
                        
						//std::cout << "i " << i << " neighborAbsIndices " << neighborAbsIndices[i] << "\n";
						
						cellIndex = vectorField.calcIndex( neighborAbsIndices[i] );
						
						//std::cout << "cellIndex " << cellIndex << "\n";
						
						cellPosition = mGrid->index2position( cellIndex );
						
						//std::cout << "cellPosition " << cellPosition << "\n";
						
						value = mGrid->gridValue(cellIndex);
						
						//std::cout << "value " << value << "\n";
						
						direction = cellPosition - objectPosition;
						
						//std::cout << "direction " << direction << "\n";
						
						distance = direction.norm();
						
						//std::cout << "distance " << distance << "\n";
						
						spaceObject = new SpaceObject( cellPosition );
						
						//std::cout << "1\n";
						
						mTmpSpaceObjects.push_back(spaceObject);
						
						//std::cout << "2\n";
						
						proxyObject->addNeighbor(spaceObject, value, direction, distance);
						
						//std::cout << "3\n";
					}
				}
				else if(mNeighborMode == CentroidSearchMode)
				{
					// create single neighbor relation whose position is the weighted (by value at grid nodes) position of each grid node within the space object's search radius and whose value is the average of all grid node values within search radius.
					
					//std::cout << "CentroidSearchMode\n";
                    
                    Eigen::VectorXf neighborRadiusVec(dim);
                    neighborRadiusVec.setConstant(neighborRadius);
                    
					dab::Array<unsigned int> searchStartPos = mGrid->position2index( objectPosition - neighborRadiusVec );
					dab::Array<unsigned int> searchEndPos = mGrid->position2index( objectPosition + neighborRadiusVec );
					Eigen::VectorXf indexPos( dim );
					unsigned int index;
					unsigned int valueCount = 0;
					float totalSumValue = 0;
					float sumValue;
                    dab::Array<float> totalIndexValue( dim );
					
					// go through all grid points from searchStartPos to searchEndPos and calculate centroid
					int absStartIndex = vectorField.calcIndex( searchStartPos );
					int absEndIndex = vectorField.calcIndex( searchEndPos );
					dab::Array<int> curRelIndex( searchStartPos );
					int absIndex = absStartIndex;
					dab::Array<unsigned int> cellIndex( dim );
					
					//std::cout << "searchStartPos " << searchStartPos << " searchEndPos " << searchEndPos << "\n";
                    
					while( absIndex < absEndIndex )
					{
						// first dimension
						for(curRelIndex[0] = searchStartPos[0]; curRelIndex[0] <= searchEndPos[0]; curRelIndex[0] += 1, absIndex++)
						{
							//std::cout << "absIndex " << absIndex << " absStartIndex " << absStartIndex << " absEndIndex " << absEndIndex << " relIndex " << curRelIndex << " searchStartPos " << searchStartPos << " searchEndPos " << searchEndPos << "\n";
							
							const Eigen::Vector3f& valueVec = fieldVectors[absIndex];
							value += valueVec;
							sumValue = valueVec.sum();
							totalSumValue += sumValue;
							vectorField.calcIndex( absIndex, cellIndex );
							
							for(unsigned int d=0; d<dim; ++d)
							{
								totalIndexValue[d] += static_cast<float>( cellIndex[d] ) * sumValue;
							}
						}
						
						if(absIndex >= absEndIndex) break;
						
						// successive dimensions
						for(unsigned int d = 0; d < dim, curRelIndex[d] > searchEndPos[d]; ++d)
						{
							curRelIndex[d] = searchStartPos[d];
							curRelIndex[d + 1] += 1;
							absIndex -= indexOffset[d] * ( searchEndPos[d] - searchStartPos[d] + 1);
							absIndex += indexOffset[d + 1];
						}
					}
                    
					if(totalSumValue > 0.0)
					{
						for(unsigned int d=0; d<dim; ++d) totalIndexValue[d] /= totalSumValue;
						value /= static_cast<float>( valueCount );
						
                        Eigen::VectorXf resultPosition = mGrid->index2position( totalIndexValue );
						
						//std::cout << "res pos " << resultPosition << " value " << value << "\n";
                        
						spaceObject = new SpaceObject( resultPosition );
						direction = resultPosition - objectPosition;
						distance = direction.norm();
						
						mTmpSpaceObjects.push_back(spaceObject);
						proxyObject->addNeighbor(spaceObject, value, direction, distance);
						
						//std::cout << "added neighbor to " << i << " : nCount " << proxyObject->neighborGroup()->neighborCount() << "\n";
					}
				}
			}
		}
		
		// debug
		/*
         std::cout << "grid space debug\n";
         for(unsigned int i=0; i<objectCount; ++i)
         {
         proxyObject = pObjects[i];
         std::cout << i << " nCount " << proxyObject->neighborGroup()->neighborCount() << "\n";
         }
         */
        
		//std::cout << "GridAlg updateNeighbors() end\n";
	}
	catch(Exception& e)
	{
        e += Exception("SPAVE ERROR: failed to update neighbors", __FILE__, __FUNCTION__, __LINE__);
		throw;
	}
}

GridAlg::operator std::string() const
{
    return info();
}

std::string
GridAlg::info() const
{
    std::stringstream ss;
    
    ss << "GridAlg\n";
    ss << SpaceAlg::info();

	return ss.str();
}