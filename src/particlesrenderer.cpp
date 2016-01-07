#include "particlesrenderer.h"

#include <QImage>
#include <QDebug>

ParticlesRenderer::ParticlesRenderer(const ParticleConfiguration &config) :
    _emetter            (config),
    _iVAO               (0),
    _iVBOPosition       (0),
    _iVBOColor          (0),
    _iVBORotation       (0),
    _iVBOSize           (0),
    _iAttribPosition    (0),
    _iAttribColor       (1),
    _iAttribRotation    (2),
    _iAttribSize        (3),
    _iTexture           (0)
{
    initializeOpenGLFunctions();

    // Creates 1 Buffer object for POSITIONS ------------------------------------------------------------------------
    glGenBuffers(1, &_iVBOPosition);
    glBindBuffer(GL_ARRAY_BUFFER, _iVBOPosition);
    {
        glBufferData(GL_ARRAY_BUFFER, _emetter.getSize() * sizeof(glm::vec3), NULL, GL_STREAM_DRAW);
    }
    glBindBuffer(GL_ARRAY_BUFFER, 0);


    // Creates 1 Buffer object for COLORS  ------------------------------------------------------------------------
    glGenBuffers(1, &_iVBOColor);
    glBindBuffer(GL_ARRAY_BUFFER, _iVBOColor);
    {
        glBufferData(GL_ARRAY_BUFFER, _emetter.getSize() * sizeof(glm::vec4), NULL, GL_STREAM_DRAW);
    }
    glBindBuffer(GL_ARRAY_BUFFER, 0);


    // Creates 1 Buffer object for ROTATIONS ------------------------------------------------------------------------
    glGenBuffers(1, &_iVBORotation);
    glBindBuffer(GL_ARRAY_BUFFER, _iVBORotation);
    {
        glBufferData(GL_ARRAY_BUFFER, _emetter.getSize() * sizeof(GLfloat), NULL, GL_STREAM_DRAW);
    }
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Creates 1 Buffer object for SIZES ------------------------------------------------------------------------
    glGenBuffers(1, &_iVBOSize);
    glBindBuffer(GL_ARRAY_BUFFER, _iVBOSize);
    {
        glBufferData(GL_ARRAY_BUFFER, _emetter.getSize() * sizeof(GLfloat), NULL, GL_STREAM_DRAW);
    }
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Creates 1 VAO
    glGenVertexArrays( 1, &_iVAO );

    // Binds the VAO, to say "we will work on this one from now"
    glBindVertexArray( _iVAO );
    {
        glBindBuffer( GL_ARRAY_BUFFER, _iVBOPosition );
        glEnableVertexAttribArray( _iAttribPosition );
        glVertexAttribPointer( _iAttribPosition, 3, GL_FLOAT, GL_FALSE, 0, 0 );

        glBindBuffer( GL_ARRAY_BUFFER, _iVBOColor );
        glEnableVertexAttribArray( _iAttribColor );
        glVertexAttribPointer( _iAttribColor, 4, GL_FLOAT, GL_FALSE, 0, 0 );

        glBindBuffer( GL_ARRAY_BUFFER, _iVBORotation );
        glEnableVertexAttribArray( _iAttribRotation );
        glVertexAttribPointer( _iAttribRotation, 1, GL_FLOAT, GL_FALSE, 0, 0 );

        glBindBuffer( GL_ARRAY_BUFFER, _iVBOSize );
        glEnableVertexAttribArray( _iAttribSize );
        glVertexAttribPointer( _iAttribSize, 1, GL_FLOAT, GL_FALSE, 0, 0 );
    }
    glBindVertexArray(0);



    QString filename = _emetter.getConfiguration().getImage();
    QImage img(filename);
    QImage img0 = img.convertToFormat(QImage::Format_RGBA8888_Premultiplied,Qt::NoOpaqueDetection);

    unsigned char* pDataImage0 = img0.scanLine(0);

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

    glBindBuffer(GL_ARRAY_BUFFER, _iVBOSize);
            glBufferData(GL_ARRAY_BUFFER, _emetter.getSize() * sizeof(float), NULL, GL_STREAM_DRAW); // Buffer orphaning, a common way to improve streaming perf. See above link for details.
            glBufferSubData(GL_ARRAY_BUFFER, 0, _emetter.getSize() * sizeof(float), _emetter.getSizeData());

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
    glDeleteBuffers(1, & _iVBOSize);
    glDeleteTextures( 1, & _iTexture );

    glDeleteVertexArrays(1, & _iVAO);
}
