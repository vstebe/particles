#ifndef __MAIN_H__
#define __MAIN_H__

#include "openglwindow.h"
#include "emetter.h"
#include "particleconfiguration.h"
#include "particlesrenderer.h"

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <assert.h>
#include <vector>

#include <glm/vec3.hpp>                 // glm::vec3
#include <glm/vec2.hpp>                 // glm::vec4
#include <glm/vec4.hpp>                 // glm::vec4
#include <glm/mat4x4.hpp>               // glm::mat4
#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective, glm::lookAtRH
#include <glm/gtc/type_ptr.hpp>         // glm::value_ptr
#include <glm/gtc/matrix_inverse.hpp>


#include <QTime>
#include <QtGui/QGuiApplication>
#include <QtGui/QScreen>
#include <QtGui/QKeyEvent>
#include <QVector>

//====================================================================================================================================

class TPGLWindow : public OpenGLWindow
{
    Q_OBJECT
public:
    enum MouseBehaviour {NONE, MOVE_EMETTER, MOVE_ATTRACT_POINT};

    TPGLWindow();
    virtual ~TPGLWindow();

    void initialize();

    void render();

    void update();


public slots:
    void setJsonData(const QString& json);
    void setMouseBehabiour(MouseBehaviour behaviour);
    void setNumberEmetters(int n);

private:

    /// Creates the GPU VertexShader
    void createGPUVertexShader();

    /// Creates the GPU FragmentShader
    void createGPUFragmentShader();

    /// Creates the GPU Program from the GPU VertexShader and GPU FragmentShader
    void createGPUProgramFromGPUShaders();

    /// Destroys GPU Program and Shaders
    void destroyGPUProgramAndShaders();

    /// Print the shader compiler information
    bool printShaderCompileInfo( GLuint _iShaderID, const std::string& _strMsg );

    //------------------------------------------------------------------------------------
    /// Get the GLSL uniform locations
    void getUniformLocations();

    /// Get the GLSL vertex attributes locations
    void getAttributeLocations();

    /// Send the uniform vars from the CPU to the GPU
    void sendUniformsToGPU();

     //------------------------------------------------------------------------------------
    /// Creates the VertexBufferObject that contains the geometry position
    void createVBO();

    /// Creates the VertexArrayObject that contains the geometry info
    void createVAOFromVBO();

    /// Destroys the VertexArrayObject created earlier
    void destroyVAOAndVBO();

    //------------------------------------------------------------------------------------
    /// Update the CameraProjection, CameraView, ObjectWorld matrices
    void updateMatrices();

    /// Handles key press event, and move the 3D object
    void keyPressEvent(QKeyEvent *);

    void mouseMoveEvent(QMouseEvent *);

    void mousePressEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);


    /// Handles resize event
    void resizeEvent(QResizeEvent*);

    //------------------------------------------------------------------------------------
    /// Creates the Texture object from a texture file
    void createTextures();

    /// Destroys all Texture objects
    void destroyTextures();

    /// Bind the textures to their texture unit
    void setupTexturesInUnit();

    //------------------------------------------------------------------------------------
    /// Creates the GPU GeometryShader
    void createGPUGeometryShader();





    GLuint      			m_iGPUProgramID;            ///< OpenGL ID for GPU Program
    GLuint      			m_iGPUVertexShaderID;       ///< OpenGL ID for GPU VertexShader
    GLuint      			m_iGPUFragmentShaderID;     ///< OpenGL ID for GPU FragmentShader
    //------------------------------------------------------------------------------------
    GLuint      			m_iVertexCount;             ///< Number of vertices in the VAO
    GLuint      			m_iTrianglesCount;          ///< Number of triangles to draw
    GLuint      			m_iElementsCount;           ///< Number of indices used for drawing
    GLuint      			m_iVAO;                     ///< VertexArrayObject used for drawing

    GLuint      			m_iAttribPosition;          ///< GLSL attribute location for vertex position
    GLuint      			m_iAttribColor;            ///< GLSL attribute location for vertex colour
    GLuint      			m_iVBOPosition;             ///< VertexBufferObject used to store position vertex data, used by the VAO
    GLuint      			m_iVBOColor;               ///< VertexBufferObject used to store position vertex colour, used by the VAO

    //------------------------------------------------------------------------------------
    glm::vec3   			m_vObjectTranslate;         ///< Store the 3D object translate component
    glm::vec3   			m_vObjectEulerAngles;       ///< Store the 3D object orientation as euler angles

    glm::mat4   			m_mtxObjectWorld;           ///< Matrix transform, for object -> world coordinates
    glm::mat4   			m_mtxCameraView;            ///< Matrix transform, for world -> camera view coordinates
    glm::mat4   			m_mtxCameraProjection;      ///< Matrix transform, for camera view -> camera projection coordinates

    GLuint      			m_iUniformWorld;            ///< GLSL uniform location for World matrix
    GLuint      			m_iUniformView;             ///< GLSL uniform location for View matrix
    GLuint      			m_iUniformProjection;       ///< GLSL uniform location for Projection matrix

    //------------------------------------------------------------------------------------
    QTime       			m_timer;                    ///< Time used to get elapsed time between 2 frames

    glm::vec3               m_vCameraPosition;          ///< Camera position - in World Space

    //------------------------------------------------------------------------------------

    GLuint                  m_iTexture;                 ///< TextureObject id, for diffuse texture
    GLuint                  m_iUniformSampler;      ///< GLSL uniform location for the sampler "u_tex"

    //------------------------------------------------------------------------------------
    GLuint      			m_iGPUGeometryShaderID;     ///< OpenGL ID for GPU GeometryShader

    bool                    m_bAlphaBlend;              ///< Use Alpha Blending ?


    std::vector<ParticlesRenderer*>     m_particlesRenderers;

    MouseBehaviour          m_mouseBehaviour;

    ParticleConfiguration   m_particleConfig;


};

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

//------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------




//====================================================================================================================================

//class GPUProgram
//{
//};

//struct Geometry
//{
//};

//struct Model
//{
//    struct SubModel
//    {


//    };


//    std::vector<SubModel> m_aSubModels;
//};


#endif // __MAIN_H__

