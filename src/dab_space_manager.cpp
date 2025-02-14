/**	\file dab_space_manager.cpp
 */

#include "dab_space_manager.h"
#include "dab_space_object.h"

using namespace dab;
using namespace dab::space;

SpaceManager::SpaceManager()
{}

SpaceManager::~SpaceManager()
{
    removeSpaces();
}

const std::vector< std::string >&
SpaceManager::spaceNames() const
{
    return mSpaces.keys();
}

std::shared_ptr<Space>
SpaceManager::space(const std::string& pSpaceName) throw (Exception)
{
    try
    {
        return mSpaces[pSpaceName];
    }
    catch (Exception& e)
    {
        e += Exception( "SPACE ERROR: space " + pSpaceName + " not Found", __FILE__, __FUNCTION__, __LINE__ );
        throw e;
        
    }
}

bool
SpaceManager::checkSpace(const std::string& pSpaceName)
{
	return mSpaces.contains(pSpaceName);
}

void
SpaceManager::addSpace(std::shared_ptr<Space> pSpace) throw (Exception)
{
    try
    {
        mSpaces.add(pSpace->name(), pSpace);
    }
    catch (Exception& e)
    {
        e += Exception( "SPACE ERROR: space " + pSpace->name() + " already exists", __FILE__, __FUNCTION__, __LINE__ );
        throw e;
    }
}

void
SpaceManager::removeSpace(const std::string& pSpaceName) throw (Exception)
{
    try
    {
        //delete mSpaces[pSpaceName];
        mSpaces.remove(pSpaceName);
    }
    catch (Exception& e)
    {
        e += Exception( "SPACE ERROR: space " + pSpaceName + " not found", __FILE__, __FUNCTION__, __LINE__ );
        throw e;
    }
}

void
SpaceManager::removeSpaces()
{
    //	unsigned int spaceCount = mSpaces.size();
    //
    //	for(unsigned int i=0; i<spaceCount; ++i)
    //	{
    //		Space* space = &(mSpaces[i]);
    //		delete space;
    //	}
    //
	mSpaces.clear();
}

void
SpaceManager::addObject(const std::string& pSpaceName, SpaceObject* pObject, bool pVisible, NeighborGroupAlg* pNeighborGroupAlg) throw (Exception)
{
	try
	{
		mSpaces[pSpaceName]->addObject(pObject, pVisible, pNeighborGroupAlg);
	}
	catch(Exception& e)
	{
        e += Exception( "SPACE ERROR: space " + pSpaceName + " not found", __FILE__, __FUNCTION__, __LINE__ );
		throw e;
	}
}

void
SpaceManager::removeObject(const std::string& pSpaceName, SpaceObject* pObject) throw (Exception)
{
	try
	{
		mSpaces[pSpaceName]->removeObject(pObject);
	}
	catch(Exception& e)
	{
        e += Exception( "SPACE ERROR: space " + pSpaceName + " not found", __FILE__, __FUNCTION__, __LINE__ );
		throw e;
	}
}

void
SpaceManager::update()throw (Exception)
{
    //	std::cout << "SpaceManager::update() begin\n";
    
	unsigned int spaceCount = mSpaces.size();
	
    //	std::cout << "spaceCount " << spaceCount << "\n";
	
	for(unsigned int i=0; i<spaceCount; ++i)
	{
        //		std::cout << "space " << mSpaces[i].name().toStdString() << " update begin\n";
        try
        {
            mSpaces[i]->update();
        }
        catch(Exception& e)
        {
            e += Exception("SPACE ERROR: Failed to update space " + mSpaces[i]->name(), __FILE__, __FUNCTION__, __LINE__);
            throw e;
        }
        
		mSpaces[i]->update();
        //		std::cout << "space " << mSpaces[i].name().toStdString() << " update end\n";
	}
	
    //	std::cout << "SpaceManager::update() end\n";
}

SpaceManager::operator std::string() const
{
	std::stringstream ss;
	
	unsigned int spaceCount = mSpaces.size();
    
	for(unsigned int i=0; i<spaceCount; ++i)
	{
        ss << "Space " << mSpaces.key(i) << "\n";
	}
	
	return ss.str();
}

std::string
SpaceManager::info(int pPropagationLevel) const
{
	std::stringstream ss;
	
	unsigned int spaceCount = mSpaces.size();
    
	for(unsigned int i=0; i<spaceCount; ++i)
	{
        ss << "Space " << mSpaces.key(i) << "\n";
		
		if(pPropagationLevel != 0) ss << mSpaces[i]->info(pPropagationLevel - 1) << "\n";
	}
	
	return ss.str();
}