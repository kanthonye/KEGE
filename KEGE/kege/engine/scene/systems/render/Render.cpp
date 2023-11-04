//
//  Render.cpp
//  KEGE
//
//  Created by Kenneth Esdaile on 3/22/23.
//

//#include "Entity.hpp"
//#include "../../components/Transform.hpp"
//#include "../../systems/camera/Camera.hpp"
//#include "../../systems/render/Render.hpp"
//#include "../../../app/Core.hpp"
//#include "../../../app/KEGE.hpp"
//#include "../../../app/Graphics.hpp"
//#include "../../../graphics/material/Material.hpp"
//#include "../../../graphics/geometry/Geometry.hpp"
//#include "../../../graphics/renderer/Renderer.hpp"
#include "../../systems/include/systems.hpp"

namespace kege{ namespace system{

    struct ObjectMatrices
    {
        kege::mat44  _transform;
        kege::mat33  _rotation;
        kege::real32 _distance;
    };

    void Render::render( const double& dms )
    {
        _render_process->execute();
        _render_process->present();
    }
    
    Render::Render( int priority )
    :   kege::EntitySystem( priority, "Render" )
    {
        _components = createComponentBitmask< component::Renderable >();


        enum{ NONE = -1, SWAPCHAIN_FRAMEBUFFER, PHASE_SWAP_BUFFER, SHADOW_BUFFER,};

        _render_process = new RenderProcess;

        _render_process->setRenderTargets
        ({
            {SWAPCHAIN_FRAMEBUFFER, new MainRenderTarget()},
            {PHASE_SWAP_BUFFER, new RenderPhaseSwapBuffer(1024, 512)}
        });

        _render_process->setRenderPhases
        ({  // render the secen to render phase framebuffer texture
            {
                new RenderPhaseMCB
                (
                    PHASE_SWAP_BUFFER, PHASE_SWAP_BUFFER,
                    {
                        //new GeneralObjectRenderTechnique(),
                        //new DebugTriangleRenderTechnique(),
                        new CelestialSurfaceRenderTechnique(),
                        //new AtmosphereVolumeRenderTechnique(),
                        //new SphericalTerrainRenderTechnique(),
                    }
                )
            },
            
            {
                new RenderPhaseMCB
                (
                    PHASE_SWAP_BUFFER, PHASE_SWAP_BUFFER,
                    {new AtmosphereRenderTechnique()}
                )
            },

            // the final render phase. which copy the render texture to the swapchain framebuffer
            {
                new MainRenderPhase
                (
                     SWAPCHAIN_FRAMEBUFFER, PHASE_SWAP_BUFFER,
                     {new TextureColorDepthCopier()}
                )
            }
        });


    }

    Render::~Render( )
    {
        _render_process.clear();
    }
}}
