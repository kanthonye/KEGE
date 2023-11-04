//
//  SphericalTerrainSubTile.hpp
//  KEGE
//
//  Created by Kenneth Esdaile on 8/26/23.
//

#ifndef SphericalTerrainSubTile_hpp
#define SphericalTerrainSubTile_hpp

#include "cgm.hpp"

namespace kege{

    struct SphericalTerrainGeometry;
    struct SphericalTerrainSubTiles;

    struct SphericalTerrainSubTile
    {
    public:

        void operator()
        (
             const int& level,
             const int& depth,
             const double& scale,
             const double& radius,
             const kege::vec3& vertices,
             SphericalTerrainGeometry* geometry
        );
        uint16_t depth()const;
        bool parent()const;
        void split();
        void merge();

        ~SphericalTerrainSubTile();
        SphericalTerrainSubTile();

        SphericalTerrainSubTiles* _subtiles;
        SphericalTerrainGeometry* _geometry;

        kege::fvec3 _world_position;
        kege::fvec3 _position;
        kege::fvec3 _quad_center;
        uint16_t _level;
        uint16_t _depth;
        double _radius;
        double _scale;
        bool _visible;

        int _id;
    };

    struct SphericalTerrainSubTiles
    {
        SphericalTerrainSubTile _nw;
        SphericalTerrainSubTile _ne;
        SphericalTerrainSubTile _se;
        SphericalTerrainSubTile _sw;
    };

}

#endif /* SphericalTerrainSubTile_hpp */
