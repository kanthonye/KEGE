//
//  GeneralObjectRenderTechnique.hpp
//  KEGE
//
//  Created by Kenneth Esdaile on 9/24/23.
//

#ifndef GeneralObjectRenderTechnique_hpp
#define GeneralObjectRenderTechnique_hpp

#include "../../render-process/RenderTechnique.hpp"

namespace kege
{
    class GeneralObjectRenderTechnique : public kege::RenderTechnique
    {
    public:

        void execute( core::CommandBuffer* command );
        ~GeneralObjectRenderTechnique();
        GeneralObjectRenderTechnique();

    private:

        std::set< EntityId >* _entities;
    };

}
#endif /* GeneralObjectRenderTechnique_hpp */
