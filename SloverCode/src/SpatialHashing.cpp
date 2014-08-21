/// @file SpatialHashing.cpp
/// @brief performs neighbour search using spatial hashing technique
// This class has been adapted from (Priscot, 2010). It has also been mentioned in the thesis.

#include "SpatialHashing.h"
#include "boost/foreach.hpp"


SpatialHashing::SpatialHashing
            (
                const int _numberOfFluidParticles,
                const float _smoothingLength
            )
{
    //initialise large prime numbers
    m_firstLargePrimeNumber = 73856093;
    m_secondLargePrimeNumber = 19349663;
    m_thirdLargePrimeNumber = 83492791;

    //initialise neighbour list
    m_initialNeighbourListSize = 50;

    //initialise hash params
    initialiseSpatialHashing(_numberOfFluidParticles, _smoothingLength);
}

SpatialHashing::~SpatialHashing()
{
    //clean up
    std::cout << "Neighbour Cleanup" << std::endl;
}

void SpatialHashing::initialiseSpatialHashing
                (
                    const int _numberOfFluidParticles,
                    const float _smoothingLength
                )
{
    //initialise cell size
    m_cellSize = _smoothingLength;

    //initialise hash map size
    m_mapSize = getNextPrimeNumber(2 * _numberOfFluidParticles);
}

int SpatialHashing::modulus(const int _a, const int _b)
{
    //perform huge integer modulus

    //issue of negative modulus : http://mathforum.org/library/drmath/view/52343.html
    //solved at stackoverflow : http://stackoverflow.com/questions/1082917/mod-of-negative-number-is-melting-my-brain

    //adapted from StackOverflow
    int modulus = _a % _b;
    return modulus < 0 ? modulus + _b : modulus;
}

//adapted from http://geekswithblogs.net/mimi02/articles/113773.aspx
int SpatialHashing::getNextPrimeNumber(const int _start)
{
    int i = _start;

    bool isPrimeNumberFound = true;
    bool isNextPrimeNumberFound = false;

    //Get Square root of number and iterate, start from 2
    int check = (int)sqrt((double)i);
    for (int j = 2; j <= check; j++)
    {
        if (i % j == 0)
        {
            isPrimeNumberFound = false;
            break;
        }
    }
    if (isPrimeNumberFound)
    {
        //i itself is the next prime number
        return i;
    }
    else
    {
        //i is not a prime number -> must search next one

        i++;
        isPrimeNumberFound = true;

        // Increment current number to find next prime number
        while (isNextPrimeNumberFound == false)
        {
            check = (int)sqrt((double)(i));

            for (int j = 2; j <= check; j++)
            {
                if (i % j == 0)
                {
                    isPrimeNumberFound = false;
                    break;
                }
            }

            if (isPrimeNumberFound)
                isNextPrimeNumberFound = true;
            else
            {
                i++;
                isPrimeNumberFound = true;
            }
        }

        if (isNextPrimeNumberFound)
        {
            //i is the next prime number

            return i;
        }
    }

    //eventually, nothing found?
    return _start;
}


std::vector<int> SpatialHashing::discretizingVector(const ngl::Vector _position)
{
    std::vector<int> pos;
    pos.reserve(3);

    pos.push_back(floor(_position.m_x / m_cellSize));
    pos.push_back(floor(_position.m_y / m_cellSize));
    pos.push_back(floor(_position.m_z / m_cellSize));

    return pos;
}

int SpatialHashing::hashPosition(const ngl::Vector _position)
{
    //discretizingVector position to array of int
    std::vector<int> discretePosition = discretizingVector(_position);

    //calculate hash key
    int key = modulus((discretePosition[0] * m_firstLargePrimeNumber) ^ (discretePosition[1] * m_secondLargePrimeNumber) ^ (discretePosition[2] * m_thirdLargePrimeNumber), m_mapSize);

    return key;
}

void SpatialHashing::refreshHashmap(std::vector<FluidParticle>& _particleList)
{
#pragma omp for schedule(dynamic, 50)
    //loop through each particle in the fluid
    for (int i = 0; i < _particleList.size(); ++i)
    {
        //hash position
        int hashKey = hashPosition(_particleList[i].getCurrentPosition());

#pragma omp critical
        //add (hashkey, particle) pair to hashmap
        m_hashMap.insert(std::pair<int, FluidParticle>(hashKey, _particleList[i]));
    }
}

void SpatialHashing::clearHashmap()
{
    //clears hashmap
    m_hashMap.clear();
}

bool SpatialHashing::inListCheck
                (
                    const int _testId,
                    std::vector<FluidParticle>& _neighbourList
                )
{
    //assume not found
    bool found = false;

    //loop through the temporary neighbour list
    for (int i = 0; i < _neighbourList.size(); i++)
    {
        if (_neighbourList[i].getId() == _testId)
        {
            found = true;
            break;
        }
    }

    return found;
}

void SpatialHashing::getNeighbouringCells
                        (
                            const ngl::Vector _centerPosition,
                            const bool _checkNeighboursAreInSmoothingLength,
                            std::vector<FluidParticle>& o_neighbourList
                        )
{
    //hash position
    int hashKey = hashPosition(_centerPosition);

    //get particles of cell[hashkey] from hash map
    std::pair<hashmap::iterator, hashmap::iterator> neighbourCandidateList = m_hashMap.equal_range(hashKey);

    //loop over neighbour list
    for (hashmap::iterator candidate = neighbourCandidateList.first; candidate != neighbourCandidateList.second; candidate++)
    {
        //get neighbour particle
        FluidParticle neighbourCandidate = (*candidate).second;

        //consider only non-duplicate candidates
        if (!inListCheck(neighbourCandidate.getId(), o_neighbourList))
        {
            //assume neighbour is valid
            bool isValidToAdd = true;

            //filter only neighbours within smoothing length if check is enabled
            if (_checkNeighboursAreInSmoothingLength)
            {
                //get separation distance between centerposition and neighbour position
                ngl::Real distance = (_centerPosition - neighbourCandidate.getCurrentPosition()).length();

                //check if distance is within smoothing length (cell size)
                if (distance > m_cellSize) isValidToAdd = false;
            }

            //add neighbour particle to neighbour list if valid
            if (isValidToAdd) o_neighbourList.push_back(neighbourCandidate);
        }
    }
}

std::vector<FluidParticle> SpatialHashing::searchFluidParticleNeighbours(const ngl::Vector _centerPosition)
{
    //initialise temp neighbour lists
    std::vector<FluidParticle> neighbourList;
    //neighbourList.reserve(m_initialNeighbourListSize);

    std::vector<FluidParticle> neighbourTempList;
    //neighbourTempList.reserve(m_initialNeighbourListSize);

    //first, get neighbours @ centerposition
    getNeighbouringCells(_centerPosition, false, neighbourTempList);

    //then, get neighbours from bounding box [centerPos - cellSize, centerPos + cellSize]
    ngl::Vector limit(m_cellSize, m_cellSize, m_cellSize);
    std::vector<int> discreteMinBounds = discretizingVector(_centerPosition - limit);
    std::vector<int> discreteMaxBounds = discretizingVector(_centerPosition + limit);

    //loop from minBound to maxBound and find neighbours in hashmap
    ngl::Real sampleStep = 0.85;
    for (ngl::Real x = discreteMinBounds[0]; x < discreteMaxBounds[0]; x += sampleStep)
    {
        for (ngl::Real y = discreteMinBounds[1]; y < discreteMaxBounds[1]; y += sampleStep)
        {
            for (ngl::Real z = discreteMinBounds[2]; z < discreteMaxBounds[2]; z += sampleStep)
            {
                //fill in neighbour list with neighbours @ test position(x,y,z)
                getNeighbouringCells(ngl::Vector(x, y, z), true, neighbourTempList);
            }
        }
    }

    //do a final distance check to ensure neighbours are within smoothing length
    for (int i = 0; i < neighbourTempList.size(); ++i)
    {
        //get separation distance between centerposition and neighbour position
        ngl::Real distance = (_centerPosition - neighbourTempList[i].getCurrentPosition()).length();

        //add to final list only if distance is within smoothing length (cell size)
        if (distance < m_cellSize) neighbourList.push_back(neighbourTempList[i]);
    }

    //return final list
    return neighbourList;
}
