/// @file Particle.cpp
/// @brief the parent class that represents a particle in space and used to represent all entities, such as the fluid particle and rbd

#include "Particle.h"

Particle::Particle
            (
                const int _id,
                const ngl::Real _mass,
                const ngl::Vector _position,
                const ngl::Colour _colour,
                const ngl::Real _collisionRadius,
                const ngl::Real _drawRadius,
                const ngl::Vector _velocity,
                const ngl::Vector _acceleration,
                const ngl::Vector _netForce
            )
{
    //init params
    m_id = _id;

    m_mass = _mass;

    m_currentPosition = _position;
    m_lastPosition = m_currentPosition;

    m_currentVelocity = _velocity;
    m_lastVelocity = m_currentVelocity;

    m_currentAcceleration = _acceleration;
    m_lastAcceleration = m_currentAcceleration;

    m_netForce = _netForce;

    m_collisionRadius = _collisionRadius;

    m_drawRadius = _drawRadius;

    m_particleColour = _colour;
}

Particle::~Particle()
{
}

void Particle::setPosition(const ngl::Vector _position)
{
    //set positions to new value
    m_lastPosition = _position;
    m_currentPosition = _position;
}

void Particle::updatePosition(const ngl::Vector _position)
{
    //save current value
    m_lastPosition = m_currentPosition;

    //update to new value
    m_currentPosition = _position;
}

void Particle::setVelocity(const ngl::Vector _velocity)
{
    //set velocities to new value
    m_lastVelocity = _velocity;
    m_currentVelocity = _velocity;
}

void Particle::updateVelocity(const ngl::Vector _velocity)
{
    //save current value
    m_lastVelocity = m_currentVelocity;

    //update to new value
    m_currentVelocity = _velocity;
}

void Particle::setAcceleration(const ngl::Vector _acceleration)
{
    //set velocities to new value
    m_lastAcceleration = _acceleration;
    m_currentAcceleration = _acceleration;
}

void Particle::updateAcceleration(const ngl::Vector _acceleration)
{
    //save current value
    m_lastAcceleration = m_currentAcceleration;

    //update to new value
    m_currentAcceleration = _acceleration;
}
