//
//  noise.hpp
//  column-major
//
//  Created by Kenneth A Esdaile on 7/13/18.
//  Copyright © 2018 Kenneth A Esdaile. All rights reserved.
//

#ifndef noise_hpp
#define noise_hpp
#include "coremath.hpp"
namespace kege{namespace perlin{
 
    double noise(double x, double y, double z);
    double noise(double x, double y);
    void setseed(int seed);
}}
#endif /* noise_hpp */
