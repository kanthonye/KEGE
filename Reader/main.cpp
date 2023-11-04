//
//  main.cpp
//  Reader
//
//  Created by Kenneth Esdaile on 7/18/23.
//

#include <map>
#include <iostream>
#include "FileReader.hpp"

void parseBody(kege::FileReader& reader)
{
    if ( reader.ischar( "{" ) )
    {
        reader.skip( " {\t\n" );
        while ( !reader.ischar( "}" ) && !reader.eof()  )
        {
            reader.skip( " \t\n" );
            reader.read( ":\n" );
            const char* name = reader.getstr();
            reader.skip( " :\t\n" );
            reader.read( "\n" );
            const char* value = reader.getstr();
            std::cout << name <<":  " << value <<"\n";
            reader.skip( " \n/t" );
        }
        if ( !reader.ischar( "}" ) )
        {
            throw std::runtime_error( "[ error ] missing '}'\n" );
        }
        reader.skip( " }\t" );
    }
    else if ( !reader.eof() )
    {
        throw std::runtime_error( "[ error ] missing '{'\n" );
    }
}
#include <vector>
int main(int argc, const char * argv[])
{
    int32_t count = 0;
    const int32_t mesh_width = 4;
    const int32_t w1 = mesh_width + 1;
    const int32_t w2 = w1 * 2;
    struct QuadIndices{ uint32_t i[24]; };
    std::vector< QuadIndices > idata;
    idata.resize( mesh_width );
    for(int y = 0.0f; y < mesh_width; y+=2)
    {
        for(int x = 0.0f; x < mesh_width; x+=2)
        {
            idata[ count ].i[0] = x + y * w1;
            idata[ count ].i[1] = idata[ count ].i[0] + w1;
            idata[ count ].i[2] = idata[ count ].i[1] + 1;
            idata[ count ].i[3] = idata[ count ].i[2];
            idata[ count ].i[4] = idata[ count ].i[0] + 1;
            idata[ count ].i[5] = idata[ count ].i[0];

            std::cout <<"{"<< idata[ count ].i[0] <<","<< idata[ count ].i[1] <<","<< idata[ count ].i[2]<<"}\n";
            std::cout <<"{"<< idata[ count ].i[3] <<","<< idata[ count ].i[4] <<","<< idata[ count ].i[5]<<"}\n\n";

            idata[ count ].i[ 6] = idata[ count ].i[4] + 1;
            idata[ count ].i[ 7] = idata[ count ].i[4];
            idata[ count ].i[ 8] = idata[ count ].i[3];
            idata[ count ].i[ 9] = idata[ count ].i[2];
            idata[ count ].i[10] = idata[ count ].i[2] + 1;
            idata[ count ].i[11] = idata[ count ].i[6];

            std::cout <<"{"<< idata[ count ].i[6] <<","<< idata[ count ].i[7] <<","<< idata[ count ].i[8]<<"}\n";
            std::cout <<"{"<< idata[ count ].i[9] <<","<< idata[ count ].i[10] <<","<< idata[ count ].i[11]<<"}\n\n";

            idata[ count ].i[12] = idata[ count ].i[ 2];
            idata[ count ].i[13] = idata[ count ].i[ 1];
            idata[ count ].i[14] = idata[ count ].i[ 1] + w1;
            idata[ count ].i[15] = idata[ count ].i[14];
            idata[ count ].i[16] = idata[ count ].i[14] + 1;
            idata[ count ].i[17] = idata[ count ].i[12];

            std::cout <<"{"<< idata[ count ].i[12] <<","<< idata[ count ].i[13] <<","<< idata[ count ].i[14]<<"}\n";
            std::cout <<"{"<< idata[ count ].i[15] <<","<< idata[ count ].i[16] <<","<< idata[ count ].i[17]<<"}\n\n";

            idata[ count ].i[18] = idata[ count ].i[17];
            idata[ count ].i[19] = idata[ count ].i[16];
            idata[ count ].i[20] = idata[ count ].i[16] + 1;
            idata[ count ].i[21] = idata[ count ].i[20];
            idata[ count ].i[22] = idata[ count ].i[10];
            idata[ count ].i[23] = idata[ count ].i[18];

            std::cout <<"{"<< idata[ count ].i[18] <<","<< idata[ count ].i[19] <<","<< idata[ count ].i[20]<<"}\n";
            std::cout <<"{"<< idata[ count ].i[21] <<","<< idata[ count ].i[22] <<","<< idata[ count ].i[23]<<"}\n\n";
            count++;
        }
    }
    return 0;

    kege::FileReader reader;
    if(reader.open("/Users/kae/Projects/kege-vulkan-2/KEGE/kege/assets/config/settings.config"))
    {
        while ( !reader.eof() )
        {
            reader.skip(" \t\n");
            reader.read( " :\t\n" );

            if( reader.ischar( ":" ) )
            {
                reader.skip( " :\t\n" );
                parseBody( reader );
            }
            else if ( !reader.eof() )
            {
                throw std::runtime_error( "[ error ] missing ':'\n" );
            }
        }
    };

//    std::cout << "Hello, World!\n";
    return 0;
}
