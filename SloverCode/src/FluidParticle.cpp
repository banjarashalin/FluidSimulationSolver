/// @file FluidParticle.cpp
/// inherited from the Particle class and used to represent the fluid particles in the simulation

#include "FluidParticle.h"

FluidParticle::FluidParticle
            (
                const int _id,
                const ngl::Real _mass,
                const ngl::Real _density,
                const ngl::Vector _position,
                const ngl::Vector _velocity,
                const ngl::Real _viscosityConstant,
                const ngl::Real _gasConstant,
                const ngl::Real _surfaceTensionCoefficient,
                const ngl::Real _surfaceTensionThreshold,
                const ngl::Real _surfaceColorCoefficient,
                const ngl::Real _interfaceTensionCoefficient,
                const ngl::Real _interfaceTensionThreshold,
                const ngl::Real _interfaceColorCoefficient,
                const ngl::Colour _colour,
                const ngl::Real _collisionRadius,
                const ngl::Real _drawRadius,
                const std::string _name
            ) :
            Particle(_id, _mass, _position, _colour, _collisionRadius, _drawRadius, _velocity)
{
    //init params
    m_name = _name;

    m_density = _density;
    m_restDensity = _density;

    m_scalerPressure = 0;

    m_viscosityConstant = _viscosityConstant;
    m_gasConstant = _gasConstant;

    m_surfaceTensionCoefficient = _surfaceTensionCoefficient;
    m_surfaceTensionThreshold = _surfaceTensionThreshold;
    m_surfaceColorCoefficient = _surfaceColorCoefficient;

    m_interfaceTensionCoefficient = _interfaceTensionCoefficient;
    m_interfaceTensionThreshold = _interfaceTensionThreshold;
    m_interfaceColorCoefficient = _interfaceColorCoefficient;
}

FluidParticle::~FluidParticle()
{
}

void FluidParticle::resetForces()
{
    Particle::resetNetForce();

    m_pressureForce = 0;
    m_viscosityForce = 0;
    m_surfaceTensionForce = 0;
    m_gravityForce = 0;
}

