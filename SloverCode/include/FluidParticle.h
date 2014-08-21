#ifndef FLUIDPARTICLE_H
#define FLUIDPARTICLE_H

#include "Particle.h"

/// @file FluidParticle.h
/// @brief the basic unit of the lagrangian simulation
/// @author Shalin K Banjara
/// @version 1.0
/// @date 21.10.11
/// Revision History :
/// Initial Version 21.10.11
/// @class FluidParticle
/// @brief stores all attributes for a fluid for use with SPH

class FluidParticle : public Particle
{
public:
    /// @brief ctor
    /// @param[in] _id the id
    /// @param[in] _mass the mass
    /// @param[in] _density the density
    /// @param[in] _position the position
    /// @param[in] _velocity the velocity
    /// @param[in] _viscosityConstant the viscosity constant
    /// @param[in] _gasConstant the gas constant
    /// @param[in] _surfaceTensionCoefficient the surface tension coefficient
    /// @param[in] _surfaceTensionThreshold the surface tension threshold
    /// @param[in] _surfaceColorCoefficient the surface tension color
    /// @param[in] _interfaceTensionCoefficient the interface tension coefficient
    /// @param[in] _interfaceTensionThreshold the interface tension threshold
    /// @param[in] _interfaceColorCoefficient the interface tension color
    /// @param[in] _colour the visual colour of the particle
    /// @param[in] _collisionRadius the radius for collision detection
    /// @param[in] _drawRadius the radius to represent it
    /// @param[in] _name the name of the fluid to which this particle belongs
    FluidParticle
        (
            const int _id,
            const ngl::Real _mass = 0,
            const ngl::Real _density = 0,
            const ngl::Vector _position = 0,
            const ngl::Vector _velocity = 0,
            const ngl::Real _viscosityConstant = 0,
            const ngl::Real _gasConstant = 0,
            const ngl::Real _surfaceTensionCoefficient = 0,
            const ngl::Real _surfaceTensionThreshold = 0,
            const ngl::Real _surfaceColorCoefficient = 0,
            const ngl::Real _interfaceTensionCoefficient = 0,
            const ngl::Real _interfaceTensionThreshold = 0,
            const ngl::Real _interfaceColorCoefficient = 0,
            const ngl::Colour _colour = 0,
            const ngl::Real _collisionRadius = 0,
            const ngl::Real _drawRadius = 0,
            const std::string _name = "na"
        );

    /// @brief dtor
    ~FluidParticle();

    /// @brief reset the accumulated force
    void resetForces();

    /// @brief get the name of the fluid to which this particle belongs to
    inline std::string getName() const { return m_name; }

    inline void setScalerPressure(const ngl::Real _scalerPressure) {m_scalerPressure = _scalerPressure;}

    /// @brief get the pressure force
    inline ngl::Vector getPressureForce() const { return m_pressureForce; }

    /// @brief set the pressure force
    /// @param[in] _pressureForce force value
    inline void setPressureForce(const ngl::Vector _pressureForce) { m_pressureForce = _pressureForce; }

    /// @brief get the viscosity force
    inline ngl::Vector getViscosityForce() const { return m_viscosityForce; }

    /// @brief set the viscosity force
    /// @param[in] _viscosityForce force value
    inline void setViscosityForce(const ngl::Vector _viscosityForce) { m_viscosityForce = _viscosityForce; }

    /// @brief get the surface tension force
    inline ngl::Vector getSurfaceTensionForce() const { return m_surfaceTensionForce; }

    /// @brief set the surface tension force
    /// @param[in] _surfaceTensionForce force value
    inline void setSurfaceTensionForce(const ngl::Vector _surfaceTensionForce) { m_surfaceTensionForce = _surfaceTensionForce; }

    /// @brief get the interface tension force
    inline ngl::Vector getInterfaceTensionForce() const { return m_interfaceTensionForce; }

    /// @brief set the interface tension force
    /// @param[in] _interfaceTensionForce force value
    inline void setInterfaceTensionForce(const ngl::Vector _interfaceTensionForce) { m_interfaceTensionForce = _interfaceTensionForce; }

    /// @brief get the gravitational force
    inline ngl::Vector getGravityForce() const { return m_gravityForce; }

    /// @brief set the gravitational force
    /// @param[in] _gravityForce force value
    inline void setGravityForce(const ngl::Vector _gravityForce) { m_gravityForce = _gravityForce; }

    /// @brief get the pressure
    inline ngl::Real getScalerPressure() const { return m_scalerPressure; }

    /// @brief get the density
    inline ngl::Real getDensity() const { return m_density; }

    /// @brief set the density
    /// @param[in] _d density value
    inline void setDensity(const ngl::Real _d) { m_density = _d; }

    /// @brief get the rest density
    inline ngl::Real getRestDensity() const { return m_restDensity; }

    /// @brief set the rest density
    /// @param[in] _v density value
    inline void setRestDensity(const ngl::Real _v) { m_restDensity = _v; }

    /// @brief get the gas constant
    inline ngl::Real getGasConstant() const { return m_gasConstant; }

    /// @brief set the gas constant force
    /// @param[in] _c value
    inline void setGasConstant(const ngl::Real _c) { m_gasConstant = _c; }

    /// @brief get the viscosity constant
    inline ngl::Real getViscosityConstant() const { return m_viscosityConstant; }

    /// @brief set the viscosity constant
    /// @param[in] _viscosityConstant value
    inline void setViscosityConstant(const ngl::Real _viscosityConstant) { m_viscosityConstant = _viscosityConstant; }

    /// @brief get the surface tension coefficient
    inline ngl::Real getSurfaceTensionCoefficient() const { return m_surfaceTensionCoefficient; }

    /// @brief set the surface interface coefficient
    /// @param[in] _surfaceTensionCoefficient coefficient value
    inline void setSurfaceTensionCoefficient(const ngl::Real _surfaceTensionCoefficient) { m_surfaceTensionCoefficient = _surfaceTensionCoefficient; }

    /// @brief get the surface tension threshold
    inline ngl::Real getSurfaceTensionThreshold() const { return m_surfaceTensionThreshold; }

    /// @brief set the surface tension threshold
    /// @param[in] _surfaceTensionThreshold threshold value
    inline void setSurfaceTensionThreshold(const ngl::Real _surfaceTensionThreshold) { m_surfaceTensionThreshold = _surfaceTensionThreshold; }

    /// @brief get the surface tension color
    inline ngl::Real getSurfaceColorCoefficient() const { return m_surfaceColorCoefficient; }

    /// @brief set the surface color
    /// @param[in] _surfaceColorCoefficient color value
    inline void setSurfaceColorCoefficient(const ngl::Real _surfaceColorCoefficient) { m_surfaceColorCoefficient = _surfaceColorCoefficient; }

    /// @brief get the interface tension coefficient
    inline ngl::Real getInterfaceTensionCoefficient() const { return m_interfaceTensionCoefficient; }

    /// @brief set the surface tension coefficient
    /// @param[in] _interfaceTensionCoefficient coefficient value
    inline void setInterfaceTensionCoefficient(const ngl::Real _interfaceTensionCoefficient) { m_interfaceTensionCoefficient = _interfaceTensionCoefficient; }

    /// @brief get the interface tension threshold
    inline ngl::Real getInterfaceTensionThreshold() const { return m_interfaceTensionThreshold; }

    /// @brief set the interface tension threshold
    /// @param[in] _interfaceTensionThreshold threshold value
    inline void setInterfaceTensionThreshold(const ngl::Real _interfaceTensionThreshold) { m_interfaceTensionThreshold = _interfaceTensionThreshold; }

    /// @brief get the interface tension color
    inline ngl::Real getInterfaceColorCoefficient() const { return m_interfaceColorCoefficient; }

    /// @brief set the interface color
    /// @param[in] _interfaceColorCoefficient color value
    inline void setInterfaceColorCoefficient(const ngl::Real _interfaceColorCoefficient) { m_interfaceColorCoefficient = _interfaceColorCoefficient; }

private:
    /// @brief name of fluid for identification by user later on
    std::string m_name;

    /// @brief density at particle
    ngl::Real m_density;

    /// @brief rest density at particle
    ngl::Real m_restDensity;

    /// @brief pressure at particle
    ngl::Real m_scalerPressure;

    /// @brief pressure force
    ngl::Vector m_pressureForce;

    /// @brief viscosity force
    ngl::Vector m_viscosityForce;

    /// @brief surface tension force
    ngl::Vector m_surfaceTensionForce;

    /// @brief interface tension force
    ngl::Vector m_interfaceTensionForce;

    /// @brief gravity force
    ngl::Vector m_gravityForce;

    /// @brief gas constant
    ngl::Real m_gasConstant;

    /// @brief viscosity constant
    ngl::Real m_viscosityConstant;

    /// @brief surface tension coefficient
    ngl::Real m_surfaceTensionCoefficient;

    /// @brief surface tension threshold
    ngl::Real m_surfaceTensionThreshold;

    /// @brief surface color coefficient
    ngl::Real m_surfaceColorCoefficient;

    /// @brief interface tension coefficient
    ngl::Real m_interfaceTensionCoefficient;

    /// @brief interface tension threshold
    ngl::Real m_interfaceTensionThreshold;

    /// @brief interface color coefficient
    ngl::Real m_interfaceColorCoefficient;

};

#endif // FLUIDPARTICLE_H
