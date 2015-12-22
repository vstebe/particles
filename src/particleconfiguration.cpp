#include "particleconfiguration.h"

#include <QFile>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QDebug>

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

    _color = parseColor(root["color"].toObject());

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
    glm::vec3 min(obj["x"].toObject()["min"].toDouble(),
                  obj["y"].toObject()["min"].toDouble(),
                  obj["z"].toObject()["min"].toDouble());
    glm::vec3 max(obj["x"].toObject()["max"].toDouble(),
                  obj["y"].toObject()["max"].toDouble(),
                  obj["z"].toObject()["max"].toDouble());
    return ParticleConfiguration::Range<glm::vec3>(min, max);
}

glm::vec4 ParticleConfiguration::parseColor(const QJsonObject &obj)
{
    float r = (float) obj["r"].toDouble();
    float g = (float) obj["g"].toDouble();
    float b = (float) obj["b"].toDouble();
    float a;
    if(obj["a"].isDouble())
        a = obj["a"].toDouble();
    else
        a = 1.f;
    return glm::vec4(r,g,b,a);
}

float ParticleConfiguration::getLifeTime() const
{
    return _lifeTime;
}

float ParticleConfiguration::getCreationTime() const
{
    return _creationTime;
}

const glm::vec4 &ParticleConfiguration::getColor() const
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
