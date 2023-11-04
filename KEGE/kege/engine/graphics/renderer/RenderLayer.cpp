//
//  RenderLayer.cpp
//  KEGE
//
//  Created by Kenneth Esdaile on 8/10/23.
//

#include "RenderLayers.hpp"
#include "RenderLayer.hpp"
namespace kege
{
    void RenderLayer::insert( const kege::ObjectParameter& objparam, const kege::Ref< kege::Geometry >& geometry, const kege::Ref< kege::ShaderEffect >& effect )
    {
        auto m = _index_group.find( geometry );
        if ( m == _index_group.end() )
        {
            uint32_t group_index = (uint32_t) _groups.size();
            _index_group[ geometry ] = group_index;
            EffectsGroup* group = new EffectsGroup;
            group->_layer = this;
            group->insert( objparam, effect );
            _groups.push_back( group );
            return;
        }
        _groups[ m->second ]->insert( objparam, effect );
    }

    void RenderLayer::execute( core::CommandBuffer* command )
    {
        for ( auto& m : _index_group )
        {
            m.first->bind( command );
            _groups[ m.second ]->execute( command, m.first.ref() );
        }
    }
    
    void RenderLayer::clear()
    {
        _groups.clear();
        _index_group.clear();
    }
    RenderLayer::~RenderLayer()
    {
        clear();
    }
    RenderLayer::RenderLayer()
    : _render_order( UN_ODERED )
    {}
}
