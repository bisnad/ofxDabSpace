/** \file dab_space.cpp
*/

#include "dab_space.h"
#include "dab_space_alg.h"
#include "dab_space_neighbor_group_alg.h"
#include "dab_space_proxy_object.h"

using namespace dab;
using namespace dab::space;

Space::Space(const std::string& pName, SpaceAlg* pSpaceAlg )
: mName( pName )
, mSpaceAlg( pSpaceAlg )
{}

Space::~Space()
{
	delete mSpaceAlg;
    
	mVisibleObjects.clear();
	mNeighborObjects.clear();
    
	while( mObjects.size() > 0 ) removeObject( mObjects[0]->spaceObject() );
}

const std::string&
Space::name() const
{
	return mName;
}

unsigned int
Space::dim() const
{
	return mSpaceAlg->dim();
}

const Eigen::VectorXf&
Space::minPos() const
{
	return mSpaceAlg->minPos();
}

const Eigen::VectorXf&
Space::maxPos() const
{
	return mSpaceAlg->maxPos();
}

SpaceAlg*
Space::spaceAlg()
{
	return mSpaceAlg;
}

bool
Space::checkObject(SpaceObject* pSpaceObject) const
{
	unsigned int objectCount = mObjects.size();
	for(unsigned int i=0; i<objectCount; ++i) if( mObjects[i]->spaceObject() == pSpaceObject ) return true;
	return false;
}

SpaceProxyObject*
Space::object(SpaceObject* pSpaceObject) throw (Exception)
{
	unsigned int objectCount = mObjects.size();
	for(unsigned int i=0; i<objectCount; ++i) if( mObjects[i]->spaceObject() == pSpaceObject ) return mObjects[i];
	
    throw Exception( "SPACE ERROR: Space Object Not Found in Space " + mName, __FILE__, __FUNCTION__, __LINE__ );
}

std::vector<SpaceProxyObject*>&
Space::objects()
{
	return mObjects;
}

void
Space::addObject(SpaceObject* pObject, bool pVisible, NeighborGroupAlg* pNeighborGroupAlg) throw (Exception)
{
	unsigned int dim = mSpaceAlg->dim();
    
    // check parameter dimension
    if( pObject->dim() != dim ) throw Exception( "SPACE ERROR: Space Object Dimension " + std::to_string(pObject->dim()) + " doesn't match Space " + mName + " dimension " + std::to_string(dim), __FILE__, __FUNCTION__, __LINE__ );
    // check if object is already stored
    if( checkObject(pObject) == true ) throw Exception( "SPACE ERROR: Space Object Already Stored in Space " + mName, __FILE__, __FUNCTION__, __LINE__ );
	
	// create new neighbor group and store proxy object
	NeighborGroup* neighborGroup = new NeighborGroup(pObject, this, pVisible, pNeighborGroupAlg);
	pObject->addNeighborGroup(neighborGroup);
	SpaceProxyObject* proxyObject = new SpaceProxyObject(pObject, neighborGroup);
	mObjects.push_back(proxyObject);
}

void
Space::setObject(SpaceObject* pObject, bool pVisible, NeighborGroupAlg* pNeighborGroupAlg) throw (Exception)
{
	// check if object is already stored
	if( checkObject(pObject) == false ) throw Exception( "SPACE ERROR: Space Object Not Stored in Space " + mName, __FILE__, __FUNCTION__, __LINE__ );
	
	// update neighbor group
	NeighborGroup* neighborGroup = pObject->neighborGroup(mName);
	neighborGroup->setVisible( pVisible );
	neighborGroup->setNeighborGroupAlg( pNeighborGroupAlg );
}

void
Space::removeObject(SpaceObject* pObject) throw (Exception)
{
	// search for correponding proxy object, remove both proxy object and neighbor group
	unsigned int proxyIndex;
	SpaceProxyObject* proxyObject = nullptr;
	
	unsigned int objectCount = mObjects.size();
	for(unsigned int i=0; i<objectCount; ++i)
	{
		if( mObjects[i]->spaceObject() == pObject )
		{
			proxyObject = mObjects[i];
			proxyIndex = i;
			break;
		}
	}
	
	if( proxyObject == nullptr ) throw Exception( "SPACE ERROR: Space Object Not Found in Space " + mName, __FILE__, __FUNCTION__, __LINE__ );
	
	NeighborGroup* neighborGroup = proxyObject->neighborGroup();
	mObjects.erase(mObjects.begin() + proxyIndex);
	pObject->removeNeighborGroup(neighborGroup);
    
	delete proxyObject;
	delete neighborGroup;
}

void
Space::removeObjects() throw (Exception)
{
    mVisibleObjects.clear();
	mNeighborObjects.clear();
    
    try
    {
        while( mObjects.size() > 0 ) removeObject( mObjects[0]->spaceObject() );
    }
    catch(dab::Exception& e)
    {
        throw;
    }
}

void
Space::update() throw (Exception)
{
	try
	{
		updateObjects();
		
//		std::cout << "Space " << mName << " update\n";
//		std::cout << "total count " << mObjects.size() << "\n";
//		std::cout << "visible count " << mVisibleObjects.size() << "\n";
//		std::cout << "neighbor count " << mNeighborObjects.size() << "\n";
		
		mSpaceAlg->updateStructure( mVisibleObjects );
		mSpaceAlg->updateNeighbors( mNeighborObjects );
		
		//std::cout << "Space " << mName << " update end\n";
	}
	catch(Exception& e)
	{
        e += Exception("SPACE ERROR: failed to update space " + mName, __FILE__, __FUNCTION__, __LINE__);
		throw e;
	}
}

void
Space::updateObjects() throw (Exception)
{
	try
	{
		mVisibleObjects.clear();
		mNeighborObjects.clear();
		
		bool fixedSize = mSpaceAlg->fixedSize();
		const Eigen::VectorXf& minPos = mSpaceAlg->minPos();
		const Eigen::VectorXf& maxPos = mSpaceAlg->maxPos();
		unsigned int dim = minPos.rows();
		bool withinBounds;
		
		unsigned int objectCount = mObjects.size();
		SpaceProxyObject* proxyObject;
		
		if(fixedSize == true)
		{
			for(unsigned int oI=0; oI<objectCount; ++oI)
			{
				proxyObject = mObjects[oI];
                
				const Eigen::VectorXf& position = proxyObject->position();
				withinBounds = true;
				
				for(unsigned int d=0; d<dim; ++d)
				{
					if( position[d] < minPos[d] || position[d] > maxPos[d] )
					{
						withinBounds = false;
						break;
					}
				}
				
				if( withinBounds == true)
				{
					if( proxyObject->visible() == true ) mVisibleObjects.push_back( proxyObject );
					if( proxyObject->canHaveNeighbors() == true && proxyObject->maxNeighborCount() > 0 ) mNeighborObjects.push_back( proxyObject );
				}
			}
		}
		else
		{
			Eigen::VectorXf tmpMinPos = minPos;
			Eigen::VectorXf tmpMaxPos = maxPos;
            
			for(unsigned int oI=0; oI<objectCount; ++oI)
			{
				proxyObject = mObjects[oI];
                
				const Eigen::VectorXf& position = proxyObject->position();
				
				for(unsigned int d=0; d<dim; ++d)
				{
					if( tmpMinPos[d] > position[d] ) tmpMinPos[d] = position[d];
					if( tmpMaxPos[d] < position[d] ) tmpMaxPos[d] = position[d];
				}
				
				if( proxyObject->visible() == true ) mVisibleObjects.push_back( proxyObject );
				if( proxyObject->canHaveNeighbors() == true ) mNeighborObjects.push_back( proxyObject );
			}
            
			if( tmpMinPos != minPos || tmpMaxPos != maxPos ) mSpaceAlg->resize(tmpMinPos, tmpMaxPos);
		}
	}
	catch(Exception& e)
	{
        e += Exception("SPACE ERROR: failed to update objects in space " + mName, __FILE__, __FUNCTION__, __LINE__);
		throw e;
	}
}

Space::operator std::string() const
{
    std::stringstream stream;
    
    stream << "name: " << mName << "\n";
    stream << "spaceAlg: " << *mSpaceAlg << "\n";
	stream << "objectCount: " << mObjects.size() << "\n";
	
	return stream.str();
}

std::string
Space::info(int pPropagationLevel) const
{
    std::stringstream stream;

    stream << "name: " << mName << "\n";
    stream << "spaceAlg: " << *mSpaceAlg << "\n";
	stream << "objectCount: " << mObjects.size() << "\n";
    
	if(pPropagationLevel != 0)
	{
		unsigned int objectCount = mObjects.size();
        
		for(unsigned int i=0; i<objectCount; ++i)
		{
            stream << "object " << mObjects[i]->info(pPropagationLevel - 1) << "\n";
		}
	}
	
	return stream.str();
}