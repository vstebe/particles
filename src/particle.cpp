#include "particle.h"

#include <iostream>
#include <math.h>

glm::vec3 Particle::_gravityForce(0.f, 0.f, 0.f);
glm::vec3 Particle::_windForceLeft(0.f, 0.f, 0.f);
glm::vec3 Particle::_windForceRight(0.f, 0.f, 0.f);

Particle::Particle(const glm::vec3 &pos, const glm::vec3 &velocity, float lifeSpan) :
    _pos(pos),
    _velocity(velocity),
    _lifeSpan(lifeSpan),
    _totalTime(0.f)
{

}

void Particle::update(float time) {
    float secTime = time / 1000.f;

    glm::vec3 windForce;



    _velocity += (_gravityForce)  * secTime;
    _totalTime += time;

    _pos += _velocity * secTime;


}

bool Particle::isAlive() {
    return _totalTime <= _lifeSpan;
}


const glm::vec3& Particle::getPosition() const {
    return _pos;
}
