#define GLM_SWIZZLE_XYZW

#include "tpglwindow.h"

#include <QDebug>
#include <QTimer>

#define CAMERA_SENSITIVITY 5



static const float fCameraZInit = 2.5f;

//====================================================================================================================================
TPGLWindow::TPGLWindow() :
      m_mtxCameraView       (0)
    , m_mtxCameraProjection (0)
    , m_iUniformView        (0)
    , m_iUniformProjection  (0)
    , m_iUniformSampler     (0)
    , m_vCameraCenter (0,0,-1)
    , m_bAlphaBlend         ( false )
    , m_particlesRenderers   (0)
    , m_mouseBehaviour      ( NONE )
    , m_ceiling             ()
    , m_floor               ()
{
    m_vCameraPosition   = glm::vec3(0,0,fCameraZInit);

    m_timer.start();

    QTimer * timer = new QTimer();
        QObject::connect(timer, SIGNAL(timeout()), this, SLOT(update()));
        timer->start( 10 );

        setMouseTracking(true);
}

//====================================================================================================================================
TPGLWindow::~TPGLWindow()
{
    destroyGPUProgramAndShaders();
}


//====================================================================================================================================
void TPGLWindow::destroyGPUProgramAndShaders()
{
    m_GPUProgramParticles.destroy();
    m_GPUProgramObjects.destroy();
}

//====================================================================================================================================
void TPGLWindow::getUniformLocations()
{

    m_iUniformView              = m_GPUProgramParticles.getUniformLocation("u_mtxView");
    m_iUniformProjection        = m_GPUProgramParticles.getUniformLocation("u_mtxProjection");
    m_iUniformSampler           = m_GPUProgramParticles.getUniformLocation("u_tex");


    m_iUniformViewObj              = m_GPUProgramObjects.getUniformLocation("u_mtxView");
    m_iUniformProjectionObj        = m_GPUProgramObjects.getUniformLocation("u_mtxProjection");
    m_iUniformSamplerObj           = m_GPUProgramObjects.getUniformLocation("u_tex");
}


//====================================================================================================================================
void TPGLWindow::sendParticlesUniformsToGPU()
{
    glUniformMatrix4fv(m_iUniformView, 1, GL_FALSE, glm::value_ptr(m_mtxCameraView) );
    glUniformMatrix4fv(m_iUniformProjection, 1, GL_FALSE, glm::value_ptr(m_mtxCameraProjection) );
    glUniform1i( m_iUniformSampler , 0 );
}

void TPGLWindow::sendObjectsUniformsToGPU()
{
    glUniformMatrix4fv(m_iUniformViewObj, 1, GL_FALSE, glm::value_ptr(m_mtxCameraView) );
    glUniformMatrix4fv(m_iUniformProjectionObj, 1, GL_FALSE, glm::value_ptr(m_mtxCameraProjection) );
    glUniform1i( m_iUniformSamplerObj , 0 );
}


//====================================================================================================================================
void TPGLWindow::initializeGL()
{
    initializeOpenGLFunctions();

    // Prints the GL Version
    TP_LOG("GL Version : %s\n",(char*)glGetString(GL_VERSION));

    m_GPUProgramParticles.createFromFiles("VS.glsl", "GS.glsl", "FS.glsl");
    m_GPUProgramObjects.createFromFiles("VS_obj.glsl", "", "FS_obj.glsl");

    m_ceiling.init("./data/nico.png", 1.f);
    m_floor.init("./data/nico.png", -1.f);

    getUniformLocations();
}

//====================================================================================================================================
void TPGLWindow::updateMatrices()
{
    const glm::vec3 vRight  (1,0,0);
    const glm::vec3 vUp     (0,1,0);
    const glm::vec3 vFront  (0,0,-1);


    // Projection matrix is "simply" a perspective matrix, so use the right tool function from GLM
    m_mtxCameraProjection       = glm::perspective(glm::radians(45.0f), (float)width()/height(), 0.1f, 100.f);


    // View matrix is built from the common "lookAt" paradigm, using the RH (Right-Handed) coordinate system
    m_mtxCameraView             = glm::lookAtRH( m_vCameraPosition, m_vCameraCenter, vUp );

}

//====================================================================================================================================
void TPGLWindow::update()
{

    float   fTimeElapsed        = (float) m_timer.restart();

    for(unsigned int i=0; i<m_particlesRenderers.size(); i++)
        m_particlesRenderers[i]->update(fTimeElapsed);

    updateMatrices();

    QOpenGLWidget::update();
}

void TPGLWindow::setJsonData(const QString &json)
{
    m_particleConfig = ParticleConfiguration(json);
    if(m_particlesRenderers.empty())
        setNumberEmetters(1);
    else
        setNumberEmetters(m_particlesRenderers.size());
}

void TPGLWindow::setMouseBehabiour(TPGLWindow::MouseBehaviour behaviour)
{
    m_mouseBehaviour = behaviour;
    m_vCameraCenter = glm::vec3(0,0,-1);
    m_vCameraPosition   = glm::vec3(0,0,fCameraZInit);
    if(m_particlesRenderers.size()> 0) {
        setNumberEmetters(m_particlesRenderers.size());
    }
    updateMatrices();
}

void TPGLWindow::setNumberEmetters(int n)
{
    makeCurrent();

    for(unsigned int i=0; i<m_particlesRenderers.size(); i++)
        delete m_particlesRenderers[i];
    m_particlesRenderers.clear();

    for(int i=0; i<n; i++)
        m_particlesRenderers.push_back(new ParticlesRenderer(m_particleConfig));

    switch(n) {
        case 1:
            m_particlesRenderers[0]->getEmetter()->setOrigin(glm::vec3(0,0,0));
        break;
        case 2:
            m_particlesRenderers[0]->getEmetter()->setOrigin(glm::vec3(-0.33,0,0));
            m_particlesRenderers[1]->getEmetter()->setOrigin(glm::vec3(0.33,0,0));
        break;
        case 3:
            m_particlesRenderers[0]->getEmetter()->setOrigin(glm::vec3(-0.33,0,-0.2));
            m_particlesRenderers[1]->getEmetter()->setOrigin(glm::vec3(0.33,0,-0.2));
            m_particlesRenderers[2]->getEmetter()->setOrigin(glm::vec3(0,0,0.2));
        break;
        default:
            m_particlesRenderers[0]->getEmetter()->setOrigin(glm::vec3(-0.33,0,0));
            m_particlesRenderers[1]->getEmetter()->setOrigin(glm::vec3(0,0,-0.33));
            m_particlesRenderers[2]->getEmetter()->setOrigin(glm::vec3(0,0,0.33));
            m_particlesRenderers[3]->getEmetter()->setOrigin(glm::vec3(0.33,0,0));
        break;
    }
}

//====================================================================================================================================
void TPGLWindow::paintGL()
{


    // Specifies the viewport size -----------------------------------------------------------------
    //const float retinaScale = devicePixelRatio();
   //glViewport( 0, 0, 800 * retinaScale, 400 * retinaScale );

    // Specify winding order Counter ClockZise (even though it's default on OpenGL) ----------------
    glFrontFace( GL_CCW );

    // Enables (Back) Face Culling -----------------------------------------------------------------
    //glEnable( GL_CULL_FACE );
    //glCullFace( GL_BACK );


    if( m_bAlphaBlend )
    {
        // Disables Depth Test --------------------------------------------------------------------------
        glDisable( GL_DEPTH_TEST );

        // Enables Depth Write -------------------------------------------------------------------------
        glDepthMask( GL_FALSE );

        // Enables Alpha Blending ----------------------------------------------------------------------
        glEnable( GL_BLEND );
    //    glBlendFunc( GL_ONE, GL_ONE_MINUS_SRC_ALPHA );
        glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
    }
    else
    {
        // Disables Depth Test --------------------------------------------------------------------------
        glDisable( GL_DEPTH_TEST );

        // Enables Depth Write -------------------------------------------------------------------------
        glDepthMask( GL_FALSE );

        // Enables Alpha Blending ----------------------------------------------------------------------
        glEnable( GL_BLEND );
        glBlendFunc( GL_SRC_ALPHA, GL_ONE );
    //    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
    }

    // Specify the color to use when clearing theframebuffer --------------------------------------
    glClearColor( 0.2f, 0.1f, 0.2f, 0.0f );

    // Clears the framebuffer ---------------------------------------------------------------------
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT );


    // Starts using the given GPU Program ---------------------------------------------------------
    m_GPUProgramObjects.bind();
    {
        // Sends the uniforms var from the CPU to the GPU -----------------------------------------
        sendObjectsUniformsToGPU();

        m_ceiling.render();
        m_floor.render();

    }
    // Stops using the GPU Program ----------------------------------------------------------------
    m_GPUProgramObjects.unbind();

    // Starts using the given GPU Program ---------------------------------------------------------
    m_GPUProgramParticles.bind();
    {
        // Sends the uniforms var from the CPU to the GPU -----------------------------------------
        sendParticlesUniformsToGPU();

        for(unsigned int i=0; i<m_particlesRenderers.size(); i++)
            m_particlesRenderers[i]->render();

    }
    // Stops using the GPU Program ----------------------------------------------------------------
    m_GPUProgramParticles.unbind();


}

void TPGLWindow::mousePressEvent(QMouseEvent *) {

}

void TPGLWindow::mouseReleaseEvent(QMouseEvent *) {

}

//====================================================================================================================================
void TPGLWindow::resizeGL(int, int)
{
    // Force the update of the perspective matrix
    updateMatrices();
}

void TPGLWindow::mouseMoveEvent(QMouseEvent * event) {
    float x = 2 * fCameraZInit * event->x() / this->width() - fCameraZInit;
    float y = 2 * fCameraZInit * (this->height() - event->y()) / this->height() - fCameraZInit;

    glm::vec4 test(0,0,0,1);
    glm::vec4 projected = m_mtxCameraProjection * m_mtxCameraView * test;
    qDebug() << "projected " << projected.x << " " << projected.y << " " << projected.z << " " << projected.w;

    glm::vec4 screenVec(x,y, fCameraZInit -  0.19019, fCameraZInit);
    glm::vec3 worldVec(glm::inverse(m_mtxCameraProjection * m_mtxCameraView) * screenVec);

    for(unsigned int i=0; i<m_particlesRenderers.size(); i++)
        m_particlesRenderers[i]->getEmetter()->setUseCustomAttractPoint(false);
    switch(m_mouseBehaviour) {
    case MOVE_EMETTER:
            for(unsigned int i=0; i<m_particlesRenderers.size(); i++)
                m_particlesRenderers[i]->getEmetter()->setOrigin(worldVec);
        break;
    case MOVE_ATTRACT_POINT:
        for(unsigned int i=0; i<m_particlesRenderers.size(); i++) {
            m_particlesRenderers[i]->getEmetter()->setUseCustomAttractPoint(true);
            m_particlesRenderers[i]->getEmetter()->setCustomAttractPoint(worldVec);
        }
        break;
    default://nothing to do
        break;
    }

    if(m_mouseBehaviour == MOVE_VIEW) {
        m_vCameraCenter = worldVec;
        updateMatrices();
    }


}

//====================================================================================================================================
void TPGLWindow::keyPressEvent(QKeyEvent* _pEvent)
{
    if(QEvent::KeyPress == _pEvent->type() )
    {
//        float   fTimeElapsed= (float) m_timer.elapsed();

        float   fMoveSpeed          = 0.07f;// * fTimeElapsed;

        glm::vec3 translation = glm::normalize(m_vCameraCenter - m_vCameraPosition);

        glm::mat4 rotation = glm::rotate(glm::mat4(1.f), 90.0f, glm::vec3(0.0, 1.0, 0.0));

        glm::vec3 test = glm::vec3(rotation * glm::vec4(translation, 1.f));
        qDebug() << test.x << " " << test.y << " " << test.z;
        // handle key press to move the 3D object
        switch( _pEvent->key() )
        {
        case Qt::Key_B:
            m_bAlphaBlend = !m_bAlphaBlend;
            break;

        case Qt::Key_Z:
            m_vCameraPosition += fMoveSpeed * glm::normalize(m_vCameraCenter - m_vCameraPosition);
            break;

        case Qt::Key_S:
            m_vCameraPosition -= fMoveSpeed * glm::normalize(m_vCameraCenter - m_vCameraPosition);
            break;

        case Qt::Key_Q:
            m_vCameraPosition += fMoveSpeed * test;
            m_vCameraCenter += fMoveSpeed * test;
            break;

        case Qt::Key_D:
            m_vCameraPosition -= fMoveSpeed * test;
            m_vCameraCenter -= fMoveSpeed * test;
            break;

        case Qt::Key_Escape:
            this->close();
            break;

        }
    }
}

//====================================================================================================================================
bool TPGLWindow::printShaderCompileInfo(GLuint _iShaderID, const std::string& _strMsg)
{
    int iInfologLength = 0;
    int iCharsWritten  = 0;
    char* strInfoLog;

    glGetShaderiv(_iShaderID, GL_INFO_LOG_LENGTH, & iInfologLength);

    if( iInfologLength > 1 )
    {
        strInfoLog = new char[ iInfologLength + 1 ];
        glGetShaderInfoLog(_iShaderID, iInfologLength, & iCharsWritten, strInfoLog);

        std::cerr << "--------------"<< std::endl << "compilation de " << _strMsg <<  " : "<<std::endl << strInfoLog <<std::endl<< "--------------"<< std::endl;

        delete [] strInfoLog;
    }

    return ( 1 == iInfologLength );
}

