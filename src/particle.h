#ifndef PARTICLE_H
#define PARTICLE_H

#include <glm/vec3.hpp>

class Particle
{
public:
    Particle(const glm::vec3&  pos, const glm::vec3& velocity, float lifeSpan);
    void update(float time);
    bool isAlive();
    const glm::vec3& getPosition() const;

    static glm::vec3 _gravityForce;
    static glm::vec3 _windForceRight;
    static glm::vec3 _windForceLeft;

protected:
    glm::vec3  _pos;
    glm::vec3 _velocity;


    float _totalTime;
    float _lifeSpan;

};

#endif // PARTICLE_H
