//
//  grad.hpp
//  dev0
//
//  Created by Kenneth A Esdaile on 7/15/18.
//  Copyright © 2018 Kenneth Anthony Esdaile. All rights reserved.
//

#ifndef grad_hpp
#define grad_hpp
#include <vector>
#include "vec3.hpp"
namespace kege{
    
    class grad
    {
    public:
        
        struct color
        {
            color(real v, real r, real g, real b) : value(v), colr(r,g,b) {}
            color(real v, const vec3& c) : value(v), colr(c) {}
            vec3  colr;
            real value;
        };
        
        void add(real val, ubyte r, ubyte g, ubyte b);
        void add(real val, const vec3& color);
        vec3 get(real val)const;
        
    private:
        
        std::vector< grad::color > gradients;
    };
}
#endif /* grad_hpp */
