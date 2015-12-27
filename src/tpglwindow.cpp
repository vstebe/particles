#define GLM_SWIZZLE_XYZW

#include "tpglwindow.h"

#include <QDebug>

#define CAMERA_SENSITIVITY 10

//====================================================================================================================================
std::string readFileSrc(const std::string& _rstrFilePath)
{
    //--------------------------------------------------------------------------------------------------------------------
    // precondition
    TP_ASSERT( _rstrFilePath.size() > 0 , "Invalid parameter _rstrFilePath(\"%s\") : size() must not be 0.\n", _rstrFilePath.c_str() );
    //-----------------------------------------------------------------------------

    TP_LOG("Opening file %s\n", _rstrFilePath.c_str() );

    std::string strContent;


    std::ifstream file(_rstrFilePath.c_str());
    TP_ASSERT( file, "Could not open file %s\n", _rstrFilePath.c_str() );

    file.seekg(0, std::ios::end);
    strContent.reserve(file.tellg());
    file.seekg(0, std::ios::beg);
    strContent.assign((std::istreambuf_iterator<char>(file)),std::istreambuf_iterator<char>());


    //-----------------------------------------------------------------------------
    // postcondition
    TP_ASSERT( strContent.size() > 0 ,  "Invalid content read strContent(\"%s\") : size() must not be 0.\n", strContent.c_str() );
    //-----------------------------------------------------------------------------

    return strContent;
}
//------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------
// DATA related

#define TP_PATH_TO_DATA "data/"

static const float fCameraZInit = 1.f;

//====================================================================================================================================
TPGLWindow::TPGLWindow()
    : m_iGPUProgramID       (0)
    , m_iGPUVertexShaderID  (0)
    , m_iGPUFragmentShaderID(0)
//    , m_iUniformcolor      (0)
    , m_iVertexCount        (0)
    , m_iTrianglesCount     (0)
    , m_iElementsCount      (0)
    , m_iVAO                (0)
    , m_iAttribPosition     (0)
    , m_iAttribColor       (0)
    , m_iVBOPosition        (0)
    , m_iVBOColor          (0)
    , m_vObjectTranslate    (0)
    , m_vObjectEulerAngles  (0)
    , m_mtxObjectWorld      (0)
    , m_mtxCameraView       (0)
    , m_mtxCameraProjection (0)
    , m_iUniformWorld       (0)
    , m_iUniformView        (0)
    , m_iUniformProjection  (0)
    , m_iTexture            (0)
    , m_iUniformSampler (0)
    , m_iGPUGeometryShaderID(0)         //--------------------------------------
    , m_bAlphaBlend         ( false )
    , m_particlesRenderers   (0)
    , m_mouseBehaviour      ( NONE )
    , m_vCameraCenter (0,0,-1)
{
    m_vCameraPosition   = glm::vec3(0,0,fCameraZInit);

    m_timer.start();

}

//====================================================================================================================================
TPGLWindow::~TPGLWindow()
{
    destroyGPUProgramAndShaders();

    destroyVAOAndVBO();

    destroyTextures();
}

//====================================================================================================================================
void TPGLWindow::createGPUVertexShader()
{
    //--------------------------------------------------------------------------------------------------------------------
    // precondition
    TP_ASSERT( 0 == m_iGPUVertexShaderID , "m_iGPUVertexShaderID should be 0 (here it is '%d') - have you already called this function ?.\n", m_iGPUVertexShaderID );
    //--------------------------------------------------------------------------------------------------------------------

    std::string strVertexShader = readFileSrc( TP_PATH_TO_DATA "VS.glsl");
    const char* strSrc = strVertexShader.c_str();

    m_iGPUVertexShaderID = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(m_iGPUVertexShaderID, 1, & strSrc, 0);
    glCompileShader(m_iGPUVertexShaderID);

    printShaderCompileInfo( m_iGPUVertexShaderID, "VS.glsl");

    //--------------------------------------------------------------------------------------------------------------------
    // postcondition
    TP_ASSERT( 0 != m_iGPUVertexShaderID ,  "m_iGPUVertexShaderID should not be 0 (here it is '%d') - Did glCreateShader(...) succeed ?.\n", m_iGPUVertexShaderID );
    //--------------------------------------------------------------------------------------------------------------------
}

//====================================================================================================================================
void TPGLWindow::createGPUGeometryShader()
{
    //--------------------------------------------------------------------------------------------------------------------
    // precondition
    TP_ASSERT( 0 == m_iGPUGeometryShaderID , "m_iGPUGeometryShaderID should be 0 (here it is '%d') - have you already called this function ?.\n", m_iGPUGeometryShaderID );
    //--------------------------------------------------------------------------------------------------------------------

    std::string strGeometryShader = readFileSrc( TP_PATH_TO_DATA "GS.glsl");
    const char* strSrc = strGeometryShader.c_str();

    m_iGPUGeometryShaderID = glCreateShader(GL_GEOMETRY_SHADER);
    glShaderSource(m_iGPUGeometryShaderID, 1, & strSrc, 0);
    glCompileShader(m_iGPUGeometryShaderID);

    printShaderCompileInfo( m_iGPUGeometryShaderID, "GS.glsl");

    //--------------------------------------------------------------------------------------------------------------------
    // postcondition
    TP_ASSERT( 0 != m_iGPUGeometryShaderID ,  "m_iGPUGeometryShaderID should not be 0 (here it is '%d') - Did glCreateShader(...) succeed ?.\n", m_iGPUGeometryShaderID );
    //--------------------------------------------------------------------------------------------------------------------
}

//====================================================================================================================================
void TPGLWindow::createGPUFragmentShader()
{
    //--------------------------------------------------------------------------------------------------------------------
    // precondition
    TP_ASSERT( 0 == m_iGPUFragmentShaderID ,    "m_iGPUFragmentShaderID should be 0 (here it is '%d') - have you already called this function ?.\n", m_iGPUFragmentShaderID );
    //--------------------------------------------------------------------------------------------------------------------

    std::string strFragmentShader = readFileSrc( TP_PATH_TO_DATA "FS.glsl");
    const char* strSrc = strFragmentShader.c_str();

    m_iGPUFragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(m_iGPUFragmentShaderID, 1, & strSrc, 0);
    glCompileShader(m_iGPUFragmentShaderID);

    printShaderCompileInfo( m_iGPUFragmentShaderID, "FS.glsl");
    //--------------------------------------------------------------------------------------------------------------------
    // postcondition
    TP_ASSERT( 0 != m_iGPUFragmentShaderID ,    "m_iGPUFragmentShaderID should not be 0 (here it is '%d') - Did glCreateShader(...) succeed ?.\n", m_iGPUFragmentShaderID );
    //--------------------------------------------------------------------------------------------------------------------
}



//====================================================================================================================================
void TPGLWindow::createGPUProgramFromGPUShaders()
{
    //--------------------------------------------------------------------------------------------------------------------
    // precondition
    TP_ASSERT( 0 == m_iGPUProgramID ,   "m_iGPUProgramID should be 0 (here it is '%d') - have you already called this function ?.\n", m_iGPUProgramID );
    //--------------------------------------------------------------------------------------------------------------------

    // TODO : attach (and later detach) the new GeommetryShader so that the GPUProgram will link with it !

    // Creates the GPU Program
    m_iGPUProgramID = glCreateProgram();

    // Attaches a Vertex and a Fragmen Shader
    glAttachShader(m_iGPUProgramID, m_iGPUVertexShaderID);
    glAttachShader(m_iGPUProgramID, m_iGPUFragmentShaderID);
    glAttachShader(m_iGPUProgramID, m_iGPUGeometryShaderID);

    // Links the GPU Program to finally make it effective
    // NB : this will "copy" the attached Shaders into the Program, kinda like static C library linking
    glLinkProgram(m_iGPUProgramID);

    // Now you can detach the Shaders !
    // NB : You can even delete the Shaders
    //      if you don't want to use them for other GPU Programs
    //      => they are not needed, since linking "copied" them into the Program !
    // NB2: on some mobile device, the driver may not behave properly,
    // and won't like to have the Shaders to be detached !... But this is out of scope.
    glDetachShader(m_iGPUProgramID, m_iGPUVertexShaderID);
    glDetachShader(m_iGPUProgramID, m_iGPUFragmentShaderID);
    glDetachShader(m_iGPUProgramID, m_iGPUGeometryShaderID);


    //--------------------------------------------------------------------------------------------------------------------
    // postcondition
    TP_ASSERT( 0 != m_iGPUProgramID ,   "m_iGPUProgramID should not be 0 (here it is '%d') - Did glCreateProgram(...) succeed ?.\n", m_iGPUProgramID );
    //--------------------------------------------------------------------------------------------------------------------
}

//====================================================================================================================================
void TPGLWindow::destroyGPUProgramAndShaders()
{
    //--------------------------------------------------------------------------------------------------------------------
    // precondition
    TP_ASSERT( 0 != m_iGPUProgramID ,           "m_iGPUProgramID should not be 0 (here it is '%d') - Did you call createGPUProgramFromGPUShaders() ?.\n", m_iGPUProgramID );
    TP_ASSERT( 0 != m_iGPUVertexShaderID ,      "m_iGPUVertexShaderID should not be 0 (here it is '%d') - Did you call createGPUVertexShader() ?.\n", m_iGPUVertexShaderID );
   TP_ASSERT( 0 != m_iGPUFragmentShaderID ,    "m_iGPUFragmentShaderID should not be 0 (here it is '%d') - Did you call createGPUProgramFromGPUShaders() ?.\n", m_iGPUFragmentShaderID );
    //--------------------------------------------------------------------------------------------------------------------

    glDeleteProgram( m_iGPUProgramID );

    glDeleteShader( m_iGPUVertexShaderID );
    glDeleteShader( m_iGPUFragmentShaderID );
    glDeleteShader( m_iGPUGeometryShaderID );
}

//====================================================================================================================================
void TPGLWindow::getUniformLocations()
{
    //--------------------------------------------------------------------------------------------------------------------
    // precondition
    TP_ASSERT( 0 != m_iGPUProgramID , "m_iGPUProgramID should not be 0 (here it is '%d') - Did you call createGPUProgramFromGPUShaders() ?.\n", m_iGPUProgramID );
    //--------------------------------------------------------------------------------------------------------------------

    m_iUniformWorld             = glGetUniformLocation(m_iGPUProgramID, "u_mtxWorld");
    m_iUniformView              = glGetUniformLocation(m_iGPUProgramID, "u_mtxView");
    m_iUniformProjection        = glGetUniformLocation(m_iGPUProgramID, "u_mtxProjection");

    // Retrieves the uniform location for the texture SAMPLER used in the GLSL Fragment Shader
    m_iUniformSampler       = glGetUniformLocation( m_iGPUProgramID, "u_tex" );
}


//====================================================================================================================================
void TPGLWindow::getAttributeLocations()
{
    //--------------------------------------------------------------------------------------------------------------------
    // precondition
    TP_ASSERT( 0 != m_iGPUProgramID , "m_iGPUProgramID should not be 0 (here it is '%d') - Did you call createGPUProgramFromGPUShaders() ?.\n", m_iGPUProgramID );
    //--------------------------------------------------------------------------------------------------------------------


}


//====================================================================================================================================
void TPGLWindow::sendUniformsToGPU()
{
    //
    glUniformMatrix4fv(m_iUniformWorld, 1, GL_FALSE, glm::value_ptr(m_mtxObjectWorld) );
    glUniformMatrix4fv(m_iUniformView, 1, GL_FALSE, glm::value_ptr(m_mtxCameraView) );
    glUniformMatrix4fv(m_iUniformProjection, 1, GL_FALSE, glm::value_ptr(m_mtxCameraProjection) );

    // Specify on which texture unit the uniform texture sampler must be bound
    glUniform1i( m_iUniformSampler , 0 );

}

//====================================================================================================================================
void TPGLWindow::createVBO()
{



}

//====================================================================================================================================
void TPGLWindow::createVAOFromVBO()
{

}

//====================================================================================================================================
void TPGLWindow::destroyVAOAndVBO()
{

}



//====================================================================================================================================
void TPGLWindow::createTextures()
{

}

//====================================================================================================================================
void TPGLWindow::destroyTextures()
{
}


//====================================================================================================================================
void TPGLWindow::setupTexturesInUnit()
{

}

//====================================================================================================================================
void TPGLWindow::initializeGL()
{
    initializeOpenGLFunctions();

    // Prints the GL Version
    TP_LOG("GL Version : %s\n",(char*)glGetString(GL_VERSION));


    createGPUVertexShader();
    createGPUGeometryShader();
    createGPUFragmentShader();
    createGPUProgramFromGPUShaders();

    getUniformLocations();
    getAttributeLocations();

    createVBO();
    createVAOFromVBO();

    QString glType;
      QString glVersion;
      QString glProfile;

      // Get Version Information
      glType = (context()->isOpenGLES()) ? "OpenGL ES" : "OpenGL";
      glVersion = reinterpret_cast<const char*>(glGetString(GL_VERSION));

      // Get Profile Information
    #define CASE(c) case QSurfaceFormat::c: glProfile = #c; break
      switch (format().profile())
      {
        CASE(NoProfile);
        CASE(CoreProfile);
        CASE(CompatibilityProfile);
      }
    #undef CASE

      // qPrintable() will print our QString w/o quotes around it.
      qDebug() << qPrintable(glType) << qPrintable(glVersion) << "(" << qPrintable(glProfile) << ")";



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

    // Finally, build the World matrix for the model taking into account its translate and orientation
    glm::mat4 Model             = glm::translate(glm::mat4(1.0f), m_vObjectTranslate);
    m_mtxObjectWorld            = glm::rotate(Model, glm::radians(-90.0f), vRight);
    m_mtxObjectWorld            = glm::rotate(m_mtxObjectWorld, m_vObjectEulerAngles.y, vFront);
}

//====================================================================================================================================
void TPGLWindow::update()
{
    // NB : Just like render(), this method is called every frame, as often/fast as possible

//    static int siFrameID        = 0;

    // Update light position, so that it is animated
    float   fTimeElapsed        = (float) m_timer.restart();


    for(int i=0; i<m_particlesRenderers.size(); i++)
        m_particlesRenderers[i]->update(fTimeElapsed);

    // make sure the matrix data are init to some valid values
    updateMatrices();



    //    siFrameID++;
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
    update();
    //--------------------------------------------------------------------------------------------------------------------
    // precondition
    TP_ASSERT( 0 != m_iGPUProgramID , "m_iGPUProgramID should not be 0 (here it is '%d') - Did you call createGPUProgramFromGPUShaders() ?.\n", m_iGPUProgramID );
    //--------------------------------------------------------------------------------------------------------------------


    // Specifies the viewport size -----------------------------------------------------------------
    //const float retinaScale = devicePixelRatio();
    //glViewport( 0, 0, width() * retinaScale, height() * retinaScale );

    // Specify winding order Counter ClockZise (even though it's default on OpenGL) ----------------
    glFrontFace( GL_CCW );

    // Enables (Back) Face Culling -----------------------------------------------------------------
    glEnable( GL_CULL_FACE );
    glCullFace( GL_BACK );


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
    glClearColor( 0.5f, 0.f, 0.5f, 0.0f );

    // Clears the framebuffer ---------------------------------------------------------------------
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT );


    // Starts using the given GPU Program ---------------------------------------------------------
    glUseProgram( m_iGPUProgramID );
    {
        // Setup the Texture to be used
        setupTexturesInUnit();

        // Sends the uniforms var from the CPU to the GPU -----------------------------------------
        sendUniformsToGPU();

        for(int i=0; i<m_particlesRenderers.size(); i++)
            m_particlesRenderers[i]->render();

    }
    // Stops using the GPU Program ----------------------------------------------------------------
    glUseProgram( 0 );
}

void TPGLWindow::mousePressEvent(QMouseEvent *) {

}

void TPGLWindow::mouseReleaseEvent(QMouseEvent *) {

}

//====================================================================================================================================
void TPGLWindow::resizeGL(int w, int h)
{
    // Force the update of the perspective matrix
    updateMatrices();
}

void TPGLWindow::mouseMoveEvent(QMouseEvent * event) {
    float x = 1.f - 2.f * (float) event->x() / this->width();
    float y = 1.f - 2.f * (float) (this->height() - event->y()) / this->height();

    glm::vec4 vec(-fCameraZInit*x,0, -fCameraZInit*y, 1);

    glm::vec4 vec2 = vec * glm::inverse(m_mtxCameraProjection * m_mtxCameraView * m_mtxObjectWorld  );

    glm::vec4 test(1.f, 0.f, 1.f, 1.f);
    glm::vec4 test2 = m_mtxCameraProjection * m_mtxCameraView * m_mtxObjectWorld * test;

    glm::vec3 final(vec2.x, 0.f, vec2.y);

    for(unsigned int i=0; i<m_particlesRenderers.size(); i++)
        m_particlesRenderers[i]->getEmetter()->setUseCustomAttractPoint(false);
    switch(m_mouseBehaviour) {
    case MOVE_EMETTER:
            for(unsigned int i=0; i<m_particlesRenderers.size(); i++)
                m_particlesRenderers[i]->getEmetter()->setOrigin(final);
        break;
    case MOVE_ATTRACT_POINT:
        for(unsigned int i=0; i<m_particlesRenderers.size(); i++) {
            m_particlesRenderers[i]->getEmetter()->setUseCustomAttractPoint(true);
            m_particlesRenderers[i]->getEmetter()->setCustomAttractPoint(final);
        }
        break;
    }

    if(m_mouseBehaviour == MOVE_VIEW) {
        m_vCameraCenter.x = final.x * CAMERA_SENSITIVITY;
        m_vCameraCenter.y = final.z * CAMERA_SENSITIVITY;
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

