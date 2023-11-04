//
//  bitvec.cpp
//  FazeOne
//
//  Created by Kenneth A Esdaile on 10/31/19.
//  Copyright © 2019 Kenneth A Esdaile. All rights reserved.
//
#include <string>
#include "bitvec.hpp"
namespace kege{
    
    void bitvec::operator ()( unsigned long index, bool value )
    {
        unsigned long cel = index / 64;
        unsigned long bit = index % 64;
        if( value == true )
            _bits[ cel ] = (_bits[ cel ] | (1 << bit));
        else
            _bits[ cel ] = (_bits[ cel ] & (~(1 << bit)));
    }
    bool bitvec::operator []( unsigned long index )const
    {
        unsigned long cel = index / 64;
        unsigned long bit = index % 64;
        return (_bits[ cel ] & (1 << bit));
    }
    void bitvec::operator =( unsigned long index )
    {
        for( int i=0 ; i<_size ; i++ ) _bits[i] = index;
    }
    
    void bitvec::resize( long size )
    {
        size = size / 64;
        unsigned long* arry;/// = new unsigned long[size];

        if ( size < _size )
        {
            arry = new unsigned long[ size ];
            memcpy(arry, _bits, sizeof(unsigned long)*size);
        }
        else if ( size > _size )
        {
            arry = new unsigned long[ size ];
            memcpy(arry, _bits, sizeof(unsigned long)*_size);
        }
        else
        {
            arry = new unsigned long[ size ];
            memcpy(arry, _bits, sizeof(unsigned long)*size);
        }
        clear();
        
        _bits = arry;
        _size = size;
    }
    
    void bitvec::clear()
    {
        if ( _bits != nullptr)
        {
            delete _bits;
            _bits = nullptr;
            _size = 0;
        }
    }
    
    bitvec::bitvec( unsigned long size )
    :   _bits( nullptr )
    ,   _size( 0 )
    {
        resize( size );
        memset( _bits, 0x0, sizeof(long) * _size );
    }
    
    bitvec::~ bitvec()
    {
        clear();
    }
    
    bitvec::bitvec()
    :   _bits( nullptr )
    ,   _size( 0 )
    {}
    
    
    
    bool operator ==( const bitvec& b, unsigned short index )
    {
        return b[ index ];
    }
    bool operator ==( unsigned short index, const bitvec& b )
    {
        return b[ index ];
    }
    
    bool operator !=( const bitvec& b, unsigned short index )
    {
        return !b[ index ];
    }
    bool operator !=( unsigned short index, const bitvec& b )
    {
        return !b[ index ];
    }
    
    bool operator ==( const bitvec& a, const bitvec& b )
    {
        for( int i=0 ; i<a._size && i<b._size ; i++ )
        {
            if ( a._bits[i] != b._bits[i] )
            {
                return false;
            }
        }
        return true;
    }
    bool operator !=( const bitvec& a, const bitvec& b )
    {
        for( int i=0 ; i<a._size && i<b._size ; i++ )
        {
            if ( a._bits[i] == b._bits[i] )
            {
                return false;
            }
        }
        return true;
    }
}
