//
//  SphericalTerrainTile.hpp
//  KEGE
//
//  Created by Kenneth Esdaile on 8/26/23.
//

#ifndef KEGE_SPHERICAL_TERRAIN_GEOMETRY_HPP
#define KEGE_SPHERICAL_TERRAIN_GEOMETRY_HPP

#include "../../../../app/Graphics.hpp"
#include "../../../../graphics/device/core/UniformLayout.hpp"
#include "../../../../graphics/geometry/Geometry.hpp"
#include "../../spherical-terrain/modules/SphericalTerrainSubTile.hpp"

namespace kege{
    
    class DrawInstanceBuffer : public kege::UniformLayout1
    {
    public:
        uint32_t _drawcount = 0;
        uint32_t _capacity = 0;
    };

    class SphericalTerrainGeometry : public kege::Geometry
    {
    public:

        enum FaceId{ FRONT, BACK, LEFT, RIGHT, ABOVE, BELOW, MAX_FACE_COUNT };

        void draw( core::CommandBuffer* command_buffer, uint32_t first_instance, uint32_t instance_count )const;
        const std::vector< Ref< kege::DrawInstanceBuffer > >& getDrawInstanceBuffers()const;
        void pushInstanceBuffer( uint32_t count, const kege::mat24* instance );
        void beginInstanceBufferArray();
        void endInstanceBufferArray();

        SphericalTerrainSubTile& subtile();

        const kege::vec3& xaxis()const;
        const kege::vec3& yaxis()const;
        const kege::vec3& zaxis()const;
        double radius()const;
        FaceId faceId()const;

        SphericalTerrainGeometry( FaceId face_id, double radius, const kege::mat43& points );
        ~SphericalTerrainGeometry();

    private:

        std::vector< Ref< kege::DrawInstanceBuffer > > _instance_buffers;
        SphericalTerrainSubTile _subtile;

        uint32_t _instance_data_stride;
        uint32_t _buffer_count;
        double   _radius;

        kege::vec3 _xaxis;
        kege::vec3 _yaxis;
        kege::vec3 _zaxis;
        FaceId _face_id;
        friend SphericalTerrainSubTile;
    };

    enum { MAX_SPHERICAL_TERRAIN_INSTANCES = 262144 };
}
#endif /* KEGE_SPHERICAL_TERRAIN_GEOMETRY_HPP */
