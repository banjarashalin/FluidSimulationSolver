#ifndef SPATIALHASHING_H
#define SPATIALHASHING_H

#include "map"

#include "FluidParticle.h"

/// @typedef hashmap multimap structure for the spatial hash map
typedef std::multimap<int, FluidParticle> hashmap;

/// @file SpatialHashing.h
/// @brief handles neighbour search using spatial hashing
/// @author Shalin Kiran Banjara
/// @version 1.0
/// @date 25.11.11
/// Revision History :
/// Initial Version 25.11.11
/// @class SpatialHashing
/// @brief implements spatial hashing for the search of fluid particle neighbours

class SpatialHashing
{
public:
    /// @brief ctor
    /// @param[in] _numberOfFluidParticles the fluid particle count
    /// @param[in] _smoothingLength the smoothing length
    SpatialHashing
            (
                const int _numberOfFluidParticles,
                const float _smoothingLength
            );

    /// @brief dtor
    ~SpatialHashing();

    /// @brief initialiseSpatialHashing the spatial structure
    /// @param[in] _numberOfFluidParticles the fluid particle count
    /// @param[in] _smoothingLength the smoothing length
    void initialiseSpatialHashing
            (
                const int _numberOfFluidParticles,
                const float _smoothingLength
            );

    /// @brief refresh the hash map at each iteration
    /// @param[in] _particleList the fluid particles
    void refreshHashmap(std::vector<FluidParticle>& _particleList);

    /// @brief clear the hash map
    void clearHashmap();

    /// @brief return neighbours of a particle
    /// @param[in] _centerPosition the particle's position
    std::vector<FluidParticle> searchFluidParticleNeighbours(const ngl::Vector _centerPosition);

    /// @brief set the cell size of the spatial structure
    /// @param[in] _size the new cell size
    inline void setCellSize(const ngl::Real _size) { m_cellSize = _size; }


private:
    /// @brief the spatial hash map
    hashmap m_hashMap;

    /// @brief the cell size used for spatial calculations
    float m_cellSize;

    /// @brief the map size
    int m_mapSize;

    /// @brief large prime number 1
    int m_firstLargePrimeNumber;

    /// @brief large prime number 2
    int m_secondLargePrimeNumber;

    /// @brief large prime number 3
    int m_thirdLargePrimeNumber;

    /// @brief reserved space size for neighbours
    int m_initialNeighbourListSize;

private:
    /// @brief calculate the modulus of 2 numbers
    /// @param[in] _a the first number
    /// @param[in] _b the second number
    int modulus
            (
                const int _a,
                const int _b
            );

    /// @brief return the next prime number
    /// @param[in] _start the start number from which to find the next prime number
    int getNextPrimeNumber(const int _start);

    /// @brief discretise an array of vectors into an array of integers
    /// @param[in] _position the vector position to discretize
    std::vector<int> discretizingVector(const ngl::Vector _position);

    /// @brief hash a position to a hash key
    /// @param[in] _position the position to hash
    int hashPosition(const ngl::Vector _position);

    /// @brief determine neighbours of a fluid particle, given its position
    /// @param[in] _centerPosition the position of the fluid particle
    /// @param[in] _checkNeighboursAreInSmoothingLength flag to determine whether only neighbours within the smoothing length are added
    /// @param[out] o_neighbourList stores the list neighbours
    void getNeighbouringCells
                    (
                        const ngl::Vector _centerPosition,
                        const bool _checkNeighboursAreInSmoothingLength,
                        std::vector<FluidParticle>& o_neighbourList
                    );

    /// @brief check if a neighbour is a duplicate
    /// @param[in] _testId the id of the neighbour to check in the list
    /// @param[in] _neighbourList the list of neighbour that will be used for the duplicate check
    bool inListCheck
                    (
                        const int _testId,
                        std::vector<FluidParticle>& _neighbourList
                    );


};

#endif // SPATIALHASHING_H
