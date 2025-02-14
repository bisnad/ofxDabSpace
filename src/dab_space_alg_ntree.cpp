/** \file dab_space_alg_ntree.cpp
 */

#include "dab_space_alg_ntree.h"

using namespace dab;
using namespace dab::space;

NTreeAlg::NTreeAlg()
: SpaceAlg(2)
, mTree(mMinPos, mMaxPos)
, mTreeVisitor( mMinPos.rows() )
{}

NTreeAlg::NTreeAlg( unsigned int pDim )
: SpaceAlg( pDim )
, mTree(mMinPos, mMaxPos)
, mTreeVisitor( mMinPos.rows() )
{}

NTreeAlg::NTreeAlg( const Eigen::Matrix<float, Eigen::Dynamic, 1>& pMinPos, const Eigen::Matrix<float, Eigen::Dynamic, 1>& pMaxPos ) throw (Exception)
: SpaceAlg( pMinPos, pMaxPos )
, mTree(mMinPos, mMaxPos)
, mTreeVisitor( mMinPos.rows() )
{
    if( mMinPos.rows() != mMaxPos.rows() ) throw Exception("SPACE ERROR: Ntree minPos dimension " + std::to_string(mMinPos.rows()) + " doesn't match maxPos dimension " + std::to_string(mMaxPos.rows()), __FILE__, __FUNCTION__, __LINE__);
}

NTreeAlg::~NTreeAlg()
{}

void
NTreeAlg::resize(const Eigen::Matrix<float, Eigen::Dynamic, 1>& pMinPos, const Eigen::Matrix<float, Eigen::Dynamic, 1>& pMaxPos) throw (Exception)
{
    if( pMinPos.rows() != mMinPos.rows() ) throw Exception("SPACE ERROR: supplied minPos dimension " + std::to_string(pMinPos.rows()) + " doesn't match stored minPos dimension " + std::to_string(mMinPos.rows()), __FILE__, __FUNCTION__, __LINE__);
    if( pMaxPos.rows() != mMaxPos.rows() ) throw Exception("SPACE ERROR: supplied maxPos dimension " + std::to_string(pMaxPos.rows()) + " doesn't match stored maxPos dimension " + std::to_string(mMaxPos.rows()), __FILE__, __FUNCTION__, __LINE__);
    
	try
	{
		SpaceAlg::resize( pMinPos, pMaxPos );
		mTree.resize(mMinPos, mMaxPos);
		mTreeVisitor.clearTree(mTree);
	}
	catch(Exception& e)
	{
		throw;
	}
}

void
NTreeAlg::updateStructure( std::vector< SpaceProxyObject* >& pObjects ) throw (Exception)
{
    if(pObjects.size() > 0 && pObjects[0]->dim() != dim()) throw Exception("SPACE ERROR: object dimension " + std::to_string(pObjects[0]->dim()) + " doesn't match ntree dimension " + std::to_string(dim()), __FILE__, __FUNCTION__, __LINE__);
    
    mTreeVisitor.updateTree(mTree, pObjects);
}

void
NTreeAlg::updateNeighbors( std::vector< SpaceProxyObject* >& pObjects ) throw (Exception)
{
    if(pObjects.size() > 0 && pObjects[0]->dim() != dim()) throw Exception("SPACE ERROR: object dimension " + std::to_string(pObjects[0]->dim()) + " doesn't match ntree dimension " + std::to_string(dim()), __FILE__, __FUNCTION__, __LINE__);
    
    mTreeVisitor.calcNeighbors(mTree, pObjects);
}

NTreeAlg::operator std::string() const
{
    return info();
}

std::string
NTreeAlg::info() const
{
    std::stringstream stream;
    
    stream << "NTreeAlg\n";
    stream << SpaceAlg::info();
    
    return stream.str();
}