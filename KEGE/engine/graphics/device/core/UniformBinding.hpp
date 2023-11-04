//
//  UniformBinding.hpp
//  KEGE
//
//  Created by Kenneth Esdaile on 10/20/23.
//

#ifndef KEGE_CORE_UNIFORM_BINDING
#define KEGE_CORE_UNIFORM_BINDING

#include "kege-string.hpp"
#include "Texture.hpp"
#include "Sampler.hpp"
#include "Buffer.hpp"

namespace kege{namespace core{

    class UniformBinding : public kege::RefCounter
    {
    public:

        enum BufferType
        {
            BUFFER,
            DYNAMIC_BUFFER,
            MAX_BUFFER_COUNT
        };

        enum ImageType
        {
            COMBINED_IMAGE_SAMPLER = MAX_BUFFER_COUNT,
            STORAGE_IMAGE,
            DYNAMIC_IMAGE
        };

        virtual const core::Texture* getTexture( uint32_t i )const{ return nullptr; }
        virtual core::Texture* getTexture( uint32_t i ){ return nullptr; }

        virtual const core::Buffer* getBuffer( uint32_t i )const{ return nullptr; }
        virtual core::Buffer* getBuffer( uint32_t i ){ return nullptr; }

        virtual const core::Sampler* getSampler()const{ return nullptr; }
        virtual core::Sampler* getSampler(){ return nullptr; }

        virtual uint32_t getCount()const = 0;

        uint32_t getBindingIndex()const{ return _binding; }
        uint32_t getLayoutType()const{ return _layout_type; }

        virtual ~UniformBinding(){}

        const kege::string& name()const{ return _name; }

    protected:

        UniformBinding( uint32_t layout_type, uint32_t binding, const kege::string& name )
        :   _layout_type( layout_type )
        ,   _binding( binding )
        ,   _name( name )
        {}

    protected:

        uint32_t _layout_type;
        uint32_t _binding;
        kege::string _name;
    };

}}









namespace kege{

    class BufferBinding : public core::UniformBinding
    {
    public:

        BufferBinding( const kege::string& name, uint32_t binding, const kege::Ref< core::Buffer >& buffer );

        void setBuffer( uint32_t i, const kege::Ref< core::Buffer >& buffer );
        const core::Buffer* getBuffer( uint32_t i )const;
        core::Buffer* getBuffer( uint32_t i );
        uint32_t getCount()const;

        ~BufferBinding();

    private:

        kege::Ref< core::Buffer > _buffer[1];
    };

}

namespace kege{

    template< uint32_t SIZE > class BufferArrayBinding : public core::UniformBinding
    {
    public:

        BufferArrayBinding( const kege::string& name, uint32_t binding, const std::array< kege::Ref< core::Buffer >, SIZE > &buffers )
        :   core::UniformBinding( BUFFER, binding, name )
        ,   _buffers{}
        {
            for(int i=0; i<SIZE; i++) _buffers[i] = buffers[i];
        }

        BufferArrayBinding( const kege::string& name, uint32_t binding, const kege::Ref< core::Buffer > (&buffers)[ SIZE ] )
        :   core::UniformBinding( BUFFER, binding, name )
        ,   _buffers{ buffers }
        {}

        void setBuffer( uint32_t i, const kege::Ref< core::Buffer >& buffer )
        {
            _buffers[i] = buffer;
        }

        const core::Buffer* getBuffer( uint32_t i )const
        {
            return _buffers[i].ref();
        }

        core::Buffer* getBuffer( uint32_t i )
        {
            return _buffers[i].ref();
        }

        virtual uint32_t getCount()const
        {
            return SIZE;
        }

        ~BufferArrayBinding()
        {
            for(int i=0; i<SIZE; i++) _buffers[i].clear();
        }

    private:

        kege::Ref< core::Buffer > _buffers[ SIZE ];
    };

}




namespace kege{

    class TextureSamplerBinding : public core::UniformBinding
    {
    public:

        TextureSamplerBinding( const kege::string& name, uint32_t binding, const kege::Ref< core::Sampler >& sampler, const kege::Ref< core::Texture > &texture );

        void setTexture( uint32_t i, const kege::Ref< core::Texture >& buffer );
        const core::Texture* getTexture( uint32_t i )const;
        core::Texture* getTexture( uint32_t i );

        const core::Sampler* getSampler()const;
        core::Sampler* getSampler();

        uint32_t getCount()const;
        
        ~TextureSamplerBinding();

    private:

        kege::Ref< core::Texture > _texture[ 1 ];
        kege::Ref< core::Sampler > _sampler;
    };

}

namespace kege{

    template< uint32_t SIZE > class TextureSamplerArrayBinding : public core::UniformBinding
    {
    public:

        TextureSamplerArrayBinding( const kege::string& name, uint32_t binding, const kege::Ref< core::Sampler > &sampler, const std::array< kege::Ref< core::Texture >, SIZE > &textures )
        :   core::UniformBinding( COMBINED_IMAGE_SAMPLER, binding, name )
        ,   _sampler( sampler )
        {
            for(int i=0; i<SIZE; i++) _textures[i] = textures[i];
        }

        TextureSamplerArrayBinding( const kege::string& name, uint32_t binding, const kege::Ref< core::Sampler > &sampler, const std::vector< kege::Ref< core::Texture > > &textures )
        :   core::UniformBinding( COMBINED_IMAGE_SAMPLER, binding, name )
        ,   _sampler( sampler )
        {
            for(int i=0; i<SIZE; i++) _textures[i] = textures[i];
        }

        TextureSamplerArrayBinding( const kege::string& name, uint32_t binding, const kege::Ref< core::Sampler > &sampler, const kege::Ref< core::Texture > (&textures)[ SIZE ] )
        :   core::UniformBinding( COMBINED_IMAGE_SAMPLER, binding, name )
        ,   _textures{ textures }
        ,   _sampler( sampler )
        {}

        void setTexture( uint32_t i, const kege::Ref< core::Texture >& buffer )
        {
            _textures[ i ] = buffer;
        }

        const core::Texture* getTexture( uint32_t i )const
        {
            return _textures[ i ].ref();
        }

        core::Texture* getTexture( uint32_t i )
        {
            return _textures[ i ].ref();
        }

        const core::Sampler* getSampler()const
        {
            return _sampler.ref();
        }

        core::Sampler* getSampler()
        {
            return _sampler.ref();
        }

        uint32_t getCount()const
        {
            return SIZE;
        }

        ~TextureSamplerArrayBinding()
        {
            for(int i=0; i<SIZE; i++) _textures[i].clear();
            _sampler.clear();
        }

    private:

        kege::Ref< core::Texture > _textures[ SIZE ];
        kege::Ref< core::Sampler > _sampler;
    };

}



namespace kege{

    class ImageStorageBinding : public core::UniformBinding
    {
    public:

        ImageStorageBinding( const kege::string& name, uint32_t binding, const kege::Ref< core::Texture > &texture );

        void setTexture( uint32_t i, const kege::Ref< core::Texture >& buffer );
        const core::Texture* getTexture( uint32_t i )const;
        core::Texture* getTexture( uint32_t i );
        uint32_t getCount()const;

        ~ImageStorageBinding();

    private:

        kege::Ref< core::Texture > _texture[ 1 ];
    };

}

namespace kege{

    template< uint32_t SIZE > class ImageStorageArrayBinding : public core::UniformBinding
    {
    public:

        ImageStorageArrayBinding( const kege::string& name, uint32_t binding, const kege::Ref< core::Texture > (&textures)[ SIZE ] )
        :   core::UniformBinding( STORAGE_IMAGE, binding, name )
        ,   _textures{ textures }
        {}

        ImageStorageArrayBinding( const kege::string& name, uint32_t binding, const std::array< kege::Ref< core::Texture >, SIZE > &textures )
        :   core::UniformBinding( STORAGE_IMAGE, binding, name )
        ,   _textures{}
        {
            for(int i=0; i<SIZE; i++) _textures[i] = textures[i];
        }

        void setTexture( uint32_t i, const kege::Ref< core::Texture >& buffer )
        {
            _textures[ i ] = buffer;
        }

        const core::Texture* getTexture( uint32_t i )const
        {
            return _textures[ i ].ref();
        }

        core::Texture* getTexture( uint32_t i )
        {
            return _textures[ i ].ref();
        }

        uint32_t getCount()const
        {
            return SIZE;
        }

        ~ImageStorageArrayBinding()
        {
            for(int i=0; i<SIZE; i++) _textures[ i ].clear();
        }

    private:

        kege::Ref< core::Texture > _textures[ SIZE ];
    };

}

#endif /* KEGE_CORE_UNIFORM_BINDING */
