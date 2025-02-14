/** \file dab_space_alg.h
*/

#ifndef _dab_space_alg_h_
#define _dab_space_alg_h_

#include <iostream>
#include <vector>
#include <Eigen/Dense>
#include "dab_exception.h"

namespace dab
{

namespace space
{

class SpaceProxyObject;

class SpaceAlg
{
public:
	SpaceAlg( unsigned int mDim );
	SpaceAlg( const Eigen::VectorXf& pMinPos, const Eigen::VectorXf& pMaxPos ) throw (Exception);
	virtual ~SpaceAlg();
	
	bool fixedSize() const;
	unsigned int dim() const;
	const Eigen::VectorXf& minPos() const;
	const Eigen::VectorXf& maxPos() const;
	
	virtual void resize(const Eigen::VectorXf& pMinPos, const Eigen::VectorXf& pMaxPos) throw (Exception);
	virtual void updateStructure( std::vector<SpaceProxyObject*>& pObjects ) throw (Exception);
	virtual void updateNeighbors( std::vector<SpaceProxyObject*>& pObjects ) throw (Exception);
    
	/**
     \brief print space alg information
     */
	operator std::string() const;
    
    /**
     \brief print space alg information
     */
    std::string info() const;
    
  	/**
     \brief retrieve textual space info
     \param pOstream output text stream
     \param pSpace space
     */
 	friend std::ostream& operator << ( std::ostream& pOstream, const SpaceAlg& pAlg )
    {
		pOstream << std::string(pAlg);
        
        return pOstream;
    };
	
protected:
	SpaceAlg();
    
	bool mFixedSize;
	Eigen::VectorXf mMinPos;
	Eigen::VectorXf mMaxPos;
};
    
};
    
};

#endif
