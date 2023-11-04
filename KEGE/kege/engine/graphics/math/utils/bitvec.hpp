//
//  bitvec.hpp
//  FazeOne
//
//  Created by Kenneth A Esdaile on 10/31/19.
//  Copyright © 2019 Kenneth A Esdaile. All rights reserved.
//

#ifndef bitvec_hpp
#define bitvec_hpp
namespace kege{
    
    class bitvec
    {
    public:
        
        friend bool operator ==( const bitvec& a, const bitvec& b );
        friend bool operator !=( const bitvec& a, const bitvec& b );
        
        void operator ()( unsigned long index, bool value );
        bool operator []( unsigned long index )const;
        void operator  =( unsigned long index );
        
        void resize( long size );
        void clear();
        
        bitvec( unsigned long n );
        ~ bitvec();
        bitvec();
        
    private:
        
        unsigned long* _bits;
        unsigned long _size;
    };
}
#endif /* bitvec_hpp */
