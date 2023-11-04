//
//  RenderGroup.hpp
//  KEGE
//
//  Created by Kenneth Esdaile on 8/10/23.
//

#ifndef KEGE_RENDER_GROUP_HPP
#define KEGE_RENDER_GROUP_HPP

#include <map>
#include <vector>
#include <unordered_map>
#include "../../memory/Ref.hpp"
#include "../math/cgm.hpp"
#include "../geometry/Geometry.hpp"
#include "../material/Material.hpp"
#include "../device/core/Framebuffer.hpp"
#include "../device/core/CommandBuffer.hpp"

namespace kege
{
    class Effect;
    class Renderer;
    class EffectsGroup;
    class RenderLayer;
    class RenderLayers;

    struct GlobalParameter
    {
        kege::fmat44 _projection;
        kege::fmat44 _transform;
        kege::fmat44 _padding[2];
    };

    struct ObjectParameter
    {
        kege::mat44  _transform;
        kege::mat33  _rotation;
        kege::real32 _distance;
    };

    class RenderGroup : public kege::RefCounter
    {
    public:

        void execute( core::CommandBuffer* command, const kege::Geometry* geometry );
        void insert( const kege::ObjectParameter& param );
        void clear();
        ~RenderGroup();
        RenderGroup();

    private:

        std::vector< ObjectParameter > _elements;
        EffectsGroup* _effect;
        friend EffectsGroup;
    };
}



#endif /* KEGE_RENDER_GROUP_HPP */
