//
//  SphericalQuadtreeTerrain.hpp
//  KEGE
//
//  Created by Kenneth Esdaile on 9/4/23.
//

#ifndef KEGE_SPHERICAL_TERRAIN_HPP
#define KEGE_SPHERICAL_TERRAIN_HPP

#include "../../spherical-terrain/modules/SphericalTerrainGeometry.hpp"

namespace kege{

    class SphericalQuadtreeTerrain : public kege::RefCounter
    {
    public:

        void collectVisibleSubTiles
        (
            kege::SphericalTerrainGeometry* geometry,
            kege::mat24* instance, uint32_t& count
        );

        void submitVisibleSubTiles
        (
            kege::SphericalTerrainGeometry* geometry,
            kege::SphericalTerrainSubTile& subtile,
            kege::mat24* instance, uint32_t& count
        );

        void updateSubTile
        (
            kege::SphericalTerrainSubTile& subtile,
            const kege::vec3& camera_position
        );

        void updateSubtilePosition
        (
            kege::SphericalTerrainSubTile& subtile
        );

        void doFrustumCulling
        (
            kege::SphericalTerrainSubTile& subtile,
            const kege::Frustum<float>& frustum
        );

        double resolution
        (
            kege::SphericalTerrainSubTile& subtile,
            const kege::vec3& camera_position
        );
        
        SphericalTerrainGeometry* geometries( uint16_t i );

        const kege::mat33& rotation()const;
        const kege::vec3& position()const;

        double maxResolution()const;
        uint32_t maxDepth()const;

        kege::mat33& rotation();
        kege::vec3& position();

        double radius()const;

        void update( const double& dms, const kege::vec3& eye );
        void render( const double& dms );

        SphericalQuadtreeTerrain( double radius );
        ~SphericalQuadtreeTerrain();

    private:

        kege::Ref< SphericalTerrainGeometry > _geometries[ 6 ];
        kege::mat33 _rotation;
        kege::vec3 _position;

        double _max_resolution;
        double _max_depth;
        double _radius;
    };
}
#endif /* KEGE_SPHERICAL_TERRAIN_HPP */
