//
//  BitOppFunc.cpp
//  CodeSampler
//
//  Created by Kenneth A Esdaile on 5/26/21.
//

#include "BitOppFunc.hpp"
namespace kege
{
    bool BitsAreSubset( unsigned long a, unsigned long b )
    {
        unsigned long z = a & b;
        return ( !((a < b) ? a == z : b == z) );
    }
    
    void SetBit( unsigned long& bits, unsigned int index )
    {
        unsigned int bit = index % 64;
        bits = (bits | (1 << bit));
    }
    
    void SetBit( unsigned int& bits, unsigned int index )
    {
        unsigned int bit = index % 32;
        bits = (bits | (1 << bit));
    }
    
    void SetBit( unsigned short& bits, unsigned int index )
    {
        unsigned int bit = index % 16;
        bits = (bits | (1 << bit));
    }
    
    void SetBit( unsigned char& bits, unsigned int index )
    {
        unsigned int bit = index % 8;
        bits = (bits | (1 << bit));
    }
    
    void SetBit( long& bits, unsigned int index )
    {
        unsigned int bit = index % 64;
        bits = (bits | (1 << bit));
    }
    
    void SetBit( int& bits, unsigned int index )
    {
        unsigned int bit = index % 32;
        bits = (bits | (1 << bit));
    }
    
    void SetBit( short& bits, unsigned int index )
    {
        unsigned int bit = index % 16;
        bits = (bits | (1 << bit));
    }
    
    void SetBit( char& bits, unsigned int index )
    {
        unsigned int bit = index % 8;
        bits = (bits | (1 << bit));
    }
    
    
    void ClearBit( unsigned long& bits, unsigned int index )
    {
        unsigned int bit = index % 64;
        bits = (bits & (~(1 << bit)));
    }
    
    void ClearBit( unsigned int& bits, unsigned int index )
    {
        unsigned int bit = index % 32;
        bits = (bits & (~(1 << bit)));
    }
    
    void ClearBit( unsigned short& bits, unsigned int index )
    {
        unsigned int bit = index % 16;
        bits = (bits & (~(1 << bit)));
    }
    
    void ClearBit( unsigned char& bits, unsigned int index )
    {
        unsigned int bit = index % 8;
        bits = (bits & (~(1 << bit)));
    }
    
    void ClearBit( long& bits, unsigned int index )
    {
        unsigned int bit = index % 64;
        bits = (bits & (~(1 << bit)));
    }
    
    void ClearBit( int& bits, unsigned int index )
    {
        unsigned int bit = index % 32;
        bits = (bits & (~(1 << bit)));
    }
    
    void ClearBit( short& bits, unsigned int index )
    {
        unsigned int bit = index % 16;
        bits = (bits & (~(1 << bit)));
    }
    
    void ClearBit( char& bits, unsigned int index )
    {
        unsigned int bit = index % 8;
        bits = (bits & (~(1 << bit)));
    }
    
    
    
    void ToggleBit( unsigned long& bits, unsigned int index )
    {
        unsigned int bit = index % 64;
        bits ^= 1 << bit;
    }
    void ToggleBit( unsigned int& bits, unsigned int index )
    {
        unsigned int bit = index % 32;
        bits ^= 1 << bit;
    }
    void ToggleBit( unsigned short& bits, unsigned int index )
    {
        unsigned int bit = index % 16;
        bits ^= 1 << bit;
    }
    void ToggleBit( unsigned char& bits, unsigned int index )
    {
        unsigned int bit = index % 8;
        bits ^= 1 << bit;
    }
    void ToggleBit( long& bits, unsigned int index )
    {
        unsigned int bit = index % 64;
        bits ^= 1 << bit;
    }
    void ToggleBit( int& bits, unsigned int index )
    {
        unsigned int bit = index % 32;
        bits ^= 1 << bit;
    }
    void ToggleBit( short& bits, unsigned int index )
    {
        unsigned int bit = index % 16;
        bits ^= 1 << bit;
    }
    void ToggleBit( char& bits, unsigned int index )
    {
        unsigned int bit = index % 8;
        bits ^= 1 << bit;
    }
    
    
    
    bool CheckBit( const unsigned long& bits, unsigned int index )
    {
        //unsigned int bit = index % 64;
        //return (bit >> index) & 1UL;
        return (bits & (1 << (index % 64)));
    }
    bool CheckBit( const unsigned int& bits, unsigned int index )
    {
        //unsigned int bit = index % 32;
        //return (bit >> index) & 1UL;
        return (bits & (1 << (index % 32)));
    }
    bool CheckBit( const unsigned short& bits, unsigned int index )
    {
//        unsigned int bit = index % 16;
//        return (bit >> index) & 1UL;
        return (bits & (1 << (index % 16)));
    }
    bool CheckBit( const unsigned char& bits, unsigned int index )
    {
//        unsigned int bit = index % 8;
//        return (bit >> index) & 1UL;
        return (bits & (1 << (index % 8)));
    }
    bool CheckBit( const long& bits, unsigned int index )
    {
//        unsigned int bit = index % 64;
//        return (bit >> index) & 1UL;
        return (bits & (1 << (index % 64)));
    }
    bool CheckBit( const int& bits, unsigned int index )
    {
//        unsigned int bit = index % 32;
//        return (bit >> index) & 1UL;
        return (bits & (1 << (index % 32)));
    }
    bool CheckBit( const short& bits, unsigned int index )
    {
//        unsigned int bit = index % 16;
//        return (bit >> index) & 1UL;
        return (bits & (1 << (index % 16)));
    }
    bool CheckBit( const char& bits, unsigned int index )
    {
//        unsigned int bit = index % 8;
//        return (bit >> index) & 1UL;
        return (bits & (1 << (index % 8)));
    }
    
    
//    /**
//     check if bit n is set
//     */
//    inline bool CheckBit( integer bits, unsigned int n )
//    {
//        return (bits & (1 << n));
//    }
    
}
