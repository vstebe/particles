#ifndef PARTICLESRENDERER_H
#define PARTICLESRENDERER_H

#include "emetter.h"
#include "particleconfiguration.h"

#include <QOpenGLFunctions_3_3_Core>

/**
 * @brief Renders the particles. The renderer create an emitter, updates it and draw the particles
 */
class ParticlesRenderer :  protected QOpenGLFunctions_3_3_Core
{
public:
    /**
     * @brief Creates a renderer
     * @param config json config file to use
     */
    ParticlesRenderer(const ParticleConfiguration& config);

    /**
     * @brief Update the emitter
     * @param fTimeElapsed time since the last update
     */
    void update(float fTimeElapsed);

    /**
     * @brief Draw the particles
     */
    void render();

    /**
     * @brief Get the emitter which is used
     * @return
     */
    Emetter * getEmetter();

    ~ParticlesRenderer();

protected:
    Emetter _emetter;

    GLuint  _iVAO;

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
