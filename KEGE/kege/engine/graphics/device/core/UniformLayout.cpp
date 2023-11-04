//
//  UniformLayout.cpp
//  KEGE
//
//  Created by Kenneth Esdaile on 10/20/23.
//

#include "UniformLayout.hpp"

namespace kege{namespace core{

    void UniformLayout::setUniformHandle( const kege::Ref< UniformHandle >& handle )const
    {
        _handle = handle;
    }
    const UniformHandle* UniformLayout::getUniformHandle()const
    {
        return _handle.ref();
    }
    UniformHandle* UniformLayout::getUniformHandle()
    {
        return _handle.ref();
    }

    void UniformLayout::update()
    {
        if ( _handle == nullptr ) return;
        _handle->update( this );
    }

    UniformLayout:: ~UniformLayout()
    {
        _handle.clear();
    }
}}
