/** \file dab_space_objects_analyzer.cpp
 */

#include "dab_space_objects_analyzer.h"
#include "dab_space_neighbor_group.h"
#include "dab_space_object.h"

using namespace dab;
using namespace dab::space;

SpaceProxyObjectGroup::SpaceProxyObjectGroup()
{}

SpaceProxyObjectGroup::~SpaceProxyObjectGroup()
{
	removeObjects();
}

unsigned int
SpaceProxyObjectGroup::objectCount() const
{
//	( const_cast<ofMutex*>(&mLock) )->lock();
//	( const_cast<ofMutex*>(&mLock) )->unlock();
    
	return mSpaceObjects.size();
}

const SpaceProxyObject*
SpaceProxyObjectGroup::proxyObject( unsigned int pIndex) const throw (Exception)
{
	( const_cast<ofMutex*>(&mLock) )->lock();
    
    if( pIndex >= mSpaceObjects.size() )
    {
        ( const_cast<ofMutex*>(&mLock) )->unlock();
        throw Exception("SPACE ERROR: index out of range", __FILE__, __FUNCTION__, __LINE__);
    }
    
	SpaceProxyObject* proxyObj = mSpaceObjects[pIndex];
	
	( const_cast<ofMutex*>(&mLock) )->unlock();
	
	return proxyObj;
}

const SpaceObject*
SpaceProxyObjectGroup::spaceObject( unsigned int pIndex) const throw (Exception)
{
	( const_cast<ofMutex*>(&mLock) )->lock();
    
    if( pIndex >= mSpaceObjects.size() )
    {
        ( const_cast<ofMutex*>(&mLock) )->unlock();
        throw Exception("SPACE ERROR: index out of range", __FILE__, __FUNCTION__, __LINE__);
    }
    
	SpaceObject* spaceObj = mSpaceObjects[pIndex]->spaceObject();
	
	( const_cast<ofMutex*>(&mLock) )->unlock();
	
	return spaceObj;
}

const NeighborGroup*
SpaceProxyObjectGroup::neighborGroup( unsigned int pIndex) const throw (Exception)
{
	( const_cast<ofMutex*>(&mLock) )->lock();
    
    if( pIndex >= mSpaceObjects.size() )
    {
        ( const_cast<ofMutex*>(&mLock) )->unlock();
        throw Exception("SPACE ERROR: index out of range", __FILE__, __FUNCTION__, __LINE__);
    }
    
	NeighborGroup* neighGroup = mSpaceObjects[pIndex]->neighborGroup();
	
	( const_cast<ofMutex*>(&mLock) )->unlock();
	
	return neighGroup;
}

void
SpaceProxyObjectGroup::addObject(SpaceObject* pSpaceObject)
{
	mLock.lock();
    
	mSpaceObjects.push_back( new SpaceProxyObject(pSpaceObject, nullptr) );
    
	mLock.unlock();
}

void
SpaceProxyObjectGroup::addObject(NeighborGroup* pNeighborGroup )
{
	mLock.lock();
    
	mSpaceObjects.push_back( new SpaceProxyObject(pNeighborGroup->spaceObject(), pNeighborGroup) );
    
	mLock.unlock();
}

void
SpaceProxyObjectGroup::removeObject(SpaceObject* pSpaceObject)
{
	mLock.lock();
    	
	int oC = mSpaceObjects.size();
	for(int i=0; i<oC; --i)
	{
		if(mSpaceObjects[i]->spaceObject() == pSpaceObject && mSpaceObjects[i]->neighborGroup() == nullptr)
		{
			delete mSpaceObjects[i];
			mSpaceObjects.erase(mSpaceObjects.begin() + i);
		}
	}
	
	mLock.unlock();
}

void
SpaceProxyObjectGroup::removeObject(NeighborGroup* pNeighborGroup)
{
	mLock.lock();
    
	int oC = mSpaceObjects.size();
	for(int i=oC - 1; i>=0; --i)
	{
		if(mSpaceObjects[i]->spaceObject() == pNeighborGroup->spaceObject() && mSpaceObjects[i]->neighborGroup() == pNeighborGroup)
		{
			delete mSpaceObjects[i];
			mSpaceObjects.erase(mSpaceObjects.begin() + i);
		}
	}
	
	mLock.unlock();
}

void
SpaceProxyObjectGroup::removeObjects()
{
	mLock.lock();
    
	unsigned int oC = mSpaceObjects.size();
	for(unsigned int i=0; i<oC; ++i) delete mSpaceObjects[i];
	mSpaceObjects.clear();
	
	mLock.unlock();
}




SpaceObjectsAnalyzer::SpaceObjectsAnalyzer()
: mModifiable(true)
{}

SpaceObjectsAnalyzer::SpaceObjectsAnalyzer(std::string pName)
: mName(pName)
, mModifiable(true)
{}

SpaceObjectsAnalyzer::~SpaceObjectsAnalyzer()
{
	unsigned int objectGroupCount = mSpaceObjectGroups.size();
	for(unsigned int i=0; i<objectGroupCount; ++i) delete &( mSpaceObjectGroups[i] );
    
	mSpaceObjectGroups.clear();
}

const std::string&
SpaceObjectsAnalyzer::name() const
{
	return mName;
}

bool
SpaceObjectsAnalyzer::modifiable() const
{
	return mModifiable;
}

bool
SpaceObjectsAnalyzer::checkObjectGroup( const std::string& pGroupName ) const
{
	return mSpaceObjectGroups.contains(pGroupName);
}

void
SpaceObjectsAnalyzer::addObjectGroup( const std::string& pGroupName ) throw (Exception)
{
    if(mModifiable == false) throw Exception("SPACE ERROR: this analyzer doesn't permit to manually add object groups", __FILE__, __FUNCTION__, __LINE__);
    
	mLock.lock();
    
    if(mSpaceObjectGroups.contains(pGroupName) == true)
    {
        mLock.unlock();
        throw Exception("SPACE ERROR: object group name: " + pGroupName + " already exists", __FILE__, __FUNCTION__, __LINE__);
    }
    
	SpaceProxyObjectGroup* spaceObjectGroup = new SpaceProxyObjectGroup();
	mSpaceObjectGroups.add( pGroupName, spaceObjectGroup );
	
	mLock.unlock();
}

void
SpaceObjectsAnalyzer::removeObjectGroup( unsigned int pGroupIndex ) throw (Exception)
{
    if(mModifiable == false) throw Exception("SPACE ERROR: this analyzer doesn't permit to manually add object groups", __FILE__, __FUNCTION__, __LINE__);
    
	mLock.lock();
    
    if( mSpaceObjectGroups.size() <= pGroupIndex )
    {
        mLock.unlock();
        throw Exception("SPACE ERROR: object group index: " + std::to_string(pGroupIndex) + " out of bounds!", __FILE__, __FUNCTION__, __LINE__);
    }

	delete mSpaceObjectGroups[pGroupIndex];
	mSpaceObjectGroups.remove(pGroupIndex);
	
	mLock.unlock();
}

void
SpaceObjectsAnalyzer::removeObjectGroup( const std::string& pGroupName ) throw (Exception)
{
    if(mModifiable == false) throw Exception("SPACE ERROR: this analyzer doesn't permit to manually add object groups", __FILE__, __FUNCTION__, __LINE__);
    
	mLock.lock();
    
    if( mSpaceObjectGroups.contains(pGroupName) == false )
    {
        mLock.unlock();
        throw Exception("SPACE ERROR: object group name: " + pGroupName + " not found!", __FILE__, __FUNCTION__, __LINE__);
    }

	delete mSpaceObjectGroups[pGroupName];
	mSpaceObjectGroups.remove(pGroupName);
	
	mLock.unlock();
}

void
SpaceObjectsAnalyzer::addObject( unsigned int pGroupIndex, const SpaceObject* pSpaceObject ) throw (Exception)
{
	mLock.lock();
    
    if(mSpaceObjectGroups.size() <= pGroupIndex)
    {
        mLock.unlock();
        throw Exception("SPACE ERROR: object group index: " + std::to_string(pGroupIndex) + " out of bounds!", __FILE__, __FUNCTION__, __LINE__);
    }
	
	mSpaceObjectGroups[pGroupIndex]->addObject( const_cast<SpaceObject*>( pSpaceObject ) );
	
	mLock.unlock();
}

void
SpaceObjectsAnalyzer::addObjects( unsigned int pGroupIndex, std::vector< const SpaceObject* >& pSpaceObjects ) throw (Exception)
{
	mLock.lock();
    
    if(mSpaceObjectGroups.size() <= pGroupIndex)
    {
        mLock.unlock();
        throw Exception("SPACE ERROR: object group index: " + std::to_string(pGroupIndex) + " out of bounds!", __FILE__, __FUNCTION__, __LINE__);
    }
	
	unsigned int objectCount = pSpaceObjects.size();
	for(unsigned int i=0; i<objectCount; ++i) mSpaceObjectGroups[pGroupIndex]->addObject( const_cast<SpaceObject*>( pSpaceObjects[i] ) );
	
	mLock.unlock();
}

void
SpaceObjectsAnalyzer::addObject( const std::string& pGroupName, const SpaceObject* pSpaceObject ) throw (Exception)
{
	mLock.lock();
    
    if( mSpaceObjectGroups.contains(pGroupName) == false )
    {
        mLock.unlock();
        throw Exception("SPACE ERROR: object group name: " + pGroupName + " not found!", __FILE__, __FUNCTION__, __LINE__);
    }
    
	mSpaceObjectGroups[pGroupName]->addObject( const_cast<SpaceObject*>( pSpaceObject ) );
	
	mLock.unlock();
}

void
SpaceObjectsAnalyzer::addObjects( const std::string& pGroupName, std::vector< const SpaceObject* >& pSpaceObjects ) throw (Exception)
{
	mLock.lock();
    
    if( mSpaceObjectGroups.contains(pGroupName) == false )
    {
        mLock.unlock();
        throw Exception("SPACE ERROR: object group name: " + pGroupName + " not found!", __FILE__, __FUNCTION__, __LINE__);
    }
	
	unsigned int objectCount = pSpaceObjects.size();
	for(unsigned int i=0; i<objectCount; ++i) mSpaceObjectGroups[pGroupName]->addObject(const_cast<SpaceObject*>( pSpaceObjects[i] ) );
	
	mLock.unlock();
}

void
SpaceObjectsAnalyzer::addObject( unsigned int pGroupIndex, const NeighborGroup* pNeighborGroup ) throw (Exception)
{
	mLock.lock();
    
    if(mSpaceObjectGroups.size() <= pGroupIndex)
    {
        mLock.unlock();
        throw Exception("SPACE ERROR: object group index: " + std::to_string(pGroupIndex) + " out of bounds!", __FILE__, __FUNCTION__, __LINE__);
    }
	
	mSpaceObjectGroups[pGroupIndex]->addObject( const_cast<NeighborGroup*>( pNeighborGroup ) );
	
	mLock.unlock();
}

void
SpaceObjectsAnalyzer::addObjects( unsigned int pGroupIndex, std::vector< const NeighborGroup* >& pNeighborGroups ) throw (Exception)
{
	mLock.lock();
    
    if(mSpaceObjectGroups.size() <= pGroupIndex)
    {
        mLock.unlock();
        throw Exception("SPACE ERROR: object group index: " + std::to_string(pGroupIndex) + " out of bounds!", __FILE__, __FUNCTION__, __LINE__);
    }
	
	unsigned int objectCount = pNeighborGroups.size();
	for(unsigned int i=0; i<objectCount; ++i) mSpaceObjectGroups[pGroupIndex]->addObject( const_cast<NeighborGroup*>( pNeighborGroups[i] ) );
	
	mLock.unlock();
}

void
SpaceObjectsAnalyzer::addObject( const std::string& pGroupName, const NeighborGroup* pNeighborGroup ) throw (Exception)
{
	mLock.lock();
    
    if( mSpaceObjectGroups.contains(pGroupName) == false )
    {
        mLock.unlock();
        throw Exception("SPACE ERROR: object group name: " + pGroupName + " not found!", __FILE__, __FUNCTION__, __LINE__);
    }
    
	mSpaceObjectGroups[pGroupName]->addObject( const_cast<NeighborGroup*>( pNeighborGroup ) );
	
	mLock.unlock();
}

void
SpaceObjectsAnalyzer::addObjects( const std::string& pGroupName, std::vector< const NeighborGroup* >& pNeighborGroups ) throw (Exception)
{
	mLock.lock();
    
    if( mSpaceObjectGroups.contains(pGroupName) == false )
    {
        mLock.unlock();
        throw Exception("SPACE ERROR: object group name: " + pGroupName + " not found!", __FILE__, __FUNCTION__, __LINE__);
    }
	
	unsigned int objectCount = pNeighborGroups.size();
	for(unsigned int i=0; i<objectCount; ++i) mSpaceObjectGroups[pGroupName]->addObject( const_cast<NeighborGroup*>( pNeighborGroups[i] ) );
	
	mLock.unlock();
}

void
SpaceObjectsAnalyzer::removeObject( unsigned int pGroupIndex, const SpaceObject* pSpaceObject ) throw (Exception)
{
	mLock.lock();
    
    if(mSpaceObjectGroups.size() <= pGroupIndex)
    {
        mLock.unlock();
        throw Exception("SPACE ERROR: object group index: " + std::to_string(pGroupIndex) + " out of bounds!", __FILE__, __FUNCTION__, __LINE__);
    }
    
	mSpaceObjectGroups[pGroupIndex]->removeObject( const_cast<SpaceObject*>( pSpaceObject ) );
	
	mLock.unlock();
}

void
SpaceObjectsAnalyzer::removeObject( const std::string& pGroupName, const SpaceObject* pSpaceObject ) throw (Exception)
{
	mLock.lock();
    
    if( mSpaceObjectGroups.contains(pGroupName) == false )
    {
        mLock.unlock();
        throw Exception("SPACE ERROR: object group name: " + pGroupName + " not found!", __FILE__, __FUNCTION__, __LINE__);
    }
	
	mSpaceObjectGroups[pGroupName]->removeObject( const_cast<SpaceObject*>( pSpaceObject ) );
	
	mLock.unlock();
}

void
SpaceObjectsAnalyzer::removeObject( unsigned int pGroupIndex, const NeighborGroup* pNeighborGroup ) throw (Exception)
{
	mLock.lock();
    
    if(mSpaceObjectGroups.size() <= pGroupIndex)
    {
        mLock.unlock();
        throw Exception("SPACE ERROR: object group index: " + std::to_string(pGroupIndex) + " out of bounds!", __FILE__, __FUNCTION__, __LINE__);
    }
	
	mSpaceObjectGroups[pGroupIndex]->removeObject( const_cast<NeighborGroup*>( pNeighborGroup ) );
	
	mLock.unlock();
}

void
SpaceObjectsAnalyzer::removeObject( const std::string& pGroupName, const NeighborGroup* pNeighborGroup ) throw (Exception)
{
	mLock.lock();
    
    if( mSpaceObjectGroups.contains(pGroupName) == false )
    {
        mLock.unlock();
        throw Exception("SPACE ERROR: object group name: " + pGroupName + " not found!", __FILE__, __FUNCTION__, __LINE__);
    }
	
	mSpaceObjectGroups[pGroupName]->removeObject( const_cast<NeighborGroup*>( pNeighborGroup ) );
	
	mLock.unlock();
}

void
SpaceObjectsAnalyzer::removeObjects( unsigned int pGroupIndex ) throw (Exception)
{
	mLock.lock();
    
    if(mSpaceObjectGroups.size() <= pGroupIndex)
    {
        mLock.unlock();
        throw Exception("SPACE ERROR: object group index: " + std::to_string(pGroupIndex) + " out of bounds!", __FILE__, __FUNCTION__, __LINE__);
    }
	
	mSpaceObjectGroups[pGroupIndex]->removeObjects( );
	
	mLock.unlock();
}

void
SpaceObjectsAnalyzer::removeObjects( const std::string& pGroupName ) throw (Exception)
{
	mLock.lock();
    
    if( mSpaceObjectGroups.contains(pGroupName) == false )
    {
        mLock.unlock();
        throw Exception("SPACE ERROR: object group name: " + pGroupName + " not found!", __FILE__, __FUNCTION__, __LINE__);
    }
	
	mSpaceObjectGroups[pGroupName]->removeObjects( );
	
	mLock.unlock();
}

const Eigen::VectorXf&
SpaceObjectsAnalyzer::result( unsigned int pResultIndex ) const throw (Exception)
{
    if(mResults.size() <= pResultIndex)
    {
        throw Exception("SPACE ERROR: analyzer result index: " + std::to_string(pResultIndex) + " out of bounds!", __FILE__, __FUNCTION__, __LINE__);
    }
	
	return const_cast< const Eigen::VectorXf& >( mResults[pResultIndex] );
}

const Eigen::VectorXf&
SpaceObjectsAnalyzer::result( const std::string& pResultName ) const throw (Exception)
{
    if(mResults.contains(pResultName) == false)
    {
        throw Exception("SPACE ERROR: analyzer result name: " + pResultName + " not found!", __FILE__, __FUNCTION__, __LINE__);
    }
	
	return const_cast< const Eigen::VectorXf& >( mResults[pResultName] );
}

const IndexMap< std::string, Eigen::VectorXf >&
SpaceObjectsAnalyzer::results() const
{
	return mResults;
}

void
SpaceObjectsAnalyzer::analyze()
{
	std::cout << "SpaceDataAnalyzer " << mName << " analyze " << "\n";
	unsigned int groupCount = mSpaceObjectGroups.size();
	for(unsigned int i=0; i<groupCount; ++i)
	{
		std::cout << i << ". objectGroup " << mSpaceObjectGroups.key(i) << "\n";
		
		SpaceProxyObjectGroup* spaceObjectGroup = mSpaceObjectGroups[i];
		
		unsigned int objectCount = spaceObjectGroup->objectCount();
		
		for(unsigned int j=0; j<objectCount; j++)
		{
			if(spaceObjectGroup->neighborGroup(j) == nullptr) std::cout << j << ". spaceObject " << spaceObjectGroup->spaceObject(j)->position() << "\n";
			else std::cout << j << ". neigborGroup " << spaceObjectGroup->neighborGroup(j)->name() << "\n";
		}
	}
}