//
//  CelestialSurfaceRenderTechnique.hpp
//  KEGE
//
//  Created by Kenneth Esdaile on 10/14/23.
//

#ifndef CelestialSurfaceRenderTechnique_hpp
#define CelestialSurfaceRenderTechnique_hpp

#include "../../render-process/RenderTechnique.hpp"

namespace kege
{
    struct Batch;

    class CelestialSurfaceRenderTechnique : public kege::RenderTechnique
    {
    public:

        enum
        {
            CAMERA_TRANSFORM          = 0,
            FACE_ORIENTATION_MATRICES = 1,
            INSTANCE_BATCH_BUFFER     = 2,
            HEIGHT_MAP                = 3
        };

        void setInput( uint32_t input, const core::UniformLayout* asset );
        void drawGeometry( kege::Batch* batch, const kege::Geometry* geometry );
        void setFaceRotationSSBO( const kege::mat34 orientations[6] );
        void bindGeometry( const kege::Geometry* geometry );
        void execute( core::CommandBuffer* command );
        void begin( core::CommandBuffer* command );
        void render();
        void end();
        ~CelestialSurfaceRenderTechnique();
        CelestialSurfaceRenderTechnique();

    private:

        kege::UniformLayout1 _face_orientation_matrices;
        core::CommandBuffer* _command_buffer;
        std::set< EntityId >* _entities;
    };
}


//technique->setTransformMatrices( orientations );
//technique->setOrientationMatrices( orientations );
//technique->bindGeometry( _cubesphere.getGeometry() );
//for( int i=0; i < manager->getBatchCount(); i++ )
//{
//    kege::Batch* batch = manager->getBatch( i );
//    technique->drawGeometry( batch, _cubesphere.getGeometry() );
//}


#endif /* CelestialSurfaceRenderTechnique_hpp */
