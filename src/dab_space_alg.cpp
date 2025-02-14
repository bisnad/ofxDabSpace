/** \file dab_space_alg.cpp
*/

#include "dab_space_alg.h"

using namespace dab;
using namespace dab::space;

SpaceAlg::SpaceAlg()
: mFixedSize(false)
{}

SpaceAlg::SpaceAlg( unsigned int pDim )
: mMinPos( pDim )
, mMaxPos( pDim )
, mFixedSize(false)
{}

SpaceAlg::SpaceAlg( const Eigen::VectorXf& pMinPos, const Eigen::VectorXf& pMaxPos ) throw (Exception)
: mMinPos( pMinPos )
, mMaxPos( pMaxPos )
, mFixedSize(true)
{
    if(mMinPos.rows() != mMaxPos.rows()) throw Exception("SPACE ERROR: mismatch between minPos dim " + std::to_string(mMinPos.rows()) + " and maxPos dim " + std::to_string(mMaxPos.rows()), __FILE__, __FUNCTION__, __LINE__ );
}

SpaceAlg::~SpaceAlg()
{}

bool
SpaceAlg::fixedSize() const
{
	return mFixedSize;
}

unsigned int
SpaceAlg::dim() const
{
	return mMinPos.rows();
}

const Eigen::VectorXf&
SpaceAlg::minPos() const
{
	return mMinPos;
}

const Eigen::VectorXf&
SpaceAlg::maxPos() const
{
	return mMaxPos;
}

void
SpaceAlg::resize(const Eigen::VectorXf& pMinPos, const Eigen::VectorXf& pMaxPos) throw (Exception)
{
	if( mFixedSize == true ) throw Exception("SPACE ERROR: space size is fixed and cannot be resized", __FILE__, __FUNCTION__, __LINE__ );
	else
	{
		if( mMinPos.rows() != pMinPos.rows() ) throw Exception( "SPACE ERROR: dimension of min pos " + std::to_string(pMinPos.rows()) + " doesn't match space dimension " + std::to_string(mMinPos.rows()), __FILE__, __FUNCTION__, __LINE__ );
		if( mMaxPos.rows() != pMaxPos.rows() ) throw Exception( "SPACE ERROR: dimension of max pos " + std::to_string(pMaxPos.rows()) + " doesn't match space dimension " + std::to_string(mMaxPos.rows()), __FILE__, __FUNCTION__, __LINE__ );
        unsigned int _dim = mMaxPos.rows();
        for(int d=0; d<_dim; ++d)
        {
            if( pMinPos[d] > pMaxPos[d] ) throw Exception( "SPACE ERROR: at dimension " + std::to_string(d) + " minimum space position " + std::to_string(pMinPos[d]) + " is larger than maximum space position " + std::to_string(pMaxPos[d]), __FILE__, __FUNCTION__, __LINE__ );
        }
        
		mMinPos = pMinPos;
		mMaxPos = pMaxPos;
	}
}

void
SpaceAlg::updateStructure( std::vector<SpaceProxyObject*>& pObjects ) throw (Exception)
{
    //std::cout << "SpaceAlg::updateStructure( std::vector<SpaceProxyObject*>& pObjects ) throw (Exception)\n";
}

void
SpaceAlg::updateNeighbors( std::vector<SpaceProxyObject*>& pObjects ) throw (Exception)
{
    //std::cout << "SpaceAlg::updateNeighbors( std::vector<SpaceProxyObject*>& pObjects ) throw (Exception)\n";
}

SpaceAlg::operator std::string() const
{
    return info();
}

std::string
SpaceAlg::info() const
{
    std::stringstream stream;
    
    unsigned int _dim = mMaxPos.rows();
	
    stream << "minPos [ ";
	for(unsigned int d=0; d<_dim; ++d) stream << mMinPos[d] << " ";
	stream << "]\n";
    
    stream << "maxPos [ ";
	for(unsigned int d=0; d<_dim; ++d) stream << mMaxPos[d] << " ";
	stream << "]\n";
    
	return stream.str();
}