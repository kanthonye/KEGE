//
//  RenderLayer.hpp
//  KEGE
//
//  Created by Kenneth Esdaile on 8/10/23.
//

#ifndef KEGE_RENDER_LAYER_HPP
#define KEGE_RENDER_LAYER_HPP

#include "../renderer/EffectsGroup.hpp"

namespace std
{
    template <>
    struct hash< kege::Ref< kege::Geometry > >
    {
        size_t operator()( const kege::Ref< kege::Geometry>& geometry ) const
        {
            return (size_t) geometry.ref();
        }
    };
}

namespace kege
{
    class RenderLayer : public kege::RefCounter
    {
    public:

        void insert( const kege::ObjectParameter& objparam, const kege::Ref< kege::Geometry >& geometry, const kege::Ref< kege::ShaderEffect >& effect );
        
        enum RenderOrder{ UN_ODERED, ASCE_ORDER, DESC_ORDER };
        
        void execute( core::CommandBuffer* command );
        void clear();
        ~RenderLayer();
        RenderLayer();

    private:

        std::unordered_map< kege::Ref< kege::Geometry >, uint32_t > _index_group;
        /* each distinct geometry correspond to an effects group index
         */

        std::vector< kege::Ref< EffectsGroup > > _groups;
        /* this vector of effects group, group instance base on share effects
         */

        RenderOrder _render_order;
        /* this indicate whether visible objects should be ordered or not base
         on their distance from the camera. descending order - farthest to closest,
         ascending order - closest to farthest, or un ordered - order isn't important.
         */

        RenderLayers* _process;
        /* the parent process of this layer
         */
        friend RenderLayers;
    };
}

#endif /* KEGE_RENDER_LAYER_HPP */
