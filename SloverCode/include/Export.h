#ifndef EXPORT_H
#define EXPORT_H

#include <iostream>
#include "ngl/Vector.h"
#include "EnvironmentAndRigidBody.h"
#include <vector>

/// @file Export.h
/// @brief the basic unit of the lagrangian simulation
/// @author Shalin K Banjara
/// @version 1.0
/// @date 21.10.11
/// Revision History :
/// Initial Version 21.10.11
/// @class Export
/// @brief writes the simulation data on a specified path as geo files

class Export
{
public:
    /// @brief ctor
    /// @param[in] _exportPath path to save the geo files
    Export(std::string _exportPath);

    /// @brief write point positions (fluid and sphere) to file
    /// @param[in] _fluidNameList the name of the file to be written on disk
    /// @param[in] _fluidParticlesPositionsList the position list to write out
    void exportDataOut
                    (
                        std::vector<std::string> _fluidNameList,
                        std::vector<std::vector<ngl::Vector> > _fluidParticlesPositionsList,
                        EnvironmentAndRigidBody* _environment
                    );

    /// @brief write point positions (fluid) to a file
    /// @param[in] _fluidParticlePositionsList the position list to write out
    /// @param[in] _filename the name of the file on disk
    void writePointData
            (
                std::vector<ngl::Vector>& _fluidParticlePositionsList,
                const std::string& _filename
            );
private:
    /// @brief stores the path to which the files are to be written out
    std::string m_exportPath;

    /// @brief stores the frame number
    int m_frameNumber;
};

#endif // EXPORT_H
