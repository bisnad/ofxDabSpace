/** \file dab_space_shape.h
 * important: contrary to iso space library, this class can only be used for 3D space
*/

#ifndef _dab_space_shape_h_
#define _dab_space_shape_h_

#include <iostream>
#include <Eigen/Dense>
#include "ofNode.h"
#include "dab_geom_geometry.h"
#include "dab_geom_cuboid.h"
#include "dab_space_object.h"

namespace dab
{

namespace space
{

class SpaceShape : public SpaceObject, public ofNode
{
public:
    /**
     \brief create shape
     \param pGeometry shape geometry
     */
    SpaceShape( std::shared_ptr<geom::Geometry> pGeometry);

    /**
     \brief destructor
     */
    virtual ~SpaceShape();
    
    void onPositionChanged();
	void onOrientationChanged();
	void onScaleChanged();
    
    /**
     \brief return geometry
     \return geometry
     */
    const std::shared_ptr<geom::Geometry> geometry() const;

    /**
     \brief return geometry
     \return geometry
     */
    std::shared_ptr<geom::Geometry> geometry();

    void setGeometryChanged();
    void setTransformChanged();
    
    /**
     \brief set geometry
     \param pGeometry geometry
     replaces current geometry with new geometry
     */
    void setGeometry( std::shared_ptr<geom::Geometry> pGeometry );

	/**
     \brief set space object position
     \param pPosition space object position
     \exception Exception if position has wrong dimension
     */
	void setPosition(const Eigen::VectorXf& pPosition) throw (Exception);
    
	/**
     \brief change space object position
     \param pPosition space object position offset
     \exception Exception if  position has wrong dimension
     */
	void changePosition(const Eigen::VectorXf& pPosition) throw (Exception);
    
    /**
     \brief set space object orientation
     \param pOrientation space object orientation
     */
	void setOrientation(const Eigen::Quaternionf& pOrientation);
    
    /**
     \brief set space object scale
     \param pScale space object scale
     */
	void setScale(const Eigen::Vector3f& pScale);
    
    /**
     \brief return axis aligned bounding box (in world coordinates)
     */
    const geom::Cuboid& AABB() const;
    
    /**
     \brief return axis aligned bounding box (in object coordinates)
     */
    const geom::Cuboid& ocAABB() const;
    
    /**
     \brief return closest point
     \param pRefPoint reference point (in world coordinates)
     \param pResPoint result point (in world coordinates)
     \remarks fails if dimension of reference and result points don't match dimension of shape
     */
    void closestPoint(const glm::vec3& pRefPoint, glm::vec3& pResPoint);
    
	glm::vec3 world2object( const glm::vec3& pWorldPos ) const;
    void world2object( const glm::vec3& pWorldPos, glm::vec3& pObjectPos ) const;
	glm::vec3 object2world( const glm::vec3& pObjectPos ) const;
    void object2world( const glm::vec3& pObjectPos, glm::vec3& pWorldPos ) const;
  
    /**
     \brief obtain textual space shape information
     */
    virtual operator std::string() const;
    
    /**
     \brief obtain textual space shape information
     \param pPropagationLevel how far the propagation method proceeds through composite classes (-1: unlimited, 0: no proceeding, >0: limited proceeding)
     */
    virtual std::string info(int pPropagationLevel = 0) const;
    
    /**
     \brief retrieve textual space shape info
     \param pOstream output text stream
     \param pShape space shape
     */
    friend std::ostream& operator << ( std::ostream& pOstream, const SpaceShape& pShape )
    {
        pOstream << std::string(pShape);
        
        return pOstream;
    }; 
    
protected:
    /**
     \brief default constructor
     */
    SpaceShape();
    
    void update();
    
    /**
     \brief update axis aligned bounding box of geometry
     */
	void updateAABB();
    
    /**
     \brief update homogenous matrix
     */
    void updateMatrix();
    
    std::shared_ptr<geom::Geometry> mGeometry; /// \brief shape geometry
    geom::Cuboid mObjectAABB; /// \brief axis aligned bounding box (object space)
    geom::Cuboid mWorldAABB; /// \brief axis aligned bounding box (world space)
    
    bool mGeometryChanged;
    bool mTransformChanged;

	glm::mat4x4 mObject2WorldTransformMatrix;
	glm::mat4x4 mWorld2ObjectTransformMatrix;
};

};

};


#endif
