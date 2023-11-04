//
//  TextureColorDepthCopier.hpp
//  KEGE
//
//  Created by Kenneth Esdaile on 9/30/23.
//

#ifndef TextureColorDepthCopier_hpp
#define TextureColorDepthCopier_hpp

#include "../../render-process/RenderTechnique.hpp"

namespace kege
{
    class TextureColorDepthCopier : public kege::RenderTechnique
    {
    public:

        void execute( core::CommandBuffer* command );
        TextureColorDepthCopier();
    };
}
#endif /* TextureColorDepthCopier_hpp */
