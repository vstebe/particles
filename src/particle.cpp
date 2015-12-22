#include "particle.h"

#include <iostream>
#include <math.h>



Particle::Particle(const glm::vec3 &pos, const glm::vec3 &velocity, float lifeTime) :
    _position(pos),
    _velocity(velocity),
    _lifeTime(lifeTime),
    _rotation(0.f),
    _rotationVelocity(2.f)
{

}

Particle::Particle(const glm::vec3 &pos, const glm::vec3 &velocity) :
    _position(pos),
    _velocity(velocity),
    _lifeTime(-1),
    _rotation(0.f),
    _rotationVelocity(2.f)
{

}

Particle::Particle() :
    _position(0),
    _velocity(0),
    _lifeTime(-1),
    _rotation(0.f),
    _rotationVelocity(2.f)
{

}


bool Particle::isAlive() const {
    return _lifeTime > 0;
}


const glm::vec3& Particle::getPosition() const {
    return _position;
}


void Particle::setPosition(const glm::vec3 &position)
{
    _position = position;
}

const glm::vec3& Particle::getVelocity() const
{
    return _velocity;
}

void Particle::setVelocity(const glm::vec3 &velocity)
{
    _velocity = velocity;
}

float Particle::getLifeTime() const
{
    return _lifeTime;
}

void Particle::setLifeTime(float lifeTime)
{
    _lifeTime = lifeTime;
}

float Particle::getRotation() const
{
    return _rotation;
}

void Particle::setRotation(float rotation) {
    _rotation = rotation;
    while(_rotation > 2.f*M_PI) _rotation-= 2.f*M_PI;
}

float Particle::getRotationVelocity() const {
    return _rotationVelocity;
}

void Particle::setRotationVelocity(float rotationVelocity)
{
    _rotationVelocity = rotationVelocity;
}

