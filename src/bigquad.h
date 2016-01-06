#ifndef BIGQUAD_H
#define BIGQUAD_H

#include <QOpenGLFunctions_3_3_Core>
#include <glm/glm.hpp>

/**
 * @brief 3D object representing a simple horizontal plane with a texture
 */
class BigQuad :  protected QOpenGLFunctions_3_3_Core
{
public:
    /**
     * @brief Creates the quad
     */
    BigQuad();

    /**
     * @brief Init VBOs with a texture and the plane height
     * @param textureFilename filename of the texture to use
     * @param height height of the plane
     */
    void init(const QString& textureFilename, float height);
    ~BigQuad();

    /**
     * @brief Draw the quad
     */
    void render();

protected:
    glm::vec3   _points[4];
    glm::vec3   _normals[4];
    glm::vec2   _texCoords[4];

    GLuint  _iVAO;

    GLuint  _iVBOPosition;
    GLuint  _iVBONormal;
    GLuint  _iVBOTexCoord;

    GLuint  _iAttribPosition;
    GLuint  _iAttribNormal;
    GLuint  _iAttribTexCoord;

    GLuint  _iTexture;
};

#endif // BIGQUAD_H
