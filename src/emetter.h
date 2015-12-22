#ifndef EMETTER_H
#define EMETTER_H

#include <vector>
#include <list>

#include "particle.h"
#include "particleconfiguration.h"

class Emetter
{
public:
    Emetter(const QString& filename);
    glm::vec3* getData();
    glm::vec4* getColorData();

    unsigned int getSize() const;

    const ParticleConfiguration& getConfiguration() const;

    void setActive(bool active);
    bool isActive() const;

    void update(float time);
    void setOrigin(const glm::vec3& origin);

    glm::vec3 mouse;

protected:
    glm::vec3 randomInitialSpeed();

    glm::vec3 * _data;
    glm::vec4 * _colorData;

    ParticleConfiguration _config;
    glm::vec3 _origin;
    bool _isActive;

    QVector<Particle> _particles;

    float _timeLastCreation;
};

#endif // EMETTER_H
