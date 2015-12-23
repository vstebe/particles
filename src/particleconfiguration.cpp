#include "particleconfiguration.h"

#include <QFile>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QDebug>


ParticleConfiguration::ParticleConfiguration() :
    _image(""),
    _attractForceNorm(0.f),
    _attractForcePoint(0),
    _attractForceSet(false),
    _forces(),
    _initialSpeed()
{

}

ParticleConfiguration::ParticleConfiguration(const QString& json)
{

    QJsonDocument doc = QJsonDocument::fromJson(json.toUtf8());

    QJsonObject root = doc.object();
    QJsonArray forces = root["forces"].toArray();

    for(int i=0; i<forces.size(); i++) {
        QJsonObject jsonForce = forces[i].toObject();
         _forces.append(parseVec3(jsonForce));
    }

    if(root["initial-speed"].toObject()["x"].isDouble()) {
        glm::vec3 value = parseVec3(root["initial-speed"].toObject());
        _initialSpeed = Range<glm::vec3>(value,value);
    } else {
        _initialSpeed = parseRangeVec3(root["initial-speed"].toObject());
    }


    _image = root["image"].toString();

    _lifeTime = (float) root["life-time"].toDouble();

    _maxParticles = root["max-particles"].toInt();

    _creationTime = (float) root["creation-time"].toDouble();


    if(root["attract-force"].isObject()) {
        _attractForceSet = true;
        _attractForcePoint = parseVec3(root["attract-force"].toObject()["point"].toObject());
        _attractForceNorm = root["attract-force"].toObject()["norm"].toDouble();
    } else {
        _attractForceSet = false;
    }

    _rotationVelocity = 0.f;
    if(root["rotation-velocity"].isDouble())
        _rotationVelocity = root["rotation-velocity"].toDouble();

    _size = Range<float>(0.1f, 0.1f);
    if(root["size"].isObject()) {
        _size = Range<float>(root["size"].toObject()["min"].toDouble(),
                             root["size"].toObject()["max"].toDouble());
    } else if(root["size"].isDouble()) {
        _size = Range<float>(root["size"].toDouble(), root["size"].toDouble());
    }

    _deathSize = _size;
    if(root["death-size"].isObject()) {
        _deathSize = Range<float>(root["death-size"].toObject()["min"].toDouble(),
                             root["death-size"].toObject()["max"].toDouble());
    } else if(root["death-size"].isDouble()) {
        _deathSize = Range<float>(root["death-size"].toDouble(), root["death-size"].toDouble());
    }

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
