//
//  Ellipsoid.hpp
//  KEGE
//
//  Created by Kenneth Esdaile on 3/1/23.
//

#ifndef KEGE_ELLISPOID
#define KEGE_ELLISPOID
#include "../geometry/MeshInfo.hpp"
namespace kege
{
    class Ellipsoid : public MeshInfo
    {
        public: Ellipsoid( float x_radius, float y_radius, int cols, int rows );
    };


    void genEllipsoidVertices( float x_radius, float y_radius, int cols, int rows, std::vector< char >& buffer );
    void genEllipsoidIndices( int cols, int rows, std::vector< char >& buffer );
}
#endif /* KEGE_ELLISPOID */
