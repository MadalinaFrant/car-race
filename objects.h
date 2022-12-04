#pragma once

#include <string>
#include <vector>

#include "core/gpu/mesh.h"
#include "utils/glm_utils.h"

using namespace std;


namespace objects
{

    void GenPoints(vector<glm::vec3> points, 
                vector<glm::vec3> *extPoints, vector<glm::vec3> *intPoints,
                glm::vec3 extDist, glm::vec3 intDist);

    vector<glm::vec3> GenMorePoints(vector<glm::vec3> points, float K);

    // Creeaza pista
    Mesh* CreateRaceTrack(const std::string &name,
                        vector<glm::vec3> extPoints, vector<glm::vec3> intPoints,
                        vector<VertexFormat> *vertices, vector<unsigned int> *indices);

    // Creeaza cub
    Mesh* CreateCube(const std::string &name, glm::vec3 leftBottomCorner, float length);

    // Creeaza piramida
    Mesh* CreatePyramid(const std::string &name, glm::vec3 leftBottomCorner, float length);


    float DistToLine(glm::vec3 a, glm::vec3 b, glm::vec3 p);
}
