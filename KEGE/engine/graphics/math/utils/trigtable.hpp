/*! @author     Kenneth Anthony Esdaile
 *  @date       created on 5/25/14
 *  @copyright  Copyright (c) 2015 kae. All rights reserved.
 *  @file       DegTrigTble.h
 *  @project    computer graphics mathematics. */
#ifndef vm_trigtable_h
#define vm_trigtable_h
#include "coremath.hpp"
namespace kege{
    
    class trigtable
    {
        enum trigtype
        {
            RADIAN_TABLE,
            DEGREE_TABLE
        };
        
        void generate(trigtype t = DEGREE_TABLE);
        
        bool writeCSV(const char * filename);
        bool readCSV(const char * filename);
        bool write(const char * filename);
        bool read(const char * filename);
        
        trigtype gettabletype()const;
        
        float cos(float val);
        float sin(float val);
        float tan(float val);
        
        trigtable(trigtype t);
        ~trigtable();
        trigtable();
        
    private:
        
        int to_index(float val);
        
        struct trigrow
        {
            float sin;
            float cos;
            float tan;
        };
        
        trigtype  type;
        static trigrow * table;
    };
}
#endif /* defined(__KEM__trig_table__) */
