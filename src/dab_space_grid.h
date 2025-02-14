/** \file dab_space_grid.h
 */

#ifndef _dab_space_grid_h_
#define _dab_space_grid_h_

#include <Eigen/Dense>
#include "dab_math_vector_field.h"
#include "dab_exception.h"

namespace dab
{
    
namespace space
{
    
enum GridValueSetMode
{
    Nearest,
    Interpol
};

class SpaceGrid
{
public:
    /**
     \brief create space grid
     \param pValueDim dimension of values stored at each grid location
     \param pSubdivisionCount number of gread subdivisions in each dimension
     \param pMinPos minimum position of grid
     \param pMaxPos maximum position of grid
     */
    SpaceGrid(unsigned int pValueDim, const dab::Array<unsigned int>& pSubdivisionCount, const Eigen::VectorXf& pMinPos, const Eigen::VectorXf& pMaxPos) throw (Exception);
    
    /**
     \brief copy constructor
     \param pSpaceGrid space grid
     */
    SpaceGrid(const SpaceGrid& pSpaceGrid);
    
    /**
     \brief destructor
     */
    ~SpaceGrid();
    
    /**
     \brief assignment operator
     \param pSpaceGrid space grid to copy
     \return SpaceGrid
     \remarks fails if space grids are not compatible
     */
    const SpaceGrid& operator= ( const SpaceGrid& pSpaceGrid );
    
    /**
     \brief return dimension of space grid
     \return dimension of space grid
     */
    unsigned int dim() const;
    
    /**
     \brief return dimension of space grid
     \return dimension of space grid
     */
    unsigned int gridDim() const;
    
    /**
     \brief return dimension of values
     \return dimension of values
     */
    unsigned int valueDim() const;
    
    /**
     \brief return subdivision count
     \return subdivision count
     */
    const dab::Array<unsigned int>& subdivisionCount() const;
    
    /**
     \brief return vector field
     \return vector field
     */
    const math::VectorField<float>& vectorField() const;
    
    /**
     \brief return vector field
     \return vector field
     */
    math::VectorField<float>& vectorField();
    
    /**
     \brief return minimum position
     \return minimum position
     */
    const Eigen::VectorXf& minPos() const;
    
    void setMinPos( const Eigen::VectorXf& pMinPos ) throw (Exception);
    
    /**
     \brief return maximum position
     \return maximum position
     */
    const Eigen::VectorXf& maxPos() const;
    
    void setMaxPos( const Eigen::VectorXf& pMaxPos ) throw (Exception);
    
    /**
     \brief return grid index
     \param pPosition position
     */
    dab::Array<unsigned int> position2index( const Eigen::VectorXf& pPosition ) const throw (Exception);
    
    /**
     \brief calculate grid index
     \param pPosition position
     \param pIndex resulting index
     */
    void position2index( const Eigen::VectorXf& pPosition, unsigned int& pIndex ) const throw (Exception);
    
    /**
     \brief calculate grid index
     \param pPosition position
     \param pIndex resulting index
     */
    void position2index( const Eigen::VectorXf& pPosition, dab::Array<unsigned int>& pIndex ) const throw (Exception);
    
    /**
     \brief calculate grid index
     \param pPosition position
     \param pIndex resulting index
     */
    void position2index( const Eigen::VectorXf& pPosition, dab::Array<float>& pIndex ) const throw (Exception);
    
    Eigen::VectorXf index2position( unsigned int pIndex ) const throw (Exception);
    
    Eigen::VectorXf index2position( const dab::Array<unsigned int>& pIndex ) const throw (Exception);
    
    Eigen::VectorXf index2position( const dab::Array<float>& pIndex ) const throw (Exception);
    
    /**
     \brief get value at grid position
     \param pIndex grid index
     \return pGridValue grid value
     */
    const Eigen::VectorXf& value(unsigned int pIndex) const throw (Exception);
    
    /**
     \brief get value at grid position
     \param pGridPosition grid position (with regard to subdivision count)
     \return pGridValue grid value
     */
    const Eigen::VectorXf& value(const dab::Array<unsigned int>& pGridPosition) const throw (Exception);
    
    /**
     \brief return interpolated value
     \param pPosition position (with regard to grid size)
     \return interpolated value
     */
    Eigen::VectorXf value(const Eigen::VectorXf& pPosition) const throw (Exception);
    
    /**
     \brief return interpolated value
     \param pPosition position (with regard to grid size)
     \param pValue interpolated value (written)
     */
    void value(Eigen::VectorXf& pPosition, Eigen::VectorXf& pValue) const throw (Exception);
    
    /**
     \brief get value at grid position
     \param pIndex grid index
     \return pGridValue grid value
     */
    const Eigen::VectorXf& gridValue(unsigned int pIndex) const throw (Exception);
    
    /**
     \brief get value at grid position
     \param pGridPosition grid position (with regard to subdivision count)
     \return pGridValue grid value
     */
    const Eigen::VectorXf& gridValue(const dab::Array<unsigned int>& pGridPosition) const throw (Exception);
    
    /**
     \brief set values
     \param pVectorField vector field
     \remarks fails if vector field doesn't match grid settings
     */
    void setValues( const math::VectorField<float>& pVectorField ) throw (Exception);
    
    /**
     \brief set values
     \param pValue
     
     set all grid values
     */
    void setValues(const Eigen::VectorXf& pValue) throw (Exception);
    
    /**
     \brief change values
     \param pValue
     
     change all grid values
     */
    void changeValues(const Eigen::VectorXf& pValue ) throw (Exception);
    
    /**
     \brief set value
     \param pPosition position
     \param pValue
     \param pSetMode value set mode
     */
    void setValue(const Eigen::VectorXf& pPosition, const Eigen::VectorXf& pValue, GridValueSetMode pSetMode = Interpol ) throw (Exception);
    
    /**
     \brief change value
     \param pPosition position
     \param pValue
     \param pSetMode value set mode
     */
    void changeValue(const Eigen::VectorXf& pPosition, const Eigen::VectorXf& pValue, GridValueSetMode pSetMode = Interpol ) throw (Exception);
    
    /**
     \brief set value at grid location
     \param pIndex grid index
     \param pGridValue grid value
     */
    void setGridValue(unsigned int pIndex, const Eigen::VectorXf& pGridValue) throw (Exception);
    
    /**
     \brief set value at grid location
     \param pGridPosition grid position
     \param pGridValue grid value
     */
    void setGridValue(const dab::Array<unsigned int>& pGridPosition, const Eigen::VectorXf& pGridValue) throw (Exception);
    
    /**
     \brief get textural grid information
     */
    virtual operator std::string() const;
    
    /**
     \brief retrieve textual space grid info
     \param pOstream output text stream
     \param pGrid space grid
     */ 
    friend std::ostream& operator << ( std::ostream& pOstream, const SpaceGrid& pGrid )
    {
        pOstream << std::string(pGrid);
        
        return pOstream;
    };
    
protected:
    /**
     \brief default constructor
     */
    SpaceGrid();
    
    /**
     \brief dimension of space grid
     */
    unsigned int mGridDim;
    
    /**
     \brief dimension of values
     */
    unsigned int mValueDim;
    
    /**
     \brief minimum position of grid
     */
    Eigen::VectorXf mMinPos;
    
    /**
     \brief maximum position of grid
     */
    Eigen::VectorXf mMaxPos;
    
    /**
     \brief n dimensional field of vector values
     */
    math::VectorField<float> mVectorField;
    
    /**
     \brief input position to grid position scale
     */
    Eigen::VectorXf mPositionScale;
};
    
};
    
};

#endif