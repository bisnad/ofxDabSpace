/** \file dab_space_alg_rtree.cpp
 */

#include "dab_space_alg_rtree.h"
#include "dab_space_proxy_object.h"
#include "dab_geom_cuboid.h"
#include "dab_geom_cuboid_tools.h"

using namespace dab;
using namespace dab::space;

RTreeAlg::RTreeAlg()
: SpaceAlg(3)
, mTree( mMinPos.rows() )
, mClosestPointType(ClosestPointAABB)
{}


RTreeAlg::RTreeAlg( const Eigen::Vector3f& pMinPos, const Eigen::Vector3f& pMaxPos )
: SpaceAlg( pMinPos, pMaxPos )
, mTree( mMinPos.rows() )
, mClosestPointType(ClosestPointAABB)
{}

RTreeAlg::~RTreeAlg()
{}

ClosestShapePointType
RTreeAlg::closestShapePointType() const
{
	return mClosestPointType;
}

void
RTreeAlg::setClosestPointType(ClosestShapePointType pClosestPointType)
{
	mClosestPointType = pClosestPointType;
}

void
RTreeAlg::updateStructure( std::vector< SpaceProxyObject* >& pObjects ) throw (Exception)
{
    if(pObjects.size() > 0 && pObjects[0]->dim() != 3) throw Exception("SPACE ERROR: object dimension " + std::to_string(pObjects[0]->dim()) + " is not 3D ", __FILE__, __FUNCTION__, __LINE__);
    
	try
	{
		mTree.RemoveAll();
        
		unsigned int objectCount = pObjects.size();
		
        //		std::cout << "objectCount " << objectCount << "\n";
		
		for(unsigned int i=0; i<objectCount; ++i)
		{
			SpaceProxyObject* proxyObject = pObjects[i];
			SpaceObject* spaceObject = proxyObject->spaceObject();
            
			if(proxyObject->visible() == true)
			{
				SpaceShape* spaceShape = dynamic_cast<SpaceShape*>( spaceObject );
				
				if(spaceShape != nullptr)
				{
					const geom::Cuboid& aabb = spaceShape->AABB();
					const glm::vec3 minPos = aabb.minPos();
					const glm::vec3 maxPos = aabb.maxPos();
					std::array<float, 3>  minArray = { minPos.x, minPos.y, minPos.z };
					std::array<float, 3>  maxArray = { maxPos.x, maxPos.y, maxPos.z };

					mTree.Insert(minArray.data(), maxArray.data(), proxyObject);
					
					// std::cout << "insert shape " << i << " aabb " << aabb << "\n";
				}
				else
				{
					float neighborRadius = proxyObject->neighborRadius();
					const Eigen::VectorXf& position = proxyObject->position();
                    float minPos[3];
                    float maxPos[3];
                    for(int d=0; d<3; ++d)
                    {
                        minPos[d] = position[0] - neighborRadius;
                        maxPos[d] = position[0] + neighborRadius;
                    }
                    
					mTree.Insert(minPos, maxPos, proxyObject);
					
					// std::cout << "insert agent " << i << " aabb " << aabb << "\n";
				}
			}
		}
	}
	catch(Exception& e)
	{
		throw;
	}
	
    //	std::cout << "RTreeAlg::updateStructure( QVector< SpaceProxyObject* >& pObjects ) end\n";
}

void
RTreeAlg::updateNeighbors( std::vector< SpaceProxyObject* >& pObjects ) throw (Exception)
{
    if(pObjects.size() > 0 && pObjects[0]->dim() != 3) throw Exception("SPACE ERROR: object dimension " + std::to_string(pObjects[0]->dim()) + " is not 3D ", __FILE__, __FUNCTION__, __LINE__);
    
	try
	{
		unsigned int dim = 3;
        
		geom::Cuboid cuboid;
        glm::vec3 closestPoint;
		glm::vec3 direction;
		float distance;
        
		unsigned int objectCount = pObjects.size();
		
        //		std::cout << "objectCount " << objectCount << "\n";
		
		for(unsigned int i=0; i<objectCount; ++i)
		{
			SpaceProxyObject* proxyObject = pObjects[i];
			SpaceObject* spaceObject = proxyObject->spaceObject();
            
			if(proxyObject->canHaveNeighbors() == true)
			{
				proxyObject->removeNeighbors();
                
                geom::Cuboid searchAABB;
                std::vector<SpaceProxyObject*> searchResults;
				
				SpaceShape* spaceShape = dynamic_cast<SpaceShape*>( spaceObject );
				
				if(spaceShape != nullptr)
				{
					searchAABB = spaceShape->AABB();
				}
				else
				{
					float neighborRadius = proxyObject->neighborRadius();
					//if(neighborRadius == 0.0) neighborRadius = 0.00001;
					const Eigen::VectorXf& position = proxyObject->position();
                    
					searchAABB = geom::Cuboid( glm::vec3( position[0] - neighborRadius, position[1] - neighborRadius, position[2] - neighborRadius ), glm::vec3( position[0] + neighborRadius, position[1] + neighborRadius, position[2] + neighborRadius ) );
				}
				
				// std::cout << "search neighbors " << searchAABB << ": ";
				
				//mTree.Search(baabb.minPos().values(), baabb.maxPos().values(), searchResults);
				const glm::vec3 minPos = searchAABB.minPos();
				const glm::vec3 maxPos = searchAABB.maxPos();
				std::array<float, 3>  minArray = { minPos.x, minPos.y, minPos.z };
				std::array<float, 3>  maxArray = { maxPos.x, maxPos.y, maxPos.z };

				mTree.Search(minArray.data(), maxArray.data(), searchResults);
				
				unsigned int resultCount = searchResults.size();
				
				// std::cout << "resultCount " << resultCount << "\n";
                
				if(mClosestPointType == ClosestPointAABB)
				{
					for(unsigned int j=0; j<resultCount; ++j)
					{
						SpaceShape* shape = dynamic_cast<SpaceShape*>( searchResults[j]->spaceObject() );
						
						if( shape == nullptr ) continue;
						
						//SpaceShape* shape = static_cast<SpaceShape*>( searchResults[j]->spaceObject() );
						
						// create temporary cuboid geometry for closest point calculation
						cuboid = shape->AABB();
                        
						// calculate closest point from space obejct to cuboid surface
                        const Eigen::VectorXf& proxyPos = proxyObject->position();
						glm::vec3 searchPos(proxyPos[0], proxyPos[1], proxyPos[2]);
						geom::CuboidTools::get().closestPoint( searchPos, closestPoint, cuboid);
						
						direction = closestPoint - searchPos;
						distance = glm::length(direction);
						
						bool createNeighborResult = proxyObject->addNeighbor(shape, distance, Eigen::Vector3f(direction[0], direction[1], direction[2]));
					}
				}
				else if(mClosestPointType == ClosestPointShape)
				{
					for(unsigned int j=0; j<resultCount; ++j)
					{
						SpaceShape* shape = dynamic_cast<SpaceShape*>( searchResults[j]->spaceObject() );
                        
						if( shape == nullptr ) continue;
						
						//SpaceShape* shape = static_cast<SpaceShape*>( searchResults[j]->spaceObject() );
                        
						// calculate closest point from space obejct to shape
                        const Eigen::VectorXf& proxyPos = proxyObject->position();
						glm::vec3 searchPos(proxyPos[0], proxyPos[1], proxyPos[2]);
                        
						shape->closestPoint(searchPos, closestPoint);
						direction = closestPoint - searchPos;
						distance = glm::length(direction);
						bool createNeighborResult = proxyObject->addNeighbor(shape, distance, Eigen::Vector3f(direction[0], direction[1], direction[2]));
					}
				}
			}
		}
	}
	catch(Exception& e)
	{
        e += Exception("SPACE ERROR: failed to update RTreeAlg", __FILE__, __FUNCTION__, __LINE__);
		throw;
	}
	
    //	std::cout << "RTreeAlg::updateNeighbors( QVector< SpaceProxyObject* >& pObjects ) end\n";
}

RTreeAlg::operator std::string() const
{
    return info();
}

std::string
RTreeAlg::info() const
{
    std::stringstream stream;
    
    stream << "RTreeAlg\n";
    stream << SpaceAlg::info() << "\n";
    
	return stream.str();
}