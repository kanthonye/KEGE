//
//  RenderTechnique.hpp
//  KEGE
//
//  Created by Kenneth Esdaile on 9/24/23.
//

#ifndef RenderTechnique_hpp
#define RenderTechnique_hpp

#include "KEGE.hpp"
#include "Core.hpp"
#include "Graphics.hpp"
#include "Entity.hpp"

namespace kege
{
    class RenderPhase;

    class RenderTechnique : public kege::RefCounter
    {
    public:

        virtual void execute( core::CommandBuffer* command ) = 0;
        void setRenderPhase( kege::RenderPhase* phase );
        virtual ~RenderTechnique();

    protected:

        RenderTechnique( const kege::string& shaderfile );

    protected:

        kege::Ref< core::Shader > _shader;
        kege::RenderPhase* _render_phase;
        friend kege::RenderPhase;
    };

    struct ObjectMatrices
    {
        kege::mat44  _transform;
        kege::mat33  _rotation;
        kege::real32 _distance;
    };

    struct CameraMatrices
    {
        kege::mat44 _projection;
        kege::mat44 _transform;
        kege::vec4  _position;

        kege::real _aspect_ratio;
        kege::real _fov;
        kege::real _near;
        kege::real _far;
    };
}
#endif /* RenderTechnique_hpp */
