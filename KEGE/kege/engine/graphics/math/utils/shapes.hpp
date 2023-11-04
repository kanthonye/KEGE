//
//  shapes.hpp
//  kege-rendering-engine
//
//  Created by Kenneth A Esdaile on 2/13/22.
//

#ifndef shapes_hpp
#define shapes_hpp
#include "vec3.hpp"
#include "mat33.hpp"
namespace kege
{
    template< typename type > class SphereT
    {
    public:
        kege::Vec3< type > center;
        type radius;
    };
    
    typedef kege::SphereT< float  > Sphere;
    typedef kege::SphereT< double > dSphere;
    typedef kege::SphereT< int    > iSphere;
}
    
    
namespace kege
{
    template< typename type > class SquareT
    {
    public:
        kege::Vec2< type > center;
        type length;
    };
}
    
namespace kege
{
    template< typename type > class RectT
    {
    public:
        kege::Vec3< type > center;
        type length;
    };
    
    typedef kege::RectT< float  > Rect;
    typedef kege::RectT< double > dRect;
    typedef kege::RectT< int    > iRect;
}
    
    
namespace kege
{
    template< typename type > class AABBT
    {
    public:
        kege::Vec3< type > min;
        kege::Vec3< type > max;
    };
    
    typedef kege::AABBT< float  > AABB;
    typedef kege::AABBT< double > dAABB;
    typedef kege::AABBT< int    > iAABB;
}
    
    
namespace kege
{
    template< typename type > class OBBT
    {
    public:
        kege::Vec3< type > center;
        kege::Vec3< type > length;
        kege::Mat33< type > axes;
    };
    
    typedef kege::OBBT< float  > OBB;
    typedef kege::OBBT< double > dOBB;
    typedef kege::OBBT< int    > iOBB;
}
#endif /* shapes_hpp */
