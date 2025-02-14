

## ofxDabSpace

**Author**:  Daniel Bisig - Coventry University, UK - [ad5041@coventry.ac.uk](ad5041@coventry.ac.uk) - Zurich University of the Arts, CH - [daniel.bisig@zhdk.ch](daniel.bisig@zhdk.ch)

**Dependencies**: [ofxDabBase](https://bitbucket.org/dbisig/ofxdabbase_011/src/master/), [ofxDabMath](https://bitbucket.org/dbisig/ofxdabmath_011/src/master/), [ofxDabGeom](https://bitbucket.org/dbisig/ofxdabgeom_011/src/master/), [ANN](https://ignite.apache.org/docs/latest/machine-learning/binary-classification/ann#:~:text=An%20approximate%20nearest%20neighbor%20search,good%20as%20the%20exact%20one.) (included), [kdtree](https://github.com/jtsiomb/kdtree) (included)

---

## Summary

ofxDabSpace provides functionality to conduct nearest neighbourhood calculations between points in spaces of arbitrarily high dimensionality. The code is compatible with OpenFrameworks 0.11 and has been tested on Windows and MacOS. The following classes are available.

**SpaceObject**: an object that possesses a position in a space

**SpaceShape**: an object that possesses a shape, position, orientation, and scale in space. 

**Space**: Space within which objects exists and which hosts the algorithm for calculating nearest neighbours.

**SpaceManager**: Handles all Spaces

**SpaceGrid**: A vector field with spatial extensions.

**SpaceGridTools**: Calculates distance fields for surfaces (supports three dimensions only at the moment)

**SpaceAlgorithm**: Base class for calculating nearest neighbours

**SpaceAlgorithmANN**: Calculates nearest neighbours using the "Approximate Nearest Neighbourhood" method.

**GridAlg**: Calculates nearest neighbours by partitioning the space into a fixed grid and comparing space objects within neighbouring grid cells.

**KDTreeAlg**: Calculates nearest neighbours using a K-dimensional Tree for space partitioning.

**NtreeAlg**: Calculates nearest neighbours using the principle of Quadtrees or Octtrees but in arbitrary dimensions.

**RTreeAlg**: Calculates nearest neighbours between spatial objects with possess shapes other than points.

**PermanentNeighborsAlg**: Handles distance calculations between space objects that have been manually set to be permanent neighbours.

**SpaceClusterAnalyzer**: Detects clusters among spatial objects

**SpaceNeighborRelation**: stores the direction and distance between a space object and another neighbouring space object.

**SpaceProxyObject**: wraps a space object together with a neighbour group

**NeighborGroup**: Stores all neighbourhood relationships of a single space object.

**NeighborGroupAlg**: Handles how neighbourhood relationships are added to a neighbour group depending on distance and neighbour count limits.

**Neighbors**: stores all the neighbour groups to which a space object belongs to.