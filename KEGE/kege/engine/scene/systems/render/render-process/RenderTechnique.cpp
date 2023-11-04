//
//  RenderTechnique.cpp
//  KEGE
//
//  Created by Kenneth Esdaile on 9/24/23.
//

#include "../render-process/RenderProcess.hpp"
#include "../render-process/RenderTechnique.hpp"
namespace kege
{
    RenderTechnique::RenderTechnique( const kege::string& shaderfile )
    {
        _shader = KEGE::core()->graphics()->shaderLibrary().load( shaderfile );
    }

    void RenderTechnique::setRenderPhase( kege::RenderPhase* phase )
    {
        _render_phase = phase;
    }
    
    RenderTechnique::~RenderTechnique()
    {
        _shader.clear();
    }
}
