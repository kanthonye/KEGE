//
//  BatchManager.hpp
//  KEGE
//
//  Created by Kenneth Esdaile on 10/14/23.
//

#ifndef BatchManager_hpp
#define BatchManager_hpp

#include "cgm.hpp"
#include "KEGE.hpp"
#include "Core.hpp"
#include "Graphics.hpp"
#include "UniformLayout.hpp"

namespace kege{

    /**
     * @struct BatchBuffer
     *
     * BatchBuffer is a structure used for managing rendering batches within
     * the terrain. It holds information about the draw count and capacity of
     * a batch. its hold instance information for rendering each quadtree leaf
     *
     *@see core::UniformAssetSingle
     */
    struct Batch : public kege::UniformLayout1
    {
        uint32_t _drawcount = 0;
        uint32_t _capacity = 0;
    };
    
    /**
     * @class BatchManager
     *
     * BatchManager is a class responsible for managing and submitting batches of instances for rendering.
     * It dynamically manages batches of different sizes with a maximum capacity calculated using a specified
     * base and exponent. The size of a batch is adjusted dynamically based on the number of batch elements 
     * being submitted. Batchs are added and remove base to the number of elements.
     *
     * @see Batch
     *
     * @note It's important to understand the interaction between BatchManager and Batch classes for efficient rendering.
     *
     * @public
     */
    class BatchManager
    {
    public:

        /**
         * @fn submitBatch
         *
         * Submits a batch of instances for rendering.
         *
         * @param instance An array of quadtree data.
         * @param count The count of instances to submit.
         */
        void submitBatch
        (
            kege::mat24* instance,
            uint32_t count
        );

        /**
         * @fn getBatchBuffer
         *
         * Get BatchBuffer at index i
         *
         * @return The BatchBuffer at index i
         */
        kege::Batch* getBatch( uint32_t i );

        /**
         * @fn getBatchBufferCount
         *
         * Get total BatchBuffer count
         *
         * @return The BatchBuffer count
         */
        uint32_t getBatchCount()const;

        /**
         * @fn getSizeCap
         *
         * Calculates the size capacity based on a count of instances.
         *
         * @param count The count of instances.
         * @return The size capacity.
         */
        uint32_t getSizeCap
        (
            uint32_t count
        );

        /**
         * @fn beginBatchSubmission
         *
         * Starts a batch submission operation.
         */
        void beginBatchSubmition();

        /**
         * @fn endBatchSubmission
         *
         * Ends a batch submission operation.
         */
        void endBatchSubmition();

        /**
         * @fn sizeCap
         *
         * Returns the max capacity a batch support.
         *
         * @return The max batch capacity.
         */
        uint32_t sizeCap()const;

        /**
         * @fn BatchManager
         *
         * Constructor for the BatchManager class.
         *
         * @param base The base value for size capacity calculation.
         * @param exponent The exponent for size capacity calculation.
         * @param stride The stride used for managing batches.
         */
        BatchManager( uint32_t binding_index, uint32_t base, uint32_t exponent, uint32_t stride, const kege::string& name );
        ~BatchManager();

    private:

        std::vector< Ref< kege::Batch > > _batch_buffers;
        uint32_t _binding_index;
        uint32_t _exponent;
        uint32_t _size_cap;
        uint32_t _stride;
        uint32_t _index;
        uint32_t _base;

        kege::string _name;
    };

}
#endif /* BatchManager_hpp */
