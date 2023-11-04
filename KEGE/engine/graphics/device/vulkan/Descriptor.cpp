//
//  Descriptor.cpp
//  KEGE
//
//  Created by Kenneth Esdaile on 2/8/23.
//

#include "kege-logger.hpp"
#include "../../device/vulkan/UniformHandle.hpp"
#include "../../device/vulkan/Descriptor.hpp"
#include "../../device/vulkan/Device.hpp"

namespace kege{namespace vk{

    DescriptorPool::DescriptorPool
    (
        uint32_t maxsets,
        const std::vector< VkDescriptorPoolSize >& poolsizes,
        vk::DescriptorSetAllocator* allocator
    )
    :   _poolsizes( poolsizes )
    ,   _allocator( allocator )
    ,   _max_sets( maxsets )
    ,   _next( nullptr )
    ,   _prev( nullptr )
    ,   _count( 0 )
    {
        VkDescriptorPoolCreateInfo info = {};
        info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        info.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
        info.poolSizeCount = static_cast< uint32_t>( poolsizes.size() );
        info.pPoolSizes = poolsizes.data();
        info.maxSets = maxsets;

        VkResult result = vkCreateDescriptorPool( vk::Device::get()->logicalDevice(), &info, nullptr, &_handle);
        if ( result != VK_SUCCESS )
        {
            KEGE_ERROR( "%s: Failed to create descriptor pool", vk::vkGetError( result ) );
            return;
        }
    }

    VkResult DescriptorPool::allocate
    (
        const vk::DescriptorSetLayout* descriptor_set_layout,
        VkDescriptorSet& descriptor
    )
    {
        VkDescriptorSetAllocateInfo alloc_info{};
        alloc_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
        alloc_info.pSetLayouts = &descriptor_set_layout->handle();
        alloc_info.descriptorPool = _handle;
        alloc_info.descriptorSetCount = 1;
        VkResult result = vkAllocateDescriptorSets( vk::Device::get()->logicalDevice(), &alloc_info, &descriptor );
        if ( result != VK_SUCCESS )
        {
            KEGE_ERROR( vk::vkGetError( result ) );
            return result;
        }
        _count++;
        return result;
    }

    const vk::DescriptorSetAllocator* DescriptorPool::allocator()const
    {
        return _allocator;
    }

    vk::DescriptorSetAllocator* DescriptorPool::allocator()
    {
        return _allocator;
    }

    void DescriptorPool::release( VkDescriptorSet& descriptor )
    {
        if ( descriptor != VK_NULL_HANDLE )
        {
            vk::Device::get()->waitIdle();
            vkFreeDescriptorSets( vk::Device::get()->logicalDevice(), _handle, 1, &descriptor );
            descriptor = VK_NULL_HANDLE;
            _count--;
        }

        /**
         * if ( !this->_prev ) then this descriptor-pool is not in the available
         * descriptor list. In this case we need to append it to the back of the list.
         */
        if ( !this->_prev && _allocator->_tail != this )
        {
            if ( _allocator->_tail == nullptr )
            {
                _allocator->_tail = _allocator->_head = this;
            }
            else
            {
                _allocator->_tail->_next = this;
                this->_prev = _allocator->_tail;
                _allocator->_tail = this;
            }
        }
    }

    DescriptorPool::~DescriptorPool()
    {
        if ( _handle != VK_NULL_HANDLE )
        {
            vkDestroyDescriptorPool( vk::Device::get()->logicalDevice(), _handle, nullptr );
            _handle = VK_NULL_HANDLE;
        }
        _poolsizes.clear();
    }

}}


namespace kege{namespace vk{

    void DescriptorSetAllocator::update
    (
        std::vector< VkWriteDescriptorSet >& writes,
        VkDescriptorSet& descriptor
    )
    {
        for (auto& write :  writes ) 
            write.dstSet = descriptor;
        vkUpdateDescriptorSets( vk::Device::get()->logicalDevice(), (uint32_t) writes.size(), writes.data(), 0, nullptr );
    }

    vk::DescriptorPool* DescriptorSetAllocator::allocate
    (
        std::vector< VkWriteDescriptorSet >& writes, VkDescriptorSet& descriptor
    )
    {
        if( _head )
        {
            /**
             * if( _current_pool->_count < _current_pool->_max_sets ) then their are available descriptor-set
             * that can be allocated.
             */
            if( _head->_count < _head->_max_sets )
            {
                _head->allocate( _descriptor_set_layout, descriptor );
                update( writes, descriptor );
                std::cout <<"DescriptorSetLayout: " <<this <<" | " << _descriptor_set_layout->bindings()->bindings()[0]._name <<" : " << descriptor;
                std::cout  <<" | pool: "<<_tail <<" | "<<"pool_size: " << _head->_max_sets << " | count: "<< _head->_count <<"\n";

                /**
                 * if( current-pool->count >= current-pool->max-sets ) then the descriptor-pool ran out
                 * of avaiblable descriptor-set to allocate. in this case get the next descriptor-pool
                 * in the list
                 */
                if( _head->_count >= _head->_max_sets )
                {
                    vk::DescriptorPool* pool = _head;
                    _head = _head->_next;
                    if( _head )
                    {
                        _head->_prev = nullptr;
                    }
                    /**
                     * else if the head of the list _curr is null then the list is empty, so the tail should  be null also.
                     */
                    else
                    {
                        _tail = nullptr;
                    }
                    return pool;
                }
                return _head;
            }
        }

        /**
         * if no descriptor-pool with available descriptor-set, we need to allocate a new descriptor-pool
         */
        uint32_t maxsets = pow( 8, _poolsize );
        _poolsize = min( _poolsize + 1, 5 );

        std::vector< VkDescriptorPoolSize > poolsizes;
        for ( auto& binding : _descriptor_set_layout->bindings()->bindings() )
        {
            poolsizes.push_back({ binding._params.descriptorType, binding._params.descriptorCount * maxsets });
        }

        _tail = _head = new vk::DescriptorPool( maxsets, poolsizes, this );
        _descriptor_pools.push_back( _head );
        return allocate( writes, descriptor );
    }

    const vk::DescriptorSetLayout* DescriptorSetAllocator::descriptorSetLayout()const
    {
        return _descriptor_set_layout;
    }

    void DescriptorSetAllocator::purge()
    {
        _descriptor_pools.clear();
    }

    DescriptorSetAllocator::DescriptorSetAllocator( vk::DescriptorSetLayout* descriptor_set_layout )
    :   _descriptor_set_layout( descriptor_set_layout )
    ,   _head( nullptr )
    ,   _tail( nullptr )
    ,   _poolsize( 0 )
    {}

    DescriptorSetAllocator::~DescriptorSetAllocator()
    {
        purge();
    }
}}


namespace kege{namespace vk{

    DescriptorSetLayout::DescriptorSetLayout
    (
        const kege::Ref< vk::DescriptorSetLayoutBindings >& dslbs,
        std::set< vk::PipelineLayout* >* pipeline_layout_set
    )
    :   _allocator( this )
    ,   _pipeline_layouts( nullptr )
    ,   _handle( VK_NULL_HANDLE )
    {
        std::vector< VkDescriptorSetLayoutBinding > bindings;
        for (auto& b : dslbs->bindings() ) bindings.push_back( b._params );

        VkDescriptorSetLayoutCreateInfo layoutInfo = {};
        layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
        layoutInfo.bindingCount = static_cast< uint32_t>( bindings.size() ); // Number of bindings in the layout
        layoutInfo.pBindings = bindings.data(); // Pointer to array of bindings
        VkDevice device = vk::Device::get()->logicalDevice();
        if ( vkCreateDescriptorSetLayout( device, &layoutInfo, nullptr, &_handle) != VK_SUCCESS )
        {
            KEGE_FATAL("Failed to create descriptor set layout!");
            return;
        }

        _pipeline_layouts = pipeline_layout_set;
        _bindings = dslbs;
    }

    const vk::DescriptorSetLayoutBindings* DescriptorSetLayout::bindings()const
    {
        return _bindings.ref();
    }

    const std::set< vk::PipelineLayout* >* DescriptorSetLayout::pipelineLayoutSet()const
    {
        return _pipeline_layouts;
    }

    const VkDescriptorSetLayout& DescriptorSetLayout::handle()const
    {
        return _handle;
    }

    DescriptorSetAllocator* DescriptorSetLayout::allocator()
    {
        return &_allocator;
    }

    DescriptorSetLayout::~DescriptorSetLayout()
    {
        _allocator.purge();
        if ( _handle != VK_NULL_HANDLE )
        {
            vkDestroyDescriptorSetLayout( vk::Device::get()->logicalDevice(), _handle, nullptr );
            _handle = VK_NULL_HANDLE;
        }
        _bindings.clear();
    }

}}

namespace kege{namespace vk{

    PipelineLayout::PipelineLayout
    (
        VkPipelineBindPoint pipeline_bind_point,
        const std::vector< kege::Ref< vk::DescriptorSetLayout > >& descriptor_set_layouts,
        const std::vector< VkPushConstantRange >& push_constant_ranges
    )
    :   _descriptor_set_layouts( descriptor_set_layouts )
    ,   _pipeline_bind_point( pipeline_bind_point )
    ,   _handle( VK_NULL_HANDLE )
    {
        std::vector< const VkDescriptorSetLayout > layouts;
        for (int i=0; i < descriptor_set_layouts.size(); i++)
        {
            layouts.push_back( descriptor_set_layouts[i]->handle() );
        }

        VkPipelineLayoutCreateInfo create_info{};
        create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
        create_info.setLayoutCount = static_cast< uint32_t >( layouts.size() );
        create_info.pSetLayouts = layouts.data();
        create_info.pushConstantRangeCount = 0;

        //setup push constants
        if ( !push_constant_ranges.empty() )
        {
            create_info.pPushConstantRanges = push_constant_ranges.data();
            create_info.pushConstantRangeCount = static_cast< uint32_t >( push_constant_ranges.size() );
        }

        VkResult result = vkCreatePipelineLayout( vk::Device::get()->logicalDevice(), &create_info, nullptr, &_handle );
        if ( result != VK_SUCCESS )
        {
            KEGE_ERROR( vkGetError( result ) );
            return;
        }
    }

    VkPipelineBindPoint PipelineLayout::bindPoint()const
    {
        return _pipeline_bind_point;
    }

    const VkPipelineLayout PipelineLayout::handle()const
    {
        return _handle;
    }

    PipelineLayout::~PipelineLayout()
    {
        if ( _handle != VK_NULL_HANDLE )
        {
            vkDestroyPipelineLayout( vk::Device::get()->logicalDevice(), _handle, nullptr );
            _handle = VK_NULL_HANDLE;
        }
    }
}}







namespace kege{namespace vk{

//    int getDescriptorTypeNumericValue( VkDescriptorType type )
//    {
//        switch ( type )
//        {
//            case VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER: return '1';
//            case VK_DESCRIPTOR_TYPE_STORAGE_BUFFER: return '2';
//            case VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER: return '3';
//            case VK_DESCRIPTOR_TYPE_STORAGE_IMAGE: return '4';
//            case VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE: return '5';
//            case VK_DESCRIPTOR_TYPE_SAMPLER: return '6';
//            case VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC: return '7';
//            case VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC: return '8';
//            case VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER: return '9';
//            case VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER: return '0';
//            default: throw std::runtime_error("unsupported VkDescriptorType vk::getDescriptorTypeNumericValue( VkDescriptorType )"); break;
//        }
//        return -1;
//    }

    class DescriptorWriter
    {
    public:

        const std::vector< VkWriteDescriptorSet >& getWriteDescriptorSets()const;
        std::vector< VkWriteDescriptorSet >& getWriteDescriptorSets();

        void writeDescriptorSetBuffer( const core::UniformBinding* binding );
        void writeDescriptorSetImage( const core::UniformBinding* binding );
        void integrateUniformLayout( const core::UniformLayout* asset );

        VkDescriptorBufferInfo* newDescriptorBufferInfo( uint32_t n );
        VkDescriptorImageInfo* newDescriptorImageInfo( uint32_t n );
        VkWriteDescriptorSet* newDescriptorWriteSet();

        operator bool()const;

        DescriptorWriter( const core::UniformLayout* layout );

    private:

        std::vector< VkWriteDescriptorSet > _write_descriptor_sets;
        std::array<VkDescriptorBufferInfo, 32> _descriptor_buffer_infos;
        std::array<VkDescriptorImageInfo, 32> _descriptor_image_infos;
        uint32_t _buffer_info_count;
        uint32_t _image_info_count;
    };

    const std::vector< VkWriteDescriptorSet >& DescriptorWriter::getWriteDescriptorSets()const
    {
        return _write_descriptor_sets;
    }

    std::vector< VkWriteDescriptorSet >& DescriptorWriter::getWriteDescriptorSets()
    {
        return _write_descriptor_sets;
    }

    void DescriptorWriter::writeDescriptorSetBuffer
    (
        const core::UniformBinding* binding
    )
    {
        VkWriteDescriptorSet* write = newDescriptorWriteSet();
        VkDescriptorBufferInfo* info = newDescriptorBufferInfo( binding->getCount() );

        if ( binding->getBuffer( 0 )->vulkan()->bufferType() == core::Buffer::UNIFORM )
            write->descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        else if ( binding->getBuffer( 0 )->vulkan()->bufferType() == core::Buffer::STORAGE )
            write->descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;

        std::cout <<"\n";
        for ( int i = 0; i < binding->getCount(); i++ )
        {
            const vk::Buffer* buffer = binding->getBuffer( i )->vulkan();

            info[i] = {};
            info[i].buffer = buffer->handle();
            info[i].range  = buffer->sizeInBytes();
            info[i].offset = 0;
            std::cout << binding->name() <<" -> " <<info[i].buffer <<": [range: " <<info[i].range  <<"] : [offset: " <<info[i].offset;
        }

        write->pImageInfo = nullptr; // Optional
        write->pTexelBufferView = nullptr; // Optional
        write->dstBinding = binding->getBindingIndex();
        write->descriptorCount = binding->getCount(); // Number of descriptors to update
        write->dstArrayElement = 0; // Start from element 0
        write->pBufferInfo = info;

        std::cout <<"] : [descriptorCount: " <<write->descriptorCount <<"] : [pBufferInfo: " <<write->pBufferInfo <<"]\n";
    }

    void DescriptorWriter::writeDescriptorSetImage
    (
        const core::UniformBinding* binding
    )
    {
        VkWriteDescriptorSet* write = newDescriptorWriteSet();
        VkDescriptorImageInfo* info = newDescriptorImageInfo( binding->getCount() );

        if ( binding->getLayoutType() == core::UniformBinding::STORAGE_IMAGE )
            write->descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;
        else if ( binding->getLayoutType() == core::UniformBinding::COMBINED_IMAGE_SAMPLER )
            write->descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;

        for ( int i = 0; i < binding->getCount(); i++ )
        {
            const vk::Texture* texture = binding->getTexture( i )->vulkan();

            info[i] = {};
            info[i].imageView = texture->image().view().handle();

            if( binding->getSampler() )
                info[i].sampler = binding->getSampler()->vulkan()->handle();

            if ( binding->getLayoutType() == core::UniformBinding::STORAGE_IMAGE )
                info[i].imageLayout = VK_IMAGE_LAYOUT_GENERAL;
            else if ( binding->getLayoutType() == core::UniformBinding::COMBINED_IMAGE_SAMPLER )
                info[i].imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        }

        write->pBufferInfo = nullptr; // Optional
        write->pTexelBufferView = nullptr; // Optional
        write->dstBinding = binding->getBindingIndex();
        write->descriptorCount = binding->getCount(); // Number of descriptors to update
        write->dstArrayElement = 0; // Start from element 0
        write->pImageInfo = info;
    }

    void DescriptorWriter::integrateUniformLayout( const core::UniformLayout *layout )
    {
        for (int i = 0; i < layout->getCount(); i++ )
        {
            switch ( layout->getBinding( i )->getLayoutType() )
            {
                case core::UniformBinding::STORAGE_IMAGE:
                case core::UniformBinding::COMBINED_IMAGE_SAMPLER:
                    writeDescriptorSetImage ( layout->getBinding( i ) );
                    break;

                case core::UniformBinding::BUFFER:
                case core::UniformBinding::DYNAMIC_BUFFER:
                    writeDescriptorSetBuffer ( layout->getBinding( i ) );
                    break;

                default: throw std::runtime_error("DescriptorWriter::integrateUniformLayout() : Unsupported UniformBinding::LayoutType.");
                    break;
            }
        }
    }

    VkDescriptorBufferInfo* DescriptorWriter::newDescriptorBufferInfo( uint32_t n )
    {
        VkDescriptorBufferInfo* info = &_descriptor_buffer_infos[ _buffer_info_count + n ];
        _buffer_info_count += n;
        return info;
    }

    VkDescriptorImageInfo* DescriptorWriter::newDescriptorImageInfo( uint32_t n )
    {
        VkDescriptorImageInfo* info = &_descriptor_image_infos[ _image_info_count + n ];
        _image_info_count += n;
        return info;
    }

    VkWriteDescriptorSet* DescriptorWriter::newDescriptorWriteSet()
    {
        _write_descriptor_sets.push_back({});
        _write_descriptor_sets[ _write_descriptor_sets.size() - 1 ].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        return &_write_descriptor_sets[ _write_descriptor_sets.size() - 1 ];
    }

    DescriptorWriter::DescriptorWriter( const core::UniformLayout* layout )
    :   _buffer_info_count( 0 )
    ,   _image_info_count( 0 )
    {
        integrateUniformLayout( layout );
    }

    DescriptorWriter::operator bool()const
    {
        return !_write_descriptor_sets.empty();
    }


    int32_t PipelineLayoutManager::getDescriptorSetLayoutindex( uint64_t binding_key, const core::UniformLayout* asset, const std::vector< VkWriteDescriptorSet >& writes )
    {
        auto m = _mapped_bindings.find( binding_key );
        if ( m != _mapped_bindings.end() )
        {
            for (auto& a : m->second )
            {
                bool match_found = true;
                for ( int i=0; i<a->bindings().size(); i++ )
                {
                    if( a->bindings()[i]._name != asset->getBinding( i )->name() )
                    {
                        match_found = false;
                        break;
                    }
                    if(a->bindings()[i]._params.descriptorType != writes[i].descriptorType)
                    {
                        match_found = false;
                        break;
                    }
                    if(a->bindings()[i]._params.descriptorCount != writes[i].descriptorCount)
                    {
                        match_found = false;
                        break;
                    }
                    if(a->bindings()[i]._params.binding != writes[i].dstBinding)
                    {
                        match_found = false;
                        break;
                    }
                }
                if( match_found ) return a->getDescriptorSetLayoutIndex();
            }
        }
        return -1;
    }

    int32_t PipelineLayoutManager::getDescriptorSetLayoutindex( uint64_t binding_key, const std::vector< vk::DescriptorSetLayoutBinding >& bindings )
    {
        auto m = _mapped_bindings.find( binding_key );
        if ( m != _mapped_bindings.end() )
        {
            for (auto& a : m->second )
            {
                bool match_found = true;
                for ( int i=0; i<a->bindings().size(); i++ )
                {
                    if( a->bindings()[i]._name != bindings[i]._name )
                    {
                        match_found = false;
                        break;
                    }
                    if(a->bindings()[i]._params.descriptorType != bindings[i]._params.descriptorType)
                    {
                        match_found = false;
                        break;
                    }
                    if(a->bindings()[i]._params.descriptorCount != bindings[i]._params.descriptorCount)
                    {
                        match_found = false;
                        break;
                    }
                    if(a->bindings()[i]._params.binding != bindings[i]._params.binding)
                    {
                        match_found = false;
                        break;
                    }
                }
                if( match_found ) return a->getDescriptorSetLayoutIndex();
            }
        }
        //_mapped_bindings.size()
        //_mapped_bindings[ binding_key ].push_back( new vk::DescriptorSetLayoutBindings( bindings) )
        return -1;
    }

    void bindingHash
    (
        //uint32_t              setindex,
        uint32_t              binding,
        VkDescriptorType      descriptor_type,
        uint32_t              descriptor_count,
        int64_t&              hash
    )
    {
        //hash = (hash * 54059) ^ (setindex * 76963);
        hash = (hash * 54059) ^ (binding * 76963);
        hash = (hash * 54059) ^ (descriptor_type * 76963);
        hash = (hash * 54059) ^ (descriptor_count * 76963);
    }

    uint64_t PipelineLayoutManager::getBindingKey( const core::UniformLayout* asset, const std::vector< VkWriteDescriptorSet >& writes )
    {
        int64_t seed = 37;
        int64_t hashs[ writes.size() ];
        for (int i=0; i<writes.size(); i++ )
        {
            bindingHash
            (
                writes[i].dstBinding,
                writes[i].descriptorType,
                writes[i].descriptorCount,
                seed
            ); 

            hashs[i] = asset->getBinding( i )->name().hash();
        }
        for (int i=0; i<writes.size(); i++ ) seed += hashs[ i ];
        return seed;
    }

    uint64_t  PipelineLayoutManager::getBindingKey( const std::vector< vk::DescriptorSetLayoutBinding >& bindings )
    {
        int64_t seed = 37;
        int64_t hashs[ bindings.size() ];
        for (int i=0; i<bindings.size(); i++ )
        {
            bindingHash
            (
                //setindex,
                bindings[i]._params.binding,
                bindings[i]._params.descriptorType,
                bindings[i]._params.descriptorCount,
                seed
            );

            hashs[i] = bindings[i]._name.hash();
        }
        for (int i=0; i<bindings.size(); i++ ) seed += hashs[ i ];
        return seed;
    }

    kege::Ref< core::UniformHandle > PipelineLayoutManager::createUniformHandle
    (
        const core::UniformLayout* asset
    )
    {
        DescriptorWriter writes( asset );
        if( writes )
        {
            uint64_t binding_key = getBindingKey( asset, writes.getWriteDescriptorSets() );
            uint32_t descriptor_set_signature = getDescriptorSetLayoutindex( binding_key, asset, writes.getWriteDescriptorSets() );
            vk::DescriptorSetLayout* descriptor_set_layout = getDescriptorSetLayout( descriptor_set_signature );
            if ( descriptor_set_layout == nullptr )
            {
                std::vector< vk::DescriptorSetLayoutBinding > bindings;
                for ( int i=0; i<writes.getWriteDescriptorSets().size(); i++ )
                {
                    VkDescriptorSetLayoutBinding dslb;
                    dslb.descriptorCount = writes.getWriteDescriptorSets()[i].descriptorCount;
                    dslb.descriptorType = writes.getWriteDescriptorSets()[i].descriptorType;
                    dslb.binding = writes.getWriteDescriptorSets()[i].dstBinding;
                    dslb.stageFlags = 
                    {
                        VK_SHADER_STAGE_VERTEX_BIT |
                        VK_SHADER_STAGE_FRAGMENT_BIT |
                        VK_SHADER_STAGE_COMPUTE_BIT
                    };
                    bindings.push_back({ dslb, asset->getBinding( i )->name() });
                }
                descriptor_set_layout = createDescriptorSetLayout( bindings );
            }

            VkDescriptorSet descriptor = VK_NULL_HANDLE;
            vk::DescriptorPool* pool = descriptor_set_layout->allocator()->allocate( writes.getWriteDescriptorSets(), descriptor );
            return new vk::UniformHandle( descriptor, pool );
        }
        return nullptr;
    }

    void PipelineLayoutManager::updateUniformAsset
    (
        vk::UniformHandle* handle, core::UniformLayout* asset
    )
    {
        DescriptorWriter writes( asset );
        if( writes )
        {
            handle->_descriptor_pool->allocator()->update( writes.getWriteDescriptorSets(), handle->_descriptor_set );
        }
    }

    vk::PipelineLayout* PipelineLayoutManager::createPipelineLayout
    (
        VkPipelineBindPoint pipeline_bind_point,
        const std::vector< kege::Ref< vk::DescriptorSetLayout > >& descriptor_set_layouts,
        const std::vector< VkPushConstantRange >& push_constant_ranges
    )
    {
        vk::PipelineLayout* pl = new PipelineLayout( pipeline_bind_point, descriptor_set_layouts, push_constant_ranges );
        _pipeline_layouts.push_back( pl );
        insertIntoPipelineLayoutSets( descriptor_set_layouts, pl );
        return pl;
    }

    std::vector< kege::Ref< vk::DescriptorSetLayout > > PipelineLayoutManager::createDescriptorSetLayouts
    (
        VkShaderStageFlagBits shader_type,
        SpvReflectShaderModule& shader_module
    )
    {
        uint32_t uniform_count = 0;
        SpvReflectResult result = spvReflectEnumerateDescriptorBindings( &shader_module, &uniform_count, nullptr );
        if (result != SPV_REFLECT_RESULT_SUCCESS)
        {
            throw std::runtime_error( "spvReflect failed getting totall number of unifrom." );
        }

        std::vector< SpvReflectDescriptorBinding* > spv_descriptor_binding;
        spv_descriptor_binding.resize( uniform_count );
        result = spvReflectEnumerateDescriptorBindings( &shader_module, &uniform_count, spv_descriptor_binding.data() );
        if (result != SPV_REFLECT_RESULT_SUCCESS)
        {
            throw std::runtime_error( "spvReflect failed getting uniform info." );
        }

        // integrate descriptor-set-layout
        std::map< uint32_t, std::vector< vk::DescriptorSetLayoutBinding > > bindings;
        for (const auto& spv_binding : spv_descriptor_binding)
        {
            vk::DescriptorSetLayoutBinding binding = {};
            binding._params.descriptorType = toVkDescriptorType( spv_binding->descriptor_type );
            binding._params.descriptorCount = spv_binding->count;
            binding._params.binding = spv_binding->binding;
            binding._params.stageFlags = shader_type;
            binding._params.pImmutableSamplers = nullptr;

            if( spv_binding->type_description->type_name )
                binding._name = spv_binding->type_description->type_name;
            else
                binding._name = spv_binding->name;

            bindings[ spv_binding->set ].push_back( binding );
        }

        std::vector< kege::Ref< vk::DescriptorSetLayout > > descriptor_set_layouts;
        for ( const auto& set : bindings )
        {
            descriptor_set_layouts.push_back( createDescriptorSetLayout( set.second ) );
        }
        return descriptor_set_layouts;
    }

    vk::DescriptorSetLayout* PipelineLayoutManager::createDescriptorSetLayout
    (
        const std::vector< vk::DescriptorSetLayoutBinding >& bindings
    )
    {
        uint64_t binding_key = getBindingKey( bindings );
        uint32_t descriptor_set_layout_index = getDescriptorSetLayoutindex( binding_key, bindings );
        vk::DescriptorSetLayout* dsl = getDescriptorSetLayout( descriptor_set_layout_index );
        if ( dsl == nullptr )
        {
            descriptor_set_layout_index = (uint32_t)_descriptor_set_layouts.size();
            vk::DescriptorSetLayoutBindings* dslbs = new vk::DescriptorSetLayoutBindings( descriptor_set_layout_index, bindings );
            std::set< vk::PipelineLayout* >* pipeline_layout_set = getPipelineLayoutSet( descriptor_set_layout_index );
            dsl = new vk::DescriptorSetLayout( dslbs, pipeline_layout_set );
            _descriptor_set_layouts.push_back( dsl );
            _mapped_bindings[ binding_key ].push_back( dslbs );
        }
        return dsl;
    }

    vk::DescriptorSetLayout* PipelineLayoutManager::getDescriptorSetLayout( uint32_t descriptor_set_signature )
    {
        if ( descriptor_set_signature < _descriptor_set_layouts.size() )
        {
            return _descriptor_set_layouts[ descriptor_set_signature ].ref();
        }
        return nullptr;
    }

    std::set< vk::PipelineLayout* >* PipelineLayoutManager::getPipelineLayoutSet
    (
        uint64_t descriptor_set_signature
    )
    {
        auto m = _pipeline_layout_sets.find( descriptor_set_signature );
        if ( m != _pipeline_layout_sets.end() )
        {
            return &m->second;
        }
        return &_pipeline_layout_sets[ descriptor_set_signature ];
    }

    void PipelineLayoutManager::insertIntoPipelineLayoutSets
    (
        const std::vector< kege::Ref< vk::DescriptorSetLayout > >& descriptor_set_layouts,
        vk::PipelineLayout* pipeline_layout
    )
    {
        for (int i=0; i < descriptor_set_layouts.size(); i++)
        {
            uint64_t signature = descriptor_set_layouts[i]->bindings()->getDescriptorSetLayoutIndex();
            _pipeline_layout_sets[ signature ].insert( pipeline_layout );
        }
    }

    void PipelineLayoutManager::destruct()
    {
        _descriptor_set_layouts.clear();
        _pipeline_layout_sets.clear();
        _pipeline_layouts.clear();
    }

    PipelineLayoutManager::~PipelineLayoutManager()
    {
        destruct();
    }

    PipelineLayoutManager::PipelineLayoutManager()
    {}
}}
