//
//  EffectsGroup.cpp
//  KEGE
//
//  Created by Kenneth Esdaile on 8/10/23.
//

#include "EffectsGroup.hpp"
#include "RenderLayer.hpp"
#include "RenderLayers.hpp"
namespace kege
{
    void EffectsGroup::insert( const kege::ObjectParameter& objparam, const kege::Ref< kege::ShaderEffect >& effect )
    {
        auto m = _index_group.find( effect );
        if ( m == _index_group.end() )
        {
            uint32_t group_index = (uint32_t) _groups.size();
            _index_group[ effect ] = group_index;
            RenderGroup* group = new RenderGroup;
            group->_effect = this;
            group->insert( objparam );
            _groups.push_back( group );
            return;
        }
        _groups[ m->second ]->insert( objparam );
    }
    void EffectsGroup::execute( core::CommandBuffer* command, const kege::Geometry* geometry )
    {
        for ( auto& m : _index_group )
        {
            //command->bindShader( m.first->shader() );
            m.first->shader()->bind( command );
//            if ( m.first->parameters() )
//            {
//                m.first->parameters()->bind( command );
//            }
            _groups[ m.second ]->execute( command, geometry );
        }
    }
    void EffectsGroup::clear()
    {
        _groups.clear();
        _index_group.clear();
    }
    EffectsGroup::~EffectsGroup()
    {
        clear();
    }
    EffectsGroup::EffectsGroup()
    : _layer( nullptr )
    {}
}
