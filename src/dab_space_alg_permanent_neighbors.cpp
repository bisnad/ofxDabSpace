/** \file dab_space_alg_permanent_neighbors.cpp
*/

#include "dab_space_alg_permanent_neighbors.h"
#include "dab_space_neighbor_relation.h"
#include "dab_space_proxy_object.h"

using namespace dab;
using namespace dab::space;

PermanentNeighborsAlg::PermanentNeighborsAlg()
: SpaceAlg(2)
{}

PermanentNeighborsAlg::PermanentNeighborsAlg( unsigned int pDim )
: SpaceAlg( pDim )
{}

PermanentNeighborsAlg::PermanentNeighborsAlg( const Eigen::Vector3f& pMinPos, const Eigen::Vector3f& pMaxPos )
: SpaceAlg( pMinPos, pMaxPos )
{}

PermanentNeighborsAlg::~PermanentNeighborsAlg()
{}

void
PermanentNeighborsAlg::updateNeighbors( std::vector< SpaceProxyObject* >& pObjects ) throw (Exception)
{
	try
	{
		unsigned int dim = mMinPos.rows();
        
		//std::cout << "ManualSpace::updateNeighbors()\n";
        
		SpaceNeighborRelation* neighborRelation;
		SpaceProxyObject* object;
		SpaceObject* neighbor;
		float distance;
		Eigen::Vector3f direction(dim);
        
		// gather objects for which to calculate neighbors
		unsigned int objectCount = pObjects.size();
		for(unsigned int objectNr=0; objectNr<objectCount; ++objectNr)
		{
			object = pObjects[objectNr];
            
			if(object->canHaveNeighbors() == true)
			{
                std::vector<SpaceNeighborRelation*>& neighborRelations = object->neighborGroup()->neighborRelations();
				unsigned int neighborCount = neighborRelations.size();
				
				for(unsigned int neighborNr=0; neighborNr < neighborCount; ++neighborNr)
				{
					neighborRelation = neighborRelations[neighborNr];
					neighbor = neighborRelation->neighbor();
					
					direction = neighbor->position() - object->position();
					distance = direction.norm();
					
					neighborRelation->setDirection(direction);
					neighborRelation->setDistance(distance);
				}
			}
		}
        
		//std::cout << "ManualSpace::updateNeighbors() done\n";
	}
	catch(Exception& e)
	{
        e += Exception("SPACE ERROR: failed to update neighbors", __FILE__, __FUNCTION__, __LINE__);
		throw e;
	}
}


PermanentNeighborsAlg::operator std::string() const
{
    std::stringstream stream;
    
    stream << "PermanentNeighborsAlg\n";
    
    stream << SpaceAlg::operator std::string();
    
	return stream.str();
}