//
//  ECO.cpp
//  KEGE
//
//  Created by Kenneth Esdaile on 3/23/23.
//

#include "ECO.hpp"
namespace kege{

    bool ECO::has( uint32_t type )const
    {
        return _bitmask.has( type );
    }

    void ECO::erase( uint32_t type )
    {
        _bitmask.erase( type );
    }

    void ECO::set( uint32_t type )
    {
        _bitmask.set( type );
    }

    void ECO::clear()
    {
        _bitmask.clear();
    }
}

namespace kege{

    bool Bitmask::has( uint32_t type )const
    {
        //std::cout<<"bitmask: "<<std::bitset<16>(_bitmask)<<"\n";
        //std::cout<<"   type: "<<std::bitset<16>((1UL << type))<<"\n";
        //std::cout<<" result: "<<std::bitset<16>((1UL << type) & _bitmask)<<"\n";
        return ((1UL << type) & _bitmask) != 0;
    }

    void Bitmask::erase( uint32_t type )
    {
        _bitmask &= ~(1UL << type);
    }

    void Bitmask::set( uint32_t type )
    {
        uint64_t mask = (1L << type);
        _bitmask = _bitmask | mask;
    }

    void Bitmask::clear()
    {
        _bitmask = 0;
    }

    Bitmask::Bitmask()
    : _bitmask(0)
    {}

}
