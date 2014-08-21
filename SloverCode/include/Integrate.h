#ifndef INTEGRATE_H
#define INTEGRATE_H

#include "Particle.h"
#include "QString"

/// @file Integrate.h
/// @brief manages the integration of acceleration using two different methods
/// @author Shalin Banjara
/// @version 1.0
/// @date 30.11.11
/// Revision History :
/// Initial Version 30.11.11
/// @class Integrate
/// @brief handles integration to find next position and velocity of particles

class Integrate
{
public:
    /// @brief ctor
    Integrate();

    /// @brief integrate next velocity and position of a particle
    /// @param[in,out] io_fluidParticle the particle that is being affected
    void integrateNext(Particle& io_fluidParticle);

    /// @brief get the Integration method
    inline std::string getIntegrateType() const { return m_IntegrateType; }

    /// @brief get the timestep value
    inline ngl::Real getTimestep() const { return m_timestep; }

    /// @brief set the timestep for the simulation solver
    /// @param[in] _timestep new timestep
    inline void setTimestep(const ngl::Real _timestep) { m_timestep = _timestep; }


private:
    /// @brief Integration method
    std::string m_IntegrateType;

    /// @brief timestep of simulation
    ngl::Real m_timestep;


private:
    /// @brief integrate a particle using semi implicit euler
    /// @param[in,out] io_fluidParticle the particle that is being affected
    void calculateSemiImplicitEuler(Particle& io_fluidParticle);

    /// @brief integrate a particle using leapfrog
    /// @param[in,out] io_fluidParticle the particle that is being affected
    void calculateLeapfrog(Particle& io_fluidParticle);

};

#endif // INTEGRATE_H
