#include "particlesrenderer.h"

#include <QImage>

ParticlesRenderer::ParticlesRenderer(const ParticleConfiguration &config) :
    _emetter            (config),
    _iAttribColor       (1),
    _iAttribPosition    (0),
    _iAttribRotation    (2)
{
    initializeOpenGLFunctions();

    // Creates 1 Buffer object for POSITIONS ------------------------------------------------------------------------
    glGenBuffers(1, &_iVBOPosition);

    // Binds the Buffer, to say "we will work on this one from now"
    glBindBuffer(GL_ARRAY_BUFFER, _iVBOPosition);
    {
        // Defines the type of Buffer data, the size in bytes, fills it with the given positions,
        //      and finally specifies what kind of Draws we will do (so that the driver can optimize the how it will access it)
        glBufferData(GL_ARRAY_BUFFER, _emetter.getSize() * sizeof(glm::vec3), NULL, GL_STREAM_DRAW);
    }
    // Unbinds the Buffer, we are done working on it !
    glBindBuffer(GL_ARRAY_BUFFER, 0);


    // Creates 1 Buffer object for POSITIONS ------------------------------------------------------------------------
    glGenBuffers(1, &_iVBOColor);

    // Binds the Buffer, to say "we will work on this one from now"
    glBindBuffer(GL_ARRAY_BUFFER, _iVBOColor);
    {
        // Defines the type of Buffer data, the size in bytes, fills it with the given positions,
        //      and finally specifies what kind of Draws we will do (so that the driver can optimize the how it will access it)
        glBufferData(GL_ARRAY_BUFFER, _emetter.getSize() * sizeof(glm::vec4), NULL, GL_STREAM_DRAW);
    }
    // Unbinds the Buffer, we are done working on it !
    glBindBuffer(GL_ARRAY_BUFFER, 0);


    // Creates 1 Buffer object for POSITIONS ------------------------------------------------------------------------
    glGenBuffers(1, &_iVBORotation);

    // Binds the Buffer, to say "we will work on this one from now"
    glBindBuffer(GL_ARRAY_BUFFER, _iVBORotation);
    {
        // Defines the type of Buffer data, the size in bytes, fills it with the given positions,
        //      and finally specifies what kind of Draws we will do (so that the driver can optimize the how it will access it)
        glBufferData(GL_ARRAY_BUFFER, _emetter.getSize() * sizeof(GLfloat), NULL, GL_STREAM_DRAW);
    }
    // Unbinds the Buffer, we are done working on it !
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Creates 1 VAO
    glGenVertexArrays( 1, &_iVAO );

    // Binds the VAO, to say "we will work on this one from now"
    glBindVertexArray( _iVAO );
    {
        // Binds the Buffer, to say "we will work on this one from now"
        glBindBuffer( GL_ARRAY_BUFFER, _iVBOPosition );
        // Enables the attribute id used for vtx_position
        glEnableVertexAttribArray( _iAttribPosition );
        // Gives the right params
        glVertexAttribPointer( _iAttribPosition, 3, GL_FLOAT, GL_FALSE, 0, 0 );

        // Binds the Buffer, to say "we will work on this one from now"
        glBindBuffer( GL_ARRAY_BUFFER, _iVBOColor );
        // Enables the attribute id used for vtx_position
        glEnableVertexAttribArray( _iAttribColor );
        // Gives the right params
        glVertexAttribPointer( _iAttribColor, 4, GL_FLOAT, GL_FALSE, 0, 0 );

        // Binds the Buffer, to say "we will work on this one from now"
        glBindBuffer( GL_ARRAY_BUFFER, _iVBORotation );
        // Enables the attribute id used for vtx_position
        glEnableVertexAttribArray( _iAttribRotation );
        // Gives the right params
        glVertexAttribPointer( _iAttribRotation, 1, GL_FLOAT, GL_FALSE, 0, 0 );
    }
    // UnBinds the VAO, we are done working on it !
    glBindVertexArray(0);




    QString filename = _emetter.getConfiguration().getImage();
    QImage img(filename);
    QImage img0 = img.convertToFormat(QImage::Format_RGBA8888_Premultiplied,Qt::NoOpaqueDetection);

    unsigned char* pDataImage0 = img0.scanLine(0);

    glEnable(GL_TEXTURE_2D);

    // Creates the Texture object
    glGenTextures( 1, & _iTexture );

    glBindTexture( GL_TEXTURE_2D, _iTexture );
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );

        glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA8, img0.width(), img0.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, pDataImage0 );
    }
    glBindTexture( GL_TEXTURE_2D, 0 );
}

void ParticlesRenderer::update(float fTimeElapsed)
{
    _emetter.update(fTimeElapsed / 1000.f);

    glBindBuffer(GL_ARRAY_BUFFER, _iVBOPosition);
            glBufferData(GL_ARRAY_BUFFER, _emetter.getSize() * sizeof(glm::vec3), NULL, GL_STREAM_DRAW); // Buffer orphaning, a common way to improve streaming perf. See above link for details.
            glBufferSubData(GL_ARRAY_BUFFER, 0, _emetter.getSize() * sizeof(glm::vec3), _emetter.getData());

    glBindBuffer(GL_ARRAY_BUFFER, _iVBOColor);
            glBufferData(GL_ARRAY_BUFFER, _emetter.getSize() * sizeof(glm::vec4), NULL, GL_STREAM_DRAW); // Buffer orphaning, a common way to improve streaming perf. See above link for details.
            glBufferSubData(GL_ARRAY_BUFFER, 0, _emetter.getSize() * sizeof(glm::vec4), _emetter.getColorData());

        glBindBuffer(GL_ARRAY_BUFFER, _iVBORotation);
                glBufferData(GL_ARRAY_BUFFER, _emetter.getSize() * sizeof(float), NULL, GL_STREAM_DRAW); // Buffer orphaning, a common way to improve streaming perf. See above link for details.
                glBufferSubData(GL_ARRAY_BUFFER, 0, _emetter.getSize() * sizeof(float), _emetter.getRotationData());


}

void ParticlesRenderer::render()
{
    // Specify on which texture unit the uniform texture sampler must be bound
    // Activates the texture unit 0
    glActiveTexture( GL_TEXTURE0 + 0 );
    //
    glBindTexture( GL_TEXTURE_2D, _iTexture );

    // Starts using the given VAO, and its associated VBOs ------------------------------------
    glBindVertexArray( _iVAO );
    {
        // Draws POINTS --------------------------------
        glDrawArrays( GL_POINTS, 0, _emetter.getSize() );
    }
     // Stops using the VAO
    glBindVertexArray( 0 );
}

Emetter *ParticlesRenderer::getEmetter()
{
    return &_emetter;
}

ParticlesRenderer::~ParticlesRenderer()
{
    glDeleteBuffers(1, & _iVBOPosition);
    glDeleteBuffers(1, & _iVBOColor);
    glDeleteBuffers(1, & _iVBORotation);
    glDeleteTextures( 1, & _iTexture );

    glDeleteVertexArrays(1, & _iVAO);
}
