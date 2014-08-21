/// @file EnvironmentAndRigidBody.cpp
/// @brief the master class that oversees the creation of the boundary and rigid bodies and manages their interaction with the fluids

#include "EnvironmentAndRigidBody.h"
#include "SimulationParameters.h"
#include "boost/foreach.hpp"

EnvironmentAndRigidBody::EnvironmentAndRigidBody()
{
    //create params from config
    SimulationParameters::initialiseEnvironment
                            (
                                m_obstacleEnabled,
                                m_boundaryPosition,
                                m_boundaryDimension,
                                m_boundaryRestitutionCoefficientForFluid,
                                m_sphereRBDList,
                                m_rbdSphereRestitutionCoefficient,
                                m_boundaryColour
                            );

    //create bbox for boundary
    m_boundaryBBox = new ngl::BBox(m_boundaryPosition, m_boundaryDimension.m_x, m_boundaryDimension.m_y, m_boundaryDimension.m_z);
}

EnvironmentAndRigidBody::~EnvironmentAndRigidBody()
{
    //clean up
    std::cout << "EnvironmentAndRigidBody Cleanup" << std::endl;

    delete m_boundaryBBox;
}


void EnvironmentAndRigidBody::checkCollision (
                                                Particle& io_fluidParticle
                                             )
{
    if (m_obstacleEnabled)
    {
        BOOST_FOREACH(Particle& rbdSphere, m_sphereRBDList)
        {
                /*-------Check Collisions against Rigid Body Spheres--------------------*/
                checkAndSolveCollisionWithRBDSphere(io_fluidParticle, rbdSphere);
        }
     }
    /*-----------Check Collision against the Boundry Box------------------*/
    checkAndSolveCollisionWithBoundary(io_fluidParticle);
}

void EnvironmentAndRigidBody::checkAndSolveCollisionWithRBDSphere
                        (
                            Particle& io_fluidParticle,
                            Particle& _rbdSphere
                        )
{

    ngl::Real restitutionCoefficient = m_rbdSphereRestitutionCoefficient;
    //initialise variables
    ngl::Vector X = io_fluidParticle.getCurrentPosition();
    ngl::Vector C = _rbdSphere.getCurrentPosition();
    ngl::Vector XC = X - C;
    ngl::Vector CX = C - X;

    ngl::Real r = io_fluidParticle.getCollisionRadius() + _rbdSphere.getCollisionRadius();

    //calculate implicit function value for a sphere
    ngl::Real Fx = XC.lengthSquared() - (r * r);

    if (Fx < 0)
    {
        //calculate contact point
        ngl::Vector CP = C + (r * (XC / XC.length()));

        //calculate normal
        ngl::Vector n = (Fx < 0 ? -1 : Fx == 0 ? 0 : 1) * (CX / CX.length());

        io_fluidParticle.updatePosition(CP);

        ngl::Real dot = n.dot(io_fluidParticle.getCurrentVelocity());

        ngl::Vector vel = io_fluidParticle.getCurrentVelocity() - ((1.0 + restitutionCoefficient) * dot * n);

        io_fluidParticle.updateVelocity(vel);
    }
}

void EnvironmentAndRigidBody::checkAndSolveCollisionWithBoundary
                        (
                            Particle& io_fluidParticle
                        )
{

    //determine bounds of box
    ngl::Real xMax = m_boundaryBBox->center().m_x + (m_boundaryBBox->width()/2.0f);
    ngl::Real xMin = m_boundaryBBox->center().m_x - (m_boundaryBBox->width()/2.0f);
    ngl::Real yMax = m_boundaryBBox->center().m_y + (m_boundaryBBox->height()/2.0f);
    ngl::Real yMin = m_boundaryBBox->center().m_y - (m_boundaryBBox->height()/2.0f);
    ngl::Real zMax = m_boundaryBBox->center().m_z + (m_boundaryBBox->depth()/2.0f);
    ngl::Real zMin = m_boundaryBBox->center().m_z - (m_boundaryBBox->depth()/2.0f);

    //move bounds closer due to difference in particle radius

    xMax -= io_fluidParticle.getCollisionRadius();
    xMin += io_fluidParticle.getCollisionRadius();
    yMax -= io_fluidParticle.getCollisionRadius();
    yMin += io_fluidParticle.getCollisionRadius();
    zMax -= io_fluidParticle.getCollisionRadius();
    zMin += io_fluidParticle.getCollisionRadius();

    //get position of particle
    ngl::Vector currentPosition = io_fluidParticle.getCurrentPosition();

    ngl::Real restitutionCoefficient = m_boundaryRestitutionCoefficientForFluid;

    if (currentPosition.m_x > xMax)
    {
        //set position to boundary edge
        io_fluidParticle.updatePosition(ngl::Vector(xMax, io_fluidParticle.getCurrentPosition().m_y, io_fluidParticle.getCurrentPosition().m_z));

        //calculate new velocity (reflect on face normal with restitution factor
        io_fluidParticle.updateVelocity(ngl::Vector(-restitutionCoefficient * io_fluidParticle.getCurrentVelocity().m_x, io_fluidParticle.getCurrentVelocity().m_y, io_fluidParticle.getCurrentVelocity().m_z));

    }
    if (currentPosition.m_x < xMin)
    {
        //set position to boundary edge
        io_fluidParticle.updatePosition(ngl::Vector(xMin, io_fluidParticle.getCurrentPosition().m_y, io_fluidParticle.getCurrentPosition().m_z));

        //calculate new velocity (reflect on face normal with restitution factor
        io_fluidParticle.updateVelocity(ngl::Vector(-restitutionCoefficient * io_fluidParticle.getCurrentVelocity().m_x, io_fluidParticle.getCurrentVelocity().m_y, io_fluidParticle.getCurrentVelocity().m_z));
    }
    if (currentPosition.m_y > yMax)
    {
        //set position to boundary edge
        io_fluidParticle.updatePosition(ngl::Vector(io_fluidParticle.getCurrentPosition().m_x, yMax, io_fluidParticle.getCurrentPosition().m_z));

        //calculate new velocity (reflect on face normal with restitution factor
        io_fluidParticle.updateVelocity(ngl::Vector(io_fluidParticle.getCurrentVelocity().m_x, -restitutionCoefficient * io_fluidParticle.getCurrentVelocity().m_y, io_fluidParticle.getCurrentVelocity().m_z));
    }
    if (currentPosition.m_y < yMin)
    {
        //set position to boundary edge
        io_fluidParticle.updatePosition(ngl::Vector(io_fluidParticle.getCurrentPosition().m_x, yMin, io_fluidParticle.getCurrentPosition().m_z));

        //calculate new velocity (reflect on face normal with restitution factor
        io_fluidParticle.updateVelocity(ngl::Vector(io_fluidParticle.getCurrentVelocity().m_x, -restitutionCoefficient * io_fluidParticle.getCurrentVelocity().m_y, io_fluidParticle.getCurrentVelocity().m_z));
    }
    if (currentPosition.m_z > zMax)
    {
        io_fluidParticle.updatePosition(ngl::Vector(io_fluidParticle.getCurrentPosition().m_x, io_fluidParticle.getCurrentPosition().m_y, zMax));

        //calculate new velocity (reflect on face normal with restitution factor
        io_fluidParticle.updateVelocity(ngl::Vector(io_fluidParticle.getCurrentVelocity().m_x, io_fluidParticle.getCurrentVelocity().m_y, -restitutionCoefficient * io_fluidParticle.getCurrentVelocity().m_z));
    }
    if (currentPosition.m_z < zMin)
    {
        io_fluidParticle.updatePosition(ngl::Vector(io_fluidParticle.getCurrentPosition().m_x, io_fluidParticle.getCurrentPosition().m_y, zMin));

        //calculate new velocity (reflect on face normal with restitution factor
        io_fluidParticle.updateVelocity(ngl::Vector(io_fluidParticle.getCurrentVelocity().m_x, io_fluidParticle.getCurrentVelocity().m_y, -restitutionCoefficient * io_fluidParticle.getCurrentVelocity().m_z));
    }

}
