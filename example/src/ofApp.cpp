#include "ofApp.h"
#include <Eigen/Dense>
#include "dab_exception.h"
#include "dab_math.h"
#include "dab_array.h"
#include "dab_geom_spline_surface.h"
#include "dab_geom_mesh_tools.h"
#include "dab_space.h"
#include "dab_space_manager.h"
#include "dab_space_alg.h"
#include "dab_space_alg_permanent_neighbors.h"
#include "dab_space_alg_kdtree.h"
#include "dab_space_object.h"
#include "dab_space_proxy_object.h"
#include "dab_space_neighbor_relation.h"
#include "dab_space_neighbor_group.h"
#include "dab_space_neighbors.h"
#include "dab_space_neighbor_group_alg.h"
#include "dab_space_grid.h"
#include "dab_space_grid_tools.h"
#include "dab_space_shape.h"
#include "dab_geom_line.h"
#include "dab_space_ntree_tests.h"

//--------------------------------------------------------------
void ofApp::setup()
{

	try
	{
		dab::space::SpaceManager& spaceManager = dab::space::SpaceManager::get();
		unsigned int objectDim = 3;
		dab::space::SpaceAlg* alg = new dab::space::KDTreeAlg(objectDim);
		std::shared_ptr<dab::space::Space> space(new dab::space::Space("space", alg));

		spaceManager.addSpace(space);

		unsigned int objectCount = 10000;

		for (unsigned int i = 0; i<objectCount; ++i)
		{
			spaceManager.addObject("space", new dab::space::SpaceObject(objectDim), true);
		}

		//dab::space::NtreeTests::get().runTests();

		//// bezier spline surface test
		//	std::vector< glm::vec3 > controls(20);
		//	for (int cI = 0; cI < controls.size(); ++cI) controls[cI] = glm::vec3(ofRandom(-1.0, 1.0), ofRandom(-1.0, 1.0), ofRandom(-1.0, 1.0));

		//	dab::geom::SplineSurface splineSurface(controls, { 5, 4 });
		//	splineSurface.setClosed({ false,false });
		//	splineSurface.setSubdivisionCount({ 4,4 });

		//	std::shared_ptr<dab::geom::BezierSurfaceAlg> surfaceAlg = std::shared_ptr<dab::geom::BezierSurfaceAlg>(new dab::geom::BezierSurfaceAlg());
		//	dab::geom::BezierSurfaceTangentAlg tangentAlg(dab::geom::AutoBezierTangentMode);
		//	tangentAlg.setTension(0.75);
		//	surfaceAlg->setTangentAlg(tangentAlg);
		//	splineSurface.setSplineAlg(surfaceAlg);
		//	splineSurface.update();
		//
		//        std::shared_ptr<dab::geom::Mesh> mesh( new dab::geom::Mesh());
		//        dab::geom::MeshTools::get().createMesh(splineSurface, *mesh, dab::geom::MeshTools::TriangleIndexMode);
		//
		//
		//        //
		//        //        //dab::geom::MeshTools::get().MeshTools(splineSurface, mesh, dab::geom::MeshTools::PointIndexMode);
		//        //        //dab::geom::MeshTools::get().MeshTools(splineSurface, mesh, dab::geom::MeshTools::LineHIndexMode);
		//        //        //dab::geom::MeshTools::get().MeshTools(splineSurface, mesh, dab::geom::MeshTools::LineVIndexMode);
		//        //        //dab::geom::MeshTools::get().MeshTools(splineSurface, mesh, dab::geom::MeshTools::LineTriangleIndexMode);
		//        //        //dab::geom::MeshTools::get().MeshTools(splineSurface, mesh, dab::geom::MeshTools::LineQuadIndexMode);
		//        //        dab::geom::MeshTools::get().createMesh(splineSurface, mesh, dab::geom::MeshTools::TriangleIndexMode)
		//
		//        //std::shared_ptr<dab::geom::Line> lineObject( new dab::geom::Line( ofVec3f(-1.0, 0.0, 0.0), ofVec3f(1.0, 0.0, 0.0) ) );
		//        //std::shared_ptr<dab::geom::Cuboid> cubeObject( new dab::geom::Cuboid( ofVec3f(-1.0, -1.0, -1.0), ofVec3f(1.0, 1.0, 1.0) ) );
		//        dab::space::SpaceShape* spaceShape = new dab::space::SpaceShape(mesh);
		//
		//
		//        glm::vec3 worldRefPos(0.5, 1.0, 0.0);
		//		glm::vec3 worldClosestPos;
		//
		//        spaceShape->closestPoint(worldRefPos, worldClosestPos);
		//
		//        std::cout << "shape worldClosestPos " << worldClosestPos << "\n";
		////
		////        //spaceShape->setPosition( ofVec3f(0.0, 0.5, 0.0) );
		////
		////        spaceShape->ofNode::setPosition(0.0, 0.5, 0.0);
		//        spaceShape->setScale(0.5);
		////        //spaceShape->setOrientation(ofVec3f( 30.0, 0.0, 0.0 ));
		////
		////        spaceShape->closestPoint(worldRefPos, worldClosestPos);
		////
		////        std::cout << "shape worldClosestPos " << worldClosestPos << "\n";
		////
		////        std::cout << "spaceShape\n" << *spaceShape << "\n";
		//
		//        dab::space::SpaceGrid* distanceGrid = dab::space::SpaceGridTools::get().createDistanceGrid(std::array<unsigned int, 3>({ 5, 5, 5 }), ofVec3f( -1.0, -1.0, -1.0 ), ofVec3f( 1.0, 1.0, 1.0 ), spaceShape);
		//

		std::shared_ptr<dab::geom::Line> lineObject( new dab::geom::Line( glm::vec3(0.0, 0.0, 0.0), glm::vec3(1.0, 0.0, 0.0) ) );
		dab::space::SpaceShape* spaceShape = new dab::space::SpaceShape(lineObject);
		
		glm::vec3 worldRefPos(0.5, 1.0, 0.0);
		glm::vec3 worldClosestPos;
		
		lineObject->closestPoint(worldRefPos, worldClosestPos);
		std::cout << "geom worldClosestPos " << worldClosestPos << "\n";

		spaceShape->closestPoint(worldRefPos, worldClosestPos);
		
		std::cout << "shape worldClosestPos " << worldClosestPos << "\n";
		
		spaceShape->setPosition(Eigen::Vector3f(0.0, 0.5, 0.0));
		
		spaceShape->closestPoint(worldRefPos, worldClosestPos);
		
		std::cout << "shape worldClosestPos " << worldClosestPos << "\n";
		//
		//
		////        dab::space::SpaceGrid grid( 2, dab::Array<unsigned int>( {9, 9} ), Eigen::Vector2f(0.0, 0.0), Eigen::Vector2f(1.0, 1.0) );
		////
		////        std::cout << "grid\n" << grid << "\n";
		////
		////        Eigen::Vector2f posVec( 0.0, 0.0 );
		////
		////        while( posVec[0] <= 1.0 )
		////        {
		////            std::cout << "orig pos " << posVec.transpose();
		////            std::cout << " rindex " << grid.position2index( posVec );
		////            std::cout << " rpos " << grid.index2position( grid.position2index( posVec ) ).transpose() << "\n";
		////            posVec[0] += 0.05;
		////        }
	}
	catch (dab::Exception& e)
	{
		std::cout << e << "\n";
	}

}

//--------------------------------------------------------------
void ofApp::update() {

}

//--------------------------------------------------------------
void ofApp::draw() {

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {

}
