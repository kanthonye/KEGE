//
//  SphericalTerrainSubTile.cpp
//  KEGE
//
//  Created by Kenneth Esdaile on 8/26/23.
//

#include "../../spherical-terrain/modules/SphericalQuadtreeTerrain.hpp"
#include "../../spherical-terrain/modules/SphericalTerrainGeometry.hpp"
#include "../../spherical-terrain/modules/SphericalTerrainSubTile.hpp"
namespace kege
{

    void SphericalTerrainSubTile::operator()
    (
         const int& level,
         const int& depth,
         const double& scale,
         const double& radius,
         const kege::vec3& position,
         SphericalTerrainGeometry* geometry
    )
    {
        _geometry = geometry;

        _quad_center = position;
        _position = _geometry->radius() * kege::normalize( _quad_center );
        _radius = radius;
        _scale = scale;
        _depth = depth;
        _level = level;

        _subtiles = nullptr;
    }

    uint16_t SphericalTerrainSubTile::depth()const
    {
        return _depth;
    }

    bool SphericalTerrainSubTile::parent()const
    {
        return _subtiles != nullptr;
    }

    void SphericalTerrainSubTile::split()
    {
        const int   level  = _level + 1;
        const int   depth  = _depth + 1;
        const double scale  = _scale * 0.5;
        const double radius = _radius * 0.5;
        /* this radius is the radius of the sub sphere and use for culling tiles.
         not to be confused with the cube radius/lengths or the root sphere which
         is sphere_radius. the cube lengths are computed into the vertices. so no
         need to store it
         */

        enum{ SW,SE,NW,NE };
        kege::fvec3 p[4];
        p[ NE ] = _quad_center + _geometry->xaxis() * scale + _geometry->zaxis() * scale;
        p[ NW ] = _quad_center - _geometry->xaxis() * scale + _geometry->zaxis() * scale;
        p[ SW ] = _quad_center - _geometry->xaxis() * scale - _geometry->zaxis() * scale;
        p[ SE ] = _quad_center + _geometry->xaxis() * scale - _geometry->zaxis() * scale;

        // construct child nodes
        _subtiles = new SphericalTerrainSubTiles;
        _subtiles->_sw.operator()( level, depth, scale, radius, p[ SW ], _geometry );
        _subtiles->_se.operator()( level, depth, scale, radius, p[ SE ], _geometry );
        _subtiles->_nw.operator()( level, depth, scale, radius, p[ NW ], _geometry );
        _subtiles->_ne.operator()( level, depth, scale, radius, p[ NE ], _geometry );

//        _subtiles->_sw._id = SW;
//        _subtiles->_se._id = SE;
//        _subtiles->_nw._id = NW;
//        _subtiles->_ne._id = NE;
    }

    void SphericalTerrainSubTile::merge()
    {
        if ( _subtiles )
        {
            _subtiles->_ne.merge();
            _subtiles->_nw.merge();
            _subtiles->_se.merge();
            _subtiles->_sw.merge();

            delete _subtiles;
            _subtiles = nullptr;
        }
    }

    SphericalTerrainSubTile::~SphericalTerrainSubTile()
    {
        merge();
    }

    SphericalTerrainSubTile::SphericalTerrainSubTile()
    :   _subtiles( nullptr )
    ,   _position( 0.0 )
    ,   _radius( 0.0 )
    ,   _depth( 0 )
    ,   _visible( true )
    ,   _id(4)
    {}

}
