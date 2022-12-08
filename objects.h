#pragma once

#include <string>
#include <vector>

#include "core/gpu/mesh.h"
#include "utils/glm_utils.h"

using namespace std;


namespace objects
{
    /* Genereaza multimea punctelor paralele exterioare si interioare,
    la distantele date */
    void GenPoints(vector<glm::vec3> points, 
                vector<glm::vec3> &extPoints, vector<glm::vec3> &intPoints,
                glm::vec3 extDist, glm::vec3 intDist);

    /* Genereaza mai multe puncte intre punctele date, adaugand K puncte 
    intre fiecare 2 puncte */
    vector<glm::vec3> GenMorePoints(vector<glm::vec3> points, float K);

    /* Creeaza pista */
    Mesh* CreateRacetrack(const std::string &name, bool continuous, 
                        vector<glm::vec3> extPoints, vector<glm::vec3> intPoints,
                        vector<VertexFormat> &vertices, vector<unsigned int> &indices);

    /* Creeaza cub cu coltul de stanga jos si lungimea date */
    Mesh* CreateCube(const std::string &name, glm::vec3 leftBottomCorner, float length);

    /* Creeaza piramida cu coltul de stanga jos si lungimea date */
    Mesh* CreatePyramid(const std::string &name, glm::vec3 leftBottomCorner, float length);
}
