/// @file SPHSolver.cpp
/// @brief the main class that performs the fluid simulation and visualisation

#include "SPHSolver.h"
#include "SimulationParameters.h"
#include "ngl/ShaderManager.h"
#include "ngl/TransformStack.h"
#include "ngl/Util.h"
#include "ngl/VBOPrimitives.h"

SPHSolver::SPHSolver()
{
    //create particles and initialise parameters
    SimulationParameters::initialiseSPHFluidSolver
            (
                m_smoothingLength,
                m_fluidParticleList,
                m_visualizationType,
                m_drawFluidParticles,
                m_fluidNameList
            );

    //precompute kernel weight multipliers
    m_weightPoly = (315.0 / (64.0 * ngl::PI * pow(m_smoothingLength, 9)));
    m_weightPolyGradient = (-945.0 / (32.0 * ngl::PI * pow(m_smoothingLength, 9)));
    m_weightPolyLaplacian = (-945.0 / (32.0 * ngl::PI * pow(m_smoothingLength, 9)));
    m_weightPressureGradient = (-45.0 / (ngl::PI * pow(m_smoothingLength, 6)));
    m_weightViscosityLaplacian = (45.0 / (ngl::PI * pow(m_smoothingLength, 6)));

    //create neighbouring structure
    m_spatialHashing = new SpatialHashing(m_fluidParticleList.size(), m_smoothingLength);

    m_iteration = 0;

//    for (int i = 0; i < m_fluidNameList.size(); ++i)
//    {
//    std::cout << "names of particle are :: " << m_fluidNameList[i] << std::endl;
//    }
}

SPHSolver::~SPHSolver()
{
    //clean up
    std::cout << "Fluid SPHSolver Cleanup" << std::endl;

    //delete objects
    delete m_spatialHashing;
}

void SPHSolver::updateFluid
                (
                    EnvironmentAndRigidBody* io_environment,
                    Integrate* _integrate
                )
{
    //clear neighbour map
    m_spatialHashing->clearHashmap();

#pragma omp parallel

{
    //refresh neighbour list
    m_spatialHashing->refreshHashmap(m_fluidParticleList);
}

#pragma omp parallel
{
#pragma omp for schedule(dynamic, 50)
    //density-pressure calculations
    for (int i = 0; i < m_fluidParticleList.size(); ++i)
    {
        /*------Search and gather neighbour fluid particles for the current fluid particle-----------------------------------------*/

        std::vector<FluidParticle> neighbourList = m_spatialHashing->searchFluidParticleNeighbours(m_fluidParticleList[i].getCurrentPosition());

        /*-----Calculate Density----------------------------------------------------------------------------*/

        ngl::Real density = 0.0;
        for (int j = 0; j < neighbourList.size(); j++)
        {
            //accumulate density
            density += neighbourList[j].getMass() * KernelPoly((m_fluidParticleList[i].getCurrentPosition() - neighbourList[j].getCurrentPosition()).length());
        }

        m_fluidParticleList[i].setDensity(density);

        /*-----Calculate Scaler Pressure Value------------------------------------------------------ */

        ngl::Real _scalerPressure = 0;

        _scalerPressure = m_fluidParticleList[i].getGasConstant() * (m_fluidParticleList[i].getDensity() - m_fluidParticleList[i].getRestDensity());

        m_fluidParticleList[i].setScalerPressure(_scalerPressure);
    }

#pragma omp for schedule(dynamic, 50)
    /*----Calculation of Fluid Forces---------------------------------------------------------------------*/

    for (int i = 0; i < m_fluidParticleList.size(); ++i)
    {
        //getCurrentPosition() net forces accumulate
        m_fluidParticleList[i].resetForces();

        //get neighbours of current particle
        std::vector<FluidParticle> neighbourList = m_spatialHashing->searchFluidParticleNeighbours(m_fluidParticleList[i].getCurrentPosition());

        //calculate internal forces of current particle
        ngl::Real massPerDensity = 0;
        ngl::Vector seperationVector = 0;
        ngl::Real seperationLength = 0.0;

        ngl::Vector pressure = 0;
        ngl::Vector viscosity = 0;

        ngl::Vector surfaceGradient = 0;
        ngl::Real surfaceLaplacian = 0;

        ngl::Vector surfaceTensionGradient = 0;
        ngl::Real surfaceTensionLaplacian = 0;
        ngl::Vector interfaceTensionGradient = 0;
        ngl::Real interfaceTensionLaplacian = 0;


            for (int j = 0; j < neighbourList.size(); j++)
            {
                /*----Get neighbour fluid particle----------------------------------*/
                FluidParticle neighbour = neighbourList[j];

                /*-----Calculating repetative values---------------------------------*/

                massPerDensity = neighbour.getMass() / neighbour.getDensity();
                seperationVector = m_fluidParticleList[i].getCurrentPosition() - neighbour.getCurrentPosition();
                seperationLength = seperationVector.length();

                /*------Calculating the pressure and viscosity force on the current particle applied by the neighbour particle----------*/

                if (neighbour.getId() != m_fluidParticleList[i].getId())
                {
                    /* Accumulate all the Pressure Force for the current fluid particle */

                    pressure = pressure + calculatePressureForceComponent(m_fluidParticleList[i].getScalerPressure(),m_fluidParticleList[i].getDensity(),
                                                                          neighbour.getScalerPressure(),neighbour.getDensity(),
                                                                          neighbour.getMass(),seperationVector);

                    /* Accumulate all Viscosity Force for the current fluid particle */

                    viscosity = viscosity + calculateViscosityForceComponent(massPerDensity,m_fluidParticleList[i].getCurrentVelocity(),
                                                                             neighbour.getCurrentVelocity(),seperationLength);

                }

                /* Accumulate Surface Tension Gradinet by calculating and accumulating Surface Tension Gradient Component */

                surfaceTensionGradient = surfaceTensionGradient +
                                         calculateSurfaceTennsionGradientComponent(massPerDensity, m_fluidParticleList[i].getSurfaceColorCoefficient(),seperationVector);

                /* Accumulate Surface Tension Laplacian by calculating and accumulating Surface Tension Laplacian Component */

                surfaceTensionLaplacian = surfaceTensionLaplacian +
                                          calculateSurfaceTensionLaplacianComponent(massPerDensity,m_fluidParticleList[i].getSurfaceColorCoefficient(),seperationLength);

                /* Accumulate Interface Tension Gradient by calculating and accumulating Interface Tension Gradient Component */

                interfaceTensionGradient = surfaceTensionGradient +
                                           calculateInterfaceTensionGradientComponent(massPerDensity,m_fluidParticleList[i].getInterfaceColorCoefficient(),seperationVector);

                /* Accumulate Interface Tension Laplacian by calculating and accumulating Interface Tension Laplacian */

                interfaceTensionLaplacian = surfaceTensionLaplacian +
                                            calculateInterfaceTensionLaplacianComponent(massPerDensity,m_fluidParticleList[i].getInterfaceColorCoefficient(),seperationLength);

            }

    /*---Calculating and Setting Pressure Force --------------------------------------------------------------------------*/

    ngl::Vector pressureForce = 0;

    pressureForce = -1.0 * m_fluidParticleList[i].getDensity() * pressure;

    m_fluidParticleList[i].setPressureForce(pressureForce);

    /*---Calculating and Setting Viscosity Force -------------------------------------------------------------------------*/

    ngl::Vector viscosityForce = 0;

    viscosityForce = viscosity * m_fluidParticleList[i].getViscosityConstant();

    m_fluidParticleList[i].setViscosityForce(viscosityForce);

    /*---Calculating and Setting Surface Tension Force ---------------------------------------------------------------------*/

    ngl::Vector surfaceTensionForce = 0;

    m_fluidParticleList[i].setSurfaceTensionForce(surfaceTensionForce);
    if (surfaceTensionGradient.length() > m_fluidParticleList[i].getSurfaceTensionThreshold())
        {
            surfaceTensionForce = -1.0 * m_fluidParticleList[i].getSurfaceTensionCoefficient() * normalizeVector(surfaceTensionGradient) * surfaceTensionLaplacian;

            m_fluidParticleList[i].setSurfaceTensionForce(surfaceTensionForce);
        }

    /*---Calculating and Setting Interface Tension Force----------------------------------------------------------------------*/

    ngl::Vector interfaceTensionForce = 0;

    m_fluidParticleList[i].setInterfaceTensionForce(interfaceTensionForce);
        if (interfaceTensionGradient.length() > m_fluidParticleList[i].getInterfaceTensionThreshold())
        {
            interfaceTensionForce = -1.0 * m_fluidParticleList[i].getInterfaceTensionCoefficient() * normalizeVector(interfaceTensionGradient) * interfaceTensionLaplacian;

            m_fluidParticleList[i].setInterfaceTensionForce(interfaceTensionForce);
        }

    /*--- Calculating and setting the gravity force------------------------------------------------------------------*/

    ngl::Vector gravityForce;

    ngl::Real _x = 0; ngl::Real _y = 0; ngl::Real _z = 0;
    _y =  m_fluidParticleList[i].getDensity() * SimulationParameters::s_accelerationOfFreeFall;
    gravityForce.set(_x,_y,_z);

    m_fluidParticleList[i].setGravityForce(gravityForce);

    /*---Calculating and Setting Acceleration by calculating the total resultant forces and dividing then by density----------------------------*/

    ngl::Vector netForce = 0;

    netForce = m_fluidParticleList[i].getGravityForce() + m_fluidParticleList[i].getPressureForce() + m_fluidParticleList[i].getViscosityForce() +
               m_fluidParticleList[i].getSurfaceTensionForce() + m_fluidParticleList[i].getInterfaceTensionForce();

    m_fluidParticleList[i].setNetForce(netForce);

    ngl::Vector acceleration = 0;

    acceleration = m_fluidParticleList[i].getNetForce() / m_fluidParticleList[i].getDensity();

    m_fluidParticleList[i].updateAcceleration(acceleration);

    }

#pragma omp for schedule(dynamic, 50)
    /*-----Integration of fluid and calculating its movement--------*/

    for (int i = 0; i < m_fluidParticleList.size(); ++i)
    {

            _integrate->integrateNext(m_fluidParticleList[i]);
    }

#pragma omp for schedule(dynamic, 50)
    /*----Checking and Handling Collision of Fluid with the Environment Box and Rigid Bodies--------------*/
    for (int i = 0; i < m_fluidParticleList.size(); ++i)
    {
        io_environment->checkCollision(m_fluidParticleList[i]);
    }
}
}


ngl::Vector SPHSolver::calculatePressureForceComponent(
                                                        ngl::Real _fluidParticleScalerPressure,
                                                        ngl::Real _fluidParticleDensity,
                                                        ngl::Real _neighbourFluidParticleScalerPressure,
                                                        ngl::Real _neighbourFluidParticleDensity,
                                                        ngl::Real _neighboutFluidParticleMass,
                                                        ngl::Vector _seperationVector
                                                    )
{
      ngl::Vector pressureForceComponent = 0;

      ngl::Real _fluidParticleDensitySquared = pow (_fluidParticleDensity,2);

      ngl::Real _neighbourFluidParticleDensitySquared = pow(_neighbourFluidParticleDensity,2);

      ngl::Vector pressureGradient = KernelPressureGradient(_seperationVector);

      pressureForceComponent = ((_fluidParticleScalerPressure / _fluidParticleDensitySquared) + (_neighbourFluidParticleScalerPressure/_neighbourFluidParticleDensitySquared))
                            * _neighboutFluidParticleMass * pressureGradient;

      return pressureForceComponent;
}

ngl::Vector SPHSolver::calculateViscosityForceComponent(
                                                         ngl::Real _massPerDensity,
                                                         ngl::Vector _fluidParticleCurrentVelocity,
                                                         ngl::Vector _neighbourFluidParticleCurrentVelocity,
                                                         ngl::Real _seperationLength
                                                     )
{
    ngl::Vector viscosityForceComponent = 0;

    ngl::Real viscosityLaplacian = KernelViscosityLaplacian(_seperationLength);

    viscosityForceComponent = _massPerDensity * (_neighbourFluidParticleCurrentVelocity - _fluidParticleCurrentVelocity) * viscosityLaplacian;

    return viscosityForceComponent;
}

ngl::Vector SPHSolver::calculateSurfaceTennsionGradientComponent(
                                                                  ngl::Real _massPerDensity,
                                                                  ngl::Real _surfaceColourCoefficient,
                                                                  ngl::Vector _seperationVector
                                                              )
{
    ngl::Vector surfaceTensionGradientComponent = 0;

    ngl::Vector surfaceGradient =  KernelPolyGradient(_seperationVector);

    surfaceTensionGradientComponent = _massPerDensity * _surfaceColourCoefficient * surfaceGradient;

    return surfaceTensionGradientComponent;
}

ngl::Real SPHSolver::calculateSurfaceTensionLaplacianComponent(
                                                                ngl::Real _massPerDensity,
                                                                ngl::Real _surfaceColourCoefficient,
                                                                ngl::Real _seperationLength
                                                            )

{
    ngl::Real surfaceTensionLaplacianComponent = 0;

    ngl::Real surfaceLaplacian = KernelPolyLaplacian(_seperationLength);

    surfaceTensionLaplacianComponent = _massPerDensity * _surfaceColourCoefficient * surfaceLaplacian;

    return surfaceTensionLaplacianComponent;
}

ngl::Real SPHSolver::KernelPoly(const ngl::Real _seperationLength)
{
    ngl::Real weight = m_weightPoly * (pow(pow(m_smoothingLength, 2) - pow(_seperationLength, 2), 3));

    return weight;
}

ngl::Vector SPHSolver::KernelPolyGradient(const ngl::Vector _seperationVector)
{
    ngl::Vector weight = 0;
    ngl::Real seperationLength = _seperationVector.length();

    weight = m_weightPolyGradient * _seperationVector * pow(pow(m_smoothingLength, 2) - pow(seperationLength, 2), 2);

    return weight;
}

ngl::Real SPHSolver::KernelPolyLaplacian(const ngl::Real _seperationLength)
{
    ngl::Real weight = m_weightPolyLaplacian * (pow(m_smoothingLength, 2) - pow(_seperationLength, 2)) * ((3.0 * pow(m_smoothingLength, 2)) - (7.0 * pow(_seperationLength, 2)));

    return weight;
}

ngl::Vector SPHSolver::KernelPressureGradient(const ngl::Vector _seperationVector)
{
    ngl::Vector weight = 0;
    ngl::Real seperationLength = _seperationVector.length();

    weight = m_weightPressureGradient * pow(m_smoothingLength - seperationLength, 2) * normalizeVector(_seperationVector);

    return weight;
}

ngl::Real SPHSolver::KernelViscosityLaplacian(const ngl::Real _seperationLength)
{
    ngl::Real weight = m_weightViscosityLaplacian * (m_smoothingLength - _seperationLength);

    return weight;
}

ngl::Vector SPHSolver::normalizeVector(const ngl::Vector _vector)
{
    ngl::Vector v = _vector;
    if (_vector.length() > 0.0001f) v.normalize();

    return v;
}

ngl::Vector SPHSolver::calculateInterfaceTensionGradientComponent(ngl::Real _massPerDensity, ngl::Real _interfaceColourCoefficient, ngl::Vector _seperationVector)
{

    ngl::Vector interfaceTensionGradientComponent;
    ngl::Vector surfaceGradient =  KernelPolyGradient(_seperationVector);

    interfaceTensionGradientComponent = _massPerDensity * _interfaceColourCoefficient * surfaceGradient;

    return interfaceTensionGradientComponent;

}

ngl::Real SPHSolver::calculateInterfaceTensionLaplacianComponent(ngl::Real _massPerDensity, ngl::Real _interfaceColourCoefficient, ngl::Real _seperationLength)

{

    ngl::Real interfaceTensionLaplacianComponent;
    ngl::Real surfaceLaplacian = KernelPolyLaplacian(_seperationLength);

    interfaceTensionLaplacianComponent = _massPerDensity * _interfaceColourCoefficient * surfaceLaplacian;

    return interfaceTensionLaplacianComponent;
}

std::vector<std::vector<ngl::Vector> > SPHSolver::getFluidParticlePositionList()
{
    std::vector<std::vector<ngl::Vector> > fluidParticlePositionsList;

    for (int i = 0; i< m_fluidNameList.size(); ++i)
    {
        std::string name = m_fluidNameList[i];
        std::vector<ngl::Vector> positionList;
        positionList.reserve(m_fluidParticleList.size());

        for (int k = 0; k<m_fluidParticleList.size(); ++k)
        {
            if (m_fluidParticleList[k].getName() == name)
            {
                positionList.push_back(m_fluidParticleList[k].getCurrentPosition());
            }
        }
        fluidParticlePositionsList.push_back(positionList);
    }
    return fluidParticlePositionsList;
}
