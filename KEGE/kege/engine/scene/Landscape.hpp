//
//  Landscape.hpp
//  KEGE
//
//  Created by Kenneth Esdaile on 10/14/23.
//

#ifndef Landscape_hpp
#define Landscape_hpp

#include "CubeSphereQuadtree.hpp"

namespace kege
{
    class CelestialSurfaceRenderTechnique;

    class Landscape : public kege::RefCounter
    {
    public:

        virtual void drawTerrain( kege::CelestialSurfaceRenderTechnique* technique ){}
        virtual void cullTerrain( const kege::Frustum<float>& frustum ){}
        virtual void updateTerrain ( const kege::vec3& eye ){}
        virtual void cullTerrain(){}

        virtual void setRotation( const kege::mat33& rotation ){}
        virtual void setPosition( const kege::vec3& position ){}

        virtual ~Landscape(){}
    };

}


namespace kege{namespace component{

    typedef kege::Ref< Landscape > Landscape;
    
}}
#endif /* Landscape_hpp */
