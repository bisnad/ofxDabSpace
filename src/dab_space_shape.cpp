/** \file dab_space_shape.cpp
*/

#include "dab_math_vec.h"
#include "dab_space_shape.h"
#include "dab_space_neighbors.h"

using namespace dab;
using namespace dab::space;

SpaceShape::SpaceShape()
: SpaceObject(3)
, mGeometry(nullptr)
{}

SpaceShape::SpaceShape( std::shared_ptr<geom::Geometry> pGeometry)
: SpaceObject(3)
, mGeometry(pGeometry)
, mGeometryChanged(true)
, mTransformChanged(true)
{}

SpaceShape::~SpaceShape()
{}

void
SpaceShape::onPositionChanged()
{
    const glm::vec3& newPos = ofNode::getPosition();
    mPosition[0] = newPos.x;
    mPosition[1] = newPos.y;
    mPosition[2] = newPos.z;
    
    mTransformChanged = true;
}

void
SpaceShape::onOrientationChanged()
{
    mTransformChanged = true;
}

void
SpaceShape::onScaleChanged()
{
    mTransformChanged = true;
}

const std::shared_ptr<geom::Geometry>
SpaceShape::geometry() const
{
    return mGeometry;
}

std::shared_ptr<geom::Geometry>
SpaceShape::geometry()
{
    return mGeometry;
}

void
SpaceShape::setGeometryChanged()
{
    mGeometryChanged = true;
}

void
SpaceShape::setTransformChanged()
{
    mTransformChanged = true;
}

void
SpaceShape::setGeometry( std::shared_ptr<geom::Geometry> pGeometry )
{
    mGeometry = pGeometry;
}

void
SpaceShape::setPosition(const Eigen::VectorXf& pPosition) throw (Exception)
{
    try
    {
        ofNode::setPosition(pPosition[0], pPosition[1], pPosition[2]);
        mTransformChanged = true;
    }
    catch(Exception& e)
    {
        throw;
    }
}

void
SpaceShape::changePosition(const Eigen::VectorXf& pPosition) throw (Exception)
{
    try
    {
        ofNode::move(pPosition[0], pPosition[1], pPosition[2]);
        mTransformChanged = true;
    }
    catch(Exception& e)
    {
        throw;
    }
}

void
SpaceShape::setOrientation(const Eigen::Quaternionf& pOrientation)
{
    ofNode::setOrientation(ofQuaternion(pOrientation.x(), pOrientation.y(), pOrientation.z(), pOrientation.w()));
    mTransformChanged = true;
}

void
SpaceShape::setScale(const Eigen::Vector3f& pScale)
{
    ofNode::setScale(pScale[0], pScale[1], pScale[2]);
    mTransformChanged = true;
}

const geom::Cuboid&
SpaceShape::AABB() const
{
    if(mTransformChanged == true || mGeometryChanged == true) const_cast<SpaceShape*>(this)->update();
    
    return mWorldAABB;
}

const geom::Cuboid&
SpaceShape::ocAABB() const
{
    if(mTransformChanged == true || mGeometryChanged == true) const_cast<SpaceShape*>(this)->update();
    
    return mObjectAABB;
}

void
SpaceShape::closestPoint(const glm::vec3& pRefPoint, glm::vec3& pResPoint)
{
    if(mTransformChanged == true || mGeometryChanged == true) const_cast<SpaceShape*>(this)->update();
    
	glm::vec3 refPosObject = world2object(pRefPoint);
	glm::vec3 closestPosObject;
    mGeometry->closestPoint(refPosObject, closestPosObject);
    object2world(closestPosObject, pResPoint);
}
    
SpaceShape::operator std::string() const
{
    if(mTransformChanged == true || mGeometryChanged == true) const_cast<SpaceShape*>(this)->update();
    
    std::stringstream stream;
    
    stream << "position: [ " << ofNode::getPosition() << " ]\n";
    const ofQuaternion& quat = ofNode::getOrientationQuat();
    stream << "orientation (quat): [ " << quat.x() << " " << quat.y() << " " << quat.z() << " " << quat.w() << " ]\n";
    stream << "orientation (euler): [ " << ofNode::getOrientationEuler() << " ]\n";
    stream << "scale : [ " << ofNode::getScale() << " ]\n";
    stream << "object2world matrix :\n" << mObject2WorldTransformMatrix << "\n";
	stream << "world2object matrix :\n" << mWorld2ObjectTransformMatrix << "\n";
	
	stream <<  "object AABB : ";
    stream << "[ " << mObjectAABB.minPos() << " ] ";
    stream<< "[ " << mObjectAABB.maxPos() << " ]\n";
    
    stream <<  "world AABB : ";
    stream << "[ " << mWorldAABB.minPos() << " ] ";
    stream<< "[ " << mWorldAABB.maxPos() << " ]\n";
    
    return stream.str();
}
    
std::string
SpaceShape::info(int pPropagationLevel) const
{
    if(mTransformChanged == true || mGeometryChanged == true) const_cast<SpaceShape*>(this)->update();
    
    std::stringstream stream;
    
    stream << "position: [ " << ofNode::getPosition() << " ]\n";
    const ofQuaternion& quat = ofNode::getOrientationQuat();
    stream << "orientation (quat): [ " << quat.x() << " " << quat.y() << " " << quat.z() << " " << quat.w() << " ]\n";
    stream << "orientation (euler): [ " << ofNode::getOrientationEuler() << " ]\n";
    stream << "scale : [ " << ofNode::getScale() << " ]\n";
    stream << "object2world matrix :\n" << mObject2WorldTransformMatrix << "\n";
	stream << "world2object matrix :\n" << mWorld2ObjectTransformMatrix << "\n";
	
	stream <<  "object AABB : ";
    stream << "[ " << mObjectAABB.minPos() << " ] ";
    stream<< "[ " << mObjectAABB.maxPos() << " ]\n";
    
    stream <<  "world AABB : ";
    stream << "[ " << mWorldAABB.minPos() << " ] ";
    stream<< "[ " << mWorldAABB.maxPos() << " ]\n";

	if(pPropagationLevel != 0)
	{
        stream << "\nNeighbors\n";
        stream << mNeighbors->info(pPropagationLevel - 1);
	}
	
	return stream.str();
}

void
SpaceShape::update()
{
    if(mTransformChanged == true)
    {
        mTransformChanged = false;
        
        updateMatrix();
        updateAABB();
    }
    else if(mGeometryChanged == true)
    {
        mGeometryChanged = false;
        
        updateAABB();
    }
}

void
SpaceShape::updateAABB()
{
    const glm::vec3& geomMinPos = mGeometry->minPos();
    const glm::vec3& geomMaxPos = mGeometry->maxPos();
    
    mObjectAABB.set( geomMinPos, geomMaxPos );
    mWorldAABB.set( object2world(geomMinPos), object2world(geomMaxPos) );
}

void
SpaceShape::updateMatrix()
{
    mObject2WorldTransformMatrix = ofNode::getGlobalTransformMatrix();
	mWorld2ObjectTransformMatrix = glm::inverse(mObject2WorldTransformMatrix);
}

glm::vec3
SpaceShape::world2object( const glm::vec3& pWorldPos ) const
{
    if(mTransformChanged == true || mGeometryChanged == true) const_cast<SpaceShape*>(this)->update();

	// TODO: check if that works
	return math::VectorMath::get().vec3Mat4Mul(mWorld2ObjectTransformMatrix, pWorldPos);
}

void
SpaceShape::world2object( const glm::vec3& pWorldPos, glm::vec3& pObjectPos ) const
{
    if(mTransformChanged == true || mGeometryChanged == true) const_cast<SpaceShape*>(this)->update();
    
	// TODO: check if that works
    pObjectPos = math::VectorMath::get().vec3Mat4Mul(mWorld2ObjectTransformMatrix, pWorldPos);
}

glm::vec3
SpaceShape::object2world( const glm::vec3& pObjectPos ) const
{
    if(mTransformChanged == true || mGeometryChanged == true) const_cast<SpaceShape*>(this)->update();
    
	// TODO: check if that works
	return math::VectorMath::get().vec3Mat4Mul(mObject2WorldTransformMatrix, pObjectPos);
}

void
SpaceShape::object2world( const glm::vec3& pObjectPos, glm::vec3& pWorldPos ) const
{
    if(mTransformChanged == true || mGeometryChanged == true) const_cast<SpaceShape*>(this)->update();
    
	// TODO: check if that works
    pWorldPos = math::VectorMath::get().vec3Mat4Mul(mObject2WorldTransformMatrix, pObjectPos);
}