#ifndef EMETTER_H
#define EMETTER_H

#include <vector>
#include <list>

#include "particle.h"
#include "particleconfiguration.h"

#include <QOpenGLFunctions_3_3_Core>


class Emetter
{
public:
    Emetter(const ParticleConfiguration& config);


    glm::vec3* getData();
    glm::vec4* getColorData();
    GLfloat* getRotationData();
    GLfloat* getSizeData();

    unsigned int getSize() const;

    const ParticleConfiguration& getConfiguration() const;

    void setActive(bool active);
    bool isActive() const;

    void update(float time);
    void setOrigin(const glm::vec3& origin);

    bool useCustomAttractPoint() const;
    void setUseCustomAttractPoint(bool use);

    void setCustomAttractPoint(const glm::vec3& point);

    glm::vec3 mouse;

    ~Emetter();

protected:
    glm::vec3 randomVec3(const ParticleConfiguration::Range<glm::vec3>& vec);
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
};

#endif // EMETTER_H
