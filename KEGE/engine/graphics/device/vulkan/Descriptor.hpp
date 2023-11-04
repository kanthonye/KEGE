//
//  Descriptor.hpp
//  KEGE
//
//  Created by Kenneth Esdaile on 2/8/23.
//

#ifndef KEGE_VULKAN_DESCRIPTOR_HPP
#define KEGE_VULKAN_DESCRIPTOR_HPP

#include <unordered_map>
#include <vector>
#include <map>
#include <set>
#include "Ref.hpp"
#include "../vulkan/utils.hpp"

namespace kege{namespace vk{
    class UniformHandle;
    class PipelineLayout;
    class DescriptorSetLayout;
    class DescriptorSetAllocator;
    class PipelineLayoutManager;
}}



namespace kege{namespace vk{

    struct DescriptorSetLayoutBinding
    {
        VkDescriptorSetLayoutBinding _params;
        kege::string _name;
    };

    class DescriptorSetLayoutBindings : public kege::RefCounter
    {
    public:

        DescriptorSetLayoutBindings( uint32_t descriptor_set_layout_index, const std::vector< vk::DescriptorSetLayoutBinding >& bindings)
        :   _bindings( std::move( bindings ) )
        ,   _descriptor_set_layout_index( descriptor_set_layout_index )
        {}

        /**
         * @brief Get the bindings name of this DescriptorSetLayoutBindings.
         *
         * @return A reference to the bindings name of this object.
         */
        const std::vector< vk::DescriptorSetLayoutBinding >& bindings()const{ return _bindings; }

        /**
         * @brief Get the descriptor-set-layout-index of this DescriptorSetLayout.
         *
         * @return A reference to the bindings signature of this object.
         */
        uint32_t getDescriptorSetLayoutIndex()const{ return _descriptor_set_layout_index; }

    private:

        std::vector< vk::DescriptorSetLayoutBinding > _bindings;

        /**
         * @brief The signature value is use to store this object into a map. The signature value is a combination of
         * descriptor-types combind to create a unique value.
         *
         * @see DescriptorManager
         */
        uint32_t _descriptor_set_layout_index;
    };

    /**
     * @brief DescriptorPool is a wrapper class for Vulkan descriptor pools.
     *
     * The DescriptorPool class is responsible for creating the Vulkan descriptor pools.
     * It provides a method for constructing a descriptor pool with specified parameters and exposes methods
     * to access the pool's Vulkan handle and maximum set count.
     *
     * @see DescriptorSetAllocator, DescriptorSetLayout
     */
    struct DescriptorPool : public kege::RefCounter
    {
    public:

        DescriptorPool
        (
            uint32_t _maxsets,
            const std::vector< VkDescriptorPoolSize >& poolsizes,
            vk::DescriptorSetAllocator* _allocator
        );

        VkResult allocate
        (
            const vk::DescriptorSetLayout* descriptor_set_layout,
            VkDescriptorSet& descriptor
        );

        const vk::DescriptorSetAllocator* allocator()const;
        vk::DescriptorSetAllocator* allocator();

        void release( VkDescriptorSet& descriptor );

        ~DescriptorPool();

    private:

        std::vector< VkDescriptorPoolSize > _poolsizes;
        VkDescriptorPool _handle;

        vk::DescriptorSetAllocator* _allocator;
        vk::DescriptorPool* _next;
        vk::DescriptorPool* _prev;

        uint32_t _max_sets;
        uint32_t _count;

        friend vk::DescriptorSetAllocator;
    };
    
}}



namespace kege{namespace vk{

    /**
     * @brief DescriptorSetAllocator manages the DescriptorPools and the allocation of Vulkan descriptor sets.
     *
     * The DescriptorSetAllocator class is responsible for the creation and deletion of DescriptorPool. It is
     * responsible for allocating the updating Vulkan descriptor sets.
     *
     * @see vk::DescriptorSetLayout, vk::DescriptorPool,
     */
    class DescriptorSetAllocator
    {
    public:

        /**
         * @brief Update a UniformAsset.
         *
         * @param writes A vector of VkWriteDescriptorSet structures.
         * @param descriptor Reference to a VkDescriptorSet variable to store the allocated descriptor set.
         */
        void update
        (
            std::vector< VkWriteDescriptorSet >& writes,
            VkDescriptorSet& descriptor
        );

        /**
         * @brief Allocate a Vulkan descriptor set.
         *
         * @param writes A vector of VkWriteDescriptorSet structures.
         * @param descriptor Reference to a VkDescriptorSet variable to store the allocated descriptor set.
         * @return The descriptor-pool that allocate the descriptor-set
         */
        vk::DescriptorPool* allocate
        (
            std::vector< VkWriteDescriptorSet >& writes, VkDescriptorSet& descriptor
        );

        /**
         * @brief Get the DescriptorSetLayout associated with this DescriptorSetAllocator.
         *
         * @return A reference to the DescriptorSetLayout.
         */
        const vk::DescriptorSetLayout* descriptorSetLayout()const;

        /**
         * @brief Destruct and release resources associated with the DescriptorSetAllocator.
         */
        void purge();
        
        DescriptorSetAllocator( vk::DescriptorSetLayout* descriptor_set_layout );
        ~DescriptorSetAllocator();

    private:

        std::vector< kege::Ref< vk::DescriptorPool > > _descriptor_pools;
        vk::DescriptorSetLayout* _descriptor_set_layout;
        vk::DescriptorPool* _head;
        vk::DescriptorPool* _tail;
        uint32_t _poolsize;

        friend vk::DescriptorPool;
    };

}}


    

namespace kege{namespace vk{
    
    class DescriptorSetLayout : public kege::RefCounter
    {
    public:

        /**
         * @brief The DescriptorSetLayout constructor.
         *
         * @param bindings A vector of VkDescriptorSetLayoutBinding structures specifying the layout bindings.
         * @param pipeline_layout_set A set of PipelineLayout object used for global binding uniform assets.
         */
        DescriptorSetLayout
        (
            const kege::Ref< vk::DescriptorSetLayoutBindings >& bindings,
            std::set< vk::PipelineLayout* >* pipeline_layout_set
        );

        /**
         * @brief Get the bindings associated with the descriptor set layout.
         *
         * @return A reference to a vector of VkDescriptorSetLayoutBinding structures.
         */
        const DescriptorSetLayoutBindings* bindings()const;

        /**
         * @brief Get the pipeline-layout-set associated with this DescriptorSetLayout
         *
         * @return A reference to the PipelineLayout.
         */
        const std::set< vk::PipelineLayout* >* pipelineLayoutSet()const;

        /**
         * @brief Get the Vulkan handle of the descriptor set layout.
         *
         * @return A reference to the Vulkan handle of the descriptor set layout.
         */
        const VkDescriptorSetLayout& handle()const;

        /**
         * @brief Get the DescriptorSetAllocator of this DescriptorSetLayout.
         *
         * @return A reference to the DescriptorSetAllocator of this object.
         */
        DescriptorSetAllocator* allocator();

        /**
         * @brief The DescriptorSetLayout destructor.
         */
        ~DescriptorSetLayout();

    private:

        std::set< vk::PipelineLayout* >* _pipeline_layouts;

        kege::Ref< DescriptorSetLayoutBindings > _bindings;

        DescriptorSetAllocator _allocator;

        VkDescriptorSetLayout _handle;

        friend vk::PipelineLayoutManager;
    };

}}


namespace kege{namespace vk{

    class PipelineLayout : public kege::RefCounter
    {
    public:

        PipelineLayout
        (
            VkPipelineBindPoint pipeline_bind_point,
            const std::vector< kege::Ref< vk::DescriptorSetLayout > >& descriptor_set_layouts,
            const std::vector< VkPushConstantRange >& push_constant_ranges
        );

        VkPipelineBindPoint bindPoint()const;
        const VkPipelineLayout handle()const;
        ~PipelineLayout();

    private:

        std::vector< kege::Ref< vk::DescriptorSetLayout > > _descriptor_set_layouts;
        VkPipelineBindPoint _pipeline_bind_point;
        VkPipelineLayout _handle;

        friend vk::PipelineLayoutManager;
    };

}}


namespace kege{namespace vk{

    class PipelineLayoutManager
    {
    public:

        /**
         * @brief Get a unique signature for a descriptor set layout binding configuration.
         *
         * @param bindings A vector of VkDescriptorSetLayoutBinding structures.
         * @return A unique signature value.
         */
        int32_t getDescriptorSetLayoutindex( uint64_t binding_key, const std::vector< vk::DescriptorSetLayoutBinding >& bindings );
        int32_t getDescriptorSetLayoutindex( uint64_t binding_key, const core::UniformLayout* asset, const std::vector< VkWriteDescriptorSet >& writes );

        /**
         * @brief Get a unique signature for a descriptor set layout binding configuration.
         *
         * @param writes A vector of VkWriteDescriptorSet structures.
         * @return A unique signature value.
         */
        uint64_t getBindingKey( const core::UniformLayout* asset, const std::vector< VkWriteDescriptorSet >& writes );

        /**
         * @brief Get a unique signature for a descriptor set layout binding configuration.
         *
         * @param bindings A vector of VkDescriptorSetLayoutBinding structures.
         * @return A unique signature value.
         */
        uint64_t getBindingKey( const std::vector< vk::DescriptorSetLayoutBinding >& bindings );

        /**
         * @brief Create a UniformHandle for a UniformAsset.
         *
         * @param asset A pointer to the core::UniformAsset resource.
         * @return A kege::Ref<core::UniformHandle> to the created UniformHandle.
         */
        kege::Ref< core::UniformHandle > createUniformHandle
        ( 
            const core::UniformLayout* asset
        );

        /**
         * @brief Update a UniformAsset.
         *
         * @param handle The UniformHandle associated with the UniformAsset
         * @param asset A pointer to the core::UniformAsset resource to update.
         */
        void updateUniformAsset
        (
             vk::UniformHandle* handle, core::UniformLayout* asset
        );

        vk::PipelineLayout* createPipelineLayout
        (
            VkPipelineBindPoint pipeline_bind_point,
            const std::vector< kege::Ref< vk::DescriptorSetLayout > >& descriptor_set_layout,
            const std::vector< VkPushConstantRange >& push_constant_ranges
        );

        /**
         * @brief create all the DescriptorSetLayout from the given shader-module.
         *
         * @param shader_type The enum shader of the shader_module.
         * @param shader_module The shader module to parse DescriptorSetLayoutBinding from.
         * @return A vector of to the created vk::DescriptorSetLayout.
         */
        std::vector< kege::Ref< vk::DescriptorSetLayout > > createDescriptorSetLayouts
        (
            VkShaderStageFlagBits shader_type,
            SpvReflectShaderModule& shader_module
        );

        /**
         * @brief create a DescriptorSetLayout for a given descriptor set layout binding configuration.
         *
         * @param bindings A vector of vk::DescriptorSetLayoutBinding structures.
         * @return A pointer to the created vk::DescriptorSetLayout.
         */
        vk::DescriptorSetLayout* createDescriptorSetLayout
        (
            const std::vector< vk::DescriptorSetLayoutBinding >& bindings
        );

        /**
         * @brief Get a DescriptorSetLayout based on its unique signature.
         *
         * @param descriptor_set_signature The unique signature of the descriptor set layout.
         *
         * @return A pointer to the vk::DescriptorSetLayout, nullptr otherwise.
         */
        vk::DescriptorSetLayout* getDescriptorSetLayout
        (
            uint32_t descriptor_set_signature
        );

        /**
         * @brief Get a set of VkPipelineLayout objects associated with a descriptor set signature.
         *
         * @param descriptor_set_signature The unique signature of the descriptor set layout.
         * @return A pointer to a set of VkPipelineLayout objects.
         */
        std::set< vk::PipelineLayout* >* getPipelineLayoutSet
        (
            uint64_t descriptor_set_signature
        );

        void insertIntoPipelineLayoutSets
        (
            const std::vector< kege::Ref< vk::DescriptorSetLayout > >& descriptor_set_layout,
            vk::PipelineLayout* pipeline_layout
        );

        /**
         * @brief Destruct and release resources associated with the DescriptorManager.
         */
        void destruct();

        ~PipelineLayoutManager();
        PipelineLayoutManager();
        
    private:

       std::map< uint64_t, std::vector< kege::Ref< DescriptorSetLayoutBindings > > > _mapped_bindings;

        std::vector< kege::Ref< vk::DescriptorSetLayout > > _descriptor_set_layouts;

        /**
         * @brief Map of unique signatures to sets of VkPipelineLayout objects.
         */
        std::map< uint64_t, std::set< vk::PipelineLayout* > > _pipeline_layout_sets;


        std::vector< kege::Ref< vk::PipelineLayout > > _pipeline_layouts;
    };
}}

#endif /* KEGE_VULKAN_DESCRIPTOR_HPP */
