//
//  ResMgr.hpp
//  kege-rendering-engine
//
//  Created by Kenneth A Esdaile on 8/9/22.
//

#ifndef ResMgr_hpp
#define ResMgr_hpp
#include <vector>
#include <unordered_map>
#include "TypeId.hpp"
#include "kege-string.hpp"
#include "VirDir.hpp"
#include "Ref.hpp"
#include "Singleton.hpp"
#include "Ref.hpp"
#include "kege-logger.hpp"
namespace kege
{
    struct Asset : public kege::RefCounter
    {
        Asset( const kege::string& p ): _path( p ) {}
        kege::string _path;
    };
}



namespace kege
{
    class ResMgr;

    class Resource : public kege::RefCounter
    {
    public:

        virtual void remove( const kege::string& name ) = 0;
        virtual void purge() = 0;
        const kege::string& name(){ return _name; }
        virtual ~Resource(){}

    protected:

        kege::string _name;
    };
}




namespace kege
{
    template< typename T> class Res : public kege::Ref< T >
    {
    public:

        const kege::string& name(){ return _name; }
        virtual ~Res(){}

    protected:

        void clear()override;
        kege::Resource* _resource;
        kege::string _name;
    };
}





namespace kege
{
    template<typename T> class ResourceT : public kege::Resource
    {
    public:

        struct Item
        {
            T _item;
            kege::string _name;
        };

        typedef std::vector< Item > Items;
        typedef std::unordered_map< uint64_t, Items* > ItemsContainer;

        T* add( const kege::string& name, uint64_t key, T& item );
        T* get( const kege::string& name, uint64_t key );

        T* add( const kege::string& name, T& item );
        T* get( const kege::string& name );

        void remove( const kege::string& name );
        void purge();
        
        ~ResourceT();
        ResourceT();

    protected:

        ItemsContainer _container;
        friend Res< T >;
        friend ResMgr;
    };

    template<typename T> T* ResourceT< T >::add( const kege::string& name, uint64_t key, T& item )
    {
        auto m = _container.find( key );
        if ( m == _container.end() )
        {
            Items* items = new Items;
            items->push_back({ item, name });
            _container[ key ] = items;
            return &item;
        }
        for(auto& i : *m->second )
        {
            if ( i._name == name )
            {
                i._item = item;
                break;
            }
        }
        return &item;
    }
    
    template<typename T> T * ResourceT< T >::get( const kege::string& name, uint64_t key )
    {
        auto m = _container.find( key );
        if ( m != _container.end() )
        {
            for(auto& item :  *m->second )
            {
                if ( item._name == name )
                {
                    return item._item;
                }
            }
        }
        return nullptr;
    }

    template<typename T> T* ResourceT< T >::add( const kege::string& name, T& item )
    {
        uint64_t key = name.hash();
        return add( name, key, item );
    }
    
    template<typename T> void ResourceT< T >::remove( const kege::string& name )
    {
        auto m = _container.find( name.hash() );
        if ( m != _container.end() ) 
        {
            for(typename Items::iterator i = m->second->begin(); i != m->second->end(); i++ )
            {
                if ( (*i)._name == name )
                {
                    m->second->erase( i );
                    break;
                }
            }

            if ( m->second->empty() )
            {
                delete m->second;
                m->second = nullptr;
                _container.erase( m );
            }
        }
    }
    
    template<typename T> T* ResourceT< T >::get( const kege::string& name )
    {
        uint64_t key = name.hash();
        return get( name, key );
    }

    template<typename T> void ResourceT< T >::purge()
    {
        for( auto& m : _container )
        {
            if ( m.second != nullptr ) 
            {
                m.second->clear();
                delete m.second;
                m.second = nullptr;
            }
        };
        _container.clear();
    }

    template<typename T> ResourceT< T >::~ResourceT()
    {
        this->purge();
    }
    
    template<typename T> ResourceT< T >::ResourceT()
    {
    }
}



namespace kege
{
    template< typename T> void Res< T >::clear()
    {
        if( this->_ref != nullptr )
        {
            this->_ref->RefCounter::decrementRefRefCounter();
            if( this->_ref->RefCounter::referenceRefCounter() <= 0 )
            {
                _resource->remove( _name );
                delete this->_ref;
            }
            this->_ref = nullptr;
        }
    }

}



namespace kege
{
    class Loader : public kege::RefCounter
    {
    public:

        const kege::string& name(){ return _name; }
        virtual ~Loader(){}

    protected:

        kege::string _name;
    };

}
namespace kege
{
    template< typename T > class LoaderT : public kege::Loader
    {
    public:
        
        virtual T* load(ResMgr& mgr, const kege::string& name ) = 0;
        static const kege::string& getName(){ return _name; };
        virtual ~LoaderT(){}
        
    protected:
        
        LoaderT( const kege::string& n )
        {
            _name = n;
        }
        static kege::string _name;
    };
    
    template< typename T > kege::string LoaderT< T >::_name;
}



namespace kege
{
    class ResMgr : public kege::RefCounter
    {
    public:
        
        //template<typename T> void create( const kege::string& name, const kege::string& path );
        template<typename T> T* add( const kege::string& name, T* item );
        template<typename T> void remove( const kege::string& name );
        template<typename T> T* load( const kege::string& name );
        template<typename T> T* get( const kege::string& name );
        
        template<typename L> void addLoader();
        
        void purge();
        
        ~ResMgr();
        ResMgr();
        
    protected:
        
        template<typename T> ResourceT< T >* resource();
        template<typename T> LoaderT< T >* getLoader();
        
        ResourceT< kege::Resource* > _resources;
        ResourceT< kege::Loader* > _loaders;
        //Resource< Asset > _assets;
        
    private:
        
        ResMgr& operator =( const ResMgr& ) = delete;
        ResMgr( const ResMgr& ) = delete;
    };
    
//    template<typename T> void ResMgr::create( const kege::string& name, const kege::string& path )
//    {
//        _assets.add( name, new Asset( path ) );
//    }
    template<typename T> T* ResMgr::add( const kege::string& name, T* item )
    {
        return resource< T >()->add( name, item );
    }

    template<typename T> void ResMgr::remove( const kege::string& name )
    {
        resource< T >()->remove( name );
    }

    template<typename T> T* ResMgr::load( const kege::string& fname )
    {
        LoaderT< T >* loader = getLoader< T >();
        if ( loader == nullptr )
        {
            KEGE_FATAL(" ResMgr::load< T >( ... ) - loader for file %s does not exist.", fname.c_str() );
            return nullptr;
        }
        
        const kege::string name = fname.parse_fname();
        T* t = resource< T >()->get( name );
        
        if ( t == nullptr ) 
        {
            try 
            {
                kege::string filename = kege::Singleton::get< kege::VirDir >()->fetch( fname );
                t = loader->load( *this, filename );
                resource< T >()->add( fname.parse_fname(), t );
            }
            catch ( const std::runtime_error& exception )
            {
                KEGE_ERROR(" ResMgr::load< T >() - load file %s failed.", fname.c_str() );
                throw exception;
            }
        }
        return t;
    }

    template<typename T> T* ResMgr::get( const kege::string& name )
    {
        return resource< T >()->get( name );
    }

    template<typename T> ResourceT< T >* ResMgr::resource()
    {
        uint64_t id = TypeId< ResMgr, ResourceT< T > >::getEnumTypeId();

        auto m = _resources._container.find( id );
        if( m != _resources._container.end() )
        {
            return m->second->front();
        }

        ResourceT< T >* resource = new ResourceT< T >();
        _resources.add( "",  id, resource );
        return resource;
    }
    
    template<typename T> LoaderT< T >* ResMgr::getLoader()
    {
        uint64_t id = TypeId< ResMgr, LoaderT< T > >::getEnumTypeId();

        auto m = _loaders._container.find( id );
        if( m != _loaders._container.end() )
        {
            return m->second->front();
        }

        return nullptr;
    }
    
    template<typename T> void ResMgr::addLoader()
    {
        uint64_t id = TypeId< ResMgr, LoaderT< T > >::getEnumTypeId();

        auto m = _loaders._container.find( id );
        if( m != _loaders._container.end() )
        {
            return;
        }

        _resources.add( "",  id, new T );
    }
}
#define KEGE_ResMgr ( kege::Singleton::get< ResMgr >() )
#endif /* ResMgr_hpp */
