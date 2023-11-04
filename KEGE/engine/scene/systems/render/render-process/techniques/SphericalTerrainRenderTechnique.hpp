//
//  SphericalTerrainRenderTechnique.hpp
//  KEGE
//
//  Created by Kenneth Esdaile on 9/24/23.
//

#ifndef SphericalTerrainRenderTechnique_hpp
#define SphericalTerrainRenderTechnique_hpp

#include "../../render-process/RenderTechnique.hpp"

namespace kege
{
    class SphericalTerrainRenderTechnique : public kege::RenderTechnique
    {
    public:

        void execute( core::CommandBuffer* command );
        ~SphericalTerrainRenderTechnique();
        SphericalTerrainRenderTechnique();

    private:

        enum{ CAMERA_TRANSFORM = 0, INSTANCE_BATCH_BUFFER = 1 };
        std::set< EntityId >* _entities;
    };
}

#endif /* SphericalTerrainRenderTechnique_hpp */
