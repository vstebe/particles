#ifndef EMETTER_H
#define EMETTER_H

#include <vector>
#include <list>

#include "particle.h"
#include "particleconfiguration.h"

#include <QOpenGLFunctions_3_3_Core>

/**
 * @brief The Emitter handles a particle system. It creates particles and updates physic stuff.
 */
class Emetter
{
public:
    /**
     * @brief Constructs a new emitter using the given configuration
     * @param config reference to the json configuration to use
     */
    Emetter(const ParticleConfiguration& config);

    /**
     * @brief Get the positions data ready to use in the VBO
     * @return vec3 position array of particles
     */
    glm::vec3* getData();

    /**
     * @brief Get the color data ready to use in the VBO
     * @return vec4 color of particles
     */
    glm::vec4* getColorData();

    /**
     * @brief Get the rotation data ready to use in the VBO. Each value is a radian-angle.
     * @return float angle of particles
     */
    GLfloat* getRotationData();

    /**
     * @brief Get the size data ready to use in the VBO.
     * @return vec3 size of particles
     */
    GLfloat* getSizeData();

    /**
     * @brief Get the number of particles (data size)
     * @return data size
     */
    unsigned int getSize() const;

    /**
     * @brief Get the particle configuration used by this emitter
     * @return particle configuration
     */
    const ParticleConfiguration& getConfiguration() const;

    /**
     * @brief Set the activity of the emitter. An inative emitter stops to create particles.
     * @param active true if the emitter should continue to create particles, false otherwise
     */
    void setActive(bool active);

    /**
     * @brief Returns true if the emitter is active. An inative emitter stops to create particles.
     * @return true if the emitter continues to create particles, false otherwise
     */
    bool isActive() const;

    /**
     * @brief Updates particles physic (position, speed, rotation...) and creates new particles if the emitter is active
     * @param time time since the last update
     */
    void update(float time);

    /**
     * @brief Set the point where the particles are created
     * @param origin origin point of particles
     */
    void setOrigin(const glm::vec3& origin);

    /**
     * @brief Returns true if the emitter is using a custom attract point (probably the mouse). A custom attract point overrides the point given by the configuration.
     * @return true if a custom point is used
     * @see setUseCustomAttractPoint()
     * @see setCustomAttractPoint()
     */
    bool useCustomAttractPoint() const;

    /**
     * @brief Set if the emitter shoud use a custom attraction point. A custom attract point overrides the point given by the configuration.
     * @param use true if the emitter use a custom attract point, false otherwise
     */
    void setUseCustomAttractPoint(bool use);

    /**
     * @brief Set the custom attract point. If useCustomAttractPoint is set to false, this function has no effect.
     * @param point the attract point to use
     */
    void setCustomAttractPoint(const glm::vec3& point);

    /**
     * @brief Set if the particles must bounce on floor and ceil.
     * @param use true if the particles muse bounce
     */
    void setUseFloorCeil(bool use);

    glm::vec3 mouse;

    ~Emetter();

protected:
    /**
     * @brief Returns a random vec3 in the given range
     * @param vec range where the random vec3 must be
     * @return random vec3
     */
    glm::vec3 randomVec3(const ParticleConfiguration::Range<glm::vec3>& vec);

    /**
     * @brief Returns the alpha color particle from a life with a fading effect. The alpha value is is [0,1]
     * @param life life of the particle
     * @return alpha color value of the particle
     */
    float getAlphaFromLife(float life);

    glm::vec3 * _data;
    glm::vec4 * _colorData;
    float * _rotationData;
    float * _sizeData;

    ParticleConfiguration _config;
    glm::vec3 _origin;
    bool _isActive;

    QVector<Particle> _particles;

    float _timeLastCreation;

    bool _useCustomAttractPoint;
    glm::vec3 _customAttractPoint;

    bool _useFloorCeil;
};

#endif // EMETTER_H
