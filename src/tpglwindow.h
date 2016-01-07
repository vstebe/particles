#ifndef __MAIN_H__
#define __MAIN_H__

#include "emetter.h"
#include "particleconfiguration.h"
#include "particlesrenderer.h"
#include "utils.h"
#include "gpuprogram.h"
#include "bigquad.h"

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
#include <QOpenGLWidget>
#include <QVector>

//====================================================================================================================================

/**
 * @brief Represents the 3D scene in an opengl context
 */
class TPGLWindow : public QOpenGLWidget,  protected QOpenGLFunctions_3_3_Core
{
    Q_OBJECT
public:
    /**
     * @brief Action executed when the user moves the mouse
     */
    enum MouseBehaviour {NONE, MOVE_VIEW, MOVE_EMETTER, MOVE_ATTRACT_POINT};

    /**
     * @brief Constructor
     */
    TPGLWindow();
    virtual ~TPGLWindow();

    /**
     * @brief Init the openGL context and load the programs
     */
    virtual void initializeGL();

    /**
     * @brief Draw the emitters, the ceil and the floor if wanted
     */
    virtual void paintGL();

    /**
     * @brief Reset the viewport when the widget is resized
     * @param w
     */
    virtual void resizeGL(int w, int);


public slots:
    /**
     * @brief Reset the particles renderer with a new json configuration
     * @param json configuration to use
     */
    void setJsonData(const QString& json);

    /**
     * @brief Set the action to trigger when the mouse is moved
     * @param behaviour either camera move, emitter move, attract point move or nothing
     */
    void setMouseBehabiour(MouseBehaviour behaviour);

    /**
     * @brief Set the number of emitters to draw
     * @param n number of emitters
     */
    void setNumberEmetters(int n);

    /**
     * @brief Set if floor&ceil must be displayed
     * @param show true if the floor and the ceil is draw, false otherwise
     */
    void setShowFloorCeil(bool show);

    /**
     * @brief Updates the emitters
     */
    void update();

private:
    /// Destroys GPU Program and Shaders
    void destroyGPUProgramAndShaders();

    /// Print the shader compiler information
    bool printShaderCompileInfo( GLuint _iShaderID, const std::string& _strMsg );

    //------------------------------------------------------------------------------------
    /// Get the GLSL uniform locations
    void getUniformLocations();

    /// Send the uniform vars from the CPU to the GPU
    void sendParticlesUniformsToGPU();

    /// Send the uniform vars from the CPU to the GPU
    void sendObjectsUniformsToGPU();


    //------------------------------------------------------------------------------------
    /// Update the CameraProjection, CameraView, ObjectWorld matrices
    void updateMatrices();

    /// Handles key press event, and move the 3D object
    void keyPressEvent(QKeyEvent *);

    void mouseMoveEvent(QMouseEvent *);

    void mousePressEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);

    //------------------------------------------------------------------------------------
    /// Creates the GPU GeometryShader
    void createGPUGeometryShader();





    GPUProgram      			m_GPUProgramParticles;
    GPUProgram      			m_GPUProgramObjects;

    //------------------------------------------------------------------------------------
    glm::mat4   			m_mtxCameraView;            ///< Matrix transform, for world -> camera view coordinates
    glm::mat4   			m_mtxCameraProjection;      ///< Matrix transform, for camera view -> camera projection coordinates

    GLuint      			m_iUniformView;             ///< GLSL uniform location for View matrix
    GLuint      			m_iUniformProjection;       ///< GLSL uniform location for Projection matrix
    GLuint                  m_iUniformSampler;

    GLuint      			m_iUniformViewObj;             ///< GLSL uniform location for View matrix
    GLuint      			m_iUniformProjectionObj;       ///< GLSL uniform location for Projection matrix
    GLuint                  m_iUniformSamplerObj;

    //------------------------------------------------------------------------------------
    QTime       			m_timer;                    ///< Time used to get elapsed time between 2 frames

    glm::vec3               m_vCameraPosition;          ///< Camera position - in World Space
    glm::vec3               m_vCameraDirection;

    //------------------------------------------------------------------------------------

    bool                    m_bAlphaBlend;              ///< Use Alpha Blending ?

    bool                    m_bShowFloorCeil;


    std::vector<ParticlesRenderer*>     m_particlesRenderers;

    BigQuad                 m_ceiling;
    BigQuad                 m_floor;

    MouseBehaviour          m_mouseBehaviour;

    ParticleConfiguration   m_particleConfig;


};





#endif // __MAIN_H__

