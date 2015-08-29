//Here is the file that contains my image loading function.

#ifndef LOAD_IMG_H_INCLUDED
#define LOAD_IMG_H_INCLUDED

#include <GL/glfw.h>

#include <iostream>
#include <stbimg.h>
#include <string>

bool loadCorrectly = true;

using namespace std;

GLuint LoadTexture( const char *filename, int w, int h, int magtype, int mintype )
{

    int n = 4;

    unsigned char *data = stbi_load( filename, &w, &h, &n, STBI_rgb_alpha );
    GLuint texture;

    if( data == NULL )
    {

        cout << "ERROR: '"<< filename << "' HAS BEEN MODIFIED OR IS MISSING." << endl;
        return 0;

    }else
    {

        cout << "Loaded: '" << filename << "' successfully." << endl;

    }

    glGenTextures( 1, &texture );
    glBindTexture( GL_TEXTURE_2D, texture );

    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, mintype );
    glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magtype );

    glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE, data );

    free( data );

    return texture;
}

#endif // LOAD_IMG_H_INCLUDED
