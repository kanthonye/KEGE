//
//  shared.hpp
//  dev0
//
//  Created by Kenneth A Esdaile on 9/7/18.
//  Copyright © 2018 Kenneth Anthony Esdaile. All rights reserved.
//

#ifndef memptr_hpp
#define memptr_hpp
#include <stdlib.h>
namespace kege{namespace mem{
    template <typename var> class shared;

    template <typename T> class counter
    {
    public:
        
        virtual const T* operator ->() const = 0;
        virtual T* operator ->() = 0;
        
        virtual const T* operator *() const = 0;
        virtual T* operator *() = 0;
        
        virtual ~ counter() {}
        
    protected:
        
        counter() : count(0) {}
        
    private:
        
        friend shared<T>;
        int count;
    };
    
    
    template <typename T> class ptr : public counter< T >
    {
    public:
        
        inline const T* operator ->() const {return t;}
        inline T* operator ->() {return t;}
        
        inline const T* operator *() const {return t;}
        inline T* operator *() {return t;}
        
        ptr(T* p) : t( p ) {}
        
        virtual ~ ptr()
        {
            if (t != nullptr)
            {
                delete t;
                t = nullptr;
            }
        }
        
    private:
        
        T* t;
    };
    
    
    
    
    template <typename T> class shared
    {
    public:
        
        bool operator ==(const shared& s)const { return ref == s.ref; }
        bool operator !=(const shared& s)const { return ref != s.ref; }
        
        inline const T* operator ->()const{return ref->operator*();}
        inline const T* operator  *()const{return ref->operator*();}
        inline const T* rawptr()    const {return ref->operator*();}
        
        inline T* operator ->() {return ref->operator*();}
        inline T* operator  *() {return ref->operator*();}
        inline T* rawptr() {return ref->operator*();}
        
        void operator =(const shared<T>& p) { setref( p.ref ); }
        void operator =( ptr< T >& p ) { setref( p ); }
        
        bool operator ==(const T* p)const
        {
            if (ref == nullptr) return (p == nullptr) ? true : false;
            return ref->operator*() == p;
        }
        
        bool operator !=(const T* p)const
        {
            if (ref == nullptr) return (p != nullptr) ? true : false;
            return ref->operator*() != p;
        }
        
        void deref()
        {
            if ( ref != nullptr )
            {
                ref->count--;
                if ( ref->count <= 0 )
                {
                    delete ref;
                }
                ref = nullptr;
            }
        }
        
        shared(const shared& p)
        :    ref(nullptr)
        {
            setref(p.ref);
        }
        
        shared( ptr< T >* p )
        :    ref(nullptr)
        {
            if (p == nullptr)
                return;
            setref( p );
        }
        
        ~ shared()
        {
            deref();
        }
        
        shared()
        :    ref(nullptr)
        {}
        
    private:
        
        void setref(ptr< T >* p)
        {
            deref();
            ref = p;
            if (ref != nullptr)
            {
                ref->count++;
            }
        }
        
    private:
        
        ptr< T >* ref;
    };
}}
#endif /* memptr_hpp */
