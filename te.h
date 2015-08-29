#ifndef TE_H_INCLUDED
#define TE_H_INCLUDED

#include <te_load_img.h>
#include <te_load_sound.h>

#include <GL/glfw.h>
#include <windows.h>
#include <irrKlang/irrKlang.h>

#include <string>
#include <fstream>

#include <AL/alc.h>
#include <AL/al.h>


//Version 2.0
//Added diamond square algorithm, wooh!
//New Button Class in progress

bool *TE_KEYSTATES = new bool[256];
bool *TE_KEYPRESS = new bool[256];
bool *TE_KEYMODS = new bool [4];

bool *TE_MOUSEBUTTONS = new bool[12];
bool *TE_MOUSECLICK = new bool[12];

int TE_WINDOW_WIDTH;
int TE_WINDOW_HEIGHT;
int TE_INIT_WINDOW_WIDTH;
int TE_INIT_WINDOW_HEIGHT;
float TE_SCALEX = 1.0f;
float TE_SCALEY = 1.0f;

const float TE_FRAMES_PER_SECOND = 60.0f;
float TE_RUN_FRAMES_PER_SECOND;
float TE_DELTA_TIME;
float TE_FRAME_INDEP;

float TE_CUR_SECOND;

struct TE_COORD
{

    float x;
    float y;

};

TE_COORD TE_MOUSE_POS;

struct TE_RECT
{

    float x;
    float y;
    float w;
    float h;

    void Draw( float r, float g, float b, float ang );

};

void TE_RECT::Draw( float r, float g, float b, float ang )
{

    glPushMatrix();

    glTranslatef( x, y, 0.0f );
    glRotatef( ang, 0.0f, 0.0f, 1.0f );

    glEnableClientState( GL_VERTEX_ARRAY );
    glEnableClientState( GL_COLOR_ARRAY );

    float vertices[] = {
        -w/2.0f, -h/2.0f,
        w/2.0f, -h/2.0f,
        w/2.0f, h/2.0f,
        -w/2.0f, h/2.0f
    };

    float color[] = {
        r, g, b,
        r, g, b,
        r, g, b,
        r, g, b
    };

    glVertexPointer( 2, GL_FLOAT, 0, vertices );
    glColorPointer( 3, GL_FLOAT, 0, color );

    glDrawArrays( GL_TRIANGLE_FAN, 0, 4 );

    glDisableClientState( GL_VERTEX_ARRAY );
    glDisableClientState( GL_COLOR_ARRAY );

    glPopMatrix();

}

void TE_MOUSE_BUTTON_FUNC( GLFWwindow *window, int button, int action, int mods )
{

    TE_MOUSECLICK[ button ] = false;

    // The three states of buttons.

    if( action == GLFW_PRESS ){ TE_MOUSEBUTTONS[ button ] = true; TE_MOUSECLICK[ button ] = true; }
    if( action == GLFW_RELEASE ) TE_MOUSEBUTTONS[ button ] = false;
    if( action == GLFW_REPEAT ) TE_MOUSEBUTTONS[ button ] = true;

}

void TE_MOUSE_POS_FUNC( GLFWwindow *window, double x, double y )
{

    TE_MOUSE_POS.x = x;
    TE_MOUSE_POS.y = TE_WINDOW_HEIGHT - y;

}

void TE_KEY_FUNC( GLFWwindow *window, int key, int scancode, int action, int mods )
{

    if( action == GLFW_PRESS )
    {

        TE_KEYMODS[mods] = true;
        TE_KEYSTATES[key] = true;
        TE_KEYPRESS[key] = true;

    }

    if( action == GLFW_REPEAT )
    {

        TE_KEYMODS[mods] = true;
        TE_KEYSTATES[key] = true;

    }

    if( action == GLFW_RELEASE )
    {

        if( key )TE_KEYSTATES[key] = false;

    }

}

void TE_RESHAPE( GLFWwindow *window, int width, int height )
{

    //Generic window reshape function.
    glViewport( 0, 0, (GLsizei)width, (GLsizei)height );
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    glOrtho( 0.0, width, 0.0, height, 1.0f, 5.0f );
    glMatrixMode( GL_MODELVIEW );

    glfwGetWindowSize( window, &TE_WINDOW_WIDTH, &TE_WINDOW_HEIGHT );

    TE_SCALEX = (float)TE_WINDOW_WIDTH/(float)TE_INIT_WINDOW_WIDTH;
    TE_SCALEY = (float)TE_WINDOW_HEIGHT/(float)TE_INIT_WINDOW_HEIGHT;

}

void TE_INIT( int width, int height )
{

    TE_INIT_WINDOW_WIDTH = width;
    TE_INIT_WINDOW_HEIGHT = height;

    ALCdevice *device = alcOpenDevice( NULL );
    ALCcontext *context = alcCreateContext( device, NULL );

    alcMakeContextCurrent( context );
    alcProcessContext( context );

    for( int i = 0; i < 256; i++ )
    {

        TE_KEYSTATES[i] = 0;
        TE_KEYPRESS[i] = false;
        TE_KEYMODS[i%3] = false;
        TE_MOUSEBUTTONS[i%12] = false;
        TE_MOUSECLICK[i%12] = false;

    }

    glEnable( GL_BLEND );
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

    glfwSetKeyCallback( glfwGetCurrentContext(), TE_KEY_FUNC );
    glfwSetWindowSizeCallback( glfwGetCurrentContext(), TE_RESHAPE );
    glfwSetCursorPosCallback( glfwGetCurrentContext(), TE_MOUSE_POS_FUNC );
    glfwSetMouseButtonCallback( glfwGetCurrentContext(), TE_MOUSE_BUTTON_FUNC );
    TE_RESHAPE( glfwGetCurrentContext(), width, height );

    glfwGetWindowSize( glfwGetCurrentContext(), &TE_WINDOW_WIDTH, &TE_WINDOW_HEIGHT );

}

void TE_CALC_FRAMES( float stime )
{

    float etime = glfwGetTime();

    if( etime-stime < 1000.0f/TE_FRAMES_PER_SECOND )
    {

        Sleep( ( 1000.0f/TE_FRAMES_PER_SECOND ) - ( etime-stime ) );

    }

    float calcetime = glfwGetTime();

    TE_CUR_SECOND += (calcetime - stime);

    TE_RUN_FRAMES_PER_SECOND = 1000.0f/(1000*calcetime-1000*stime);

    TE_DELTA_TIME = 1/TE_RUN_FRAMES_PER_SECOND;

}

void TE_RESET_KEYS()
{

    for( int i = 0; i < 256; i++ )
    {

        TE_MOUSECLICK[i%12] = false;
        TE_KEYPRESS[i] = false;

    }

}

void TE_DRAW_ARRAY( float *vertices, float *cols, int arsize )
{

    glEnableClientState( GL_COLOR_ARRAY );
    glEnableClientState( GL_VERTEX_ARRAY );

    glVertexPointer( 2, GL_FLOAT, 0, vertices );
    glColorPointer( 3, GL_FLOAT, 0, cols );

    glDrawArrays( GL_QUADS, 0, arsize );

    glDisableClientState( GL_COLOR_ARRAY );
    glDisableClientState( GL_VERTEX_ARRAY );


}

void TE_DRAW_ARRAY( float *vertices, float *cols, int arsize, GLuint tex )
{

    glEnableClientState( GL_COLOR_ARRAY );
    glEnableClientState( GL_VERTEX_ARRAY );

    glVertexPointer( 2, GL_FLOAT, 0, vertices );
    glColorPointer( 3, GL_FLOAT, 0, cols );

    glDrawArrays( GL_QUADS, 0, arsize );

    glDisableClientState( GL_COLOR_ARRAY );
    glDisableClientState( GL_VERTEX_ARRAY );


}
struct SpriteProperties
{

    int spriteCountWid;
    int spriteCountHei;

    GLuint tex;

};

struct TexCord
{

    //Mm! Popcorn! Most delicious struct.

    double cornOne;
    double cornTwo;
    double cornThree;
    double cornFour;

};

bool TE_AABB( TE_RECT *a, TE_RECT *b )
{

    float atopx, atopy;
    float btopx, btopy;

    float abotx, aboty;
    float bbotx, bboty;

    atopx = a->x+a->w;
    atopy = a->h+a->y;
    btopx = b->x+b->w;
    btopy = b->h+b->y;

    abotx = a->x;
    aboty = a->y;
    bbotx = b->x;
    bboty = b->y;

    return !( abotx >= btopx || bbotx >= atopx || aboty >= btopy || bboty >= atopy );

}

float TE_GET_RAND_FLOAT( float minimum, float maximum )
{

    float val = (( rand()%10000 )/10000.0)*(maximum-minimum);
    return val+minimum;

}

int TE_FIND_QUADRANT( int x, int y )
{

    if( x > 0 and y > 0 )
    {

        return 1;

    }else if( x < 0 and y < 0 )
    {

        return 3;

    }else if( x < 0 and y > 0 )
    {

        return 2;

    }else if( x > 0 and y < 0 )
    {

        return 4;

    }

    return 0;

}

double TE_GET_INCLIN( int refx, int refy, int x, int y )
{

    double relativeX = x-refx;
    double relativeY = y-refy;

    double ang = 0;

    relativeX = ( floor( relativeX*100 ) )/100;
    relativeY = ( floor( relativeY*100 ) )/100;

    if( relativeX >= -1 and relativeX <= 1 )
    {

        if( floor(relativeY) >= 0 )
        {

            ang = 90;

        }else if( floor(relativeY) <= 0 )
        {

            ang = 270;

        }

    }else if( relativeY >= -1 and relativeY <= 1 )
    {

        if( floor(relativeX) >= 0 )
        {

            ang = 0;

        }else if( floor(relativeX) <= 0 )
        {

            ang = 180;

        }

    }else if( TE_FIND_QUADRANT( relativeX, relativeY ) == 1 )
    {

        ang = atan( (relativeY)/(relativeX) )*(180/3.141);

    }else if( TE_FIND_QUADRANT( relativeX, relativeY ) == 2 )
    {

       ang = 270+(-(90-(atan( (relativeY)/(relativeX) )*(180/3.141))));

    }else if( TE_FIND_QUADRANT( relativeX, relativeY ) == 3 )
    {

        ang = (atan( (relativeY)/(relativeX) )*(180/3.141))+180;

    }else if( TE_FIND_QUADRANT( relativeX, relativeY ) == 4 )
    {

        ang = -(90-(atan( (relativeY)/(relativeX) )*(180/3.141)))+450;

    }

    return ang;

}

TE_COORD TE_INFRONT_COORDS( double ang, float speed )
{

    TE_COORD TempCord;
    TempCord.x = cos( ang*(pi/180.0) )*speed;
    TempCord.y = sin( ang*(pi/180.0) )*speed;

    return TempCord;

}

class TE_SPRITE
{

    public:

        void LoadThruFunc( const char *filename, int width, int height, int type );

        void Create( GLuint tex, int spriteCountWid, int spriteCountHei );

        void Draw( int spriteNumber, float x, float y, float wid, float hei, float ang );
        void DrawFlipped( int spriteNumber, float x, float y, float wid, float hei, float ang );

    private:

        TexCord GrabTexCord( int spriteNumber );

        SpriteProperties props;

};

void TE_SPRITE::LoadThruFunc( const char *filename, int width, int height, int type )
{

    props.tex = LoadTexture( filename, width, height, type, type );

}

void TE_SPRITE::Create( GLuint tex, int spriteCountWid, int spriteCountHei )
{

    props.spriteCountHei = spriteCountHei;
    props.spriteCountWid = spriteCountWid;

    props.tex = tex;

}

TexCord TE_SPRITE::GrabTexCord( int spriteNumber )
{

    //Actual code for grabbing the corners of the texture.

    double yAmount = spriteNumber / props.spriteCountWid;
    double xAmount = spriteNumber - props.spriteCountHei * yAmount;

    TexCord temp;

    temp.cornOne = ( 1.0/props.spriteCountWid ) * xAmount;
    temp.cornTwo = ( 1.0/props.spriteCountHei ) * yAmount;

    temp.cornThree = ( ( 1.0/props.spriteCountWid ) * xAmount ) + ( 1.0/props.spriteCountWid );
    temp.cornFour = ( ( 1.0/props.spriteCountHei ) * yAmount ) + ( 1.0/props.spriteCountHei );

    return temp;

}

//Drawing 'o course!

void TE_SPRITE::Draw( int spriteNumber, float x, float y, float wid, float hei, float ang )
{

    glPushMatrix();

        glTranslatef( x, y, 0.0f );
        glRotatef( ang, 0.0f, 0.0f, 1.0f );

        glEnable( GL_TEXTURE_2D );
        glBindTexture( GL_TEXTURE_2D, props.tex );

        TexCord temp;

        temp = GrabTexCord( spriteNumber );

        double texVerts[] = {
            temp.cornOne, temp.cornFour,
            temp.cornThree, temp.cornFour,
            temp.cornThree, temp.cornTwo,
            temp.cornOne, temp.cornTwo
        };

        double verts[] = {
            -wid/2.0, -hei/2.0,
            wid/2.0, -hei/2.0,
            wid/2.0, hei/2.0,
            -wid/2.0, hei/2.0
        };

        glEnableClientState( GL_VERTEX_ARRAY );
        glEnableClientState( GL_TEXTURE_COORD_ARRAY );

        glVertexPointer( 2, GL_DOUBLE, 0, verts );
        glTexCoordPointer( 2, GL_DOUBLE, 0, texVerts );

        glDrawArrays( GL_TRIANGLE_FAN, 0, 4 );

        glDisableClientState( GL_VERTEX_ARRAY );
        glDisableClientState( GL_TEXTURE_COORD_ARRAY );

        glDisable( GL_TEXTURE_2D );

    glPopMatrix();

}
void TE_SPRITE::DrawFlipped( int spriteNumber, float x, float y, float wid, float hei, float ang )
{

    glPushMatrix();

        glTranslatef( x, y, 0.0f );
        glRotatef( ang, 0.0f, 0.0f, 1.0f );

        glEnable( GL_TEXTURE_2D );
        glBindTexture( GL_TEXTURE_2D, props.tex );

        TexCord temp;

        temp = GrabTexCord( spriteNumber );

        double texVerts[] = {
            temp.cornOne, temp.cornFour,
            temp.cornThree, temp.cornFour,
            temp.cornThree, temp.cornTwo,
            temp.cornOne, temp.cornTwo
        };

        double verts[] = {
            wid/2.0, -hei/2.0,
            -wid/2.0, -hei/2.0,
            -wid/2.0, hei/2.0,
            wid/2.0, hei/2.0
        };

        glEnableClientState( GL_VERTEX_ARRAY );
        glEnableClientState( GL_TEXTURE_COORD_ARRAY );

        glVertexPointer( 2, GL_DOUBLE, 0, verts );
        glTexCoordPointer( 2, GL_DOUBLE, 0, texVerts );

        glDrawArrays( GL_TRIANGLE_FAN, 0, 4 );

        glDisableClientState( GL_VERTEX_ARRAY );
        glDisableClientState( GL_TEXTURE_COORD_ARRAY );

        glDisable( GL_TEXTURE_2D );

    glPopMatrix();

}
//Class for creating and using sounds.
class TE_SOUND
{

    public:
        TE_SOUND( const char *filename );
        void initSound();

        void Play( int volume );
        void Play( int volume, bool loop );

        void Pause();
        void Resume();

        void Stop();
        void Delete();

    private:
        const char *soundpath;
        int volume;

        bool isOkay;

        vector< ALuint > sourceArray;

        TE_OGG oggdat;
        ALuint buffer;

        int position;

};

TE_SOUND::TE_SOUND( const char *filename )
{

    soundpath = filename;
    position = 0;
    isOkay = false;

}

void TE_SOUND::initSound()
{

    isOkay = oggdat.LOAD_OGG( soundpath );

    if( isOkay )
    {

        //Dealing with loading in the OGG using stb_vorbis.

        int ret = sizeof( oggdat.data );

        sourceArray.push_back( ALuint() );

        //Data from earlier is loaded through a seperate function. The actual data is put together here.
        ret = stb_vorbis_stream_length_in_samples( oggdat.oggdat ) * oggdat.ogginfo.channels * 2;

        alGenBuffers( 1, &buffer );
        alGenSources( 1, &sourceArray.back() );
        alListener3f( AL_POSITION, 0.0f, 0.0f, 0.0f );

        alBufferData( buffer,
                      TE_OGG_GET_FORMAT( oggdat ),
                      oggdat.data,
                      ret,
                      oggdat.ogginfo.sample_rate );

        alSourcei( sourceArray.back(), AL_BUFFER, buffer );

    }

}

bool TE_CHECK_PLAYING( ALuint a )
{

    ALenum state;
    alGetSourcei( a, AL_SOURCE_STATE, &state );
    return (state == AL_PLAYING);

}

void TE_SOUND::Play( int volume )
{

    //Used for deleting excess sounds in the sound queue. I always want at least one source, therefore the first is preserved.
    for( unsigned i = 1; i < sourceArray.size(); i++ )
    {

        if( !TE_CHECK_PLAYING( sourceArray[i] ) )
        {

            alDeleteSources( 1, &sourceArray[i-1] );
            sourceArray.erase( sourceArray.begin()+i-1 );

        }

    }

    //Is okay as in is loaded properly, is a valid file so on. If it plays and isn't valid, it will crash.
    if( isOkay )
    {

        for( unsigned i = 0; i < sourceArray.size(); i++ )
        {

            if( !TE_CHECK_PLAYING( sourceArray[i] ) )
            {

                alSourcePlay( sourceArray[i] );

                break;

            }else
            {

                //If the first sound of the array is playing, then add a new one and play it. Once it's done, it will get deleted later.
                sourceArray.push_back( ALuint() );
                alGenSources( 1, &sourceArray.back() );
                alSourcei( sourceArray.back(), AL_BUFFER, buffer );
                alSourcePlay( sourceArray.back() );

                break;

            }

        }

    }

}

void TE_SOUND::Play( int volume, bool loop )
{
    if( isOkay )
    {

        float vol = volume/100.0f;

        alSourcef( sourceArray[0], AL_GAIN, vol );
        alSourcei( sourceArray[0], AL_LOOPING, loop );
        alSourcePlay( sourceArray[0] );

    }
}

void TE_SOUND::Pause()
{



}

void TE_SOUND::Resume()
{



}

void TE_SOUND::Stop()
{

    for( unsigned i = 0; i < sourceArray.size(); i++ )
    {

        alSourceStop( sourceArray[i] );

    }

}

//New class for creating and using buttons.

#define TE_BUTTON_CENTERX 1
#define TE_BUTTON_CENTERY 2

#define TE_BUTTON_SCALEX 3
#define TE_BUTTON_SCALEY 4

#define TE_BUTTON_KEEPXPOS 5
#define TE_BUTTON_KEEPYPOS 6

class TE_BUTTON
{

    public:
        void Create( int xPos, int yPos, int xWid, int yWid, int xAm, int yAm, const char *filename );
        void Repos( int xPos, int yPos, int xWid, int yWid );
        bool MouseIsOver();
        bool IsClicked();
        void Draw( int spritenum );

        void SetAttrib( int a );

    private:
        bool darken;
        TE_SPRITE buttonSheet;
        int xpos, ypos;
        int xwid, ywid;

        bool IsClick;

        bool CENTERX, CENTERY;
        bool SCALEX, SCALEY;
        bool KEEPX, KEEPY;

};

void TE_BUTTON::SetAttrib( int a )
{

    switch( a )
    {

        case TE_BUTTON_CENTERX:
            {

                CENTERX = true;
                break;

            }
        case TE_BUTTON_CENTERY:
            {


                CENTERY = true;
                break;

            }
        case TE_BUTTON_SCALEX:
            {

                SCALEX = true;
                break;

            }
        case TE_BUTTON_SCALEY:
            {

                 SCALEY = true;
                 break;

            }
        case TE_BUTTON_KEEPXPOS:
            {

                KEEPX = true;
                break;

            }
        case TE_BUTTON_KEEPYPOS:
            {

                KEEPY = true;
                break;

            }

    }

}

void TE_BUTTON::Create( int xPos, int yPos, int xWid, int yWid, int xAm, int yAm, const char *filename )
{

    xpos = xPos;
    ypos = yPos;

    xwid = xWid;
    ywid = yWid;

    buttonSheet.Create( 0, xAm, yAm );
    buttonSheet.LoadThruFunc( filename, 1024, 1024, GL_LINEAR );

}

void TE_BUTTON::Repos( int xPos, int yPos, int xWid, int yWid )
{

    xpos = xPos;
    ypos = yPos;

    xwid = xWid;
    ywid = yWid;


}

bool TE_BUTTON::MouseIsOver()
{

    IsClick = false;
    float tempxwid, tempywid;
    float tempx, tempy;

    tempx = xpos;
    tempy = ypos;

    if( SCALEX ) tempxwid = xwid*TE_SCALEX;
    else tempxwid = xwid;

    if( SCALEY ) tempywid = ywid*TE_SCALEY;
    else tempywid = ywid;

    if( CENTERX ) tempx = (TE_WINDOW_WIDTH - tempxwid)/2;
    if( CENTERY ) tempy = (TE_WINDOW_HEIGHT - tempywid)/2;

    if( ( TE_MOUSE_POS.x >= tempx && TE_MOUSE_POS.x <= tempx+tempxwid ) && ( TE_MOUSE_POS.y >= tempy && TE_MOUSE_POS.y <= tempy+tempywid ) )
    {

        darken = true;

    }else
    {

        darken = false;

    }

    return false;

}

void TE_BUTTON::Draw( int spritenum )
{

    float tempxwid, tempywid;

    float tempx, tempy;
    tempx = xpos;
    tempy = ypos;

    if( SCALEX ) tempxwid = xwid*TE_SCALEX;
    else if( !SCALEX ) tempxwid = xwid;

    if( SCALEY ) tempywid = ywid*TE_SCALEY;
    else if( !SCALEY ) tempywid = ywid;

    if( CENTERX ) tempx = (TE_WINDOW_WIDTH - tempxwid)/2;
    if( CENTERY ) tempy = (TE_WINDOW_HEIGHT - tempywid)/2;

    if( darken )
    {

        if( TE_MOUSEBUTTONS[ GLFW_MOUSE_BUTTON_LEFT ] )
        {

            glColor3f( 0.5f, 0.5f, 0.5f );
            buttonSheet.Draw( spritenum, (float)tempx+tempxwid/2, (float)tempy+tempywid/2, (float)tempxwid, (float)tempywid, 0.0f );
            glColor3f( 1.0f, 1.0f, 1.0f );
            IsClick = true;

        }else
        {

            glColor3f( 0.7f, 0.7f, 0.7f );
            buttonSheet.Draw( spritenum, (float)tempx+tempxwid/2, (float)tempy+tempywid/2, (float)tempxwid, (float)tempywid, 0.0f );
            glColor3f( 1.0f, 1.0f, 1.0f );

        }

    }else
    {
        buttonSheet.Draw( spritenum, (float)tempx+tempxwid/2, (float)tempy+tempywid/2, (float)tempxwid, (float)tempywid, 0.0f );
    }

}

bool TE_BUTTON::IsClicked()
{

    return IsClick;

}

#endif // TE_H_INCLUDED
