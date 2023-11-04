//
//  SphericalQuadtreeTerrain.cpp
//  KEGE
//
//  Created by Kenneth Esdaile on 9/4/23.
//

#include "../../spherical-terrain/modules/SphericalQuadtreeTerrain.hpp"

namespace kege{

    void SphericalQuadtreeTerrain::updateSubTile
    (
         kege::SphericalTerrainSubTile& subtile,
         const kege::vec3& camera_position
    )
    {
        updateSubtilePosition( subtile );
        double resolution = this->resolution( subtile, camera_position );

        if( resolution < maxResolution() )
        {
            if ( !subtile.parent() && subtile.depth() < maxDepth() )
            {
                subtile.split();
            }

            if( subtile.parent() )
            {
                updateSubTile( subtile._subtiles->_ne, camera_position );
                updateSubTile( subtile._subtiles->_nw, camera_position );
                updateSubTile( subtile._subtiles->_se, camera_position );
                updateSubTile( subtile._subtiles->_sw, camera_position );
            }
        }
        else if ( subtile.parent() )
        {
            subtile.merge();
        }
    }

    void SphericalQuadtreeTerrain::updateSubtilePosition( kege::SphericalTerrainSubTile& subtile )
    {
        subtile._world_position = subtile._position + _position;
    }

    void SphericalQuadtreeTerrain::submitVisibleSubTiles
    (
         kege::SphericalTerrainGeometry* geometry,
         kege::SphericalTerrainSubTile& subtile,
         kege::mat24* instance, uint32_t& count
    )
    {
        if ( subtile.parent() )
        {
            submitVisibleSubTiles( geometry, subtile._subtiles->_ne, instance, count );
            submitVisibleSubTiles( geometry, subtile._subtiles->_nw, instance, count );
            submitVisibleSubTiles( geometry, subtile._subtiles->_se, instance, count );
            submitVisibleSubTiles( geometry, subtile._subtiles->_sw, instance, count );
        }
        else if ( subtile._visible )
        {
            if ( count >= MAX_SPHERICAL_TERRAIN_INSTANCES )
            {
                geometry->pushInstanceBuffer( count, instance );
                count = 0;
            }
            instance[ count ][0] = kege::vec4( subtile._quad_center, subtile._scale );
            instance[ count ][1] = kege::vec4( geometry->radius(), subtile._id, 0.0f, 0.0f );
            count++;
        }
    }


    void SphericalQuadtreeTerrain::collectVisibleSubTiles
    (
         kege::SphericalTerrainGeometry* geometry,
         kege::mat24* instance, uint32_t& count
    )
    {
        geometry->beginInstanceBufferArray();
        submitVisibleSubTiles( geometry, geometry->subtile(), instance, count );
        if ( count != 0 )
        {
            geometry->pushInstanceBuffer( count, instance );
            count = 0;
        }
        geometry->endInstanceBufferArray();
    }

    void SphericalQuadtreeTerrain::doFrustumCulling
    (
         kege::SphericalTerrainSubTile& subtile,
         const kege::Frustum<float>& frustum
    )
    {
        if ( subtile.parent() )
        {
            doFrustumCulling( subtile._subtiles->_ne, frustum );
            doFrustumCulling( subtile._subtiles->_nw, frustum );
            doFrustumCulling( subtile._subtiles->_se, frustum );
            doFrustumCulling( subtile._subtiles->_sw, frustum );
        }
        else
        {
            float dp = kege::dot( frustum.GetForwardVector(), kege::normalize( subtile._world_position ) );
            if ( dp < 0.0 )
            {
                subtile._visible = false;
                return;
            }

            subtile._visible = frustum.TestSphere( subtile._quad_center, subtile._radius );
        }
    }

    double SphericalQuadtreeTerrain::resolution
    (
         kege::SphericalTerrainSubTile& subtile,
         const kege::vec3& camera_position
    )
    {
        double radius_sq = kege::sq(subtile._radius + subtile._radius);
        double dist = kege::distsq( subtile._world_position, camera_position );
        return dist / radius_sq;
    }

    SphericalTerrainGeometry* SphericalQuadtreeTerrain::geometries( uint16_t i )
    {
        return _geometries[ i ].ref();
    }

    SphericalQuadtreeTerrain::SphericalQuadtreeTerrain( double radius )
    : _max_resolution( 3 )
    , _max_depth( 24 )
    , _radius( radius )
    {
        kege::vec3 p[ 8 ];
        kege::mat43 vertices[ SphericalTerrainGeometry::MAX_FACE_COUNT ];

        float length = 1.0f;
        p[0] = kege::vec3(  length,  length,  length );
        p[1] = kege::vec3( -length,  length,  length );
        p[2] = kege::vec3( -length, -length,  length );
        p[3] = kege::vec3(  length, -length,  length );

        p[4] = kege::vec3(  length,  length, -length );
        p[5] = kege::vec3( -length,  length, -length );
        p[6] = kege::vec3( -length, -length, -length );
        p[7] = kege::vec3(  length, -length, -length );

        vertices[ SphericalTerrainGeometry::FRONT ][0] = p[0];
        vertices[ SphericalTerrainGeometry::FRONT ][1] = p[1];
        vertices[ SphericalTerrainGeometry::FRONT ][2] = p[2];
        vertices[ SphericalTerrainGeometry::FRONT ][3] = p[3];

        vertices[ SphericalTerrainGeometry::BACK ][0] = p[5];
        vertices[ SphericalTerrainGeometry::BACK ][1] = p[4];
        vertices[ SphericalTerrainGeometry::BACK ][2] = p[7];
        vertices[ SphericalTerrainGeometry::BACK ][3] = p[6];

        vertices[ SphericalTerrainGeometry::LEFT ][0] = p[1];
        vertices[ SphericalTerrainGeometry::LEFT ][1] = p[5];
        vertices[ SphericalTerrainGeometry::LEFT ][2] = p[6];
        vertices[ SphericalTerrainGeometry::LEFT ][3] = p[2];

        vertices[ SphericalTerrainGeometry::RIGHT ][0] = p[4];
        vertices[ SphericalTerrainGeometry::RIGHT ][1] = p[0];
        vertices[ SphericalTerrainGeometry::RIGHT ][2] = p[3];
        vertices[ SphericalTerrainGeometry::RIGHT ][3] = p[7];

        vertices[ SphericalTerrainGeometry::ABOVE ][0] = p[0];
        vertices[ SphericalTerrainGeometry::ABOVE ][1] = p[4];
        vertices[ SphericalTerrainGeometry::ABOVE ][2] = p[5];
        vertices[ SphericalTerrainGeometry::ABOVE ][3] = p[1];

        vertices[ SphericalTerrainGeometry::BELOW ][0] = p[7];
        vertices[ SphericalTerrainGeometry::BELOW ][1] = p[3];
        vertices[ SphericalTerrainGeometry::BELOW ][2] = p[2];
        vertices[ SphericalTerrainGeometry::BELOW ][3] = p[6];

        _geometries[ SphericalTerrainGeometry::FRONT ] = new SphericalTerrainGeometry( SphericalTerrainGeometry::FRONT, radius, vertices[ SphericalTerrainGeometry::FRONT ] );
        _geometries[ SphericalTerrainGeometry::BACK  ] = new SphericalTerrainGeometry( SphericalTerrainGeometry::BACK,  radius, vertices[ SphericalTerrainGeometry::BACK  ] );
        _geometries[ SphericalTerrainGeometry::LEFT  ] = new SphericalTerrainGeometry( SphericalTerrainGeometry::LEFT,  radius, vertices[ SphericalTerrainGeometry::LEFT  ] );
        _geometries[ SphericalTerrainGeometry::RIGHT ] = new SphericalTerrainGeometry( SphericalTerrainGeometry::RIGHT, radius, vertices[ SphericalTerrainGeometry::RIGHT ] );
        _geometries[ SphericalTerrainGeometry::ABOVE ] = new SphericalTerrainGeometry( SphericalTerrainGeometry::ABOVE, radius, vertices[ SphericalTerrainGeometry::ABOVE ] );
        _geometries[ SphericalTerrainGeometry::BELOW ] = new SphericalTerrainGeometry( SphericalTerrainGeometry::BELOW, radius, vertices[ SphericalTerrainGeometry::BELOW ] );
    }

    const kege::mat33& SphericalQuadtreeTerrain::rotation()const
    {
        return _rotation;
    }

    const kege::vec3& SphericalQuadtreeTerrain::position()const
    {
        return _position;
    }

    double SphericalQuadtreeTerrain::maxResolution()const
    {
        return _max_resolution;
    }

    uint32_t SphericalQuadtreeTerrain::maxDepth()const
    {
        return _max_depth;
    }

    kege::mat33& SphericalQuadtreeTerrain::rotation()
    {
        return _rotation;
    }

    kege::vec3& SphericalQuadtreeTerrain::position()
    {
        return _position;
    }

    double SphericalQuadtreeTerrain::radius()const
    {
        return _radius;
    }

    void SphericalQuadtreeTerrain::update( const double& dms, const kege::vec3& eye )
    {
        for( uint32_t i=0; i<6; i++ )
        {
            SphericalTerrainGeometry* geometry = geometries( i );
            updateSubTile( geometry->subtile(), eye );
        }
    }

    void SphericalQuadtreeTerrain::render( const double& dms )
    {
        std::vector< kege::mat24 > instance( MAX_SPHERICAL_TERRAIN_INSTANCES );
        uint32_t count = 0;

        for( uint32_t i=0; i<6; i++ )
        {
            SphericalTerrainGeometry* geometry = geometries( i );
            collectVisibleSubTiles( geometry, instance.data(), count );
        }
    }

    SphericalQuadtreeTerrain::~SphericalQuadtreeTerrain()
    {
        for( uint32_t i=0; i<6; i++ )
        {
            _geometries[i].clear();
        }
    }
}
