#ifndef PARTICLESRENDERER_H
#define PARTICLESRENDERER_H

#include "emetter.h"
#include "particleconfiguration.h"

#include <QOpenGLFunctions_3_3_Core>

class ParticlesRenderer :  protected QOpenGLFunctions_3_3_Core
{
public:
    ParticlesRenderer(const ParticleConfiguration& config);

    void update(float fTimeElapsed);

    void render();

    Emetter * getEmetter();

    ~ParticlesRenderer();

protected:
    Emetter _emetter;

    GLuint  _iVAO;
    GLuint  _iEBO;

    GLuint  _iVBOPosition;
    GLuint  _iVBOColor;
    GLuint  _iVBORotation;
    GLuint  _iVBOSize;

    GLuint  _iAttribPosition;
    GLuint  _iAttribColor;
    GLuint  _iAttribRotation;
    GLuint  _iAttribSize;

    GLuint  _iTexture;
};

#endif // PARTICLESRENDERER_H
