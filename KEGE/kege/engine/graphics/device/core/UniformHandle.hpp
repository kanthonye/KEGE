//
//  UniformHandle.hpp
//  KEGE
//
//  Created by Kenneth Esdaile on 9/22/23.
//

#ifndef KEGE_CORE_UNIFROM_HANDLE
#define KEGE_CORE_UNIFROM_HANDLE

#include "../../device/core/Abstraction.h"

namespace kege{namespace vk{
    class UniformHandle;
}}

namespace kege{namespace core{
    class UniformLayout;

    /**
     * @brief UniformHandle represents a slot/socket where UniformTarget objects
     * (resources such as buffers or textures) plug into so that they are available
     * to the shader.
     *
     * In the context of Vulkan or similar graphics APIs, this class typically
     * contains the descriptor-set handle, which allows the shader to access
     * uniform resources. UniformHandle acts as an interface between the shader
     * and the resources it requires.
     *
     * @note The specific implementation of UniformHandle can vary depending on
     * the graphics API used.
     *
     * @see core::UniformLayout, vk::UniformHandle
     */
    class UniformHandle : public kege::RefCounter
    {
    public:

        /**
         * @brief Get a pointer to the Vulkan-specific UniformHandle.
         * @return A pointer to the Vulkan-specific UniformHandle, or nullptr
         * if not applicable.
         */
        virtual const vk::UniformHandle* vulkan()const{ return nullptr; }

        /**
         * @brief Update the UniformHandle when an associated UniformAsset is changed.
         *
         * When an associated UniformAsset (e.g., UniformTarget) is modified, this
         * method is called to update the UniformHandle. This ensures that the shader
         * continues to have access to the correct resources.
         *
         * @param layout The UniformLayout that has been modified.
         */
        virtual void update( core::UniformLayout* layout ) = 0;

        /**
         * @brief Release any allocated data and perform cleanup.
         *
         * This method is responsible for releasing any allocated resources and
         * performing cleanup when the UniformHandle is no longer needed.
         */
        virtual void destruct() = 0;
        
        KEGE_CORE_ABSTRACTION( UniformHandle );
    };
}}
#endif /* KEGE_CORE_UNIFROM_HANDLE */
