#ifndef PARTICLE_H
#define PARTICLE_H

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

/**
 * @brief A particle contains atomic data physic : position, speed, lifetime, rotation, size....
 */
class Particle
{
public:
    /**
     * @brief Creates a new particle with a position, speed and a maximum lifetime
     * @param pos position of the particle
     * @param velocity speed of the particle
     * @param lifeTime maxs lifetime of the particle
     */
    Particle(const glm::vec3 &pos, const glm::vec3 &velocity, float lifeTime);

    /**
     * @brief Creates a new particle with a position, speed and a maximum lifetime
     * @param pos position of the particle
     * @param velocity speed of the particle
     */
    Particle(const glm::vec3 &pos, const glm::vec3 &velocity);

    /**
     * @brief Creates a new particle
     */
    Particle();

    /**
     * @brief A particle is alive if its life time is in [0 - max lifetime]
     * @return true is the particle is alive
     */
    bool isAlive() const;


    const glm::vec3& getPosition() const;
    void setPosition(const glm::vec3 &position);

    const glm::vec3& getVelocity() const;
    void setVelocity(const glm::vec3 &velocity);

    float getLifeTime() const;
    void setLifeTime(float lifeTime);

    float getRotation() const;
    void setRotation(float rotation);

    float getRotationVelocity() const;
    void setRotationVelocity(float rotationVelocity);

    float getSize() const;
    void setSize(float size);

    float getDeathSize() const;
    void setDeathSize(float size);

    const glm::vec3& getColor() const;
    void setColor(const glm::vec3& color);

protected:
    glm::vec3 _position;
    glm::vec3 _velocity;
    glm::vec3 _color;
    float _lifeTime;

    float _rotation;
    float _rotationVelocity;

    float _size;
    float _deathSize;

};

#endif // PARTICLE_H
