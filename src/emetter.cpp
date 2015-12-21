#include "emetter.h"

#include <iostream>
#include <QtGlobal>

Emetter::Emetter(unsigned int nbParticles) :
    _nbMaxParticles(nbParticles),
    _totalTime(0.f),
    _newParticleTimer(0.f),
    _origin(0.f, 0.f, 0.f),
    _isActive(false)
{
    _data = new glm::vec3[_nbMaxParticles];

    for(unsigned int i = 0; i<_nbMaxParticles; i++) {
        _data[i] = glm::vec3(0.f, 0.f, 0.f);
    }
}

void Emetter::setOrigin(const glm::vec3 &origin) {
    _origin = origin;
}

float fRandom() {
    float r = qrand();
    return 1-2*r/RAND_MAX;
}

void Emetter::setActive(bool isActive) {
     _isActive = isActive;
}

void Emetter::update(float time) {
    _totalTime += time;
    _newParticleTimer += time;

    std::list<Particle>::iterator iter = _particles.begin();
    while (iter != _particles.end())
    {
        iter->update(time);
        if (iter->isAlive() == true)
        {
            ++iter;
        }
        else
        {
            iter = _particles.erase(iter);
        }
    }

    while(_newParticleTimer > 10 && _particles.size() < _nbMaxParticles) {
        if(_isActive)
            _particles.push_back(Particle(_origin, glm::vec3(fRandom(), fRandom(), fRandom()), 500.f));
        _newParticleTimer -= 10;
    }

    int index = 0;
    iter = _particles.begin();
    while (iter != _particles.end()) {
        _data[index] = iter->getPosition();
        index ++;
        iter++;
    }
    for(unsigned int i=index; i<_nbMaxParticles; i++) {
        _data[i] = glm::vec3(0.f);
    }
}


unsigned int Emetter::getSize() const {
    return _nbMaxParticles;
}

glm::vec3 * Emetter::getData() {
    return _data;
}
