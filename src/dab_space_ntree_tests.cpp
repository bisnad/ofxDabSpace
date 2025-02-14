/** \file dab_space_ntree_tests.cpp
*/

#include "dab_space_ntree_tests.h"
#include "ofMath.h"
#include "ofUtils.h"
#include "dab_exception.h"
#include "dab_singleton.h"
#include "dab_space.h"
#include "dab_space_object.h"
#include "dab_space_alg_ntree.h"
#include "dab_space_alg_kdtree.h"
#include "dab_space_alg_ann.h"
#include "dab_space_neighbor_relation.h"

using namespace dab;
using namespace dab::space;

void
NtreeTests::runTests()
{
    try
    {
        test();
    }
    catch (dab::Exception& e)
    {
        std::cout << e << "\n";
    }
}

void
NtreeTests::test() throw (Exception)
{
    try
    {
        unsigned int objectDim = 3;
        unsigned int objectCount = 10000;
        std::vector<SpaceObject*> spaceObjects(objectCount);

        //SpaceAlg* alg = new NTreeAlg( objectDim );
        SpaceAlg* alg = new KDTreeAlg( objectDim );
        //SpaceAlg* alg = new ANNAlg( objectDim );
        Space* space = new Space( "space", alg );
        for(unsigned int i=0; i<objectCount; ++i)
        {
            spaceObjects[i] = new SpaceObject( objectDim );
            //space->addObject( spaceObjects[i], true, new NeighborGroupAlg(0.05, 8, true) );
            space->addObject( spaceObjects[i], true, new NeighborGroupAlg(0.05, 8, true) );
        }

        for(unsigned int oI=0; oI<objectCount; ++oI)
        {
            Eigen::VectorXf& objectPosition = spaceObjects[oI]->position();
            for(unsigned int d=0; d<objectDim; ++d) objectPosition[d] = ofRandom( -1.0, 1.0 );
        }

        for(unsigned int i=0; i<1000; ++i)
        {
            double time1 = static_cast<double>(ofGetElapsedTimeMillis());
            
    //			for(unsigned int oI=0; oI<objectCount; ++oI)
    //			{
    //				math::Vector<space::real>& objectPosition = spaceObjects[oI]->position();
    //				for(unsigned int d=0; d<objectDim; ++d) objectPosition.c[d] = math.random( -1.0, 1.0 );
    //			}

            space->update();

//			for(unsigned int oI=0; oI<objectCount; ++oI)
//			{
//                std::vector<SpaceNeighborRelation*>& neighborRelations = spaceObjects[oI]->neighborGroup("space")->neighborRelations();
//				unsigned int neighborCount = neighborRelations.size();
//
//				std::cout << oI << " ( " << neighborCount << " ) : ";
//
//				for(unsigned int nI=0; nI<neighborCount; ++nI)
//				{
//					std::cout <<  neighborRelations[nI]->distance() << " ";
//				}
//				std::cout << "\n";
//			}
            
            double time2 = static_cast<double>(ofGetElapsedTimeMillis());

            double timeDiff = time2 - time1;
            std::cout << "space update rate " << 1000.0 / timeDiff << "\n";
            
            
        }
    }
    catch(Exception& e)
	{
		throw;
	}
}



