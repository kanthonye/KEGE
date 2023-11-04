//
//  bitvec.hpp
//  column-major
//
//  Created by Kenneth A Esdaile on 7/13/18.
//  Copyright © 2018 Kenneth A Esdaile. All rights reserved.
//

#ifndef bitopp_hpp
#define bitopp_hpp
#include "coremath.hpp"
namespace kege{
    
    template < typename var >  class bitmask
    {
    public:
        
        friend inline bool operator ==( bitmask a, bitmask b );
        friend inline bool operator !=( bitmask a, bitmask b );
        
        inline void operator ()( var index, bool value );
        inline bool operator []( var index )const;
        inline void operator =( var index );
        inline var  operator *()const;
        
        bitmask( var n );
        bitmask();
        
    protected:
        
        var _bits;
    };
    
    template < typename var >  void bitmask< var >::operator ()( var index, bool value )
    {
        if( value == true )
            _bits = (_bits | (1 << index));
        else
            _bits = (_bits & (~(1 << index)));
    }
    template < typename var >  inline bool bitmask< var >::operator []( var index )const
    {
        return (_bits & (1 << index));
    }
    template < typename var >  inline void bitmask< var >::operator =( var index )
    {
        _bits = index;
    }
    template < typename var >  inline var bitmask< var >::operator *() const
    {
        return _bits;
    }
    
    template < typename var >  inline bool operator ==( bitmask< var > b, unsigned short index )
    {
        return b[ index ];
    }
    template < typename var >  inline bool operator ==( unsigned short index, bitmask< var > b )
    {
        return b[ index ];
    }
    
    template < typename var >  inline bool operator !=( bitmask< var > b, unsigned short index )
    {
        return !b[ index ];
    }
    template < typename var >  inline bool operator !=( unsigned short index, bitmask< var > b )
    {
        return !b[ index ];
    }
    
    template < typename var >  inline bool operator ==( bitmask< var > a, bitmask< var > b )
    {
        return b._bits == b._bits;
    }
    template < typename var >  inline bool operator !=( bitmask< var > a, bitmask< var > b )
    {
        return b._bits != b._bits;
    }
    
    template < typename var >  bitmask< var >::bitmask( var n )
    :   _bits(n)
    {}
    
    template < typename var >  bitmask< var >::bitmask()
    :   _bits(0)
    {}
    
    typedef bitmask< unsigned char  > bitmask8;
    typedef bitmask< unsigned short > bitmask16;
    typedef bitmask< unsigned int   > bitmask32;
    typedef bitmask< unsigned long  > bitmask64;
}
#endif /* bitvec_hpp */
