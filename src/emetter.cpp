#include "emetter.h"

#include <iostream>
#include <QtGlobal>
#include <QDebug>
Emetter::Emetter(const QString& filename) :
    _config(filename),
    _origin(0.f, 0.f, 0.f),
    _isActive(false),
    _timeLastCreation(0)
{
    _data = new glm::vec3[_config.getMaxParticles()];

    for(int i = 0; i<_config.getMaxParticles(); i++) {
        _data[i] = glm::vec3(0.f, 1.f, 0.f);
    }

    for(int i = 0; i<_config.getMaxParticles(); i++) {
        _particles.push_back(Particle(_origin, randomInitialSpeed()));
    }

}



void Emetter::setOrigin(const glm::vec3 &origin) {
    _origin = origin;
}

float fRandom(float LO, float HI) {
    return LO + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(HI-LO)));
}

glm::vec3 Emetter::randomInitialSpeed()
{
    float x = fRandom(_config.getInitialSpeed().min.x, _config.getInitialSpeed().max.x);
    float y = fRandom(_config.getInitialSpeed().min.y, _config.getInitialSpeed().max.y);
    float z = fRandom(_config.getInitialSpeed().min.z, _config.getInitialSpeed().max.z);
    return glm::vec3(x,y,z);
}




unsigned int Emetter::getSize() const {
    return _config.getMaxParticles();
}

const ParticleConfiguration &Emetter::getConfiguration() const
{
    return _config;
}

glm::vec3 * Emetter::getData() {
    return _data;
}

void Emetter::setActive(bool active)
{
    _isActive = active;
}

bool Emetter::isActive() const
{
    return _isActive;
}

void Emetter::update(float time)
{

    _timeLastCreation += time;

    glm::vec3 globalForce(0.f, 0.f, 0.f);

    for(int i=0; i<_config.getForces().size(); i++) {
        globalForce += _config.getForces()[i];
    }

    globalForce *= time;

    int nbAlives = 0;

    for(int i=0; i<_particles.size(); i++) {
        if(_particles[i].isAlive()) {
            _particles[i].setLifeTime(_particles[i].getLifeTime() - time);
            _particles[i].setVelocity(_particles[i].getVelocity() + globalForce);
            _particles[i].setPosition(_particles[i].getPosition() + _particles[i].getVelocity() * time);
        } else if(_isActive && _timeLastCreation >= _config.getCreationTime()) {
            _particles[i].setLifeTime(_config.getLifeTime());
            _particles[i].setVelocity(randomInitialSpeed());
            _particles[i].setPosition(_origin);

            _timeLastCreation -= _config.getCreationTime();
        }

        _data[i] = _particles[i].getPosition();

        if(_particles[i].isAlive()) nbAlives++;
    }

}


