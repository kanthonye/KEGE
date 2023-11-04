//
//  CubeSphereLandscape.hpp
//  KEGE
//
//  Created by Kenneth Esdaile on 10/15/23.
//

#ifndef CubeSphereLandscape_hpp
#define CubeSphereLandscape_hpp

#include "Landscape.hpp"
#include "CubeFaceHeightmapGenerator.hpp"

namespace kege
{

    /**
     * CubeSphereLandscape is responsible for drawing the cube sphere faces
     */
    class CubeSphereLandscape : public Landscape
    {
    public:

        /**
         * @fn drawTerrain
         *
         * Draws the terrain using a render technique.
         *
         * @param technique The render technique to use and render the landscape
         */
        void drawTerrain
        (
            kege::CelestialSurfaceRenderTechnique* technique
        );

        void cullTerrain
        (
            const kege::Frustum<float>& frustum
        );

        void cullTerrain();

        void updateTerrain
        (
            const kege::vec3& eye
        );

        /**
         * @fn cullGeometry
         *
         * Culls geometry based on a frustum and updates the count of instances.
         *
         * @param frustum The frustum for culling.
         * @param quadtree The terrain quadtree.
         * @param instance An array to store quadtree leaf node data.
         * @param count The count of terrain quadtree nodes inside the frustum.
         */
        void cullGeometry
        (
            const kege::Frustum<float>& frustum,
            CubeSphereQuadtreeNode& quadtree,
            kege::mat24* instance,
            uint32_t& count
        );

        void setRotation( const kege::mat33& rotation );
        void setPosition( const kege::vec3& position );

        CubeSphereLandscape( double radius, double resolution, uint32_t depth );

//        core::UniformLayout1 _surface_topology;
        CubeSphereQuadtree _cubesphere;
        kege::UniformLayout1 _heightmap;
        //kege::Ref< kege::Topology > _topology;
        //kege::Ref< kege::Ocean > _ocean;


        CubeFaceHeightmapGenerator _generator;
    };

}
#endif /* CubeSphereLandscape_hpp */
