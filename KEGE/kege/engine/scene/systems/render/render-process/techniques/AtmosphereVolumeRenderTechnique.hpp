//
//  AtmosphericScatteringRenderTechnique.hpp
//  KEGE
//
//  Created by Kenneth Esdaile on 10/26/23.
//

#ifndef AtmosphereVolumeRenderTechnique_hpp
#define AtmosphereVolumeRenderTechnique_hpp

#include "Cuboid.hpp"
#include "../../render-process/RenderTechnique.hpp"

namespace kege
{
    class AtmosphereVolumeRenderTechnique : public kege::RenderTechnique
    {
    public:

        void execute( core::CommandBuffer* command );
        ~AtmosphereVolumeRenderTechnique();
        AtmosphereVolumeRenderTechnique();

    private:
        
        kege::UniformLayout1 _atmosphere_data;
        kege::Ref< kege::Geometry > _geometry;
        std::set< EntityId >* _entities;
    };


    class AtmosphereMeshInfo : public MeshInfo
    {
        void genEllipsoidIndices( int cols, int rows );
        void genEllipsoidVertices( float x_radius, float y_radius, int cols, int rows );
        public: AtmosphereMeshInfo( float x_radius, float y_radius, int cols, int rows );
    };

    struct VertexPT
    {
        kege::vec3 position;
        kege::vec2 texcoord;
    };
}

#endif /* AtmosphereVolumeRenderTechnique_hpp */
