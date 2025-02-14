/** \file dab_space_grid.cpp
*/

#include "dab_space_grid.h"

using namespace dab;
using namespace dab::space;

SpaceGrid::SpaceGrid()
: mValueDim(3)
, mGridDim(3)
, mMinPos( Eigen::Vector3f( 0.0, 0.0, 0.0 ) )
, mMaxPos( Eigen::Vector3f( 1.0, 1.0, 1.0 ) )
, mVectorField( dab::Array<unsigned int>( { 10, 10, 10 } ), Eigen::Vector3f( 0.0, 0.0, 0.0 ) )
, mPositionScale(1)
{}

SpaceGrid::SpaceGrid(unsigned int pValueDim, const dab::Array<unsigned int>& pSubdivisionCount, const Eigen::VectorXf& pMinPos, const Eigen::VectorXf& pMaxPos) throw (Exception)
: mValueDim(pValueDim)
, mGridDim(pSubdivisionCount.size())
, mMinPos(pMinPos)
, mMaxPos(pMaxPos)
, mVectorField(pSubdivisionCount, Eigen::VectorXf(mGridDim))
, mPositionScale(mGridDim)
{
    if( mMinPos.rows() != mGridDim ) throw Exception("SPACE ERROR: dimension of grid minimum position " +  std::to_string(mMinPos.rows())+  " doesn't match dimension of grid " + std::to_string(mGridDim), __FILE__, __FUNCTION__, __LINE__  );
    if( mMaxPos.rows() != mGridDim ) throw Exception("SPACE ERROR: dimension of grid maximum position " +  std::to_string(mMaxPos.rows())+  " doesn't match dimension of grid " + std::to_string(mGridDim), __FILE__, __FUNCTION__, __LINE__  );

	for(unsigned int i=0; i<mGridDim; ++i)
	{
		mPositionScale[i] = ( static_cast<float>(pSubdivisionCount[i]) - 1 ) / (mMaxPos[i] - mMinPos[i]);
	}
}

SpaceGrid::SpaceGrid(const SpaceGrid& pSpaceGrid)
: mGridDim(pSpaceGrid.mGridDim)
, mValueDim(pSpaceGrid.mValueDim)
, mMinPos(pSpaceGrid.mMinPos)
, mMaxPos(pSpaceGrid.mMaxPos)
, mVectorField(pSpaceGrid.mVectorField)
, mPositionScale(pSpaceGrid.mPositionScale)
{}

SpaceGrid::~SpaceGrid()
{}

const SpaceGrid&
SpaceGrid::operator= ( const SpaceGrid& pSpaceGrid )
{
    mGridDim = pSpaceGrid.mGridDim;
    mValueDim = pSpaceGrid.mValueDim;
    mMinPos = pSpaceGrid.mMinPos;
    mMaxPos = pSpaceGrid.mMaxPos;
    mVectorField = pSpaceGrid.mVectorField;
    mPositionScale = pSpaceGrid.mPositionScale;
	
	return *this;
}

unsigned int
SpaceGrid::dim() const
{
    return mGridDim;
}

unsigned int
SpaceGrid::gridDim() const
{
    return mGridDim;
}

unsigned int
SpaceGrid::valueDim() const
{
    return mValueDim;
}


const dab::Array<unsigned int>&
SpaceGrid::subdivisionCount() const
{
    return mVectorField.size();
}

const math::VectorField<float>&
SpaceGrid::vectorField() const
{
    return mVectorField;
}

math::VectorField<float>&
SpaceGrid::vectorField()
{
    return mVectorField;
}

const Eigen::VectorXf&
SpaceGrid::minPos() const
{
    return mMinPos;
}

void
SpaceGrid::setMinPos( const Eigen::VectorXf& pMinPos ) throw (Exception)
{
    if( pMinPos.rows() != mMinPos.rows() ) throw Exception("SPACE ERROR: position provided dimension " + std::to_string(pMinPos.rows()) + " doesn't match required dimension " + std::to_string(mMinPos.rows()), __FILE__, __FUNCTION__, __LINE__ );
    
    mMinPos = pMinPos;
    
    const dab::Array<unsigned int>& fieldSize = mVectorField.size();
    
	for(unsigned int i=0; i<mGridDim; ++i)
	{
		mPositionScale[i] = ( static_cast<float>(fieldSize[i]) - 1) / (mMaxPos[i] - mMinPos[i]);
	}
}

const Eigen::VectorXf&
SpaceGrid::maxPos() const
{
    return mMaxPos;
}

void
SpaceGrid::setMaxPos( const Eigen::VectorXf& pMaxPos ) throw (Exception)
{
    if( pMaxPos.rows() != mMaxPos.rows() ) throw Exception("SPACE ERROR: position provided dimension " + std::to_string(pMaxPos.rows()) + " doesn't match required dimension " + std::to_string(mMaxPos.rows()), __FILE__, __FUNCTION__, __LINE__ );
    
    mMaxPos = pMaxPos;
    
    const dab::Array<unsigned int>& fieldSize = mVectorField.size();
    
	for(unsigned int i=0; i<mGridDim; ++i)
	{
		mPositionScale[i] = ( static_cast<float>(fieldSize[i]) - 1) / (mMaxPos[i] - mMinPos[i]);
	}
}

dab::Array<unsigned int>
SpaceGrid::position2index( const Eigen::VectorXf& pPosition ) const throw (Exception)
{
    if( pPosition.rows() != mGridDim ) throw Exception("SPACE ERROR: position provided dimension " + std::to_string(pPosition.rows()) + " doesn't match required dimension " + std::to_string(mGridDim), __FILE__, __FUNCTION__, __LINE__ );

	const dab::Array<unsigned int>& subdivisionCount = mVectorField.size();
	dab::Array<unsigned int> index(mGridDim);
	
	for(unsigned int i=0; i<mGridDim; ++i)
	{
		if(pPosition[i] <= mMinPos[i]) index[i] = 0;
		else if(pPosition[i] >= mMaxPos[i])  index[i] = static_cast<unsigned int>( static_cast<int>( subdivisionCount[i] ) - 1 );
		else index[i] = static_cast<unsigned int>( ( pPosition[i] - mMinPos[i] ) / ( mMaxPos[i] - mMinPos[i] ) * static_cast< float >( subdivisionCount[i] ) );
	}	
	
	return 	index;
}

void
SpaceGrid::position2index( const Eigen::VectorXf& pPosition, unsigned int& pIndex ) const throw (Exception)
{
    if( pPosition.rows() != mGridDim ) throw Exception("SPACE ERROR: position provided dimension " + std::to_string(pPosition.rows()) + " doesn't match required dimension " + std::to_string(mGridDim), __FILE__, __FUNCTION__, __LINE__ );
	
	pIndex = 0;
	unsigned int indexMul = 1;
	const dab::Array<unsigned int>& subdivisionCount = mVectorField.size();
    
	for(unsigned int i=0; i<mGridDim; ++i)
	{
		if(pPosition[i] >= mMaxPos[i])  pIndex += ( static_cast<int>( subdivisionCount[i] ) - 1) * indexMul;
		else if(pPosition[i] > mMinPos[i]) pIndex += static_cast<unsigned int>( (pPosition[i] - mMinPos[i]) * mPositionScale[i] ) * indexMul;
		
		indexMul *= subdivisionCount[i];
	}
}

void
SpaceGrid::position2index( const Eigen::VectorXf& pPosition, dab::Array<unsigned int>& pIndex ) const throw (Exception)
{
    if( pPosition.rows() != mGridDim ) throw Exception("SPACE ERROR: provided position dimension " + std::to_string(pPosition.rows()) + " doesn't match required dimension " + std::to_string(mGridDim), __FILE__, __FUNCTION__, __LINE__ );
    if( pIndex.size() != mGridDim ) throw Exception("SPACE ERROR: provided index dimension " + std::to_string(pIndex.size()) + " doesn't match required dimension " + std::to_string(mGridDim), __FILE__, __FUNCTION__, __LINE__ );
	
	const dab::Array<unsigned int>& subdivisionCount = mVectorField.size();
    
	for(unsigned int i=0; i<mGridDim; ++i)
	{
		if(pPosition[i] <= mMinPos[i]) pIndex[i] = 0;
		else if(pPosition[i] >= mMaxPos[i]) pIndex[i] = static_cast<unsigned int>( static_cast<int>( subdivisionCount[i] ) - 1 );
		else pIndex[i] = static_cast<unsigned int>( ( pPosition[i] - mMinPos[i] ) / ( mMaxPos[i] - mMinPos[i] ) * static_cast< float >( subdivisionCount[i] ) );
	}
}

void
SpaceGrid::position2index( const Eigen::VectorXf& pPosition, dab::Array<float>& pIndex ) const throw (Exception)
{
    if( pPosition.rows() != mGridDim ) throw Exception("SPACE ERROR: provided position dimension " + std::to_string(pPosition.rows()) + " doesn't match required dimension " + std::to_string(mGridDim), __FILE__, __FUNCTION__, __LINE__ );
    if( pIndex.size() != mGridDim ) throw Exception("SPACE ERROR: provided index dimension " + std::to_string(pIndex.size()) + " doesn't match required dimension " + std::to_string(mGridDim), __FILE__, __FUNCTION__, __LINE__ );
	
	const dab::Array<unsigned int>& subdivisionCount = mVectorField.size();
	
	for(unsigned int i=0; i<mGridDim; ++i)
	{
		if(pPosition[i] <= mMinPos[i]) pIndex[i] = 0.0;
		else if(pPosition[i] >= mMaxPos[i]) pIndex[i] = static_cast<float>( subdivisionCount[i] ) - 1.0;
		else pIndex[i] = ( pPosition[i] - mMinPos[i] ) / ( mMaxPos[i] - mMinPos[i] ) * static_cast< float >( subdivisionCount[i] );
	}
}

Eigen::VectorXf
SpaceGrid::index2position( unsigned int pIndex ) const throw (Exception)
{
    try
    {
        dab::Array<unsigned int> indexVec = mVectorField.calcIndex( pIndex );
        const dab::Array<unsigned int>& subdivisionCount = mVectorField.size();
        Eigen::VectorXf position(mGridDim);
        
        for(unsigned int i=0; i<mGridDim; ++i)
        {
            if(indexVec[i] >= subdivisionCount[i]) position[i] = mMaxPos[i];
            else position[i] = mMinPos[i] + ( static_cast<float>( indexVec[i] ) + 0.5 ) * ( mMaxPos[i] - mMinPos[i] ) / ( subdivisionCount[i] );
        }
        
        return 	position;
    }
    catch (Exception& e)
    {
        e += Exception("SPACE ERROR: failed to get position for index " + std::to_string(pIndex), __FILE__, __FUNCTION__, __LINE__);
        throw e;
    }
}

Eigen::VectorXf
SpaceGrid::index2position( const dab::Array<unsigned int>& pIndex ) const throw (Exception)
{
    if( pIndex.size() != mGridDim ) throw Exception("SPACE ERROR: provided index dimension " + std::to_string(pIndex.size()) + " doesn't match required dimension " + std::to_string(mGridDim), __FILE__, __FUNCTION__, __LINE__ );
    
	const dab::Array<unsigned int>& subdivisionCount = mVectorField.size();
	Eigen::VectorXf position(mGridDim);
	
	for(unsigned int i=0; i<mGridDim; ++i)
	{
		if(pIndex[i] >= subdivisionCount[i]) position[i] = mMaxPos[i];
		else position[i] = mMinPos[i] + ( static_cast<float>( pIndex[i] ) + 0.5 ) * ( mMaxPos[i] - mMinPos[i] ) / ( subdivisionCount[i] );
	}
	
	return 	position;
}

Eigen::VectorXf
SpaceGrid::index2position( const dab::Array<float>& pIndex ) const throw (Exception)
{
    if( pIndex.size() != mGridDim ) throw Exception("SPACE ERROR: provided index dimension " + std::to_string(pIndex.size()) + " doesn't match required dimension " + std::to_string(mGridDim), __FILE__, __FUNCTION__, __LINE__ );
    
	const dab::Array<unsigned int>& subdivisionCount = mVectorField.size();
	Eigen::VectorXf position(mGridDim);
	
	for(unsigned int i=0; i<mGridDim; ++i)
	{
		if(pIndex[i] >= subdivisionCount[i]) position[i] = mMaxPos[i];
		else position[i] = mMinPos[i] + ( pIndex[i] + 0.5 ) * ( mMaxPos[i] - mMinPos[i] ) / ( subdivisionCount[i] );
	}
	
	return 	position;
}

const Eigen::VectorXf&
SpaceGrid::value(unsigned int pIndex) const throw (Exception)
{
    try
    {
        return mVectorField.get(pIndex);
    }
    catch (Exception& e)
    {
        e += Exception("SPACE ERROR: failed to get value for index " + std::to_string(pIndex), __FILE__, __FUNCTION__, __LINE__);
        throw e;
    }
}

const Eigen::VectorXf&
SpaceGrid::value(const dab::Array<unsigned int>& pGridPosition) const throw (Exception)
{
    try
    {
        return mVectorField.get(pGridPosition);
    }
    catch (Exception& e)
    {
        std::string gridPosString = pGridPosition;
        e += Exception("SPACE ERROR: failed to get value for index " + gridPosString, __FILE__, __FUNCTION__, __LINE__);
        throw e;
    }
}

Eigen::VectorXf
SpaceGrid::value(const Eigen::VectorXf& pPosition) const throw (Exception)
{
    if( pPosition.rows() != mGridDim ) throw Exception("SPACE ERROR: provided position dimension " + std::to_string(pPosition.rows()) + " doesn't match required dimension " + std::to_string(mGridDim), __FILE__, __FUNCTION__, __LINE__ );

	// transform position
    dab::Array<float> position(mGridDim);
    
	for(unsigned int i=0; i<mGridDim; ++i)
	{
		if(pPosition[i] <= mMinPos[i]) position[i] = mMinPos[i];
		else if(pPosition[i] >= mMaxPos[i])  position[i] = mMaxPos[i];
		
		position[i] = (pPosition[i] - mMinPos[i]) * mPositionScale[i];
	}
    
    try
    {
        return mVectorField.get(position);
    }
    catch (Exception& e)
    {
        std::string gridPosString = position;
        e += Exception("SPACE ERROR: failed to get value for index " + gridPosString, __FILE__, __FUNCTION__, __LINE__);
        throw e;
    }
}

void
SpaceGrid::value(Eigen::VectorXf& pPosition, Eigen::VectorXf& pValue) const throw (Exception)
{
    if( pPosition.rows() != mGridDim ) throw Exception("SPACE ERROR: provided position dimension " + std::to_string(pPosition.rows()) + " doesn't match required dimension " + std::to_string(mGridDim), __FILE__, __FUNCTION__, __LINE__ );
    if( pValue.rows() != mValueDim ) throw Exception("SPACE ERROR: provided value dimension " + std::to_string(pValue.rows()) + " doesn't match required dimension " + std::to_string(mValueDim), __FILE__, __FUNCTION__, __LINE__ );
    
    // transform position
	dab::Array<float> position(mGridDim);
    
	for(unsigned int i=0; i<mGridDim; ++i)
	{
		if(pPosition[i] <= mMinPos[i]) position[i] = mMinPos[i];
		else if(pPosition[i] >= mMaxPos[i])  position[i] = mMaxPos[i];
		
		position[i] = (pPosition[i] - mMinPos[i]) * mPositionScale[i];
	}

    try
    {
        mVectorField.get(position, pValue);
    }
    catch (Exception& e)
    {
        std::string gridPosString = position;
        e += Exception("SPACE ERROR: failed to get value for index " + gridPosString, __FILE__, __FUNCTION__, __LINE__);
        throw e;
    }
}

const Eigen::VectorXf&
SpaceGrid::gridValue(unsigned int pIndex) const throw (Exception)
{
    try
    {
        return mVectorField.get(pIndex);
    }
    catch (Exception& e)
    {
        e += Exception("SPACE ERROR: failed to get value for index " + std::to_string(pIndex), __FILE__, __FUNCTION__, __LINE__);
        throw e;
    }
}

const Eigen::VectorXf&
SpaceGrid::gridValue(const dab::Array<unsigned int>& pGridPosition) const throw (Exception)
{
    try
    {
        return mVectorField.get(pGridPosition);
    }
    catch (Exception& e)
    {
        std::string gridPosString = pGridPosition;
        e += Exception("SPACE ERROR: failed to get value for index " + gridPosString, __FILE__, __FUNCTION__, __LINE__);
        throw e;
    }
}

void
SpaceGrid::setValues( const math::VectorField<float>& pVectorField ) throw (Exception)
{
	try
	{
		mVectorField = pVectorField;
	}
	catch(Exception& e)
	{
        e += Exception("SPACE ERROR: failed to set values ", __FILE__, __FUNCTION__, __LINE__);
        throw e;
	}
}

void
SpaceGrid::setValues(const Eigen::VectorXf& pValue) throw (Exception)
{
    if( pValue.rows() != mValueDim ) throw Exception("SPACE ERROR: provided value dimension " + std::to_string(pValue.rows()) + " doesn't match required dimension " + std::to_string(mValueDim), __FILE__, __FUNCTION__, __LINE__ );
    
	try
	{
		mVectorField.set( pValue);
	}
	catch(Exception& e)
	{
        std::string valueString;
        for(int d=0; d<pValue.rows(); ++d) valueString += std::to_string(pValue[d]) + " ";
        e += Exception("SPACE ERROR: failed to set values " + valueString, __FILE__, __FUNCTION__, __LINE__);
        throw e;
	}
}

void
SpaceGrid::changeValues(const Eigen::VectorXf& pValue ) throw (Exception)
{
    if( pValue.rows() != mValueDim ) throw Exception("SPACE ERROR: provided value dimension " + std::to_string(pValue.rows()) + " doesn't match required dimension " + std::to_string(mValueDim), __FILE__, __FUNCTION__, __LINE__ );
    
    try
	{
		mVectorField.add( pValue);
	}
	catch(Exception& e)
	{
        std::string valueString;
        for(int d=0; d<pValue.rows(); ++d) valueString += std::to_string(pValue[d]) + " ";
        e += Exception("SPACE ERROR: failed to add values " + valueString, __FILE__, __FUNCTION__, __LINE__);
        throw e;
	}
}

void
SpaceGrid::setValue(const Eigen::VectorXf& pPosition, const Eigen::VectorXf& pValue, GridValueSetMode pSetMode ) throw (Exception)
{
    if( pPosition.rows() != mGridDim ) throw Exception("SPACE ERROR: provided position dimension " + std::to_string(pPosition.rows()) + " doesn't match required dimension " + std::to_string(mGridDim), __FILE__, __FUNCTION__, __LINE__ );
    if( pValue.rows() != mValueDim ) throw Exception("SPACE ERROR: provided value dimension " + std::to_string(pValue.rows()) + " doesn't match required dimension " + std::to_string(mValueDim), __FILE__, __FUNCTION__, __LINE__ );
	
	// transform position
	dab::Array<float> position(mGridDim);
    
	for(unsigned int i=0; i<mGridDim; ++i)
	{
		if(pPosition[i] < mMinPos[i]) position[i] = mMinPos[i];
		else if(pPosition[i] > mMaxPos[i])  position[i] = mMaxPos[i];
		else position[i] = (pPosition[i] - mMinPos[i]) * mPositionScale[i];
	}
    
    try
	{
        if(pSetMode == Nearest) mVectorField.set( dab::Array<unsigned int>( position ), pValue);
        else mVectorField.set( position, pValue);
	}
	catch(Exception& e)
	{
        std::string valueString;
        for(int d=0; d<pValue.rows(); ++d) valueString += std::to_string(pValue[d]) + " ";
        e += Exception("SPACE ERROR: failed to set values " + valueString, __FILE__, __FUNCTION__, __LINE__);
        throw e;
	}
}

void
SpaceGrid::changeValue(const Eigen::VectorXf& pPosition, const Eigen::VectorXf& pValue, GridValueSetMode pSetMode ) throw (Exception)
{
    if( pPosition.rows() != mGridDim ) throw Exception("SPACE ERROR: provided position dimension " + std::to_string(pPosition.rows()) + " doesn't match required dimension " + std::to_string(mGridDim), __FILE__, __FUNCTION__, __LINE__ );
    if( pValue.rows() != mValueDim ) throw Exception("SPACE ERROR: provided value dimension " + std::to_string(pValue.rows()) + " doesn't match required dimension " + std::to_string(mValueDim), __FILE__, __FUNCTION__, __LINE__ );
	
	// transform position
	dab::Array<float> position(mGridDim);
    
	for(unsigned int i=0; i<mGridDim; ++i)
	{
		if(pPosition[i] < mMinPos[i]) position[i] = mMinPos[i];
		else if(pPosition[i] > mMaxPos[i])  position[i] = mMaxPos[i];
		else position[i] = (pPosition[i] - mMinPos[i]) * mPositionScale[i];
	}
    
    try
	{
        if(pSetMode == Nearest) mVectorField.add( dab::Array<unsigned int>( position ), pValue);
        else mVectorField.add( position, pValue);
	}
	catch(Exception& e)
	{
        std::string valueString;
        for(int d=0; d<pValue.rows(); ++d) valueString += std::to_string(pValue[d]) + " ";
        e += Exception("SPACE ERROR: failed to add values " + valueString, __FILE__, __FUNCTION__, __LINE__);
        throw e;
	}
}

void
SpaceGrid::setGridValue(unsigned int pIndex, const Eigen::VectorXf& pGridValue) throw (Exception)
{
    if( pGridValue.rows() != mValueDim ) throw Exception("SPACE ERROR: provided value dimension " + std::to_string(pGridValue.rows()) + " doesn't match required dimension " + std::to_string(mValueDim), __FILE__, __FUNCTION__, __LINE__ );
    
    try
	{
        mVectorField.set(pIndex, pGridValue);
    }
    catch(Exception& e)
    {
        std::string valueString;
        for(int d=0; d<pGridValue.rows(); ++d) valueString += std::to_string(pGridValue[d]) + " ";
        e += Exception("SPACE ERROR: failed to set grid value " + valueString + " at index " + std::to_string(pIndex), __FILE__, __FUNCTION__, __LINE__);
            throw e;
    }
}

void
SpaceGrid::setGridValue(const dab::Array<unsigned int>& pGridPosition, const Eigen::VectorXf& pGridValue) throw (Exception)
{
    if( pGridPosition.size() != mGridDim ) throw Exception("SPACE ERROR: provided position dimension " + std::to_string(pGridPosition.size()) + " doesn't match required dimension " + std::to_string(mGridDim), __FILE__, __FUNCTION__, __LINE__ );
    if( pGridValue.rows() != mValueDim ) throw Exception("SPACE ERROR: provided value dimension " + std::to_string(pGridValue.rows()) + " doesn't match required dimension " + std::to_string(mValueDim), __FILE__, __FUNCTION__, __LINE__ );
    
    try
	{
        mVectorField.set(pGridPosition, pGridValue);
    }
    catch(Exception& e)
    {
        std::string gridPositionString = pGridPosition;
        std::string valueString;
        for(int d=0; d<pGridValue.rows(); ++d) valueString += std::to_string(pGridValue[d]) + " ";
        e += Exception("SPACE ERROR: failed to set grid value " + valueString + " at grid position " + gridPositionString, __FILE__, __FUNCTION__, __LINE__);
        throw e;
    }
}

SpaceGrid::operator std::string() const
{
    std::stringstream stream;

    stream << "valueDim: " << mValueDim << "\n";
    stream << "gridDim: " << mGridDim << "\n";
    stream << "minPos: [ ";
    for(unsigned int i=0; i<mGridDim; ++i) stream << mMinPos[i] << " ";
    stream << "]\n";
    stream << "maxPos: [ ";
    for(unsigned int i=0; i<mGridDim; ++i) stream << mMaxPos[i] << " ";
    stream << "]\n";
    
	unsigned int fieldVectorCount = mVectorField.vectorCount();
	const dab::Array<unsigned int>& vectorFieldSize = mVectorField.size();
	int gridDim_1 = static_cast<int>(mGridDim) - 1;
	
	dab::Array<unsigned int>fieldVectorIndex(mGridDim);
	
	for(unsigned int i=0; i < fieldVectorCount; ++i)
	{
		const Eigen::VectorXf& fieldVectorValue = mVectorField.get(fieldVectorIndex);
        
		stream << "gridPos: ";
		stream << "[ ";
		for(unsigned int i=0; i<mGridDim; ++i) stream << fieldVectorIndex[i] << " ";
		stream << "] ";
        
		stream << "gridValue: ";
		stream << "[ ";
		for(unsigned int i=0; i<mValueDim; ++i) stream << fieldVectorValue[i] << " ";
		stream << "]";
		stream << "\n";
        
		fieldVectorIndex[0] += 1;
        
		for(int j=0; j<gridDim_1; ++j)
		{
			if(fieldVectorIndex[j] >= vectorFieldSize[j])
			{
				fieldVectorIndex[j] = 0;
				fieldVectorIndex[j + 1] += 1;
			}
		}
	}
	
	return stream.str();
}
