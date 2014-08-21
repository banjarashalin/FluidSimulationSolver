#ifndef SPHSOLVER_H
#define SPHSOLVER_H

#include "vector"
#include "SpatialHashing.h"
#include "EnvironmentAndRigidBody.h"
#include "FluidParticle.h"
#include "ngl/ShaderManager.h"
#include "ngl/TransformStack.h"
#include "Integrate.h"


/// @file SPHSolver.h
/// @brief the main class that manages the SPH simulation
/// @author Shalin Kiran Banjara
/// @version 1.0
/// @date 13.11.11
/// Revision History :
/// Initial Version 13.11.11
/// @class SPHSolver
/// @brief creates fluid particles and perform SPH fluid simulation at every iteration

class SPHSolver
{
public:
    /// @brief ctor
    SPHSolver();

    /// @brief dtor
    ~SPHSolver();

    /// @brief update the fluid state at each timestep interval
    /// @param[in,out] io_environment pointer to the enviornment class for performing collision check and resolution
    /// @param[in] _integration pointer to the integrate class used to perform integration
    void updateFluid
            (
                EnvironmentAndRigidBody* io_environment,
                Integrate* _integrate
            );

    /// @brief return the huge position list of all the fluid particles
    std::vector<std::vector<ngl::Vector> > getFluidParticlePositionList();

    /// @brief return the list storing the names of different fluid created
    inline std::vector<std::string> getFluidNameList() {return m_fluidNameList;}

    /// @brief return the shader name to be used for displaying the simulation data
    inline std::string getVisualizationType() {return m_visualizationType;}

    /// @brief return the list of fluid particles being simulated
    inline std::vector<FluidParticle> getFluidParticleList() {return m_fluidParticleList;}

    /// @brief returns the flag for drawing fluids
    inline bool getDrawFluidParticles() {return m_drawFluidParticles;}

private:

    /// @brief list of particles making up the fluid simulation
    std::vector<FluidParticle> m_fluidParticleList;

    std::vector<std::string>m_fluidNameList;

    /// @brief smoothing kernel length
    ngl::Real m_smoothingLength;

    /// @brief the neighbouring structure for the fluid particles
    SpatialHashing* m_spatialHashing;

    /// @brief pre-calculated part of the poly 6 kernel
    ngl::Real m_weightPoly;

    /// @brief pre-calculated part of the poly 6 gradient kernel
    ngl::Real m_weightPolyGradient;

    /// @brief pre-calculated part of the poly 6 laplacian kernel
    ngl::Real m_weightPolyLaplacian;

    /// @brief pre-calculated part of the pressure gradient kernel
    ngl::Real m_weightPressureGradient;

    /// @brief pre-calculated part of the viscosity laplacian kernel
    ngl::Real m_weightViscosityLaplacian;

    /// @brief iteration counter
    int m_iteration;

    /// @brief shading type of fluid particles
    std::string m_visualizationType;

    /// @brief flag to control to display fluid particles in OpenGL Window
    bool m_drawFluidParticles;

private:
    /// @brief calculate Interface Tension Gradient Component
    /// @param[in] _massPerDensity mass per density of the fluid particle
    /// @param[in] _interfaceColourCoefficient interface colour coefflicient of the fluid particle
    /// @param[in] _seperationVector distance vector between the fluid particle and the neighbour fluid particle
    ngl::Vector calculateInterfaceTensionGradientComponent
                                                (
                                                    ngl::Real _massPerDensity,
                                                    ngl::Real _interfaceColourCoefficient,
                                                    ngl::Vector _seperationVector
                                                );

    /// @brief calculate Interface Tension Laplacian Component
    /// @param[in] _massPerDensity mass per density of the fluid particle
    /// @param[in] _interfaceColourCoefficient interface colour coefflicient of the fluid particle
    /// @param[in] _seperationVector amplitube of the distance vector between the fluid particle and the neighbour fluid particle
    ngl::Real calculateInterfaceTensionLaplacianComponent
                                                (
                                                    ngl::Real _massPerDensity,
                                                    ngl::Real _interfaceColourCoefficient,
                                                    ngl::Real _seperationLength
                                                );

    /// @brief calculate Pressure Force Component
    /// @param[in] _fluidParticleScalerPressure
    /// @param[in] _fluidParticleDensity density of the fluid particle
    /// @param[in] _neighbourFluidParticleScalerPressure
    /// @param[in] _neighbourFluidDensity density of the neighbour fluid particle
    /// @param[in] _neighbourFluidParticleMass mass of the neighbour fluid particle
    /// @param[in] _seperationLength distance vector between the fluid particle and the neighbour fluid particle
    ngl::Vector calculatePressureForceComponent
                                                (   ngl::Real _fluidParticleScalerPressure,
                                                    ngl::Real _fluidParticleDensity,
                                                    ngl::Real _neighbourFluidParticleScalerPressure,
                                                    ngl::Real _neighbourFluidDensity,
                                                    ngl::Real _neighbourFluidParticleMass,
                                                    ngl::Vector _seperationLength
                                                );

    /// @brief calculate Viscosity Force Component
    /// @param[in] _massPerDensity mass per density of the fluid particle
    /// @param[in] _fluidParticleCurrentVelocity current velocity of the fluid particle
    /// @param[in] _neighbourFluidParticleCurrentVelocity current velocity of the neighbour fluid particle
    /// @param[in] _seperationLength amplitube of the distance vector between fluid particle and the neighbour fluid particle
    ngl::Vector calculateViscosityForceComponent
                                                (
                                                    ngl::Real _massPerDensity,
                                                    ngl::Vector _fluidParticleCurrentVelocity,
                                                    ngl::Vector _neighbourFluidParticleCurrentVelocity,
                                                    ngl::Real _seperationLength
                                                );

    /// @brief calculate Surface Tension Gradient Component
    /// @param[in] _massPerDensity mass per density of the fluid particle
    /// @param[in] _surfaceColourCoefficient surface colour coefficient of the fluid particle
    /// @param[in] _seperationVector distance vector between the fluid particle and the neighbour fluid particle
    ngl::Vector calculateSurfaceTennsionGradientComponent
                                                (   ngl::Real _massPerDensity,
                                                    ngl::Real _surfaceColourCoefficient,
                                                    ngl::Vector _seperationVector
                                                );

    /// @brief calculate Surface Tension Laplacian Component
    /// @param[in] _massPerDensity mass per density of the fluid particle
    /// @param[in] _surfaceColourCoefficient surface colour coefficient of the fluid particle
    /// @param[in] _seperationLength amplitude of the distance vector between the fluid particle and neighbour fluid particle
    ngl::Real calculateSurfaceTensionLaplacianComponent
                                                (   ngl::Real _massPerDensity,
                                                    ngl::Real _surfaceColourCoefficient,
                                                    ngl::Real _seperationLength
                                                );

    /// @brief calculate the poly 6 kernel
    /// @param[in] _seperationLength amplitude of the distance vector between the fluid particle and neighbour fluid particle
    ngl::Real KernelPoly
                        (
                            const ngl::Real _seperationLength
                        );

    /// @brief calculate the poly 6 gradient kernel
    /// @param[in] _seperationVector distance vector between the fluid particle and the neighbour fluid particle
    ngl::Vector KernelPolyGradient
                                (
                                    const ngl::Vector _seperationVector
                                );

    /// @brief calculate the poly 6 laplacian kernel
    /// @param[in] _seperationLength amplitude of the distance vector between the fluid particle and neighbour fluid particle
    ngl::Real KernelPolyLaplacian
                                (
                                    const ngl::Real _seperationLength
                                );

    /// @brief calculate the pressure (spiky) gradient kernel
    /// @param[in] _seperationVector distance vector between the fluid particle and the neighbour fluid particle
    ngl::Vector KernelPressureGradient
                                    (
                                        const ngl::Vector _seperationVector
                                    );

    /// @brief calculate the viscosity laplacian kernel
    /// @param[in] _seperationLength amplitude of the distance vector between the fluid particle and neighbour fluid particle
    ngl::Real KernelViscosityLaplacian
                                    (
                                        const ngl::Real _seperationLength
                                    );

    /// @brief normalise a vector
    /// @param[in] _vector the vector to normalise
    ngl::Vector normalizeVector
                            (
                                const ngl::Vector _vector
                            );

    /// @brief get a particle fluid id from its name
    /// @param[in] _name the fluid name
    int getFluidIdFromName
                        (
                            const std::string _name
                        );


};

#endif // SPHSOLVER_H
