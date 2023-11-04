//
//  BatchManager.cpp
//  KEGE
//
//  Created by Kenneth Esdaile on 10/14/23.
//
#include <cmath>
#include "BatchManager.hpp"
namespace kege
{
    kege::Batch* BatchManager::getBatch( uint32_t i )
    {
        return _batch_buffers[i].ref();
    }

    uint32_t BatchManager::getBatchCount()const
    {
        return (uint32_t)_batch_buffers.size();
    }

    void BatchManager::submitBatch
    (
        kege::mat24* instance,
        uint32_t count
    )
    {
        kege::Graphics* graphics = KEGE::core()->graphics();
        uint32_t max_count = getSizeCap( count );

        /*
         * if no buffer exist create one
         */
        if ( _batch_buffers.empty() || _index >= _batch_buffers.size() )
        {
            kege::Ref< core::Buffer > bufr = graphics->createBuffer
            (
                core::Buffer::STORAGE,
                max_count * _stride,
                instance, core::Buffer::DYNAMIC
            );

            Batch* batch = new Batch();
            batch->setBinding( 0, new kege::BufferBinding( _name, _binding_index, bufr ) );
            batch->_capacity = max_count;
            batch->_drawcount = count;
            batch->update();

            _batch_buffers.push_back( batch );
        }
        else
        {
            Batch* batch = _batch_buffers[ _index ].ref();

            /*
             * if the number of instance is greater than the current buffer capacity create a new buffer
             */
            if( count > batch->_capacity )
            {
                kege::Ref< core::Buffer > bufr = graphics->createBuffer
                (
                    core::Buffer::STORAGE,
                    max_count * _stride,
                    instance, core::Buffer::DYNAMIC
                );

                batch->setBinding( 0, new kege::BufferBinding( _name, _binding_index, bufr ) );
                batch->_capacity = max_count;
                batch->_drawcount = count;
                batch->update();
            }
            /*
             * if the number of instance can fit the current buffer capacity copy instance data to buffer
             */
            else
            {
                batch->getBinding( 0 )->getBuffer( 0 )->copyToSystemMemory( 0, count * _stride, instance );
                batch->_drawcount = count;
            }
        }

        _index++;
    }

    uint32_t BatchManager::getSizeCap( uint32_t count )
    {
        uint32_t exponent = ceil( ::log( count ) / ::log( _base ) );
        exponent = kege::min( exponent, _exponent );
        uint32_t size = pow( _base, exponent );
        return ( size < _base ) ? _base : size;
    }

    void BatchManager::beginBatchSubmition()
    {
        _index = 0;
    }

    void BatchManager::endBatchSubmition()
    {
        if ( _index < _batch_buffers.size() )
        {
            _batch_buffers.resize( _index );
        }
    }

    uint32_t BatchManager::sizeCap()const
    {
        return _size_cap;
    }

    BatchManager::~BatchManager()
    {
        _batch_buffers.clear();
    }

    BatchManager::BatchManager( uint32_t binding_index, uint32_t base, uint32_t exponent, uint32_t stride, const kege::string& name )
    :   _binding_index( binding_index )
    ,   _exponent( exponent )
    ,   _stride( stride )
    ,   _size_cap( pow( base, exponent ) )
    ,   _index( 0 )
    ,   _base( base )
    ,   _name( name )
    {
    }

}
