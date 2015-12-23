#ifndef PARTICLECONFIGURATION_H
#define PARTICLECONFIGURATION_H

#include <QString>
#include <QVector>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <QJsonObject>

class ParticleConfiguration
{
public:
    ParticleConfiguration();
    ParticleConfiguration(const QString& json);

    template<typename T> class Range {
    public:
        T min,max;
        Range(T pmin, T pmax) :min(pmin), max(pmax) {}
        Range() : min(0), max(0) {}
    };


    const QVector<glm::vec3>& getForces() const;
    const Range<glm::vec3>& getInitialSpeed() const;
    const QString& getImage() const;
    int getMaxParticles() const;
    float getLifeTime() const;
    float getCreationTime() const;
    const glm::vec4& getColor() const;

    bool isAttractForceSet() const;
    const glm::vec3& getAttractForcePoint() const;
    float getAttractForceNorm() const;

    float getRotationVelocity() const;

    const Range<float>& getSize() const;

protected:
    glm::vec3 parseVec3(const QJsonObject &obj);
    Range<glm::vec3> parseRangeVec3(const QJsonObject &obj);
    glm::vec4 parseColor(const QJsonObject& obj);


    QVector<glm::vec3> _forces;
    Range<glm::vec3> _initialSpeed;
    Range<float> _size;
    QString _image;
    int _maxParticles;
    float _lifeTime;
    float _creationTime;
    glm::vec4 _color;

    bool _attractForceSet;
    glm::vec3 _attractForcePoint;
    float _attractForceNorm;
    float _rotationVelocity;
};

#endif // PARTICLECONFIGURATION_H
