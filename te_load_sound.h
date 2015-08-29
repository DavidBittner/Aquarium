#ifndef TE_LOAD_SOUND_H_INCLUDED
#define TE_LOAD_SOUND_H_INCLUDED

#include <stb_ogg.h>
#include <vector>
#include <AL/al.h>

struct TE_OGG
{

    bool LOAD_OGG( const char *filename );

    stb_vorbis_alloc oggdatalloc;
    stb_vorbis *oggdat;
    stb_vorbis_info ogginfo;

    short *data;
    int filelength;

};

bool TE_OGG::LOAD_OGG( const char *filename )
{

    int geter;

    oggdat = stb_vorbis_open_filename( filename, &geter, &oggdatalloc );
    if( geter == VORBIS_file_open_failure ){ cout << "Failed to open file." << endl; return false; }
    ogginfo = stb_vorbis_get_info( oggdat );

    filelength = stb_vorbis_decode_filename( filename, &geter, &filelength, &data );

    if( filelength == 0 ) return false;

    cout << "Loaded: " << filename << " successfully." << endl;
    return true;

}

ALenum TE_OGG_GET_FORMAT( TE_OGG dat )
{

    bool stereo = ( dat.ogginfo.channels > 1 );
    if( stereo ) return AL_FORMAT_STEREO16;
    else return AL_FORMAT_MONO16;

}

#endif // TE_LOAD_SOUND_H_INCLUDED
