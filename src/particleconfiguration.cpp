#include "particleconfiguration.h"

#include <QFile>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QDebug>

ParticleConfiguration::ParticleConfiguration() :
    _forces(),
    _initialSpeed(),
    _size(Range<float>(1.f, 1.f)),
    _deathSize(Range<float>(1.f, 1.f)),
    _image(""),
    _maxParticles(100),
    _lifeTime(2.f),
    _creationTime(.5f),
    _color(Range<glm::vec3>(glm::vec3(1.f, 1.f, 1.f), glm::vec3(1.f, 1.f, 1.f))),
    _attractForceSet(false),
    _attractForcePoint(0),
    _attractForceNorm(-1.f),
    _rotationVelocity(0.f)
{

}

ParticleConfiguration::ParticleConfiguration(const QString& json)
{

    //Load the JSON Document
    QJsonDocument doc = QJsonDocument::fromJson(json.toUtf8());
    QJsonObject root = doc.object();

    //Get forces
    QJsonArray forces = root["forces"].toArray();
    for(int i=0; i<forces.size(); i++) {
        QJsonObject jsonForce = forces[i].toObject();
         _forces.append(parseVec3(jsonForce));
    }

    //Get the initial speed
    if(root["initial-speed"].toObject()["x"].isDouble()) {
        //If the user gave a range
        glm::vec3 value = parseVec3(root["initial-speed"].toObject());
        _initialSpeed = Range<glm::vec3>(value,value);
    } else {
        //If it's not a range
        _initialSpeed = parseRangeVec3(root["initial-speed"].toObject());
    }

    //Main parameters
    _image = root["image"].toString();
    _lifeTime = (float) root["life-time"].toDouble();
    _maxParticles = root["max-particles"].toInt();
    _creationTime = (float) root["creation-time"].toDouble();

    //Get the attract force
    if(root["attract-force"].isObject()) { //If an attract force is set
        _attractForceSet = true;
        _attractForcePoint = parseVec3(root["attract-force"].toObject()["point"].toObject());
        _attractForceNorm = root["attract-force"].toObject()["norm"].toDouble();
    } else { //The attract force is not set
        _attractForceSet = false;
    }

    //Rotation paramaters
    _rotationVelocity = 0.f;
    if(root["rotation-velocity"].isDouble())
        _rotationVelocity = root["rotation-velocity"].toDouble();

    //Size parameters
    _size = Range<float>(0.1f, 0.1f);
    if(root["size"].isObject()) { //Range
        _size = Range<float>(root["size"].toObject()["min"].toDouble(),
                             root["size"].toObject()["max"].toDouble());
    } else if(root["size"].isDouble()) {//Not range
        _size = Range<float>(root["size"].toDouble(), root["size"].toDouble());
    }

    _deathSize = _size;
    if(root["death-size"].isObject()) {
        _deathSize = Range<float>(root["death-size"].toObject()["min"].toDouble(),
                             root["death-size"].toObject()["max"].toDouble());
    } else if(root["death-size"].isDouble()) {
        _deathSize = Range<float>(root["death-size"].toDouble(), root["death-size"].toDouble());
    }

    //Colors parameters
    _color = Range<glm::vec3>(glm::vec3(1.f,1.f,1.f),glm::vec3(1.f,1.f,1.f));
    if(root["color"].isObject()) {
        _color = parseRangeVec3(root["color"].toObject());
    }
}

const QVector<glm::vec3> &ParticleConfiguration::getForces() const
{
    return _forces;
}

glm::vec3 ParticleConfiguration::parseVec3(const QJsonObject &obj)
{
    return glm::vec3(obj["x"].toDouble(0),
            obj["y"].toDouble(0),
            obj["z"].toDouble(0));
}

ParticleConfiguration::Range<glm::vec3> ParticleConfiguration::parseRangeVec3(const QJsonObject &obj)
{
    if(obj["r"].isUndefined()) {
        glm::vec3 min((obj["x"].isObject()) ? obj["x"].toObject()["min"].toDouble() : obj["x"].toDouble(),
                      (obj["y"].isObject()) ? obj["y"].toObject()["min"].toDouble() : obj["y"].toDouble(),
                      (obj["z"].isObject()) ? obj["z"].toObject()["min"].toDouble() : obj["z"].toDouble());
        glm::vec3 max((obj["x"].isObject()) ? obj["x"].toObject()["max"].toDouble() : obj["x"].toDouble(),
                      (obj["y"].isObject()) ? obj["y"].toObject()["max"].toDouble() : obj["y"].toDouble(),
                      (obj["z"].isObject()) ? obj["z"].toObject()["max"].toDouble() : obj["z"].toDouble());

        return ParticleConfiguration::Range<glm::vec3>(min, max);
    } else {
        glm::vec3 min((obj["r"].isObject()) ? obj["r"].toObject()["min"].toDouble() : obj["r"].toDouble(),
                      (obj["g"].isObject()) ? obj["g"].toObject()["min"].toDouble() : obj["g"].toDouble(),
                      (obj["b"].isObject()) ? obj["b"].toObject()["min"].toDouble() : obj["b"].toDouble());
        glm::vec3 max((obj["r"].isObject()) ? obj["r"].toObject()["max"].toDouble() : obj["r"].toDouble(),
                      (obj["g"].isObject()) ? obj["g"].toObject()["max"].toDouble() : obj["g"].toDouble(),
                      (obj["b"].isObject()) ? obj["b"].toObject()["max"].toDouble() : obj["b"].toDouble());

        return ParticleConfiguration::Range<glm::vec3>(min, max);
    }

}



float ParticleConfiguration::getLifeTime() const
{
    return _lifeTime;
}

float ParticleConfiguration::getCreationTime() const
{
    return _creationTime;
}

const ParticleConfiguration::Range<glm::vec3> &ParticleConfiguration::getColor() const
{
    return _color;
}

bool ParticleConfiguration::isAttractForceSet() const
{
    return _attractForceSet;
}

const glm::vec3 &ParticleConfiguration::getAttractForcePoint() const
{
    return _attractForcePoint;
}

float ParticleConfiguration::getAttractForceNorm() const
{
    return _attractForceNorm;
}

float ParticleConfiguration::getRotationVelocity() const
{
    return _rotationVelocity;
}

const ParticleConfiguration::Range<float> &ParticleConfiguration::getSize() const
{
    return _size;
}

const ParticleConfiguration::Range<float> &ParticleConfiguration::getDeathSize() const
{
    return _deathSize;
}

int ParticleConfiguration::getMaxParticles() const
{
    return _maxParticles;
}

const QString& ParticleConfiguration::getImage() const
{
    return _image;
}

const ParticleConfiguration::Range<glm::vec3> &ParticleConfiguration::getInitialSpeed() const
{
    return _initialSpeed;
}
