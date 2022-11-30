#include "objects.h"

#include <vector>
//#include <iostream>

#include "core/engine.h"
#include "utils/gl_utils.h"


Mesh* objects::CreateRaceTrack(const std::string &name)
{
    std::vector<glm::vec3> points = 
    {
        glm::vec3( 5.05f, 0.01f,  0.29f), // A
        glm::vec3( 4.21f, 0.01f,  1.40f), // B
        glm::vec3( 2.79f, 0.01f,  2.14f), // C
        glm::vec3( 1.50f, 0.01f,  1.95f), // D
        glm::vec3( 0.59f, 0.01f,  1.25f), // E
        glm::vec3(-0.48f, 0.01f,  0.90f), // F
        glm::vec3(-1.76f, 0.01f,  1.15f), // G
        glm::vec3(-2.83f, 0.01f,  1.43f), // H
        glm::vec3(-3.79f, 0.01f,  0.81f), // I
        glm::vec3(-4.13f, 0.01f, -0.19f), // J
        glm::vec3(-3.45f, 0.01f, -1.05f), // K
        glm::vec3(-2.11f, 0.01f, -1.55f), // L
        glm::vec3(-0.68f, 0.01f, -1.62f), // M
        glm::vec3( 0.44f, 0.01f, -0.98f), // N
        glm::vec3( 1.42f, 0.01f, -0.50f), // O
        glm::vec3( 2.58f, 0.01f, -0.78f), // P
        glm::vec3( 3.45f, 0.01f, -1.45f), // Q
        glm::vec3( 4.57f, 0.01f, -1.20f)  // R
    };

    std::vector<glm::vec3> extPoints;
    std::vector<glm::vec3> intPoints;

    std::vector<VertexFormat> vertices;
    std::vector<unsigned int> indices;

    glm::vec3 up = glm::vec3(0, 1, 0);
    glm::vec3 extDist = glm::vec3(0.5f);
    glm::vec3 intDist = glm::vec3(0.3f);

    for (int i = 0; i < points.size() - 1; i++) {
        glm::vec3 d = points[i + 1] - points[i];
        glm::vec3 p = glm::cross(d, up);
        glm::vec3 extPoint = points[i] + extDist * p;
        extPoints.push_back(extPoint);
        glm::vec3 intPoint = points[i] - intDist * p;
        intPoints.push_back(intPoint);
    }

    glm::vec3 d = points[0] - points[points.size() - 1];
    glm::vec3 p = glm::cross(d, up);
    glm::vec3 extPoint = points[points.size() - 1] + extDist * p;
    extPoints.push_back(extPoint);
    glm::vec3 intPoint = points[points.size() - 1] - intDist * p;
    intPoints.push_back(intPoint);

    float K = 1000;
    glm::vec3 dist;

    for (int i = 0; i < extPoints.size() - 1; i++) {
        dist = (extPoints[i + 1] - extPoints[i]) / glm::vec3(K);
        for (int k = 0; k < K; k++) {
            vertices.push_back(extPoints[i] + glm::vec3(k) * dist);
        }
    }
    dist = (extPoints[0] - extPoints[extPoints.size() - 1]) / glm::vec3(K);
    for (int k = 0; k < K; k++) {
        vertices.push_back(extPoints[extPoints.size() - 1] + glm::vec3(k) * dist);
    }

    for (int i = 0; i < intPoints.size() - 1; i++) {
        dist = (intPoints[i + 1] - intPoints[i]) / glm::vec3(K);
        for (int k = 0; k < K; k++) {
            vertices.push_back(intPoints[i] + glm::vec3(k) * dist);
        }
    }
    dist = (intPoints[0] - intPoints[intPoints.size() - 1]) / glm::vec3(K);
    for (int k = 0; k < K; k++) {
        vertices.push_back(intPoints[intPoints.size() - 1] + glm::vec3(k) * dist);
    }

    // for (int i = 0; i < extPoints.size(); i++) {
    //     vertices.push_back(extPoints[i]);
    // }
    // for (int i = 0; i < intPoints.size(); i++) {
    //     vertices.push_back(intPoints[i]);
    // }


    int nr = vertices.size() / 2;

    for (int i = 0; i < nr - 1; i++) {
        int j = i + nr;

        //std::cout << i << " cu " << j << " cu " << (j + 1) << "\n";
        indices.push_back(i);
        indices.push_back(j);
        indices.push_back(j + 1);
        //indices.push_back(i); // ?

        //std::cout << i << " cu " << (j + 1) << " cu " << (i + 1) << "\n";
        indices.push_back(i);
        indices.push_back(j + 1);
        indices.push_back(i + 1);
        //indices.push_back(i); // ?
    }

    //std::cout << (nr - 1) << " cu " << (nr - 1 + nr) << " cu " << nr << "\n";
    indices.push_back(nr - 1);
    indices.push_back(nr - 1 + nr);
    indices.push_back(nr);
    //indices.push_back(nr - 1); // ?

    //std::cout << (nr - 1) << " cu " << nr << " cu " << 0 << "\n";
    indices.push_back(nr - 1);
    indices.push_back(nr);
    indices.push_back(0);
    //indices.push_back(nr - 1); // ?

    Mesh* racetrack = new Mesh(name);
    //racetrack->SetDrawMode(GL_LINE_LOOP);

    racetrack->InitFromData(vertices, indices);
    return racetrack;
}

Mesh* objects::CreateCube(const std::string &name)
{

    std::vector<VertexFormat> vertices =
    {
        glm::vec3(-1, -1,  1),
        glm::vec3( 1, -1,  1),
        glm::vec3(-1,  1,  1),
        glm::vec3( 1,  1,  1),
        glm::vec3(-1, -1, -1),
        glm::vec3( 1, -1, -1),
        glm::vec3(-1,  1, -1),
        glm::vec3( 1,  1, -1)
    };

    std::vector<unsigned int> indices =
    {
        0, 1, 2,        1, 3, 2,
        2, 3, 7,        2, 7, 6,
        1, 7, 3,        1, 5, 7,
        6, 7, 4,        7, 5, 4,
        0, 4, 1,        1, 4, 5,
        2, 6, 4,        0, 2, 4,
    };

    Mesh* cube = new Mesh(name);

    cube->InitFromData(vertices, indices);
    return cube;
}

