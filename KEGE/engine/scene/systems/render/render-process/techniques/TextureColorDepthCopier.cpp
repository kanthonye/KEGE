//
//  TextureColorDepthCopier.cpp
//  KEGE
//
//  Created by Kenneth Esdaile on 9/30/23.
//

#include "../../render-process/RenderPhase.hpp"
#include "../../render-process/RenderProcess.hpp"
#include "../techniques/TextureColorDepthCopier.hpp"

namespace kege
{
    void TextureColorDepthCopier::execute( core::CommandBuffer* command )
    {
        core::UniformLayout* asset = _render_phase->getInputRenderTarget()->getCurrentUniformLayout();

        _shader->bind( command );
        _shader->bindUniformLayout( 0, asset );
        command->draw( 4, 1, 0, 0 );
    }

    TextureColorDepthCopier::TextureColorDepthCopier()
    :   kege::RenderTechnique( "color-depth-copy.shader" )
    {
    }
}
