/** \file dab_space_alg_kdtree.cpp
 */

#include "dab_space_alg_kdtree.h"
#include "dab_space_proxy_object.h"

using namespace dab;
using namespace dab::space;

KDTreeAlg::KDTreeAlg()
: SpaceAlg(2)
{
	mTree = kd_create( mMinPos.rows() );
}

KDTreeAlg::KDTreeAlg( unsigned int pDim )
: SpaceAlg( pDim )
{
	mTree = kd_create( mMinPos.rows() );
}

KDTreeAlg::KDTreeAlg( const Eigen::VectorXf& pMinPos, const Eigen::VectorXf& pMaxPos )
: SpaceAlg( pMinPos, pMaxPos )
{
	mTree = kd_create( mMinPos.rows() );
}

KDTreeAlg::~KDTreeAlg()
{
	kd_free(mTree);
}

void
KDTreeAlg::updateStructure( std::vector< SpaceProxyObject* >& pObjects ) throw (Exception)
{
	try
	{
		kd_clear( mTree );
        
		unsigned int dim = mMinPos.rows();
		unsigned int objectCount = pObjects.size();
		std::vector<double> doublePos(dim);
		
		for(unsigned int oI=0; oI < objectCount; ++oI)
		{
			SpaceProxyObject* proxyObject = pObjects[oI];
			const Eigen::VectorXf& position = proxyObject->position();
			for(unsigned int d=0; d<dim; ++d) doublePos[d] = position[d];
            
            if( kd_insert(mTree, doublePos.data(), (void*)proxyObject) != 0 ) throw Exception("SPACE ERROR: failed to insert proxyObject into kd tree", __FILE__, __FUNCTION__, __LINE__);
		}
	}
	catch(Exception& e)
	{
        e += Exception("SPACE ERROR: failed to update structure of kd tree", __FILE__, __FUNCTION__, __LINE__);
		throw e;
	}
}

void
KDTreeAlg::updateNeighbors( std::vector< SpaceProxyObject* >& pObjects ) throw (Exception)
{
	try
	{
		unsigned int dim = mMinPos.rows();
		unsigned int objectCount = pObjects.size();
		std::vector<double> doublePos(dim);
		double searchRadius;
        
		for(unsigned int oI=0; oI<objectCount; ++oI)
		{
			SpaceProxyObject* proxyObject = pObjects[oI];
			
			searchRadius = proxyObject->neighborRadius();
			const Eigen::VectorXf& position = proxyObject->position();
			for(unsigned int d=0; d<dim; ++d) doublePos[d] = position[d];
            
			mSearchResult = kd_nearest_range(mTree, doublePos.data(), searchRadius);
            
            //			std::cout << "object " << oI << " has " << kd_res_size(mSearchResult) << " neighbors \n";
			
			proxyObject->removeNeighbors();
			
			SpaceProxyObject* neighborProxyObject;
			
			kd_res_rewind(mSearchResult);
			
			if( kd_res_size(mSearchResult) > 0 )
			{
				do
				{
					neighborProxyObject = (SpaceProxyObject*)kd_res_item(mSearchResult, nullptr);
					//neighborProxyObject = (SpaceProxyObject*)kd_res_item(mSearchResult, doublePos);
                    
                    //					std::cout << "pos1: ";
                    //					for(unsigned int d=0; d<dim; ++d) std::cout << doublePos[d] << " ";
                    //					std::cout << "pos2: " << neighborProxyObject->position() << "\n";
					
					if(neighborProxyObject != proxyObject) proxyObject->addNeighbor(neighborProxyObject->spaceObject());
				}
				while( kd_res_next(mSearchResult) != 0 && proxyObject->neighborListFull() == false );
			}
			
			kd_res_free(mSearchResult);
		}
	}
	catch(Exception& e)
	{
        e += Exception("SPACE ERROR: failed to update neighbors based on kd tree", __FILE__, __FUNCTION__, __LINE__);
		throw e;
	}
}

KDTreeAlg::operator std::string() const
{
    return info();
}

std::string
KDTreeAlg::info() const
{
    std::stringstream stream;
    
    stream << "KDTreeAlg\n";
    
	stream << SpaceAlg::info();
    
	return stream.str();
}