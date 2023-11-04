//
//  VirDir.cpp
//  KEGE
//
//  Created by Kenneth Esdaile on 5/27/16.
//  Copyright © 2016 Kenneth Anthony Esdaile. All rights reserved.
//
#include <unistd.h>
#include "kege-logger.hpp"
#include "VirDir.hpp"
namespace kege{

    void VirDir::dir( const kege::string& name, const kege::string& path )
    {
        uint64_t key = name.hash();
        auto& list = _virmap[ key ];
        for( auto& i : list )
        {
            if ( name == i.first )
            {
                return;
            }
        }
        int32_t directory_index = (int32_t) _directories.size();
        list.push_back({ name, directory_index });
        _directories.push_back( path );
    }

    kege::FileReader VirDir::open( const kege::string& fname )
    {
        return kege::FileReader( this->fetch( fname ), "r" );
    }
    
    kege::string VirDir::fetch( const kege::string& name )
    {
        if( name[0] == '~' ) // format -> "~/assets/textures/rock.png"
        {
            kege::string str,token;
            str = name.skipfwd("~/");
            token = str.read("/");
            return fetch( str );
        }
        else if( name[0] == '/' ) // format -> "/users/kae/projects/engine3D/assets/textures/rock.png"
        {
            return name;
        }
        else if( name[0] == '.' ) // format -> ".ext"
        {
            uint64_t key = name.hash();
            auto m = _virmap.find( key );
            if( m != _virmap.end() )
            {
                for( auto& i : m->second )
                {
                    if ( name == i.first )
                    {
                        return fetch( _directories[ i.second ] ) + "/";
                    }
                }
            }
            return "";
        }
        else  // format -> "filename.ext" or "assets/textures/rock.png"
        {
            kege::string str,token;
            token = name.read("./");
            str = name.seekfwd("./");

            // then we have a file name
            if( str[0] == '.' )
            {
                return fetch( str ) + name;
            }
            else if( str[0] == '/' ) // then we have a file path
            {
                auto m = _virmap.find( token.hash() );
                if( m != _virmap.end() )
                {
                    for( auto& i : m->second )
                    {
                        if ( token == i.first )
                        {
                            return fetch( _directories[ i.second ] ) + str;
                        }
                    }
                }
                KEGE_ERROR( "virtual directory don't exist for input %s", name.c_str() );
                return "";
            }
        }
        return "";
    }

    bool VirDir::save( const kege::string& filename )
    {
        FILE* file = fopen(filename.c_str(), "w+");
        if(file == nullptr) return false;
        for(auto& m : _virmap )
        {
            for(auto& n : m.second )
            {
                fprintf(file, "vir\t%i : %s\n", n.second, n.first.c_str());
            }
        }
        for(auto& m : _directories )
        {
            fprintf(file, "dir\t%s\n", m.c_str() );
        }
        return true;
    }
    bool VirDir::load( const kege::string& filename )
    {
        kege::string file = readfile( filename );
        kege::string s( file.str(), file.length(), false );
        while ( s )
        {
            if( s.cmp( "vir" ) )
            {
                s.seekfwd(" ");
                s.skipfwd(" ");
                int32_t index = s.to_int32();
                s.seekfwd(" ");
                s.skipfwd(" ");
                kege::string token = s.read("\n");
                _virmap[ token.hash() ].push_back({ token, index });
            }
            else if( s.cmp( "dir" ) )
            {
                s.seekfwd(" ");
                s.skipfwd(" ");
                kege::string token = s.read("\n");
                _directories.push_back( token );
            }
            s.seekfwd("\n");
            s.skipfwd("/n");
        }

        return true;
    }

    VirDir::VirDir()
    {}


    kege::string readfile( const kege::string& filename )
    {
        FILE* file = fopen(filename.c_str(), "r");
        if ( !file )
        {
            return "";
        }

        fseek(file, 0, SEEK_END);
        uint64_t filesize = ftell(file);
        rewind(file);

        if (filesize == 0) return "";
        char* buffer = new char[ filesize + 1 ];

        fread( buffer, filesize, sizeof(char), file );
        fclose(file);

        buffer[ filesize ] = '\0';
        return kege::string( buffer, filesize, true );
    }
}

