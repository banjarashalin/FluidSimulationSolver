#ifndef SIMULATIONPARAMETERS_H
#define SIMULATIONPARAMETERS_H

#include "Particle.h"
#include "FluidParticle.h"
#include "Integrate.h"
#include <iostream>
#include "vector"

/// @file SimulationParameters.h
/// @brief reads the values from the XML file and intialises the fluid solver
/// @author Shalin K Banjara
/// @version 1.0
/// @date 23.10.11
/// Revision History :
/// Initial Version 23.10.11
/// @class SimulationParameters
/// @brief reads the values from the XML file and intialises the fluid solver

class SimulationParameters
{

public:

    /// @brief debug parameter used in all the files to enable debugging and console output
    static bool s_debug;

    /// @brief acceleration due to gravity(free fall) value
    static ngl::Real s_accelerationOfFreeFall;

    /// @brief acceleration due to gravity(free fall) value
    static ngl::Real s_gravityAcceleration;

    /// @brief colour of the boundry box
    static ngl::Colour s_boundaryColour;

    /// @brief global unique id available for creation of next particle in a fluid
    static int s_nextFluidParticleId;

    /// @brief global unique id available for creation of next passive sphere rigid body
    static int s_nextRBDSphereId;



    /// @brief initialise solver
    /// @param[out] o_smoothingLength smoothing length of neighbour structure
    /// @param[out] o_fluidParticleList fluid particle list
    /// @param[out] o_visualizationType shader to be used while displaying simulation data realtime
    /// @param[out] o_drawFluidParticles flag to check rendering of fluid particle
    /// @param[out] o_fluidNameList name list of diffierent fluids created
    static void initialiseSPHFluidSolver
                                        (
                                            ngl::Real& o_smoothingLength,
                                            std::vector<FluidParticle>& o_fluidParticleList,
                                            std::string& o_visualizationType,
                                            bool& o_drawFluidParticles,
                                            std::vector<std::string>& o_fluidNameList
                                        );


    /// @brief initialise environment
    /// @param[out] o_rbdCollsionEnabled flag to toggle rbd
    /// @param[out] o_boundaryPosition boundary position
    /// @param[out] o_boundaryDimension boundary size
    /// @param[out] o_boundaryRestitutionCoefficientForFluid boundary-fluid restitution
    /// @param[out] o_rbdSphereList list of sphere rbd
    /// @param[out] o_rbdSphereRestitutionCoefficient rbd-fluid restitution
    /// @param[out] o_boundaryColour colour of the boundry to be displayed
    static void initialiseEnvironment
                                        (
                                            bool &o_rbdCollsionEnabled,
                                            ngl::Vector &o_boundaryPosition,
                                            ngl::Vector &o_boundaryDimension,
                                            ngl::Real& o_boundaryRestitutionCoefficientForFluid,
                                            std::vector<Particle>& o_rbdSphereList,
                                            ngl::Real& o_rbdSphereRestitutionCoefficient,
                                            ngl::Colour& o_boundaryColour
                                        );

    /// @brief initialise Integrate
    /// @param[out] o_IntegrateType method to use for integration
    /// @param[out] o_timeStep timestep value for integration
    static void initialiseIntegrate
                                         (  std::string &o_IntegrateType,
                                            ngl::Real &o_timeStep
                                         );

    /// @brief initialise Camera
    /// @param[out] o_eye eye for the Camera
    /// @param[out] o_look look/target for the Camera
    /// @param[out] o_up up vector for the Camera
    static void initialiseCameraParameters
                                        (
                                            ngl::Vector &o_eye,
                                            ngl::Vector &o_look,
                                            ngl::Vector &o_up
                                        );

    /// @brief initialise the parameter required to export simulation data out to disk
    /// @param[out] o_exportEnabled flag to check exporting of simulation data
    /// @param[out] o_exportPath path, where the simulation data is to be exported
    /// @param[out] o_exportFrameInterval frame interval value at which the simulation data is to be exported
    static void initialiseExportParameters
                                        (
                                            bool& o_exportEnabled,
                                            std::string& o_exportPath,
                                            float &o_exportFrameInterval
                                        );


private:

    /// @brief utility function to split a string into a vector of floats
    /// @param[in] _str the input string to be split
    /// @param[in] _d the count of split parts to be used as the size of the vector
    static std::vector<ngl::Real> getFloatVectorFromString
                                            (
                                                const std::string _str,
                                                const int _d
                                            );

private:

    /// @brief the path of the configuration xml file
    static std::string s_settingsDocumentPath;


};

#endif // SIMULATIONPARAMETERS_H
