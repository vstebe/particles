#ifndef __GPUPROGRAM_H__
#define __GPUPROGRAM_H__


#include <QOpenGLFunctions_3_3_Core>

#include "utils.h"



class GPUProgram : protected QOpenGLFunctions_3_3_Core
{
public:
    /// CTOR
    GPUProgram();
    /// DTOR
    virtual ~GPUProgram();

    /// Returns the GLSL id of this GPU Program
    inline GLuint   getID() const;

    /// Returns the GLSL location of the uniform var with the given name
    inline GLuint   getUniformLocation( const char* _strName );

    /// Starts using this GPU Program
    void            bind();

    /// Stops using this GPU Program
    void            unbind();

    /// Creates a GPU Program from the given shader files
    bool            createFromFiles( const std::string& _rstrVertexShaderPath, const std::string& _rstrGeometryShaderPath , const std::string& _rstrFragmentShaderPath );

    /// Destroys this GPU Program
    void            destroy();

private:

    /// Creates the shader of given type, from the given file path, and returns its GLSL id
    GLuint          createShader( const std::string& _rstrShaderPath, GLenum _eShaderType );

    /// Destroys the given shader
    void            destroyShader( GLuint& _rOutShaderID );

    /// Prints the shader compiler information
    bool            printShaderCompileInfo( GLuint _iShaderID, const std::string& _strMsg );


    GLuint      m_iGPUProgramID;            ///< OpenGL ID for GPU Program
    GLuint      m_iGPUVertexShaderID;       ///< OpenGL ID for GPU VertexShader
    GLuint      m_iGPUGeometryShaderID;     ///< OpenGL ID for GPU GeometryShader - optional
    GLuint      m_iGPUFragmentShaderID;     ///< OpenGL ID for GPU FragmentShader
};

//====================================================================================================================================
GLuint GPUProgram::getID() const
{
    return m_iGPUProgramID ;
}

//====================================================================================================================================
GLuint GPUProgram::getUniformLocation( const char* _strName )
{
    //--------------------------------------------------------------------------------------------------------------------
    // precondition
    TP_ASSERT( 0 != m_iGPUProgramID, "m_iGPUProgramID should not be 0 (here it is '%d') - Did you call createFromFiles() on it ?.\n", m_iGPUProgramID );
    //--------------------------------------------------------------------------------------------------------------------

    return glGetUniformLocation( m_iGPUProgramID, _strName ); ;
}


#endif // __GPUPROGRAM_H__

