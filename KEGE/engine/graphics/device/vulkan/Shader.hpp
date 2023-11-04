//
//  Shader.hpp
//  KEGE
//
//  Created by Kenneth Esdaile on 9/17/23.
//

#ifndef Shader_hpp
#define Shader_hpp

#include "../../device/vulkan/utils.hpp"

namespace kege{namespace vk{

    class Shader : public kege::core::Shader
    {
    public:

        void render
        (
            kege::Geometry* geometry,
            uint32_t first_instance,
            uint32_t instance_count
        )const;

        void bind
        (
            kege::Geometry* geometry
        )const;

        virtual void despatch( uint32_t num_groups_x, uint32_t num_groups_y, uint32_t num_groups_z ){}
        void pushConstants( uint32_t stages, uint32_t offset, uint32_t size, const void* data );
        
        void bindUniformLayoutByName( const char* name, const core::UniformLayout* layout );
        void bindUniformLayout( uint32_t setindex, const core::UniformLayout* layout );

        int32_t getShaderResourceIndex( const char* name )const;

        void bind( core::CommandBuffer* command )const;
        const vk::PipelineLayout* pipelineLayout()const;
        const VkPipeline handle()const;

        virtual const vk::Shader* vulkan()const;
        virtual vk::Shader* vulkan();
        virtual void destruct();

        virtual ~ Shader();
        Shader();

    protected:

        std::map< int, vk::DescriptorSetLayout* > _descriptor_layouts;
        
        mutable vk::CommandBuffer* _command;
        vk::PipelineLayout* _pipeline_layout;
        VkPipeline          _handle;

        friend vk::Device;
    };

}}
#endif /* Shader_hpp */
