//
//  KEGE
//  Hashmap.hpp
//  Created by Kenneth Esdaile on 3/12/23.
//

#ifndef KEGE_HASHMAP_HPP
#define KEGE_HASHMAP_HPP
#include <stdlib.h>
#include <map>
#include <vector>
#include "kege-string.hpp"
namespace kege{

    template<typename T> class Hashmap
    {
    public:

        typedef std::map< uint64_t, std::vector< std::pair< kege::string, T > > > Content;

        void insert( const kege::string& name, const T& t )
        {
            _content[ name.hash() ].push_back({ name, t });
        }

        void remove( const kege::string& name )
        {
            uint64_t k = name.hash();
            auto m = _content.find( k );
            if( m != _content.end() )
            {
                for( auto i = m->second.begin(); i != m->second.end(); i++ )
                {
                    if( (*i).first == name )
                    {
                        m->second.erase( i );
                        break;
                    }
                }
            }
        }

        const T& get( const kege::string& name )const
        {
            uint64_t k = name.hash();
            auto m = _content.find( k );
            if( m != _content.end() )
            {
                for( auto i = m->second.begin(); i != m->second.end(); i++ )
                {
                    if( (*i).first == name )
                    {
                        return (*i).second;
                    }
                }
            }
            throw std::runtime_error( ("map index " + name + " does not exist").c_str() );
        }

        T& get( const kege::string& name )
        {
            uint64_t k = name.hash();
            auto m = _content.find( k );
            if( m != _content.end() )
            {
                for( auto i = m->second.begin(); i != m->second.end(); i++ )
                {
                    if( (*i).first == name )
                    {
                        return (*i).second;
                    }
                }
            }
            throw std::runtime_error( ("map index " + name + " does not exist").c_str() );
        }

        bool has( const kege::string& name )const
        {
            return _content.find( name.hash() ) != _content.end();
        }

        typename Hashmap< T >::Content::iterator begin()
        {
            return _content.begin();
        }

        typename Hashmap< T >::Content::iterator end()
        {
            return _content.end();
        }

        void clear()
        {
            _content.clear();
        }

    private:

        Hashmap< T >::Content _content;
    };
}
#endif /* KEGE_HASHMAP_HPP */
