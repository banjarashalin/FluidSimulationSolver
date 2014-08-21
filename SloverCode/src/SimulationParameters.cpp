#include "SimulationParameters.h"
#include <iostream>
#include "QString"
#include "pugixml.hpp"
#include "boost/tokenizer.hpp"
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/foreach.hpp>



/*---Static Variables Initialisation------------------------------*/

std::string SimulationParameters::s_settingsDocumentPath = "SolverParameters/Settings.xml";
bool SimulationParameters::s_debug = true;
ngl::Real SimulationParameters::s_accelerationOfFreeFall = -9.28;
int SimulationParameters::s_nextFluidParticleId = 0;
int SimulationParameters::s_nextRBDSphereId = -1;

std::vector<ngl::Real> SimulationParameters::getFloatVectorFromString
                                            (
                                                const std::string _str,
                                                const int _d
                                            )
{
    //split string into array of strings by ","
    std::vector<std::string> strArray;
    boost::split(strArray, _str, boost::is_any_of(","));

    //loop through strings and convert to float array
    std::vector<ngl::Real> floatArray;
    for (int i = 0; i < _d; i++) floatArray.push_back(boost::lexical_cast<ngl::Real>(strArray[i]));

    //return array of float
    return floatArray;
}

void SimulationParameters::initialiseEnvironment(
                                                        bool &o_rbdCollsionEnabled,
                                                        ngl::Vector &o_boundaryPosition,
                                                        ngl::Vector &o_boundaryDimension,
                                                        ngl::Real &o_boundaryRestitutionCoefficientForFluid,
                                                        std::vector<Particle> &o_rbdSphereList,
                                                        ngl::Real &o_rbdSphereRestitutionCoefficient,
                                                        ngl::Colour &o_boundaryColour
                                                        )
{

    /*----Load Settings from XML File----------------------------------------------*/

    pugi::xml_document doc;
    doc.load_file(s_settingsDocumentPath.c_str());

    //temporary variable
    std::vector<ngl::Real> tmp;

    pugi::xpath_node_set node = doc.select_nodes("/Settings/Environment");

    /*--- Reading Boundry Parameters-------------------------------------------*/

    tmp = getFloatVectorFromString(node.first().node().select_single_node("Boundary").node().attribute("position").value(), 3);
    o_boundaryPosition.set(tmp[0], tmp[1], tmp[2]);

    tmp = getFloatVectorFromString(node.first().node().select_single_node("Boundary").node().attribute("dimension").value(), 3);
    o_boundaryDimension.set(tmp[0], tmp[1], tmp[2]);

    o_boundaryRestitutionCoefficientForFluid = node.first().node().select_single_node("Boundary").node().attribute("restitutionForFluid").as_float();

    tmp = getFloatVectorFromString(node.first().node().select_single_node("Boundary").node().attribute("colour").value(), 3);
    o_boundaryColour.set(tmp[0],tmp[1],tmp[2],1.0);

    /*--- Read CollisionParameters-------------------------------------------------------------------*/

    node = doc.select_nodes("/Settings/Environment/Obstacles");
    o_rbdCollsionEnabled =  node.first().node().attribute("enabled").as_bool();

    o_rbdSphereRestitutionCoefficient = node.first().node().attribute("restitution").as_float();

    /*---- Creating and Storing Rigid Body Sphere -----------------------------------------*/

    node = node.first().node().select_nodes("Sphere");
    for (pugi::xpath_node_set::const_iterator it = node.begin(); it != node.end(); ++it)
    {
        //get details for an obstacle
        pugi::xpath_node node1 = *it;

        //get parameters active parameter
        tmp = getFloatVectorFromString(node1.node().attribute("position").value(), 3);
        ngl::Vector position(tmp[0], tmp[1], tmp[2]);

        ngl::Vector velocity(0,0,0);

        ngl::Real collisionRadius = node1.node().attribute("collisionRadius").as_float();

        ngl::Real drawRadius = node1.node().attribute("drawRadius").as_float();

        tmp = getFloatVectorFromString(node1.node().attribute("colour").value(), 3);
        ngl::Colour colour(tmp[0], tmp[1], tmp[2]);

        /*---Create and Add Spheres to RBDSphere List----------------------------------------------------*/

        Particle p = Particle (SimulationParameters::s_nextRBDSphereId--,0,position,colour,collisionRadius,drawRadius,velocity);

        o_rbdSphereList.push_back(p);
    }
}

void SimulationParameters::initialiseSPHFluidSolver(
                                                        ngl::Real &o_smoothingLength,
                                                        std::vector<FluidParticle> &o_fluidParticleList,
                                                        std::string &o_visualizationType,
                                                        bool& o_drawFluidParticles,
                                                        std::vector<std::string> &o_fluidNameList
                                                    )
{
    //load settings xml file
    pugi::xml_document doc;
    doc.load_file(s_settingsDocumentPath.c_str());

    //load global settings here
    pugi::xpath_node_set node = doc.select_nodes("/Settings/FluidSolver");

    //read smoothing length
    o_smoothingLength = node.first().node().attribute("smoothingLength").as_float();

    //initially reserve space for maximum growth of particle list
    int initialReservedParticleCount =node.first().node().attribute("initialReservedParticleCount").as_int();
    o_fluidParticleList.reserve(initialReservedParticleCount);

    o_visualizationType = node.first().node().attribute("visualization").value();
    o_drawFluidParticles = node.first().node().attribute("enableVisulization").as_bool();

    std::cout << "\n\t\tSmooth Particle Hydrodynamics Solver" << std::endl;
    std::cout << "\n\t Smoothing Length :: " << o_smoothingLength << "\t Visualization Shading Type :: " << o_visualizationType << "\n\n" << std::endl;

    //tmp variable
    std::vector<ngl::Real> tmp;



    /*------Getting List of Fluids-------------------------------*/
    node = doc.select_nodes("/Settings/FluidSolver/Fluid");
    for (pugi::xpath_node_set::const_iterator it = node.begin(); it != node.end(); ++it)
    {
        //get details for a fluid
        pugi::xpath_node node = *it;

        //get its active parameter
        bool isActive = node.node().attribute("active").as_bool();

        //skip this fluid if not active
        if (isActive)
        {
            //read parameters
            ngl::Real volume = boost::lexical_cast<ngl::Real>(node.node().select_single_node("Volume").node().child_value());

            ngl::Real restDensity = boost::lexical_cast<ngl::Real>(node.node().select_single_node("RestDensity").node().child_value());

            tmp = getFloatVectorFromString(node.node().select_single_node("Colour").node().attribute("value").value(), 3);
            ngl::Colour color(tmp[0], tmp[1], tmp[2]);

            std::string meshName = node.node().select_single_node("Obj").node().child_value();
            ngl::Obj mesh(meshName);

            tmp = getFloatVectorFromString(node.node().select_single_node("Obj").node().attribute("center").value(), 3);
            ngl::Vector centerOfMesh(tmp[0], tmp[1], tmp[2]);

            tmp = getFloatVectorFromString(node.node().select_single_node("Velocity").node().attribute("value").value(), 3);
            ngl::Vector velocity(tmp[0], tmp[1], tmp[2]);

            ngl::Real viscosityConstant = boost::lexical_cast<ngl::Real>(node.node().select_single_node("ViscosityConstant").node().child_value());

            ngl::Real gasConstant = boost::lexical_cast<ngl::Real>(node.node().select_single_node("GasConstant").node().child_value());

            ngl::Real surfaceTensionCoefficient = node.node().select_single_node("SurfaceTension").node().attribute("coefficient").as_float();
            ngl::Real surfaceTensionThreshold = node.node().select_single_node("SurfaceTension").node().attribute("threshold").as_float();
            ngl::Real surfaceColorCoefficient = node.node().select_single_node("SurfaceTension").node().attribute("color").as_float();

            ngl::Real interfaceTensionCoefficient = node.node().select_single_node("SurfaceInterface").node().attribute("coefficient").as_float();
            ngl::Real interfaceTensionThreshold = node.node().select_single_node("SurfaceInterface").node().attribute("threshold").as_float();
            ngl::Real interfaceColorCoefficient = node.node().select_single_node("SurfaceInterface").node().attribute("color").as_float();

            ngl::Real collisionRadius = boost::lexical_cast<ngl::Real>(node.node().select_single_node("CollisionRadius").node().child_value());

            ngl::Real drawRadius = boost::lexical_cast<ngl::Real>(node.node().select_single_node("DrawRadius").node().child_value());

            std::string name = node.node().attribute("name").value();

            //calculate mass per particle
            int particleCount = mesh.getNumVerts();
            ngl::Real mass = restDensity * (volume / ((ngl::Real)particleCount));

                for (int i = 0; i < particleCount; i++)
                {
                    FluidParticle f = FluidParticle(
                                                        SimulationParameters::s_nextFluidParticleId++,
                                                        mass,
                                                        restDensity,
                                                        centerOfMesh + mesh.getVertexAtIndex(i),
                                                        velocity,
                                                        viscosityConstant,
                                                        gasConstant,
                                                        surfaceTensionCoefficient,
                                                        surfaceTensionThreshold,
                                                        surfaceColorCoefficient,
                                                        interfaceTensionCoefficient,
                                                        interfaceTensionThreshold,
                                                        interfaceColorCoefficient,
                                                        color,
                                                        collisionRadius,
                                                        drawRadius,
                                                        name
                                                    );
                    //create particle @ mesh vertices and add to particle list
                    o_fluidParticleList.push_back (f);
                }
                std::cout << "Fluid Name :: " << name << "\t \t Fluid Particles Count  :: " << particleCount << std::endl;
                o_fluidNameList.push_back(name);

        }
    }
    std::cout << "\nTotal Number of Fluid Particles :: " << o_fluidParticleList.size() << "\n" <<std::endl;
}

void SimulationParameters::initialiseIntegrate(
                                                std::string& o_IntegrateType,
                                                ngl::Real& o_timeStep
                                              )
{

    //load settings xml file
    pugi::xml_document doc;
    doc.load_file(s_settingsDocumentPath.c_str());
    //load global settings here
    pugi::xpath_node_set node = doc.select_nodes("/Settings/Integration");

    o_IntegrateType = node.first().node().attribute("type").value();
    o_timeStep = node.first().node().attribute("timestep").as_float();

    std::cout << "\n Integration Type :: " << o_IntegrateType << "\t \t Time Step Value :: " << o_timeStep << std::endl;

    std::cout << "\t" << std::endl;
}

void SimulationParameters::initialiseCameraParameters(
                                                        ngl::Vector &o_eye,
                                                        ngl::Vector &o_look,
                                                        ngl::Vector &o_up
                                                      )
{
    //load settings xml file
    pugi::xml_document doc;
    doc.load_file(s_settingsDocumentPath.c_str());
    //load global settings here
    pugi::xpath_node_set node = doc.select_nodes("/Settings/Camera");
    std::vector<ngl::Real> tmp;

    tmp = getFloatVectorFromString(node.first().node().attribute("eye").value(), 3);

    o_eye.set(tmp[0],tmp[1],tmp[2]);

    tmp = getFloatVectorFromString(node.first().node().attribute("look").value(), 3);

    o_look.set(tmp[0],tmp[1],tmp[2]);

    tmp = getFloatVectorFromString(node.first().node().attribute("up").value(), 3);

    o_up.set(tmp[0],tmp[1],tmp[2]);

    std::cout << "\n Camera Values ---------" << "\tEye :: " << o_eye << "\tLook :: " << o_look << "\tUp Vector :: " << o_up << std::endl;
}

void SimulationParameters::initialiseExportParameters(bool &o_exportEnabled, std::string &o_exportPath, float &o_exportFrameInterval)
{
    //load settings xml file
    pugi::xml_document doc;
    doc.load_file(s_settingsDocumentPath.c_str());
    //load global settings here
    pugi::xpath_node_set node = doc.select_nodes("/Settings/Export");

    o_exportEnabled = node.first().node().attribute("enable").as_bool();

    o_exportPath = node.first().node().attribute("path").value();

    int i  = node.first().node().attribute("exportFPS").as_int();

    float temp = (1.0 / i) * 100;
    temp = floor(temp);
    temp = temp / 100.0 ;
    o_exportFrameInterval = temp ;

    std::cout << "o_exportFrameINterval :: " << o_exportFrameInterval << std::endl;

}

