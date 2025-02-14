/** \file dab_space_objects_analyze_manager.cpp
 */

#include "dab_space_objects_analyze_manager.h"

using namespace dab;
using namespace dab::space;

SpaceObjectsAnalyzeManager::SpaceObjectsAnalyzeManager()
{}

SpaceObjectsAnalyzeManager::~SpaceObjectsAnalyzeManager()
{
	unsigned int analyzerCount = mAnalyzers.size();
	
	for(unsigned int i=0; i<analyzerCount; ++i)
	{
		SpaceObjectsAnalyzer* analyzer = mAnalyzers[i];
		delete analyzer;
	}
    
    mAnalyzers.clear();
}

bool
SpaceObjectsAnalyzeManager::checkAnalyzer(const std::string& pAnalyzerName)
{
	return mAnalyzers.contains(pAnalyzerName);
}

SpaceObjectsAnalyzer*
SpaceObjectsAnalyzeManager::analyzer(const std::string& pAnalyzerName) throw (Exception)
{
    if( mAnalyzers.contains(pAnalyzerName) == false ) throw Exception("SPACE ERROR: space objects analyzer name " + pAnalyzerName + " does not exist", __FILE__, __FUNCTION__, __LINE__);
	
	return mAnalyzers[pAnalyzerName];
}

void
SpaceObjectsAnalyzeManager::addAnalyzer(SpaceObjectsAnalyzer* pAnalyzer) throw (Exception)
{
    if( mAnalyzers.contains(pAnalyzer->name()) == true) throw Exception("SPACE ERROR: space objects analyzer name " + pAnalyzer->name() + " already exists", __FILE__, __FUNCTION__, __LINE__);
	
	mAnalyzers.add(pAnalyzer->name(), pAnalyzer);
}

void
SpaceObjectsAnalyzeManager::removeAnalyzer(const std::string& pAnalyzerName) throw (Exception)
{
    if( mAnalyzers.contains(pAnalyzerName) == false ) throw Exception("SPACE ERROR: space objects analyzer name " + pAnalyzerName + " does not exist", __FILE__, __FUNCTION__, __LINE__);
	
	delete mAnalyzers[pAnalyzerName];
	mAnalyzers.remove(pAnalyzerName);
}

void
SpaceObjectsAnalyzeManager::addObjectGroup( const std::string& pAnalyzerName, const std::string& pGroupName ) throw (Exception)
{
    if( mAnalyzers.contains(pAnalyzerName) == false ) throw Exception("SPACE ERROR: space objects analyzer name " + pAnalyzerName + " does not exist", __FILE__, __FUNCTION__, __LINE__);
    
	mAnalyzers[pAnalyzerName]->addObjectGroup( pGroupName );
}

void
SpaceObjectsAnalyzeManager::removeObjectGroup( const std::string& pAnalyzerName, unsigned int pGroupIndex ) throw (Exception)
{
    if( mAnalyzers.contains(pAnalyzerName) == false ) throw Exception("SPACE ERROR: space objects analyzer name " + pAnalyzerName + " does not exist", __FILE__, __FUNCTION__, __LINE__);
    
    try
    {
        mAnalyzers[pAnalyzerName]->removeObjectGroup( pGroupIndex );
    }
    catch (Exception& e)
    {
        throw;
    }
}

void
SpaceObjectsAnalyzeManager::removeObjectGroup( const std::string& pAnalyzerName, const std::string& pGroupName ) throw (Exception)
{
    if( mAnalyzers.contains(pAnalyzerName) == false ) throw Exception("SPACE ERROR: space objects analyzer name " + pAnalyzerName + " does not exist", __FILE__, __FUNCTION__, __LINE__);
    
    try
    {
        mAnalyzers[pAnalyzerName]->removeObjectGroup( pGroupName );
    }
    catch (Exception& e)
    {
        throw;
    }
}

void
SpaceObjectsAnalyzeManager::addObject( const std::string& pAnalyzerName, unsigned int pGroupIndex, const SpaceObject* pSpaceObject ) throw (Exception)
{
    if( mAnalyzers.contains(pAnalyzerName) == false ) throw Exception("SPACE ERROR: space objects analyzer name " + pAnalyzerName + " does not exist", __FILE__, __FUNCTION__, __LINE__);

    try
    {
        mAnalyzers[pAnalyzerName]->addObject( pGroupIndex, pSpaceObject );
    }
    catch (Exception& e)
    {
        throw;
    }
}

void
SpaceObjectsAnalyzeManager::addObject( const std::string& pAnalyzerName, const std::string& pGroupName, const SpaceObject* pSpaceObject )throw (Exception)
{
    if( mAnalyzers.contains(pAnalyzerName) == false ) throw Exception("SPACE ERROR: space objects analyzer name " + pAnalyzerName + " does not exist", __FILE__, __FUNCTION__, __LINE__);
    
    try
    {
        mAnalyzers[pAnalyzerName]->addObject( pGroupName, pSpaceObject );
    }
    catch (Exception& e)
    {
        throw;
    }
}

void
SpaceObjectsAnalyzeManager::addObject( const std::string& pAnalyzerName, unsigned int pGroupIndex, const NeighborGroup* pNeighborGroup )throw (Exception)
{
    if( mAnalyzers.contains(pAnalyzerName) == false ) throw Exception("SPACE ERROR: space objects analyzer name " + pAnalyzerName + " does not exist", __FILE__, __FUNCTION__, __LINE__);
    
    try
    {
        mAnalyzers[pAnalyzerName]->addObject( pGroupIndex, pNeighborGroup );
    }
    catch (Exception& e)
    {
        throw;
    }
}

void
SpaceObjectsAnalyzeManager::addObject( const std::string& pAnalyzerName, const std::string& pGroupName, const NeighborGroup* pNeighborGroup ) throw (Exception)
{
    if( mAnalyzers.contains(pAnalyzerName) == false ) throw Exception("SPACE ERROR: space objects analyzer name " + pAnalyzerName + " does not exist", __FILE__, __FUNCTION__, __LINE__);
    
    try
    {
        mAnalyzers[pAnalyzerName]->addObject( pGroupName, pNeighborGroup );
    }
    catch (Exception& e)
    {
        throw;
    }
}

void
SpaceObjectsAnalyzeManager::removeObject( const std::string& pAnalyzerName, unsigned int pGroupIndex, const SpaceObject* pSpaceObject ) throw (Exception)
{
    if( mAnalyzers.contains(pAnalyzerName) == false ) throw Exception("SPACE ERROR: space objects analyzer name " + pAnalyzerName + " does not exist", __FILE__, __FUNCTION__, __LINE__);
    
    try
    {
        mAnalyzers[pAnalyzerName]->removeObject( pGroupIndex, pSpaceObject );
    }
    catch (Exception& e)
    {
        throw;
    }
}

void
SpaceObjectsAnalyzeManager::removeObject( const std::string& pAnalyzerName, const std::string& pGroupName, const SpaceObject* pSpaceObject ) throw (Exception)
{
    if( mAnalyzers.contains(pAnalyzerName) == false ) throw Exception("SPACE ERROR: space objects analyzer name " + pAnalyzerName + " does not exist", __FILE__, __FUNCTION__, __LINE__);
    
    try
    {
        mAnalyzers[pAnalyzerName]->removeObject( pGroupName, pSpaceObject );
    }
    catch (Exception& e)
    {
        throw;
    }
}

void
SpaceObjectsAnalyzeManager::removeObject( const std::string& pAnalyzerName, unsigned int pGroupIndex, const NeighborGroup* pNeighborGroup )throw (Exception)
{
    if( mAnalyzers.contains(pAnalyzerName) == false ) throw Exception("SPACE ERROR: space objects analyzer name " + pAnalyzerName + " does not exist", __FILE__, __FUNCTION__, __LINE__);
    
    try
    {
        mAnalyzers[pAnalyzerName]->removeObject( pGroupIndex, pNeighborGroup );
    }
    catch (Exception& e)
    {
        throw;
    }
}

void
SpaceObjectsAnalyzeManager::removeObject( const std::string& pAnalyzerName, const std::string& pGroupName, const NeighborGroup* pNeighborGroup ) throw (Exception)
{
    if( mAnalyzers.contains(pAnalyzerName) == false ) throw Exception("SPACE ERROR: space objects analyzer name " + pAnalyzerName + " does not exist", __FILE__, __FUNCTION__, __LINE__);
    
    try
    {
        mAnalyzers[pAnalyzerName]->removeObject( pGroupName, pNeighborGroup );
    }
    catch (Exception& e)
    {
        throw;
    }
}

void
SpaceObjectsAnalyzeManager::removeObjects( const std::string& pAnalyzerName, unsigned int pGroupIndex ) throw (Exception)
{
    if( mAnalyzers.contains(pAnalyzerName) == false ) throw Exception("SPACE ERROR: space objects analyzer name " + pAnalyzerName + " does not exist", __FILE__, __FUNCTION__, __LINE__);
    
    try
    {
        mAnalyzers[pAnalyzerName]->removeObjects( pGroupIndex );
    }
    catch (Exception& e)
    {
        throw;
    }
}

void
SpaceObjectsAnalyzeManager::removeObjects( const std::string& pAnalyzerName, const std::string& pGroupName ) throw (Exception)
{
    if( mAnalyzers.contains(pAnalyzerName) == false ) throw Exception("SPACE ERROR: space objects analyzer name " + pAnalyzerName + " does not exist", __FILE__, __FUNCTION__, __LINE__);
    
    try
    {
        mAnalyzers[pAnalyzerName]->removeObjects( pGroupName );
    }
    catch (Exception& e)
    {
        throw;
    }
}

const Eigen::VectorXf&
SpaceObjectsAnalyzeManager::result( const std::string& pAnalyzerName, unsigned int pResultIndex ) const throw (Exception)
{
    if( mAnalyzers.contains(pAnalyzerName) == false ) throw Exception("SPACE ERROR: space objects analyzer name " + pAnalyzerName + " does not exist", __FILE__, __FUNCTION__, __LINE__);
    
    try
    {
        mAnalyzers[pAnalyzerName]->result( pResultIndex );
    }
    catch (Exception& e)
    {
        throw;
    }
}

const Eigen::VectorXf&
SpaceObjectsAnalyzeManager::result( const std::string& pAnalyzerName, const std::string& pResultName ) const throw (Exception)
{
    if( mAnalyzers.contains(pAnalyzerName) == false ) throw Exception("SPACE ERROR: space objects analyzer name " + pAnalyzerName + " does not exist", __FILE__, __FUNCTION__, __LINE__);
    
    try
    {
        return mAnalyzers[pAnalyzerName]->result( pResultName );
    }
    catch (Exception& e)
    {
        throw;
    }
}

const IndexMap< std::string, Eigen::VectorXf >&
SpaceObjectsAnalyzeManager::results(  const std::string& pAnalyzerName ) const throw (Exception)
{
    if( mAnalyzers.contains(pAnalyzerName) == false ) throw Exception("SPACE ERROR: space objects analyzer name " + pAnalyzerName + " does not exist", __FILE__, __FUNCTION__, __LINE__);
    
	return mAnalyzers[pAnalyzerName]->results();
}

void
SpaceObjectsAnalyzeManager::update()
{
	analyze();
}

void
SpaceObjectsAnalyzeManager::analyze()
{
	unsigned int analyzerCount = mAnalyzers.size();
	
	for(unsigned int i=0; i<analyzerCount; ++i)
	{
		mAnalyzers[i]->analyze();
	}
}

SpaceObjectsAnalyzeManager::operator std::string() const
{
    return info(0);
}

std::string
SpaceObjectsAnalyzeManager::info(int pPropagationLevel) const
{
    std::stringstream ss;
    
    unsigned int analyzerCount = mAnalyzers.size();
    
	for(unsigned int i=0; i<analyzerCount; ++i)
	{
        ss << "SpaceObjectsAnalyzer: " << mAnalyzers.key(i) << "\n";
	}
    
    return ss.str();
}