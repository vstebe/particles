#include "emetter.h"

#include <glm/glm.hpp>

#include <iostream>
#include <QtGlobal>
#include <QDebug>
Emetter::Emetter(const ParticleConfiguration &config) :
    _data(NULL),
    _colorData(NULL),
    _origin(0.f, 0.f, 0.f),
    _isActive(true),
    _timeLastCreation(0),
    _config(config),
    _useCustomAttractPoint(false)
{
    _data = new glm::vec3[_config.getMaxParticles()];
    _colorData = new glm::vec4[_config.getMaxParticles()];
    _rotationData = new GLfloat[_config.getMaxParticles()];
    _sizeData = new GLfloat[_config.getMaxParticles()];

    _particles.clear();

    for(int i = 0; i<_config.getMaxParticles(); i++) {
        _data[i] = glm::vec3(0.f, 1.f, 0.f);
        _colorData[i] = _config.getColor();
        _rotationData[i] = 0.f;
        _sizeData[i] = 0.1f;
        _particles.push_back(Particle(_origin, randomInitialSpeed()));
        _particles.back().setRotationVelocity(_config.getRotationVelocity());

    }

}


void Emetter::setOrigin(const glm::vec3 &origin) {
    _origin = origin;
}

bool Emetter::useCustomAttractPoint() const
{
    return _useCustomAttractPoint;
}

void Emetter::setUseCustomAttractPoint(bool use)
{
    _useCustomAttractPoint = use;
}

void Emetter::setCustomAttractPoint(const glm::vec3 &point)
{
    _customAttractPoint = point;
}

Emetter::~Emetter()
{
    delete _data;
    delete _colorData;
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

float Emetter::getAlphaFromLife(float life)
{
    float progress = life / _config.getLifeTime();
    progress = 1 - progress;

    if(progress < 0.25f)
        return progress * 4.f;

    progress -= 0.25f;
    return 1 - progress/0.75f;
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

glm::vec4 *Emetter::getColorData()
{
    return _colorData;
}

GLfloat *Emetter::getRotationData()
{
    return _rotationData;
}

GLfloat *Emetter::getSizeData()
{
    return _sizeData;
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

    glm::vec3 attractForcePoint;
    if(_useCustomAttractPoint)
        attractForcePoint = _customAttractPoint;
    else
        attractForcePoint = _config.getAttractForcePoint();

    int nbAlives = 0;

    for(int i=0; i<_particles.size(); i++) {
        if(_particles[i].isAlive()) {
            _particles[i].setLifeTime(_particles[i].getLifeTime() - time);
            _particles[i].setVelocity(_particles[i].getVelocity() + globalForce);

            if(_config.isAttractForceSet()) {
                glm::vec3 attractForce = time * glm::normalize(attractForcePoint - _origin - _particles[i].getPosition());
                attractForce *= _config.getAttractForceNorm();
                _particles[i].setVelocity(_particles[i].getVelocity() + attractForce);
            }

            _particles[i].setRotation(_particles[i].getRotation() + _particles[i].getRotationVelocity() * time);


            _particles[i].setPosition(_particles[i].getPosition() + _particles[i].getVelocity() * time);
        } else if(_isActive && _timeLastCreation >= _config.getCreationTime()) {
            _particles[i].setLifeTime(_config.getLifeTime());
            _particles[i].setVelocity(randomInitialSpeed());
            _particles[i].setPosition(_origin);

            _particles[i].setRotation(0.f);

            _particles[i].setSize(fRandom(_config.getSize().min, _config.getSize().max));

            _timeLastCreation -= _config.getCreationTime();
        } else {
            _particles[i].setPosition(glm::vec3(-99,-99,-99));
        }


        _data[i] = _particles[i].getPosition();
        _colorData[i].a = (GLfloat) getAlphaFromLife(_particles[i].getLifeTime());
        _rotationData[i] = _particles[i].getRotation();
        _sizeData[i] = _particles[i].getSize();


        if(_particles[i].isAlive()) nbAlives++;
    }

    //qDebug() << _colorData[0].a;


}


