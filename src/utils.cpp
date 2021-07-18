#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
using std::ifstream;

class CUtils
{
    public:
    
    int readFile( const char* fileName, char* buffer, unsigned int maxFileSize )
    {
        unsigned int n;
        ifstream ifs(fileName);
        std::cout << std::endl << "Read FILE: " << fileName << std::endl;
        // ifs.open(fileName);
        ifs.read( buffer, maxFileSize );
        n = ifs.gcount();
        ifs.close();

        return n;
    }

    void printFile( const char* data, unsigned int n ){
        std::cout << std::endl << "--- FILE Read Start ----------------------------------------------" << std::endl;
        for( int i=0; i< n; i++ ){
            std::cout << data[i];
        }
        std::cout << std::endl << "--- FILE Read End ----------------------------------------------" << std::endl;    
    }

};