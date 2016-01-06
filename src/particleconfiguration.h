#ifndef PARTICLECONFIGURATION_H
#define PARTICLECONFIGURATION_H

#include <QString>
#include <QVector>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <QJsonObject>

/**
 * @brief Contains the whole configuration of a particle system. It includes forces, gravity, color data etc.. The configuration is loaded from a JSON document.
 */
class ParticleConfiguration
{
public:
    /**
     * @brief Creates a configuration with initial values
     */
    ParticleConfiguration();

    /**
     * @brief Parse a json document and creates the configuration
     * @param json json configuration
     */
    ParticleConfiguration(const QString& json);


    template<typename T>
    /**
     * @brief A range is useful for random values, we might want to generate for example a size (float) between 0 and 0.2
     */
    class Range {
    public:
        T min,max;
        Range(T pmin, T pmax) :min(pmin), max(pmax) {}
        Range() : min(0), max(0) {}
    };


    /**
     * @brief Returns the forces of the particle system
     * @return forces vector
     */
    const QVector<glm::vec3>& getForces() const;

    /**
     * @brief Returns the initial speed
     * @return initial vector speed
     */
    const Range<glm::vec3>& getInitialSpeed() const;

    /**
     * @brief Returns the texture used by particles
     * @return image filename
     */
    const QString& getImage() const;

    /**
     * @brief Get the maximum number of displayed particles
     * @return max value
     */
    int getMaxParticles() const;

    /**
     * @brief Get the maximum life time of the particle
     * @return life time in seconds
     */
    float getLifeTime() const;

    /**
     * @brief Get the time between two particles creation
     * @return time in seconds
     */
    float getCreationTime() const;

    /**
     * @brief Get a range of color to use to generate the random color of the particle
     * @return color range
     */
    const Range<glm::vec3>& getColor() const;

    /**
     * @brief Returns true if a attract force is used and set
     * @return true if there is an attract force
     */
    bool isAttractForceSet() const;

    /**
     * @brief Returns the attract force point
     * @return attraction point
     */
    const glm::vec3& getAttractForcePoint() const;

    /**
     * @brief Returns the norm to use for the attract force
     * @return attraction norm
     */
    float getAttractForceNorm() const;

    /**
     * @brief Returns the particle rotation velocity
     * @return velocity in rad/s
     */
    float getRotationVelocity() const;

    /**
     * @brief Get a range of size when the particle is created
     * @return size range
     */
    const Range<float>& getSize() const;

    /**
     * @brief Get a range of size when the particle is killed
     * @return size range
     */
    const Range<float>& getDeathSize() const;

protected:
    glm::vec3 parseVec3(const QJsonObject &obj);
    Range<glm::vec3> parseRangeVec3(const QJsonObject &obj);


    QVector<glm::vec3> _forces;
    Range<glm::vec3> _initialSpeed;
    Range<float> _size;
    Range<float> _deathSize;
    QString _image;
    int _maxParticles;
    float _lifeTime;
    float _creationTime;
    Range<glm::vec3> _color;

    bool _attractForceSet;
    glm::vec3 _attractForcePoint;
    float _attractForceNorm;
    float _rotationVelocity;
};

#endif // PARTICLECONFIGURATION_H
