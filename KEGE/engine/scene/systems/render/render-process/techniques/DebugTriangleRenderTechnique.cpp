//
//  DebugTriangleRenderTechnique.cpp
//  KEGE
//
//  Created by Kenneth Esdaile on 10/20/23.
//

#include "../../../include/systems.hpp"
#include "../../render-process/RenderPhase.hpp"
#include "../../render-process/RenderProcess.hpp"
#include "../techniques/DebugTriangleRenderTechnique.hpp"

namespace kege
{
    void DebugTriangleRenderTechnique::execute( core::CommandBuffer* command )
    {
        _shader->bind( command );
        command->draw( 3, 1, 0, 0 );
    }

    DebugTriangleRenderTechnique::DebugTriangleRenderTechnique()
    :   kege::RenderTechnique( "debug-triangle.shader" )
    {
    }
}
