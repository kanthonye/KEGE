//
//  UniformBinding.cpp
//  KEGE
//
//  Created by Kenneth Esdaile on 10/20/23.
//

#include "UniformBinding.hpp"

namespace kege{

    void BufferBinding::setBuffer( uint32_t i, const kege::Ref< core::Buffer >& buffer )
    {
        _buffer[i] = buffer;
    }

    const core::Buffer* BufferBinding::getBuffer( uint32_t i )const
    {
        return _buffer[i].ref();
    }

    core::Buffer* BufferBinding::getBuffer( uint32_t i )
    {
        return _buffer[i].ref();
    }

    uint32_t BufferBinding::getCount()const
    {
        return 1;
    }

    BufferBinding::BufferBinding( const kege::string& name, uint32_t binding, const kege::Ref< core::Buffer >& buffer )
    :   core::UniformBinding( BUFFER, binding, name )
    ,   _buffer{ buffer }
    {}

    BufferBinding::~BufferBinding()
    {
        _buffer[ 0 ].clear();
    }

}



namespace kege{

    TextureSamplerBinding::TextureSamplerBinding( const kege::string& name, uint32_t binding, const kege::Ref< core::Sampler >& sampler, const kege::Ref< core::Texture > &texture )
    :   core::UniformBinding( COMBINED_IMAGE_SAMPLER, binding, name )
    ,   _texture{ texture }
    ,   _sampler( sampler )
    {}

    void TextureSamplerBinding::setTexture( uint32_t i, const kege::Ref< core::Texture >& buffer )
    {
        _texture[ i ] = buffer;
    }

    const core::Texture* TextureSamplerBinding::getTexture( uint32_t i )const
    {
        return _texture[ i ].ref();
    }

    core::Texture* TextureSamplerBinding::getTexture( uint32_t i )
    {
        return _texture[ i ].ref();
    }

    const core::Sampler* TextureSamplerBinding::getSampler()const
    {
        return _sampler.ref();
    }

    core::Sampler* TextureSamplerBinding::getSampler()
    {
        return _sampler.ref();
    }

    uint32_t TextureSamplerBinding::getCount()const
    {
        return 1;
    }

    TextureSamplerBinding::~TextureSamplerBinding()
    {}

}



namespace kege{

    ImageStorageBinding::ImageStorageBinding( const kege::string& name, uint32_t binding, const kege::Ref< core::Texture > &texture )
    :   core::UniformBinding( STORAGE_IMAGE, binding, name )
    ,   _texture{ texture }
    {}

    void ImageStorageBinding::setTexture( uint32_t i, const kege::Ref< core::Texture >& buffer )
    {
        _texture[ i ] = buffer;
    }

    const core::Texture* ImageStorageBinding::getTexture( uint32_t i )const
    {
        return _texture[ i ].ref();
    }

    core::Texture* ImageStorageBinding::getTexture( uint32_t i )
    {
        return _texture[ i ].ref();
    }

    uint32_t ImageStorageBinding::getCount()const
    {
        return 1;
    }

    ImageStorageBinding::~ImageStorageBinding()
    {
        _texture[0].clear();
    }

}
