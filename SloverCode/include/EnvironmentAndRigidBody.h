#ifndef ENVIRONMENTANDRIGIDBODY_H
#define ENVIRONMENTANDRIGIDBODY_H

#include "ngl/Vector.h"
#include "Particle.h"

/// @file EnvironmentAndRigidBody.h
/// @brief manages interaction between rigid body spheres and boundary and fluids
/// @author Shalin K Banjara
/// @version 1.0
/// @date 05.11.11
/// Revision History :
/// Initial Version 05.11.11
/// @class EnvironmentAndRigidBody
/// @brief stores all rigid body spheres and boundary and manages collisions of fluids with rigid body spheres and Boundary Box

class EnvironmentAndRigidBody
{
public:
    /// @brief ctor
    EnvironmentAndRigidBody();

    /// @brief dtor
    ~EnvironmentAndRigidBody();

    /// @brief detects and handles collision among particles (sphere rbd and fluid particle)
    /// @param[in,out] io_fluidParticle checks for collision with boundary and rigidbody
    void checkCollision
            (
                Particle& io_fluidParticle
            );

    /// @brief returns whether RBD interaction is enabled
    inline bool getObstacleEnabled() const { return m_obstacleEnabled; }

    /// @brief returns the restitution coefficient for fluid interaction with boundary
    inline ngl::Real getBoundaryRestitutionCoefficientForFluid() const { return m_boundaryRestitutionCoefficientForFluid; }

    /// @brief return the restitution coefficient for fluid interaction with RBD
    inline ngl::Real getRBDSphereRestitutionCoefficient() const { return m_rbdSphereRestitutionCoefficient; }

    /// @brief return the list of passive Sphere Rigid Bodies
    inline std::vector<Particle>& getRBDSphereList() { return m_sphereRBDList; }

    /// @brief set the restitution coefficient for fluid interaction with boundary
    /// @param[in] _value the new value
    inline void setBoundaryRestitutionCoefficientForFluid(const ngl::Real _value) { m_boundaryRestitutionCoefficientForFluid = _value; }

    /// @brief return a pointer to Boundary Box
    inline ngl::BBox* getBBox() {return m_boundaryBBox;}

    /// @brief return the colour of Boundary Box required for Rendering
    inline ngl::Colour getBoundaryColour() {return m_boundaryColour;}


private:
    /// @brief flag to determine if RBD interaction is enabled
    bool m_obstacleEnabled;

    /// @brief boundary dimension
    ngl::Vector m_boundaryDimension;

    /// @brief boundary centre position
    ngl::Vector m_boundaryPosition;

    /// @brief boundary bounding box object used for collision handling
    ngl::BBox* m_boundaryBBox;

    /// @brief restitution coefficient for fluid interaction with boundary
    ngl::Real m_boundaryRestitutionCoefficientForFluid;

    /// @brief restitution coefficient for fluid interaction with RBD
    ngl::Real m_rbdSphereRestitutionCoefficient;

    /// @brief list of passive Sphere Rigid Bodies
    std::vector<Particle> m_sphereRBDList;

    /// @brief colour for the Boundary Box
    ngl::Colour m_boundaryColour;

private:
    /// @brief handle collision between fluid and passive sphere rigid body
    /// @param[in,out] io_fluidParticle the particle affected by the collision resolution
    /// @param[in] _rbdSphere the rigid body sphere against which collision is checked
    void checkAndSolveCollisionWithRBDSphere
                    (
                        Particle& io_fluidParticle,
                        Particle& _rbdSphere
                    );

    /// @brief handle collision between fluid and boundary box
    /// @param[in,out] io_fluidParticle the particle affected by the collision resolution
    void checkAndSolveCollisionWithBoundary
                    (
                        Particle& io_fluidParticle
                    );

};

#endif // ENVIRONMENTANDRIGIDBODY_H
