#include "emetter.h"

#include <glm/glm.hpp>

#include <iostream>
#include <QtGlobal>
#include <QDebug>

Emetter::Emetter(const ParticleConfiguration &config) :
    _data(NULL),
    _colorData(NULL),
    _rotationData(NULL),
    _sizeData(NULL),
    _config(config),
    _origin(0.f, 0.f, 0.f),
    _isActive(true),
    _particles(),
    _timeLastCreation(0),
    _useCustomAttractPoint(false),
    _useFloorCeil(true)
{
    //Creating the arrays used in VBOs
    _data = new glm::vec3[_config.getMaxParticles()];
    _colorData = new glm::vec4[_config.getMaxParticles()];
    _rotationData = new GLfloat[_config.getMaxParticles()];
    _sizeData = new GLfloat[_config.getMaxParticles()];

    _particles.clear();

    //Data init
    for(int i = 0; i<_config.getMaxParticles(); i++) {
        _data[i] = glm::vec3(0.f, 1.f, 0.f);
        _rotationData[i] = 0.f;
        _sizeData[i] = 0.1f;
        _particles.push_back(Particle(_origin, randomVec3(_config.getInitialSpeed())));
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

void Emetter::setUseFloorCeil(bool use)
{
    _useFloorCeil = use;
}

Emetter::~Emetter()
{
    delete _data;
    delete _colorData;
    delete _sizeData;
    delete _rotationData;
}


float fRandom(float LO, float HI) {
    return LO + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(HI-LO)));
}

glm::vec3 Emetter::randomVec3(const ParticleConfiguration::Range<glm::vec3> &vec)
{
    float x = fRandom(vec.min.x, vec.max.x);
    float y = fRandom(vec.min.y, vec.max.y);
    float z = fRandom(vec.min.z, vec.max.z);
    return glm::vec3(x,y,z);
}



float Emetter::getAlphaFromLife(float life)
{
    //[0%  -  25%] fade
    //[25% -  75%] visible
    //[75% - 100%] fade

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

    //Add all static forces (wind, gravity etc) specified in the configuration
    for(int i=0; i<_config.getForces().size(); i++) {
        globalForce += _config.getForces()[i];
    }
    globalForce *= time;

    //Set the active attract point if the emitter uses it. It's either the point from the configuration or the one set with setCustomAttractPoint();
    glm::vec3 attractForcePoint;
    if(_useCustomAttractPoint)
        attractForcePoint = _customAttractPoint;
    else
        attractForcePoint = _config.getAttractForcePoint();

    //Iterates over the particles
    for(int i=0; i<_particles.size(); i++) {
        //When the particle is alive
        if(_particles[i].isAlive()) {
            //Updates lifetime
            _particles[i].setLifeTime(_particles[i].getLifeTime() - time);
            //Updates speed from static forces
            _particles[i].setVelocity(_particles[i].getVelocity() + globalForce);

            //Attract point
            if(_config.isAttractForceSet()) {
                glm::vec3 attractForce = time * glm::normalize(attractForcePoint - _origin - _particles[i].getPosition());
                //Simple attract force
                if(_config.getAttractForceNorm() != -1.f) {
                    attractForce *= _config.getAttractForceNorm();
                //Experimental attract force, simulates a gravity point
                } else {
                    float dis = glm::distance(attractForcePoint, _particles[i].getPosition());
                    float norm =  1.f / (dis * dis);;
                    if(norm > 3.f) norm = 3.f;


                    attractForce *= norm;
                }
                //Updates the velocity
                _particles[i].setVelocity(_particles[i].getVelocity() + attractForce);
            }

            //Updates rotation
            _particles[i].setRotation(_particles[i].getRotation() + _particles[i].getRotationVelocity() * time);

            //Updates position
            _particles[i].setPosition(_particles[i].getPosition() + _particles[i].getVelocity() * time);

            //If wanted, the particle must bounce if it hits the floor or the ceil.
            if(_useFloorCeil) {

                if(_particles[i].getPosition().y >= 1.f) { //Ceil
                    _particles[i].setVelocity(glm::reflect(_particles[i].getVelocity(), glm::vec3(0.f, 1.f, 0.f)));
                }
                if(_particles[i].getPosition().y <= -1.f) { //Floor
                    _particles[i].setVelocity(glm::reflect(_particles[i].getVelocity(), glm::vec3(0.f, -1.f, 0.f)));
                }
            }

        //The particle is dead, but we want to create a new particle -> init
        } else if(_isActive && _timeLastCreation >= _config.getCreationTime()) {
            //Set the initial lifetime
            _particles[i].setLifeTime(_config.getLifeTime());

            //Set the initial velocity (random)
            _particles[i].setVelocity(randomVec3(_config.getInitialSpeed()));

            //Set the initial position (emitter origin)
            _particles[i].setPosition(_origin);

            //Rotation
            _particles[i].setRotation(0.f);

            //Color
            _particles[i].setColor(randomVec3(_config.getColor()));

            //Size (random)
            _particles[i].setSize(fRandom(_config.getSize().min, _config.getSize().max));
            //Death size (random)
            _particles[i].setDeathSize(fRandom(_config.getDeathSize().min, _config.getDeathSize().max));

            //We just created a particle : update the generator time
            _timeLastCreation -= _config.getCreationTime();
        //The particle is dead and we don't want to create a new one. Hiding the particle
        } else {
            _particles[i].setPosition(glm::vec3(-99,-99,-99));
        }

        //Update arrays for VBO
        _data[i] = _particles[i].getPosition();
        _colorData[i] = glm::vec4(_particles[i].getColor(), (GLfloat) getAlphaFromLife(_particles[i].getLifeTime()));
        _rotationData[i] = _particles[i].getRotation();

        //Set particle size
        float progress = _particles[i].getLifeTime() / _config.getLifeTime();
        progress = 1 - progress;

        _sizeData[i] = _particles[i].getSize() + progress * (_particles[i].getDeathSize() - _particles[i].getSize());

    }



}


