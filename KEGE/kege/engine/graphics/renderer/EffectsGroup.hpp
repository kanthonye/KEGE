//
//  EffectsGroup.hpp
//  KEGE
//
//  Created by Kenneth Esdaile on 8/10/23.
//

#ifndef KEGE_EFFECT_GROUP_HPP
#define KEGE_EFFECT_GROUP_HPP

#include "../renderer/RenderGroup.hpp"

namespace std
{
    template <>
    struct hash< kege::Ref< kege::ShaderEffect > >
    {
        size_t operator()( const kege::Ref< kege::ShaderEffect>& effect ) const
        {
            return (size_t) effect.ref();
        }
    };
}

namespace kege
{
    class EffectsGroup : public kege::RefCounter
    {
    public:

        void insert( const kege::ObjectParameter& objparam, const kege::Ref< kege::ShaderEffect >& effect );
        void execute( core::CommandBuffer* command, const kege::Geometry* geometry );
        void clear();
        ~EffectsGroup();
        EffectsGroup();
        
    private:

        std::unordered_map< kege::Ref< ShaderEffect >, uint32_t > _index_group;
        std::vector< kege::Ref< RenderGroup > > _groups;
        RenderLayer* _layer;
        friend RenderLayer;
    };
}

#endif /* KEGE_EFFECT_GROUP_HPP */
