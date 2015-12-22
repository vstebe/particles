#include "main.h"


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
    , m_emetter             (TP_PATH_TO_DATA "fire.json")
{
    m_vCameraPosition   = glm::vec3(0,0,fCameraZInit);

    m_timer.start();

    // request the window to render in loop
    setAnimating(true);
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

    m_iAttribPosition   = glGetAttribLocation(m_iGPUProgramID, "vtx_position");
    m_iAttribColor     = glGetAttribLocation(m_iGPUProgramID, "vtx_color");
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

    m_iVertexCount = m_emetter.getSize();


    m_iElementsCount = m_iVertexCount;//( iMeriCount - 1 ) * ( iParaCount - 1 ) * 2 * 3;  // for quads split in 2

    // Creates 1 Buffer object for POSITIONS ------------------------------------------------------------------------
    glGenBuffers(1, &m_iVBOPosition);

    // Binds the Buffer, to say "we will work on this one from now"
    glBindBuffer(GL_ARRAY_BUFFER, m_iVBOPosition);
    {
        // Defines the type of Buffer data, the size in bytes, fills it with the given positions,
        //      and finally specifies what kind of Draws we will do (so that the driver can optimize the how it will access it)
        glBufferData(GL_ARRAY_BUFFER, m_iVertexCount * sizeof(glm::vec3), NULL, GL_STREAM_DRAW);
    }
    // Unbinds the Buffer, we are done working on it !
    glBindBuffer(GL_ARRAY_BUFFER, 0);


    // Creates 1 Buffer object for POSITIONS ------------------------------------------------------------------------
    glGenBuffers(1, &m_iVBOColor);

    // Binds the Buffer, to say "we will work on this one from now"
    glBindBuffer(GL_ARRAY_BUFFER, m_iVBOColor);
    {
        // Defines the type of Buffer data, the size in bytes, fills it with the given positions,
        //      and finally specifies what kind of Draws we will do (so that the driver can optimize the how it will access it)
        glBufferData(GL_ARRAY_BUFFER, m_iVertexCount * sizeof(glm::vec4), NULL, GL_STREAM_DRAW);
    }
    // Unbinds the Buffer, we are done working on it !
    glBindBuffer(GL_ARRAY_BUFFER, 0);

}

//====================================================================================================================================
void TPGLWindow::createVAOFromVBO()
{
    // Creates 1 VAO
    glGenVertexArrays( 1, &m_iVAO );

    // Binds the VAO, to say "we will work on this one from now"
    glBindVertexArray( m_iVAO );
    {
        // Binds the Buffer, to say "we will work on this one from now"
        glBindBuffer( GL_ARRAY_BUFFER, m_iVBOPosition );
        // Enables the attribute id used for vtx_position
        glEnableVertexAttribArray( m_iAttribPosition );
        // Gives the right params
        glVertexAttribPointer( m_iAttribPosition, 3, GL_FLOAT, GL_FALSE, 0, 0 );

        // Binds the Buffer, to say "we will work on this one from now"
        glBindBuffer( GL_ARRAY_BUFFER, m_iVBOColor );
        // Enables the attribute id used for vtx_position
        glEnableVertexAttribArray( m_iAttribColor );
        // Gives the right params
        glVertexAttribPointer( m_iAttribColor, 4, GL_FLOAT, GL_FALSE, 0, 0 );
    }
    // UnBinds the VAO, we are done working on it !
    glBindVertexArray(0);
}

//====================================================================================================================================
void TPGLWindow::destroyVAOAndVBO()
{
    glDeleteBuffers(1, & m_iVBOPosition);
    glDeleteBuffers(1, & m_iVBOColor);

    glDeleteVertexArrays(1, & m_iVAO);
}



//====================================================================================================================================
void TPGLWindow::createTextures()
{
    //--------------------------------------------------------------------------------------------------------------------
    // precondition
    TP_ASSERT( 0 != m_iGPUProgramID , "m_iGPUProgramID should not be 0 (here it is '%d') - Did you call createGPUProgramFromGPUShaders() ?.\n", m_iGPUProgramID );
    //--------------------------------------------------------------------------------------------------------------------

    // IMAGE EARTH
    // pDataImage0 : POINTER on IMAGE RGBA (4 bytes per pixel)
    QString filename = m_emetter.getConfiguration().getImage();
    QImage img(filename);
    QImage img0 = img.convertToFormat(QImage::Format_RGBA8888_Premultiplied,Qt::NoOpaqueDetection);

    unsigned char* pDataImage0 = img0.scanLine(0);

    glEnable(GL_TEXTURE_2D);

    // Creates the Texture object
    glGenTextures( 1, & m_iTexture );

    glBindTexture( GL_TEXTURE_2D, m_iTexture );
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );

        glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA8, img0.width(), img0.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, pDataImage0 );
    }
    glBindTexture( GL_TEXTURE_2D, 0 );
}

//====================================================================================================================================
void TPGLWindow::destroyTextures()
{
    glDeleteTextures( 1, & m_iTexture );
}


//====================================================================================================================================
void TPGLWindow::setupTexturesInUnit()
{
    // Specify on which texture unit the uniform texture sampler must be bound
    // Activates the texture unit 0
    glActiveTexture( GL_TEXTURE0 + 0 );
    //
    glBindTexture( GL_TEXTURE_2D, m_iTexture );
}

//====================================================================================================================================
void TPGLWindow::initialize()
{
    // Prints the GL Version
    TP_LOG("GL Version : %s\n",(char*)glGetString(GL_VERSION));

    m_emetter.setActive(true);

    createGPUVertexShader();
    createGPUGeometryShader();
    createGPUFragmentShader();
    createGPUProgramFromGPUShaders();

    getUniformLocations();
    getAttributeLocations();

    createVBO();
    createVAOFromVBO();

    createTextures();
}

//====================================================================================================================================
void TPGLWindow::updateMatrices()
{
    const glm::vec3 vRight  (1,0,0);
    const glm::vec3 vUp     (0,1,0);
    const glm::vec3 vFront  (0,0,-1);
    const glm::vec3 vCenter (0,0,-1);

    // Projection matrix is "simply" a perspective matrix, so use the right tool function from GLM
    m_mtxCameraProjection       = glm::perspective(glm::radians(45.0f), (float)width()/height(), 0.1f, 100.f);


    // View matrix is built from the common "lookAt" paradigm, using the RH (Right-Handed) coordinate system
    m_mtxCameraView             = glm::lookAtRH( m_vCameraPosition, vCenter, vUp );

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

    m_emetter.update(fTimeElapsed / 1000.f);

    glBindBuffer(GL_ARRAY_BUFFER, m_iVBOPosition);
            glBufferData(GL_ARRAY_BUFFER, m_iVertexCount * sizeof(glm::vec3), NULL, GL_STREAM_DRAW); // Buffer orphaning, a common way to improve streaming perf. See above link for details.
            glBufferSubData(GL_ARRAY_BUFFER, 0, m_iVertexCount * sizeof(glm::vec3), m_emetter.getData());

    glBindBuffer(GL_ARRAY_BUFFER, m_iVBOColor);
            glBufferData(GL_ARRAY_BUFFER, m_iVertexCount * sizeof(glm::vec4), NULL, GL_STREAM_DRAW); // Buffer orphaning, a common way to improve streaming perf. See above link for details.
            glBufferSubData(GL_ARRAY_BUFFER, 0, m_iVertexCount * sizeof(glm::vec4), m_emetter.getColorData());


    // make sure the matrix data are init to some valid values
    updateMatrices();



//    siFrameID++;
}

//====================================================================================================================================
void TPGLWindow::render()
{
    //--------------------------------------------------------------------------------------------------------------------
    // precondition
    TP_ASSERT( 0 != m_iGPUProgramID , "m_iGPUProgramID should not be 0 (here it is '%d') - Did you call createGPUProgramFromGPUShaders() ?.\n", m_iGPUProgramID );
    //--------------------------------------------------------------------------------------------------------------------


    // Specifies the viewport size -----------------------------------------------------------------
    const float retinaScale = devicePixelRatio();
    glViewport( 0, 0, width() * retinaScale, height() * retinaScale );

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
    glClearColor( 0.05f, 0.2f, 0.3f, 1.0f );

    // Clears the framebuffer ---------------------------------------------------------------------
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT );


    // Starts using the given GPU Program ---------------------------------------------------------
    glUseProgram( m_iGPUProgramID );
    {
        // Setup the Texture to be used
        setupTexturesInUnit();

        // Sends the uniforms var from the CPU to the GPU -----------------------------------------
        sendUniformsToGPU();

        // Starts using the given VAO, and its associated VBOs ------------------------------------
        glBindVertexArray( m_iVAO );
        {
            // Draws POINTS --------------------------------
            glDrawArrays( GL_POINTS, 0, m_iElementsCount );
        }
         // Stops using the VAO
        glBindVertexArray( 0 );

    }
    // Stops using the GPU Program ----------------------------------------------------------------
    glUseProgram( 0 );
}

void TPGLWindow::mousePressEvent(QMouseEvent *) {
    m_emetter.setActive(true);
}

void TPGLWindow::mouseReleaseEvent(QMouseEvent *) {
    m_emetter.setActive(false);
}

//====================================================================================================================================
void TPGLWindow::resizeEvent(QResizeEvent* /*_pEvent*/)
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

    //std::cout << "TEST : " << test2.x << " " << test2.y << " " << test2.z << std::endl;

    //std::cout << "WIN : " << x << " " << y << " " << 0 << std::endl;
    //std::cout << "WORLD : " << vec2.x << " " << vec2.y << " " << vec2.z << std::endl;
    //m_emetter.setOrigin(glm::vec3(vec2.x, 0.f, vec2.y));

    m_emetter.mouse = glm::vec3(vec2.x, 0.f, vec2.y);
}

//====================================================================================================================================
void TPGLWindow::keyPressEvent(QKeyEvent* _pEvent)
{
    if(QEvent::KeyPress == _pEvent->type() )
    {
//        float   fTimeElapsed= (float) m_timer.elapsed();

        float   fMoveSpeed          = 0.07f;// * fTimeElapsed;
        bool    bHasObjectMoved     = false;


        // handle key press to move the 3D object
        switch( _pEvent->key() )
        {
        case Qt::Key_B:
            m_bAlphaBlend = !m_bAlphaBlend;
            break;

        case Qt::Key_R:
            m_vObjectTranslate = glm::vec3(0, 0, 0);
            m_vCameraPosition  = glm::vec3(0,0,fCameraZInit);
            bHasObjectMoved = true;
            break;

        case Qt::Key_Minus:
            m_vObjectTranslate -= glm::vec3(0, 0, 1) * fMoveSpeed;
            bHasObjectMoved = true;
            break;

        case Qt::Key_Plus:
            m_vObjectTranslate += glm::vec3(0, 0, 1) * fMoveSpeed;
            bHasObjectMoved = true;
            break;

        case Qt::Key_Left:
            m_vObjectTranslate -= glm::vec3(1, 0, 0) * fMoveSpeed;
            bHasObjectMoved = true;
            break;

        case Qt::Key_Right:
            m_vObjectTranslate += glm::vec3(1, 0, 0) * fMoveSpeed;
            bHasObjectMoved = true;
            break;

        case Qt::Key_Up:
            m_vObjectTranslate += glm::vec3(0, 1, 0) * fMoveSpeed;
            bHasObjectMoved = true;
            break;

        case Qt::Key_Down:
            m_vObjectTranslate -= glm::vec3(0, 1, 0) * fMoveSpeed;
            bHasObjectMoved = true;
            break;

        case Qt::Key_Space:
            m_vObjectEulerAngles -= glm::vec3(0, glm::radians(1.0f), 0);
            bHasObjectMoved = true;
            break;

        case Qt::Key_Return:
        case Qt::Key_Enter:
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


//====================================================================================================================================
int main(int argc, char **argv)
{
    QGuiApplication app(argc, argv);

    QSurfaceFormat format;

    qsrand(QTime::currentTime().msec());

#ifdef Q_OS_LINUX  // what about Linux ?
    format.setMajorVersion(3);
    format.setMinorVersion(3);
    format.setProfile(QSurfaceFormat::CoreProfile);
#endif
    format.setSamples(16);
    format.setDepthBufferSize(24);

    TPGLWindow window;
    window.setFormat(format);
    window.resize(800, 600);
    window.show();

    return app.exec();
}
