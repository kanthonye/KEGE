//
//  Render.hpp
//  KEGE
//
//  Created by Kenneth Esdaile on 3/22/23.
//

#ifndef KEGE_RENDER_HPP
#define KEGE_RENDER_HPP
#include "../../ecs/EntitySystem.hpp"
#include "StdMesh.hpp"
#include "Material.hpp"
#include "../../../graphics/device/core/Device.hpp"


#include "render-process/RenderPhaseMCB.hpp"
#include "render-process/RenderProcess.hpp"
#include "render-process/MainRenderPhase.hpp"
#include "render-process/targets/MainRenderTarget.hpp"
#include "render-process/targets/RenderPhaseSwapBuffer.hpp"
#include "render-process/techniques/TextureColorDepthCopier.hpp"
#include "render-process/techniques/GeneralObjectRenderTechnique.hpp"
#include "render-process/techniques/AtmosphereRenderTechnique.hpp"
#include "render-process/techniques/AtmosphereVolumeRenderTechnique.hpp"
#include "render-process/techniques/SphericalTerrainRenderTechnique.hpp"
#include "render-process/techniques/CelestialSurfaceRenderTechnique.hpp"
#include "render-process/techniques/DebugTriangleRenderTechnique.hpp"

namespace kege{ namespace system{

    class Render : public kege::EntitySystem
    {
    public:

        void render( const double& dms );
        Render( int priority );
        ~Render();

        kege::Ref< RenderProcess > _render_process;
    };

}}

namespace kege{ namespace component{

    struct Renderable
    {
        Renderable
        (
            kege::Ref< kege::StdMesh > m,
            uint32_t render_pass,
            uint32_t layer
        )
        :   _mesh(m)
        ,   _render_pass( render_pass )
        ,   _layer( layer )
        ,   _visible( true )
        {}

        Renderable()
        {}

        kege::Ref< kege::StdMesh > _mesh;
        uint32_t _render_pass;
        uint32_t _layer;
        bool _visible;
    };

    struct Atmosphere
    {
        float _object_radius   = 1000;
        float _max_altitude    =  200;
        int   _num_in_scatter  =   60;
        int   _num_out_scatter =   10;
    };

}}
#endif /* KEGE_RENDER_HPP */
