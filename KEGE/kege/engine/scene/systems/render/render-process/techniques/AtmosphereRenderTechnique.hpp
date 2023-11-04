//
//  AtmosphereRenderTechnique.hpp
//  KEGE
//
//  Created by Kenneth Esdaile on 10/10/23.
//

#ifndef AtmosphereRenderTechnique_hpp
#define AtmosphereRenderTechnique_hpp

#include "../../render-process/RenderTechnique.hpp"

namespace kege
{
    class AtmosphereRenderTechnique : public kege::RenderTechnique
    {
    public:

        void execute( core::CommandBuffer* command );
        ~AtmosphereRenderTechnique();
        AtmosphereRenderTechnique();

    private:

        kege::UniformLayout1 _atmosphere_data;
        std::set< EntityId >* _entities;
    };
}

#endif /* AtmosphereRenderTechnique_hpp */
