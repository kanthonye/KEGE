//
//  DebugTriangleRenderTechnique.hpp
//  KEGE
//
//  Created by Kenneth Esdaile on 10/20/23.
//

#ifndef DebugTriangleRenderTechnique_hpp
#define DebugTriangleRenderTechnique_hpp

#include "../../render-process/RenderTechnique.hpp"

namespace kege
{
    class DebugTriangleRenderTechnique : public kege::RenderTechnique
    {
    public:

        void execute( core::CommandBuffer* command );
        DebugTriangleRenderTechnique();
    };
}
#endif /* DebugTriangleRenderTechnique_hpp */
