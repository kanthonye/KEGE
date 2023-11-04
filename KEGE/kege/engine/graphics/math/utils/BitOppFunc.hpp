//
//  BitOppFunc.hpp
//  CodeSampler
//
//  Created by Kenneth A Esdaile on 5/26/21.
//

#ifndef BitOppFunc_hpp
#define BitOppFunc_hpp
#include <stdio.h>
namespace kege
{
    /**
     take a numbers and negate each bit of the numbers. 
     The result change 1 to 0, and 0 to 1
     @return the resulting complement of the input
     */
    inline unsigned long bitwise_complement(unsigned long bits)
    {
        return ~bits;
    }
    
    /**
     take two numbers and do XOR on each bit of the two numbers. 
     The result is 1 if the two bits are different and 0 otherwise
     @return the resulting XOR of the two input
     */
    inline unsigned long bitwise_xor(unsigned long a, unsigned long b)
    {
        return a ^ b;
    }
    
    /**
     takes two numbers and do OR on each bit of the two numbers.
     The result is 1 if any of the two bits is 1.
     @return the resulting OR of the two input     
     */
    inline unsigned long bitwise_or(unsigned long a, unsigned long b)
    {
        return a | b;
    }
    
    /**
     takes two numbers and do AND on each bit of the two numbers.
     The result is 1 if both of the two bits is 1. 
     @return the resulting AND of the two input     
     */
    inline unsigned long bitwise_and(unsigned long a, unsigned long b)
    {
        return a & b;
    }
    
    /**
     check if all bits of b is set in the target bits
     a = 0000 1101
     b = 0011 1101
     c = 0000 1101 = a & b
     */
    inline bool IsSubset( unsigned long bits, unsigned long b )
    {
        return (bits & b) == b;
    }
    
    /**
     check if all bits of b is not set in the target bits
     */
    inline bool CheckBitsNone( unsigned long bits, unsigned long b )
    {
        return (bits & b) == 0;
    }
    
    /**
     either b is subset of this object or this object is subset of b
     */
    bool BitsAreSubset( unsigned long a, unsigned long b );
    
    /**
     Set bit value to true or false at index
     */
    void SetBit( unsigned long& bits, unsigned int index );
    void SetBit( unsigned int& bits, unsigned int index );
    void SetBit( unsigned short& bits, unsigned int index );
    void SetBit( unsigned char& bits, unsigned int index );
    void SetBit( long& bits, unsigned int index );
    void SetBit( int& bits, unsigned int index );
    void SetBit( short& bits, unsigned int index );
    void SetBit( char& bits, unsigned int index );
    
    void ClearBit( unsigned long& bits, unsigned int index );
    void ClearBit( unsigned int& bits, unsigned int index );
    void ClearBit( unsigned short& bits, unsigned int index );
    void ClearBit( unsigned char& bits, unsigned int index );
    void ClearBit( long& bits, unsigned int index );
    void ClearBit( int& bits, unsigned int index );
    void ClearBit( short& bits, unsigned int index );
    void ClearBit( char& bits, unsigned int index );
    
    void ToggleBit( unsigned long& bits, unsigned int index );
    void ToggleBit( unsigned int& bits, unsigned int index );
    void ToggleBit( unsigned short& bits, unsigned int index );
    void ToggleBit( unsigned char& bits, unsigned int index );
    void ToggleBit( long& bits, unsigned int index );
    void ToggleBit( int& bits, unsigned int index );
    void ToggleBit( short& bits, unsigned int index );
    void ToggleBit( char& bits, unsigned int index );
    
    bool CheckBit( const unsigned long& bits, unsigned int index );
    bool CheckBit( const unsigned int& bits, unsigned int index );
    bool CheckBit( const unsigned short& bits, unsigned int index );
    bool CheckBit( const unsigned char& bits, unsigned int index );
    bool CheckBit( const long& bits, unsigned int index );
    bool CheckBit( const int& bits, unsigned int index );
    bool CheckBit( const short& bits, unsigned int index );
    bool CheckBit( const char& bits, unsigned int index );
}
#endif /* BitOppFunc_hpp */
