#ifndef PARTICLE_H
#define PARTICLE_H

#include "vector"

#include "ngl/Obj.h"
#include "ngl/Colour.h"


/// @file Particle.h
/// @brief the base unit of this lagrangian simulation
/// @author Shalin K Banjara
/// @version 1.0
/// @date 23.10.11
/// Revision History :
/// Initial Version 23.10.11
/// @class Particle
/// @brief base particle class to all others, it is used and extended to represent fluid and passive rigid bodies

class Particle
{
public:
    /// @brief ctor
    /// @param[in] _id the id of the particle
    /// @param[in] _mass the mass of the particle
    /// @param[in] _position the position of the particle
    /// @param[in] _colour the colour of the particle
    /// @param[in] _collisionRadius the radius of the particle
    /// @param[in] _drawRadius radius of the spheres to be drawn
    /// @param[in] _velocity the velocity of the particle
    /// @param[in] _acceleration the acceleration of the particle
    /// @param[in] _netForce the net force acting on the partcile
    Particle
        (
            const int _id,
            const ngl::Real _mass,
            const ngl::Vector _position,
            const ngl::Colour _colour,
            const ngl::Real _collisionRadius,
            const ngl::Real _drawRadius,
            const ngl::Vector _velocity = 0,
            const ngl::Vector _acceleration = 0,
            const ngl::Vector _netForce = 0
        );

    /// @brief dtor
    ~Particle();

    /// @brief return the id
    inline int getId() const { return m_id; }

    /// @brief sets the id
    /// @param[in] _id the id
    inline void setId(const int _id) { m_id = _id; }

    /// @brief return the mass
    inline ngl::Real getMass() const { return m_mass; }

    /// @brief return the last position
    inline ngl::Vector getLastPosition() const { return m_lastPosition; }

    /// @brief return the current position
    inline ngl::Vector getCurrentPosition() const { return m_currentPosition; }

    /// @brief sets the position
    /// @param[in] _p the new position
    void setPosition(const ngl::Vector _p);

    /// @brief update the position to a new value
    /// @param[in] _p the new position
    void updatePosition(const ngl::Vector _p);

    /// @brief return the last velocity
    inline ngl::Vector getLastVelocity() const { return m_lastVelocity; }

    /// @brief return the current velocity
    inline ngl::Vector getCurrentVelocity() const { return m_currentVelocity; }

    /// @brief sets the velocity
    /// @param[in] _v the new velocity
    void setVelocity(const ngl::Vector _v);

    /// @brief update the velocity to a new value
    /// @param[in] _v the new velocity
    void updateVelocity(const ngl::Vector _v);

    /// @brief reset all the forces
    inline void resetNetForce() { m_netForce = 0; }

    /// @brief return the Net Force
    inline ngl::Vector getNetForce() const { return m_netForce; }

    /// @brief set the Net Force
    /// @param[in] _netForce the new Net Force
    inline void setNetForce(const ngl::Vector _netForce) { m_netForce = _netForce; }

    /// @brief return the last acceleration
    inline ngl::Vector getLastAcceleration() const { return m_lastAcceleration; }

    /// @brief return the current acceleration
    inline ngl::Vector getCurrentAcceleration() const { return m_currentAcceleration; }

    /// @brief set the acceleration
    /// @param[in] _acceleration the new acceleration
    void setAcceleration(const ngl::Vector _acceleration);

    /// @brief update the acceleration
    /// @param[in] _acceleration the new acceleration
    void updateAcceleration(const ngl::Vector _acceleration);

    /// @brief return the radius
    inline ngl::Real getCollisionRadius() const { return m_collisionRadius; }

    /// @brief set the radius
    /// @param[in] _radius the radius
    inline void setCollisionRadius(const ngl::Real _radius) { m_collisionRadius = _radius; }

    /// @brief return the radius
    inline ngl::Real getDrawRadius() const { return m_drawRadius; }

    /// @brief set the radius
    /// @param[in] _radius the radius
    inline void setDrawRadius(const ngl::Real _radius) { m_drawRadius = _radius; }

    /// @brief return the current colour
    inline ngl::Colour getParticleColour() const { return m_particleColour; }

    /// @brief set the colour
    /// @param[in] _c the new colour
    inline void setParticleColour(const ngl::Colour _c) { m_particleColour = _c; }

protected:
    /// @brief unique id of the particle
    int m_id;

    /// @brief mass of the particle
    ngl::Real m_mass;

    /// @brief last position of the particle
    ngl::Vector m_lastPosition;

    /// @brief position of the particle
    ngl::Vector m_currentPosition;

    /// @brief last velocity of the particle
    ngl::Vector m_lastVelocity;

    /// @brief velocity of the particle
    ngl::Vector m_currentVelocity;

    /// @brief last acceleration of the particle
    ngl::Vector m_lastAcceleration;

    /// @brief acceleration of the particle
    ngl::Vector m_currentAcceleration;

    /// @brief net force on the particle
    ngl::Vector m_netForce;

    /// @brief draw sphere radius
    ngl::Real m_drawRadius;

    /// @brief collision sphere radius
    ngl::Real m_collisionRadius;

    /// @brief colour of particle for rendering
    ngl::Colour m_particleColour;
};

#endif // PARTICLE_H
