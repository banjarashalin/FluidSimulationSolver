/// @file Integrate.cpp
/// @brief performs Integrate of acceleration to get next velocity and position

#include "Integrate.h"
#include "SimulationParameters.h"

Integrate::Integrate()
{
    SimulationParameters::initialiseIntegrate(m_IntegrateType,m_timestep);
}

void Integrate::integrateNext(Particle &io_fluidParticle)
{
    /*---Calling chosed integration of the following-------------------------------------*/
    if (m_IntegrateType == "SEMI_IMPLICIT_EULER")
    {
        calculateSemiImplicitEuler(io_fluidParticle);
    }
    if (m_IntegrateType == "LEAPFROG")
    {
        calculateLeapfrog(io_fluidParticle);
    }
}


void Integrate::calculateSemiImplicitEuler(Particle &io_fluidParticle)
{
    /*-----semi implicit euler from http://en.wikipedia.org/wiki/Semi-implicit_Euler----------------------------------------------*/

    io_fluidParticle.updateVelocity(io_fluidParticle.getCurrentVelocity() + (io_fluidParticle.getCurrentAcceleration() * m_timestep));

    io_fluidParticle.updatePosition(io_fluidParticle.getCurrentPosition() + (io_fluidParticle.getCurrentVelocity() * m_timestep));
}

void Integrate::calculateLeapfrog(Particle &io_fluidParticle)
{
    /*-----modified leapfrog from http://en.wikipedia.org/wiki/Leapfrog_Integrate-----------------------------------------------------------*/

    io_fluidParticle.updateVelocity(io_fluidParticle.getCurrentVelocity() + (((io_fluidParticle.getLastAcceleration() + io_fluidParticle.getCurrentAcceleration()) / 2.0) * m_timestep));

    io_fluidParticle.updatePosition(io_fluidParticle.getCurrentPosition() + (io_fluidParticle.getCurrentVelocity() * m_timestep) + ((io_fluidParticle.getLastAcceleration() / 2.0) * m_timestep * m_timestep));
}

