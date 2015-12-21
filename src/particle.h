#ifndef PARTICLE_H
#define PARTICLE_H

#include <glm/vec3.hpp>

class Particle
{
public:
    Particle(const glm::vec3 &pos, const glm::vec3 &velocity, float lifeTime);
    Particle(const glm::vec3 &pos, const glm::vec3 &velocity);
    Particle();

    bool isAlive() const;

    const glm::vec3& getPosition() const;
    void setPosition(const glm::vec3 &position);

    const glm::vec3& getVelocity() const;
    void setVelocity(const glm::vec3 &velocity);

    float getLifeTime() const;
    void setLifeTime(float lifeTime);

protected:
    glm::vec3 _position;
    glm::vec3 _velocity;
    float _lifeTime;

};

#endif // PARTICLE_H
