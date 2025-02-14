/** \file dab_space_cluster_analyzer.cpp
 */

#include "dab_space_cluster_analyzer.h"
#include "dab_space_neighbor_group.h"
#include "dab_space_proxy_object.h"
#include "dab_space_object.h"
#include <vector>

using namespace dab;
using namespace dab::space;


SpaceClusterAnalyzer::SpaceClusterAnalyzer()
: SpaceObjectsAnalyzer()
{}

SpaceClusterAnalyzer::SpaceClusterAnalyzer(std::string pName)
: SpaceObjectsAnalyzer(pName)
{}

SpaceClusterAnalyzer::~SpaceClusterAnalyzer()
{}

void
SpaceClusterAnalyzer::addObjectGroup( const std::string& pGroupName ) throw (Exception)
{
    if(mSpaceObjectGroups.size() > 0) throw Exception("SPACE ERROR: only one space object group is allowed for this type of analyzer", __FILE__, __FUNCTION__, __LINE__);
    
	mLock.lock();
    
	SpaceProxyObjectGroup* spaceObjectGroup = new SpaceProxyObjectGroup();
	mSpaceObjectGroups.add( pGroupName, spaceObjectGroup );
	
	mLock.unlock();
}

void
SpaceClusterAnalyzer::addObject( unsigned int pGroupIndex, const SpaceObject* pSpaceObject ) throw (Exception)
{
    throw Exception("SPACE ERROR: analyzer accepts neighbor groups only as objects", __FILE__, __FUNCTION__, __LINE__);
}

void
SpaceClusterAnalyzer::addObjects( unsigned int pGroupIndex, std::vector< const SpaceObject* >& pSpaceObjects ) throw (Exception)
{
    throw Exception("SPACE ERROR: analyzer accepts neighbor groups only as objects", __FILE__, __FUNCTION__, __LINE__);
}

void
SpaceClusterAnalyzer::addObject( const std::string& pGroupName, const SpaceObject* pSpaceObject ) throw (Exception)
{
	throw Exception("SPACE ERROR: analyzer accepts neighbor groups only as objects", __FILE__, __FUNCTION__, __LINE__);
}

void
SpaceClusterAnalyzer::addObjects( const std::string& pGroupName, std::vector< const SpaceObject* >& pSpaceObjects ) throw (Exception)
{
	throw Exception("SPACE ERROR: analyzer accepts neighbor groups only as objects", __FILE__, __FUNCTION__, __LINE__);
}

void
SpaceClusterAnalyzer::analyze()
{
	if(mSpaceObjectGroups.size() == 0) return;
    
	mLock.lock();
	
	// remove previous results
	mResults.clear();
	
	// neighbor groups that form the basis for cluster detection
	SpaceProxyObjectGroup* objectGroup = mSpaceObjectGroups[0];
	unsigned int objectCount = objectGroup->objectCount();
    
	// temporary cluster data structures
	int clusterNr = -1;
    std::map< const SpaceObject*, int > clusterMap; // space object / cluster index relations
    std::map< const SpaceObject*, int > objectMap; // space object / proxy index map
    std::map< int, std::vector<int> > indexMap; // cluster index / proxy index map
    
	// create temporary data structures
	
	// create object map
	for(unsigned int objectNr=0; objectNr < objectCount; ++objectNr)
	{
		const SpaceProxyObject* proxyObject = objectGroup->proxyObject(objectNr);
		objectMap[proxyObject->spaceObject()] = objectNr;
	}
    
	// create cluster map
	for(unsigned int objectNr=0; objectNr < objectCount; ++objectNr)
	{
		const SpaceProxyObject* proxyObject = objectGroup->proxyObject(objectNr);
		const SpaceObject* spaceObject = proxyObject->spaceObject();
		const NeighborGroup* neighborGroup = proxyObject->neighborGroup();
		unsigned int neighborCount = neighborGroup->neighborCount();
        
		if(clusterMap.find(spaceObject) == clusterMap.end()) // object is not stored in a cluster -> new cluster
		{
			clusterNr++;
			clusterMap[spaceObject] = clusterNr;
		}
        
		for(unsigned int neighborNr = 0; neighborNr < neighborCount; ++neighborNr)
		{
			const SpaceObject* neighborObject = neighborGroup->neighbor(neighborNr);
            
			if( clusterMap.find(neighborObject) == clusterMap.end() ) // neighbor object hasn't been part of a cluster before
			{
				clusterMap[neighborObject] = clusterNr;
			}
			else if( clusterMap[neighborObject] != clusterNr ) // neighbor object has been part of a different cluster before
			{
				// probably won't work like this
				clusterMap[neighborObject] = clusterNr;
			}
		}
	}
	
	// create index map
	// all proxy objects that are part of the same cluster are grouped under the cluster index
    for(auto mapIter = clusterMap.begin(); mapIter != clusterMap.end(); ++mapIter)
    {
        if( indexMap.find(mapIter->second) != indexMap.end())
        {
            indexMap[mapIter->second].push_back(objectMap[mapIter->first]);
        }
        else
        {
            indexMap[mapIter->second] = { objectMap[mapIter->first] };
        }
    }
    
	// create result vector
    std::vector<int> clusterIndices(clusterMap.size());
    int vectorIndex = 0;
    for(auto mapIter = indexMap.begin(); mapIter != indexMap.end(); ++mapIter)
    {
        clusterIndices[vectorIndex++] = mapIter->first;
    }
        
	unsigned int clusterCount = clusterIndices.size();
    
	for(unsigned int clusterNr=0; clusterNr<clusterCount; ++clusterNr)
	{
        const std::vector<int>& objectIndices = indexMap[ clusterIndices[clusterNr] ];
		unsigned int objectCount = objectIndices.size();
		
		std::string clusterName = "cluster" + std::to_string(clusterNr);
		Eigen::VectorXf clusterVector( objectCount );
		
		//std::cout << "cluster index " << clusterIndices[clusterNr] << " nr " << clusterNr << " objectCount " << objectCount << ":\n";
		
		for(unsigned int objectNr=0; objectNr < objectCount; ++objectNr)
		{
			clusterVector[objectNr] = static_cast<float>( objectIndices[objectNr] );
			
			//std::cout << "object nr " << objectNr << " index " << objectIndices[objectNr] << " id " << ( objectMap.key( objectIndices[objectNr] ) )->id() << "\n";
  		}
		
		mResults.add( clusterName, clusterVector );
		
		//std::cout << "cluster index " << clusterIndices[clusterNr] << " name " << clusterName.toStdString() << " vector " << clusterVector << "\n";
	}
	
	// destroy temporary data structures
	clusterMap.clear();
	objectMap.clear();
	indexMap.clear();
	
	mLock.unlock();
}