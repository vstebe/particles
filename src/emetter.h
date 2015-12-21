#ifndef EMETTER_H
#define EMETTER_H

#include <vector>
#include <list>

#include "particle.h"

class Emetter
{
public:
    Emetter(unsigned int nbParticles);
    glm::vec3* getData();

    unsigned int getSize() const;

    void update(float time);
    void setOrigin(const glm::vec3& origin);


    void setActive(bool isActive);

protected:
    std::list<Particle> _particles;
    unsigned int _nbMaxParticles;
    glm::vec3 * _data;
    float _totalTime;

    float _newParticleTimer;

    glm::vec3 _origin;

    bool _isActive;

};

#endif // EMETTER_H
