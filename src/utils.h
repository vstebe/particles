#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <assert.h>

//------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------
// ERROR & DEBUG related

#define DEBUG

// DEBUG_BREAK
#ifdef  WIN32
#   define  TP_DEBUG_BREAK()    __debugbreak()
#else
#   define  TP_DEBUG_BREAK()    asm("int $3")
#endif


// LOG
#define TP_LOG( _message, ...)  fprintf( stdout, (_message), __VA_ARGS__ );fflush( stdout )

// ASSERT
#ifdef DEBUG
#   define TP_ASSERT(_assert, ...)   \
        if( !(_assert) ) {  \
            TP_LOG( "Assertion failed !\n.FILE = '%s'\n.LINE='%d'\n.FUNCTION='%s'\n", __FILE__ , __LINE__, __FUNCTION__);\
            TP_LOG( __VA_ARGS__ );\
            TP_DEBUG_BREAK();\
            assert(_assert); \
        }
#else
#   define TP_ASSERT(_assert, ...)
#endif

//====================================================================================================================================
std::string readFileSrc(const std::string& _rstrFilePath);
//------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------
// DATA related

#define TP_PATH_TO_DATA "data/"

//------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------

#endif // UTILS_H
