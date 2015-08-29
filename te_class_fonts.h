#ifndef CLASS_FONT_H_INCLUDED
#define CLASS_FONT_H_INCLUDED
#include <te_load_img.h>
#include <te.h>
#include <string>

class Font
{

    public:
        void Create( int xAm, int yAm, const char *filename );
        void Write( string text, double width, double height, double x, double y, float r, float g, float b, float alph );
    private:
        GLuint fontimg;
        int xam, yam;
        TE_SPRITE font;

        int grabSpriteNum( const char ch );

};

void Font::Create( int xAm, int yAm, const char *filename )
{

    xam = xAm;
    yam = yAm;
    fontimg = LoadTexture(filename, 1024, 512, GL_NEAREST, GL_NEAREST );
    font.Create( fontimg, xam, yam );

}

int Font::grabSpriteNum( const char ch )
{

    switch( ch )
    {

        case 'A': return 33; break;
        case 'B': return 34; break;
        case 'C': return 35; break;
        case 'D': return 36; break;
        case 'E': return 37; break;
        case 'F': return 38; break;
        case 'G': return 39; break;
        case 'H': return 40; break;
        case 'I': return 41; break;
        case 'J': return 42; break;
        case 'K': return 43; break;
        case 'L': return 44; break;
        case 'M': return 45; break;
        case 'N': return 46; break;
        case 'O': return 47; break;
        case 'P': return 56; break;
        case 'Q': return 57; break;
        case 'R': return 58; break;
        case 'S': return 59; break;
        case 'T': return 60; break;
        case 'U': return 61; break;
        case 'V': return 62; break;
        case 'W': return 63; break;
        case 'X': return 48; break;
        case 'Y': return 49; break;
        case 'Z': return 50; break;

        case 'a': return 65; break;
        case 'b': return 66; break;
        case 'c': return 67; break;
        case 'd': return 68; break;
        case 'e': return 69; break;
        case 'f': return 70; break;
        case 'g': return 71; break;
        case 'h': return 72; break;
        case 'i': return 73; break;
        case 'j': return 74; break;
        case 'k': return 75; break;
        case 'l': return 76; break;
        case 'm': return 77; break;
        case 'n': return 78; break;
        case 'o': return 79; break;
        case 'p': return 88; break;
        case 'q': return 89; break;
        case 'r': return 90; break;
        case 's': return 91; break;
        case 't': return 92; break;
        case 'u': return 93; break;
        case 'v': return 94; break;
        case 'w': return 95; break;
        case 'x': return 80; break;
        case 'y': return 81; break;
        case 'z': return 82; break;

        case ' ': return 0;
        case '!': return 1;
        case '"': return 2;
        case '#': return 4;
        case '$': return 5;
        case '%': return 6;
        case '&': return 7;
        case '(': return 8;
        case ')': return 9;
        case '*': return 10;
        case '+': return 11;
        case ',': return 12;
        case '-': return 13;
        case '.': return 14;
        case '/': return 15;

        case '0': return 24;
        case '1': return 25;
        case '2': return 26;
        case '3': return 27;
        case '4': return 28;
        case '5': return 29;
        case '6': return 30;
        case '7': return 31;
        case '8': return 16;
        case '9': return 17;

        case ':': return 18;
        case ';': return 19;
        case '<': return 20;
        case '=': return 21;
        case '>': return 22;
        case '?': return 33;

        default: return 0;
    }

}

void Font::Write( string text, double width, double height, double x, double y, float r, float g, float b, float alph )
{

    glPushAttrib( GL_CURRENT_BIT );

    glColor4f( r, g, b, alph );

    for( unsigned int i = 0; i < text.length(); i++ )
    {


        font.Draw( grabSpriteNum(text[i]), x+(width*i), y, width, height, 0.0f );

    }

    glPopAttrib();

}

#endif // CLASS_FONT_H_INCLUDED
