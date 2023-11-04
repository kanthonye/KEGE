//
//  grad.cpp
//  dev0
//
//  Created by Kenneth A Esdaile on 7/15/18.
//  Copyright © 2018 Kenneth Anthony Esdaile. All rights reserved.
//

#include "grad.hpp"
namespace kege{
    
    void grad::add(real val, ubyte r, ubyte g, ubyte b)
    {
        grad::add(val, kege::rgb(r,g,b));
    }
    
    void grad::add(real val, const vec3& colr)
    {
        for (std::vector< grad::color >::iterator g=gradients.begin(); g!=gradients.end(); g++)
        {
            if ((*g).value == val)
            {
                (*g).colr = colr;
                return;
            }
        }
        gradients.push_back( grad::color(val, colr) );
    }
    
    vec3 grad::get(real val)const
    {
        if ( gradients.empty() ) return vec3(0.0f);
        
        const grad::color* prev = nullptr;
        const grad::color* curr = &gradients[0];
        for (std::vector< grad::color >::const_iterator g=gradients.begin(); g!=gradients.end(); g++)
        {
            prev = curr;
            curr = &(*g);
            if (curr->value >= val)
            {
                float max = (curr->value - prev->value);
                float pos = (curr->value - val);
                float f = pos / ((max!=0.0f)?max:1.0f);
                return lerp(curr->colr, prev->colr, f);
            }
        }
        return curr->colr;
    }
}
