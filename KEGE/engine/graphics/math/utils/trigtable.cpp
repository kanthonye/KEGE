/*! @author     Kenneth Anthony Esdaile
 *  @date       created on 5/25/14
 *  @copyright  Copyright (c) 2015 kae. All rights reserved.
 *  @file       DegTrigTble.h
 *  @project    computer graphics mathematics. */
#include <stdio.h>
#include "trigtable.hpp"
#define TRIG_COUNT 3600 /* to reduce or increase precision - increase or decrease RADIAN_COUNT */
namespace kege{
    
    const double RADIAN         = 3.141592654 / 180.0;
    const double MICRO_DEGREE   = 360.0 / (double)TRIG_COUNT;
    const double DEG_MULTIPLIER = (double)TRIG_COUNT / 360.0;
    const double MICRO_RADIAN   = _2PI_ / (double)TRIG_COUNT;
    const double RAD_MULTIPLIER = (double)TRIG_COUNT / _2PI_;
    
    void trigtable::generate(trigtable::trigtype t)
    {
        if (table) delete [] table;
        table = new trigrow[TRIG_COUNT];
        float radian = 0;
        
        if (t == DEGREE_TABLE)
        {
            for (int i=0; i<=TRIG_COUNT; i++)
            {
                table[i].sin = sin(RADIAN*radian);
                table[i].cos = cos(RADIAN*radian);
                table[i].tan = tan(RADIAN*radian);
                radian += MICRO_DEGREE;
            }
        }
        else
        {
            for (int i=0; i<=TRIG_COUNT; i++)
            {
                table[i].sin = sin(radian);
                table[i].cos = cos(radian);
                table[i].tan = tan(radian);
                radian += MICRO_RADIAN;
            }
        }
    }
    
    trigtable::trigtype trigtable::gettabletype()const
    {
        return type;
    }
    int trigtable::to_index(float val)
    {
        int n;
        if (type == DEGREE_TABLE)
        {
            n = (DEG_MULTIPLIER * val);
        }
        else
        {
            n = (RAD_MULTIPLIER * val);
        }
        return ((n < 0)? -n: n) % TRIG_COUNT;
    }
    
    float trigtable::cos(float val)
    {
        return table[ to_index(val) ].cos;
    }
    
    float trigtable::sin(float val)
    {
        return table[ to_index(val) ].sin;
    }
    
    float trigtable::tan(float val)
    {
        return table[ to_index(val) ].tan;
    }
    
    trigtable::~ trigtable()
    {
        if (table) delete [] table;
        table = nullptr;
    }
    
    trigtable::trigtable()
    :   type(DEGREE_TABLE)
    {
        generate(type);
    }
    
    trigtable::trigtable(trigtype t)
    :   type(t)
    {
        generate(type);
    }
    
    trigtable::trigrow * trigtable::table = nullptr;
    
    bool trigtable::writeCSV(const char * filename)
    {
        if (!table) return false;
        FILE *fp = nullptr;
        float radian = 0;
        if ((fp = fopen(filename, "w")) != nullptr)
        {
            fprintf(fp, "degree, sin, cos, tan\n");
            for (int i=0; i<=TRIG_COUNT; i++)
            {
                fprintf(fp, "%f, %f, %f, %f\n",radian, table[i].sin, table[i].cos, table[i].tan);
                radian += MICRO_DEGREE;
            }
            fclose(fp);
            return true;
        }
        else
            fprintf(stderr, "ERROR file: ('%s') counld not be found!\n", filename);
        return false;
    }
    
    bool trigtable::readCSV(const char * filename)
    {
        FILE *fp = nullptr;
        float radian = 0;
        if ((fp = fopen(filename, "w")) != nullptr)
        {
            char temp[16];
            fscanf(fp, "%s, %s, %s, %s\n",temp,temp,temp,temp);
            table = new trigrow[TRIG_COUNT];
            for (int i=0; i<=TRIG_COUNT; i++)
            {
                fscanf(fp, "%f, %f, %f, %f\n",&radian, &table[i].sin, &table[i].cos, &table[i].tan);
                radian += MICRO_DEGREE;
            }
            fclose(fp);
            return true;
        }
        else
            fprintf(stderr, "ERROR file: ('%s') counld not be found!\n", filename);
        return false;
    }
    
    bool trigtable::write(const char * filename)
    {
        if (!table) return false;
        FILE *fp;
        fp = fopen(filename, "wb");
        
        if (fp != nullptr)
        {
            fwrite(table, sizeof(trigrow), TRIG_COUNT, fp);
            fclose(fp);
            return true;
        }
        else
            fprintf(stderr, "ERROR file: ('%s') counld not be found!\n", filename);
        
        return false;
    }
    
    bool trigtable::read(const char * filename)
    {
        FILE *fp;
        fp = fopen(filename, "rb");
        
        if (fp != nullptr)
        {
            table = new trigrow[TRIG_COUNT];
            fread(table, sizeof(trigrow), TRIG_COUNT, fp);
            fclose(fp);
            return true;
        }
        else
            fprintf(stderr, "ERROR file: ('%s') counld not be found!\n", filename);
        
        return false;
    }
    
}

