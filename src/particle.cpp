#include "particle.h"

#include <iostream>
#include <math.h>



Particle::Particle(const glm::vec3 &pos, const glm::vec3 &velocity, float lifeTime) :
    _position(pos),
    _velocity(velocity),
    _color(1.f, 1.f, 1.f),
    _lifeTime(lifeTime),
    _rotation(0.f),
    _rotationVelocity(2.f),
    _size(1.f),
    _deathSize(1.f)
{

}

Particle::Particle(const glm::vec3 &pos, const glm::vec3 &velocity) :
    _position(pos),
    _velocity(velocity),
    _color(1.f, 1.f, 1.f),
    _lifeTime(-1),
    _rotation(0.f),
    _rotationVelocity(2.f),
    _size(1.f),
    _deathSize(1.f)
{

}


Particle::Particle() :
    _position(0),
    _velocity(0),
    _color(1.f, 1.f, 1.f),
    _lifeTime(-1),
    _rotation(0.f),
    _rotationVelocity(2.f),
    _size(1.f),
    _deathSize(1.f)
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

float Particle::getSize() const
{
    return _size;
}

void Particle::setSize(float size) {
    _size = size;
}

float Particle::getDeathSize() const
{
    return _deathSize;
}

void Particle::setDeathSize(float size) {
    _deathSize = size;
}

const glm::vec3 &Particle::getColor() const
{
    return _color;
}

void Particle::setColor(const glm::vec3 &color) {
    _color = color;
}

