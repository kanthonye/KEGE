//
//  Cuboid.hpp
//  KEGE
//
//  Created by Kenneth Esdaile on 8/20/23.
//

#ifndef KEGE_CUBOID_HPP
#define KEGE_CUBOID_HPP
#include "../geometry/MeshInfo.hpp"
namespace kege
{
    class Cuboid : public MeshInfo
    {
        public: Cuboid( const kege::vec3& center, const kege::vec3& lengths );
    };


    class SubDividedCuboid : public MeshInfo
    {
    public: SubDividedCuboid( int divisions, const kege::vec3& center, const kege::vec3& lengths );
    };


    class CubicSphere : public SubDividedCuboid
    {
    public: CubicSphere( int divisions, const kege::vec3& center, const kege::vec3& lengths );
    };

}
#endif /* Cuboid_hpp */
