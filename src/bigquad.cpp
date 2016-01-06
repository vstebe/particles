#include "bigquad.h"

#include <QImage>
#include <QDebug>
float size = 100.f;

BigQuad::BigQuad() :
    _iVAO               (0),
    _iVBOPosition       (0),
    _iVBONormal         (0),
    _iVBOTexCoord       (0),
    _iAttribPosition    (0),
    _iAttribNormal      (1),
    _iAttribTexCoord    (2),
    _iTexture           (0)
{

}

void BigQuad::init(const QString& textureFilename, float height)
{
    initializeOpenGLFunctions();

    _points[3] = glm::vec3(size, height, size);
    _points[2] = glm::vec3(size, height, -size);
    _points[1] = glm::vec3(-size, height, size);
    _points[0] = glm::vec3(-size, height, -size);

    glm::vec3 normal;
    if(height > 0)
        normal = glm::vec3(0.f, -1.f, 0.f);
    else
        normal = glm::vec3(0.f, 1.f, 0.f);

    for(int i=0; i<4; i++)
        _normals[i] = normal;

    _texCoords[0] = glm::vec2(0.f, 0.f);
    _texCoords[1] = glm::vec2(0.f, 1.f);
    _texCoords[2] = glm::vec2(1.f, 1.f);
    _texCoords[3] = glm::vec2(1.f, 0.f);

    // Creates 1 Buffer object for POSITIONS ------------------------------------------------------------------------
    glGenBuffers(1, &_iVBOPosition);

    // Binds the Buffer, to say "we will work on this one from now"
    glBindBuffer(GL_ARRAY_BUFFER, _iVBOPosition);
    {
        // Defines the type of Buffer data, the size in bytes, fills it with the given positions,
        //      and finally specifies what kind of Draws we will do (so that the driver can optimize the how it will access it)
        glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(glm::vec3), NULL, GL_STREAM_DRAW);
    }
    // Unbinds the Buffer, we are done working on it !
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Creates 1 Buffer object for POSITIONS ------------------------------------------------------------------------
    glGenBuffers(1, &_iVBONormal);

    // Binds the Buffer, to say "we will work on this one from now"
    glBindBuffer(GL_ARRAY_BUFFER, _iVBONormal);
    {
        // Defines the type of Buffer data, the size in bytes, fills it with the given positions,
        //      and finally specifies what kind of Draws we will do (so that the driver can optimize the how it will access it)
        glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(glm::vec3), NULL, GL_STREAM_DRAW);
    }
    // Unbinds the Buffer, we are done working on it !
    glBindBuffer(GL_ARRAY_BUFFER, 0);


    // Creates 1 Buffer object for TEX ------------------------------------------------------------------------
    glGenBuffers(1, &_iVBOTexCoord);

    // Binds the Buffer, to say "we will work on this one from now"
    glBindBuffer(GL_ARRAY_BUFFER, _iVBOTexCoord);
    {
        // Defines the type of Buffer data, the size in bytes, fills it with the given positions,
        //      and finally specifies what kind of Draws we will do (so that the driver can optimize the how it will access it)
        glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(glm::vec2), NULL, GL_STREAM_DRAW);
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
        glBindBuffer( GL_ARRAY_BUFFER, _iVBONormal );
        // Enables the attribute id used for vtx_position
        glEnableVertexAttribArray( _iAttribNormal );
        // Gives the right params
        glVertexAttribPointer( _iAttribNormal, 3, GL_FLOAT, GL_FALSE, 0, 0 );

        // Binds the Buffer, to say "we will work on this one from now"
        glBindBuffer( GL_ARRAY_BUFFER, _iVBOTexCoord );
        // Enables the attribute id used for vtx_position
        glEnableVertexAttribArray( _iAttribTexCoord );
        // Gives the right params
        glVertexAttribPointer( _iAttribTexCoord, 2, GL_FLOAT, GL_FALSE, 0, 0 );
    }
    // UnBinds the VAO, we are done working on it !
    glBindVertexArray(0);



    QImage img(textureFilename);
    QImage img0 = img.convertToFormat(QImage::Format_RGBA8888_Premultiplied,Qt::NoOpaqueDetection);

    unsigned char* pDataImage0 = img0.scanLine(0);

    // glEnable(GL_TEXTURE_2D);

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


    glBindBuffer(GL_ARRAY_BUFFER, _iVBOPosition);
            glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(glm::vec3), NULL, GL_STREAM_DRAW); // Buffer orphaning, a common way to improve streaming perf. See above link for details.
            glBufferSubData(GL_ARRAY_BUFFER, 0, 4 * sizeof(glm::vec3), _points);

    glBindBuffer(GL_ARRAY_BUFFER, _iVBONormal);
            glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(glm::vec3), NULL, GL_STREAM_DRAW); // Buffer orphaning, a common way to improve streaming perf. See above link for details.
            glBufferSubData(GL_ARRAY_BUFFER, 0, 4 * sizeof(glm::vec3), _normals);

    glBindBuffer(GL_ARRAY_BUFFER, _iVBOTexCoord);
            glBufferData(GL_ARRAY_BUFFER, 4 * sizeof(glm::vec2), NULL, GL_STREAM_DRAW); // Buffer orphaning, a common way to improve streaming perf. See above link for details.
            glBufferSubData(GL_ARRAY_BUFFER, 0, 4 * sizeof(glm::vec2), _texCoords);
}


void BigQuad::render()
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
        glDrawArrays( GL_TRIANGLE_STRIP, 0, 4);
    }
     // Stops using the VAO
    glBindVertexArray( 0 );
}

BigQuad::~BigQuad()
{
    glDeleteBuffers(1, & _iVBOPosition);
    glDeleteBuffers(1, &_iVBONormal);
    glDeleteBuffers(1, &_iVBOTexCoord);
    glDeleteTextures( 1, & _iTexture );

    glDeleteVertexArrays(1, & _iVAO);
}
