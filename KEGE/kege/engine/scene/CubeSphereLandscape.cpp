//
//  CubeSphereLandscape.cpp
//  KEGE
//
//  Created by Kenneth Esdaile on 10/15/23.
//

#include "CubeSphereLandscape.hpp"
#include "CelestialSurfaceRenderTechnique.hpp"

namespace kege
{
    void CubeSphereLandscape::drawTerrain( kege::CelestialSurfaceRenderTechnique* technique )
    {
        kege::BatchManager* manager = _cubesphere.getBatchBufferManager();

        technique->setInput( kege::CelestialSurfaceRenderTechnique::HEIGHT_MAP, &_heightmap );
        technique->setFaceRotationSSBO( _cubesphere.getFaceRotationMatrices() );
        technique->bindGeometry( _cubesphere.getGeometry() );
        for( int i=0; i < manager->getBatchCount(); i++ )
        {
            kege::Batch* batch = manager->getBatch( i );
            technique->drawGeometry( batch, _cubesphere.getGeometry() );
        }
    }

    void CubeSphereLandscape::cullTerrain
    (
        const kege::Frustum<float>& frustum
    )
    {
        _cubesphere.cullGeometry( frustum );
    }

    void CubeSphereLandscape::cullTerrain()
    {
        _cubesphere.cullGeometry();
    }

    void CubeSphereLandscape::updateTerrain
    (
        const kege::vec3& eye
    )
    {
        _cubesphere.update( eye );
    }

    void CubeSphereLandscape::setRotation( const kege::mat33& rotation )
    {
        _cubesphere.setRotation( rotation );
    }

    void CubeSphereLandscape::setPosition( const kege::vec3& position )
    {
        _cubesphere.setPosition( position );
    }

    CubeSphereLandscape::CubeSphereLandscape( double radius, double resolution, uint32_t depth )
    :   _cubesphere( radius, resolution, depth, 0 )
    {
        _cubesphere.buildGeometry();

//        int width, height, channels;
//        stbi_uc* pixels = stbi_load("/Users/kae/Downloads/texture_shade-1.png", &width, &height, &channels, STBI_rgb_alpha);
        core::Device* device = KEGE::core()->graphics()->device();
        kege::Ref< core::Sampler > sampler = device->createSampler
        (
            core::Sampler::LINEAR,
            core::Sampler::LINEAR,
            core::Sampler::CLAMP_TO_EDGE,
            core::Sampler::CLAMP_TO_EDGE,
            core::Sampler::CLAMP_TO_EDGE
        );
//        kege::Ref< core::Texture > texture = device->createTexture2d
//        (
//            width, height, 1, core::RGBA8_SRGB, pixels, false
//        );
//        _texture_input.setBinding(0, new kege::TextureSamplerBinding("Heightmap", 0, sampler, texture ) );

//        _texture_input ( new core::UniformTargetTexture( core::UniformTarget::COMBINED_IMAGE_SAMPLER, 3, 0, texture ) );
//        _texture_input.update();

        CubeFaceHeightmapGenerator::Params params = {};
        std::vector< kege::Ref< core::Texture > > textures = CubeFaceHeightmapGenerator::createCubeFaceTextures( 1024+1, 1024+1 );
        _generator.generatorHeightmap( params, textures );

        _heightmap.setBinding( 0, new kege::TextureSamplerArrayBinding< 6 >( "Heightmaps", 0, sampler, textures ) );
    }
}
