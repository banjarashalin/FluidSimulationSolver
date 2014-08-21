#include "Export.h"
#include "QString"
#include <fstream>

Export::Export(std::string _exportPath)
{
    m_exportPath = _exportPath;
    m_frameNumber = 0;
}

void Export::exportDataOut(std::vector<std::string> _fluidNameList, std::vector<std::vector<ngl::Vector> > _fluidParticlesPositionsList, EnvironmentAndRigidBody *_environment)
{
    m_frameNumber++;

/*---------------------------Writing the Position of the fluid particles ---------------------------------------- */

    for (int j = 0; j < _fluidNameList.size(); ++j)
    {
        //construct filename of the item for the current frame number
        QString text = QString("%1%2_Frame_%3.geo")
                        .arg(m_exportPath.c_str())
                        .arg(std::string(_fluidNameList[j]).c_str())
                        .arg(m_frameNumber);


        writePointData(_fluidParticlesPositionsList[j], text.toStdString());

    }

/*-------------------Writing the Position of the obstacle ---------------------------------------- */

    if (m_frameNumber == 1)
    {
        std::vector <ngl::Vector> _position;
        _position.clear();

        for (int i=0; i<_environment->getRBDSphereList().size(); i++)
        {
            _position.push_back(_environment->getRBDSphereList()[i].getCurrentPosition());
        }

        //construct filename of the item for the current frame number

        QString text = QString("%1%2_Frame_%3.geo")
                        .arg(m_exportPath.c_str())
                        .arg(std::string("ObstacleSphere").c_str())
                        .arg(m_frameNumber);


        writePointData(_position, text.toStdString());
    }
}

void Export::writePointData(std::vector<ngl::Vector> &_fluidParticlePositionsList, const std::string &_filename)
{
    //open file for writing
    std::ofstream outFile(_filename.c_str(), std::ios::out);

    //write header
    outFile << "PGEOMETRY V5\n";
    outFile << "NPoints " << _fluidParticlePositionsList.size() << " NPrims 0\n";
    outFile << "NPointGroups 0 NPrimGroups 0\n";
    outFile << "NPointAttrib 0 NVertexAttrib 0 NPrimAttrib 0 NAttrib 0\n";

    //loop through position list
    for (int i = 0; i < _fluidParticlePositionsList.size(); ++i)
    {
        outFile
                << _fluidParticlePositionsList[i].m_x << " "
                << _fluidParticlePositionsList[i].m_y << " "
                << _fluidParticlePositionsList[i].m_z << " "
                << _fluidParticlePositionsList[i].m_w << "\n";
    }

    //write the outro
    outFile << "beginExtra\n";
    outFile << "endExtra";

    //close file
    outFile.close();
}
