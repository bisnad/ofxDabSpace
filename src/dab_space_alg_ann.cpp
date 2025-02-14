/** \file dab_space_alg_ann.cpp
*/

#include "dab_space_alg_ann.h"
#include "dab_space_proxy_object.h"
#include "dab_space_neighbor_relation.h"

using namespace dab;
using namespace dab::space;

ANNAlg::ANNAlg()
: SpaceAlg(2)
, mNeighborObjects(nullptr)
{
	mDataPts = annAllocPts( 1, mMinPos.rows() );
	mTree = new ANNkd_tree( mDataPts, 1, mMinPos.rows() );
}

ANNAlg::ANNAlg( unsigned int pDim )
: SpaceAlg( pDim )
, mNeighborObjects(nullptr)
{
	mDataPts = annAllocPts( 1, mMinPos.rows() );
	mTree = new ANNkd_tree( mDataPts, 1, mMinPos.rows() );
}

ANNAlg::ANNAlg( const Eigen::VectorXf& pMinPos, const Eigen::VectorXf& pMaxPos )
: SpaceAlg( pMinPos, pMaxPos )
, mNeighborObjects(nullptr)
{
	mDataPts = annAllocPts( 1, mMinPos.rows() );
	mTree = new ANNkd_tree( mDataPts, 1, mMinPos.rows() );
}

ANNAlg::~ANNAlg()
{}

void
ANNAlg::updateStructure( std::vector< SpaceProxyObject* >& pObjects ) throw (Exception)
{
    //	std::cout << "ANNAlg::updateStructure( std::vector< SpaceProxyObject* >& pObjects ) begin\n";
    
	try
	{
		unsigned int dim = mMinPos.rows();
		unsigned int objectCount = pObjects.size();
		
		mNeighborObjects = &pObjects;
		
		if(objectCount == 0) return;
        
		if( mNeighborObjects != nullptr ) annDeallocPts( mDataPts );
		mDataPts = annAllocPts(objectCount, dim);
		
        std::vector< SpaceProxyObject* >& neighborObjects = *mNeighborObjects;
        
		unsigned int pointIndex = 0;
		for(unsigned int oI=0; oI < objectCount; ++oI)
		{
			SpaceProxyObject* proxyObject = neighborObjects[oI];
			const Eigen::VectorXf& position = proxyObject->position();
			for(unsigned int d=0; d<dim; d++) mDataPts[pointIndex][d] = position[d];
            
			pointIndex++;
		}
		
		delete mTree;
		mTree = new ANNkd_tree( mDataPts, objectCount, dim );
		
        //		mTree->Print( (ANNbool)true, std::cout );
        
	}
	catch(Exception& e)
	{
        e += Exception("SPACE ERROR: failed to update data structure for ANN alg", __FILE__, __FUNCTION__, __LINE__);
		throw e;
	}
	
    //	std::cout << "ANNAlg::updateStructure( std::vector< SpaceProxyObject* >& pObjects ) end\n";
}

void
ANNAlg::updateNeighbors( std::vector< SpaceProxyObject* >& pObjects ) throw (Exception)
{
    //std::cout << "ANNAlg::updateNeighbors( std::vector< SpaceProxyObject* >& pObjects ) begin\n";
    
	try
	{
		unsigned int dim = mMinPos.rows();
		unsigned int objectCount = pObjects.size();
		
		if(objectCount == 0  || mNeighborObjects->size() == 0) return;
		
		int maxNeighborCount;
		int searchNeighborCount = 0;
		float neighborRadius;
		Eigen::VectorXf neighborDirection(dim);
		
		ANNpoint mQueryPt; // query point
		ANNidxArray mNeihgborIdx = nullptr; // near neighbor indices
		ANNdistArray mDists = nullptr; // near neighbor distances
		
		mQueryPt = annAllocPt(dim);
        
		for(unsigned int oI=0; oI<objectCount; ++oI)
		{
			SpaceProxyObject* proxyObject = pObjects[oI];
			SpaceObject* spaceObject = proxyObject->spaceObject();
			const Eigen::VectorXf& position = proxyObject->position();
			for(unsigned int d=0; d<dim; d++) mQueryPt[d] = position[d];
			
			maxNeighborCount = proxyObject->maxNeighborCount();
			if( maxNeighborCount >= mNeighborObjects->size() - 1 ) maxNeighborCount = mNeighborObjects->size() - 1;
			neighborRadius = proxyObject->neighborRadius();
			
			if( searchNeighborCount != maxNeighborCount + 1)
			{
				if(mNeihgborIdx != nullptr)
				{
					delete [] mNeihgborIdx;
					delete [] mDists;
				}
				
				searchNeighborCount = maxNeighborCount + 1; // look for one more neighbor because one of the neighbors might be the object itself
                
				mNeihgborIdx = new ANNidx[searchNeighborCount]; // look for one more neighbor because one of the neighbors might be the object itself
				mDists = new ANNdist[searchNeighborCount]; // look for one more neighbor because one of the neighbors might be the object itself
			}
			
			mTree->annkSearch( mQueryPt, searchNeighborCount, mNeihgborIdx, mDists, neighborRadius * 0.1);
			
			proxyObject->removeNeighbors();
            std::vector<SpaceNeighborRelation*>& neighborRelations = proxyObject->neighborGroup()->neighborRelations();
			
			unsigned int neighborCount = 0;
			SpaceObject* neighborObject;
            std::vector< SpaceProxyObject* >& neighborObjects = *mNeighborObjects;
			
			for(unsigned int nI=0; nI < searchNeighborCount, neighborCount < maxNeighborCount; ++nI)
			{
				mDists[nI] = sqrt(mDists[nI]);
				
				if( mDists[nI] > neighborRadius ) break; // outside search radius
				
				neighborObject = neighborObjects[ mNeihgborIdx[ nI ] ]->spaceObject();
				
				if( neighborObject == spaceObject ) continue; // object and neighbor are identical
				
				Eigen::VectorXf& neighborPos = neighborObject->position();
				
				for(unsigned int d=0; d<dim; ++d) neighborDirection[d] = neighborPos[d] - position[d];
				neighborRelations.push_back( new SpaceNeighborRelation( spaceObject, neighborObject, mDists[nI], neighborDirection ) );
				neighborCount++;
			}
		}
	}
	catch(Exception& e)
	{
        e += Exception("SPACE ERROR: failed to update neighbors for ANN algorithm", __FILE__, __FUNCTION__, __LINE__);
		throw e;
	}
	
    //	std::cout << "ANNAlg::updateNeighbors( QVector< SpaceProxyObject* >& pObjects ) end\n";
}

ANNAlg::operator std::string() const
{
    return info();
}

std::string
ANNAlg::info() const
{
    std::stringstream stream;
    
    stream << "ANNAlg\n";
    stream << SpaceAlg::info();
    
	return stream.str();
}