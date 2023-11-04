//
//  UniformLayout.hpp
//  KEGE
//
//  Created by Kenneth Esdaile on 10/20/23.
//

#ifndef KEGE_CORE_UNIFORM_LAYOUT
#define KEGE_CORE_UNIFORM_LAYOUT

#include "UniformHandle.hpp"
#include "UniformBinding.hpp"

namespace kege{namespace core{

    class UniformLayout : public kege::RefCounter
    {
    public:

        void setUniformHandle( const kege::Ref< core::UniformHandle >& handle )const;
        const core::UniformHandle* getUniformHandle()const;
        core::UniformHandle* getUniformHandle();

        virtual void setBinding( uint32_t i, const kege::Ref< core::UniformBinding > &binding ) = 0;
        virtual const core::UniformBinding* getBinding( uint32_t i )const = 0;
        virtual core::UniformBinding* getBinding( uint32_t i ) = 0;
        virtual uint32_t getCount()const = 0;

        void update();

        virtual ~UniformLayout();

    protected:

        UniformLayout& operator =( const UniformLayout& ) = default;
        UniformLayout( const UniformLayout& ) = default;
        UniformLayout() = default;

    protected:

        mutable kege::Ref< core::UniformHandle > _handle;
    };

}}





namespace kege{

    template< uint32_t SIZE > class UniformLayout : public core::UniformLayout
    {
    public:


        UniformLayout( const std::array< kege::Ref< core::UniformBinding >, SIZE > &bindings )
        :   _bindings{}
        {
            for(int i=0; i<SIZE; i++) _bindings[i] = bindings[i];
        }

        UniformLayout( const kege::Ref< core::UniformBinding > (&bindings)[ SIZE ] )
        {
            for(int i=0; i<SIZE; i++) _bindings[i] = bindings[i];
        }

        UniformLayout( const kege::Ref< core::UniformBinding > &binding )
        :   _bindings{ binding }
        {}

        UniformLayout()
        {}

        void setBinding( uint32_t i, const kege::Ref< core::UniformBinding > &binding )
        {
            assert( i < SIZE && "UniformLayout::setBinding() : out of bound array index." );
            _bindings[i] = binding;
        }

        const core::UniformBinding* getBinding( uint32_t i )const
        {
            return _bindings[ i ].ref();
        }

        core::UniformBinding* getBinding( uint32_t i )
        {
            return _bindings[ i ].ref();
        }

        uint32_t getCount()const
        {
            return SIZE;
        }

        virtual ~UniformLayout()
        {
            for(int i=0; i<SIZE; i++) _bindings[ i ].clear();
        }

    private:

        kege::Ref< core::UniformBinding > _bindings[ SIZE ];
    };


    typedef UniformLayout< 1 > UniformLayout1;
    typedef UniformLayout< 2 > UniformLayout2;
    typedef UniformLayout< 3 > UniformLayout3;
    typedef UniformLayout< 4 > UniformLayout4;
    typedef UniformLayout< 5 > UniformLayout5;
    typedef UniformLayout< 6 > UniformLayout6;
}

#endif /* UniformLayout_hpp */
