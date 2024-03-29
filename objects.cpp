#include "objects.h"

#include "core/engine.h"
#include "utils/gl_utils.h"


void objects::GenPoints(vector<glm::vec3> points, 
                    vector<glm::vec3> &extPoints, vector<glm::vec3> &intPoints,
                    glm::vec3 extDist, glm::vec3 intDist)
{
    glm::vec3 up = glm::vec3(0, 1, 0);

    for (int i = 0; i < points.size() - 1; i++) {
        glm::vec3 d = points[i + 1] - points[i];
        glm::vec3 p = glm::cross(d, up);
        glm::vec3 extPoint = points[i] - extDist * p;
        extPoints.push_back(extPoint);
        glm::vec3 intPoint = points[i] + intDist * p;
        intPoints.push_back(intPoint);
    }

    glm::vec3 d = points[0] - points[points.size() - 1];
    glm::vec3 p = glm::cross(d, up);
    glm::vec3 extPoint = points[points.size() - 1] - extDist * p;
    extPoints.push_back(extPoint);
    glm::vec3 intPoint = points[points.size() - 1] + intDist * p;
    intPoints.push_back(intPoint);
}


vector<glm::vec3> objects::GenMorePoints(vector<glm::vec3> points, float K)
{
    vector<glm::vec3> morePoints;

    for (int i = 0; i < points.size() - 1; i++) {
        glm::vec3 dist = (points[i + 1] - points[i]) / glm::vec3(K);
        for (int k = 0; k < K; k++) {
            morePoints.push_back(points[i] + glm::vec3(k) * dist);
        }
    }
    glm::vec3 dist = (points[0] - points[points.size() - 1]) / glm::vec3(K);
    for (int k = 0; k < K; k++) {
        morePoints.push_back(points[points.size() - 1] + glm::vec3(k) * dist);
    }

    return morePoints;
}



Mesh* objects::CreateRacetrack(const std::string &name, bool continuous,
                            vector<glm::vec3> extPoints, vector<glm::vec3> intPoints,
                            vector<VertexFormat> &vertices, vector<unsigned int> &indices)
{
    float K = 100;

    /* Se adauga mai intai toti vertecsii corespunzatori punctelor exterioare, 
    apoi ai punctelor interioare */
    for (glm::vec3 v : GenMorePoints(extPoints, K)) {
        vertices.push_back(v);
    }
    for (glm::vec3 v : GenMorePoints(intPoints, K)) {
        vertices.push_back(v);
    }

    /* La indexul nr (jumatate din numarul tuturor vertecsilor) se va afla primul
    index corespunzator vertecsilor punctelor interioare */
    int nr = vertices.size() / 2;

    for (int i = 0; i < nr - 1; i++) { // i = vertex punct exterior
        int j = i + nr; // j = vertex punct interior corespunzator aceluiasi punct

        indices.push_back(i);
        indices.push_back(j);
        indices.push_back(j + 1);

        indices.push_back(i);
        indices.push_back(j + 1);
        indices.push_back(i + 1);

        /* Pentru a se crea efect de discontinuitate (folosit la liniile de pe 
        marginea pistei), o data la 10 puncte se va sari peste 5 puncte */
        if (!continuous) {
            if (i % 10 == 0) {
                i += 5;
            }
        }
    }

    indices.push_back(nr - 1);
    indices.push_back((nr - 1) + nr);
    indices.push_back(nr);

    indices.push_back(nr - 1);
    indices.push_back(nr);
    indices.push_back(0);

    Mesh* racetrack = new Mesh(name);

    racetrack->InitFromData(vertices, indices);
    return racetrack;
}


Mesh* objects::CreateCube(const std::string &name, glm::vec3 leftBottomCorner, float length)
{
    std::vector<VertexFormat> vertices =
    {
        leftBottomCorner,
        leftBottomCorner + glm::vec3(length, 0, 0),
        leftBottomCorner + glm::vec3(length, 0, length), 
        leftBottomCorner + glm::vec3(0, 0, length),
        leftBottomCorner + glm::vec3(0, length, 0), 
        leftBottomCorner + glm::vec3(length, length, 0), 
        leftBottomCorner + glm::vec3(length, length, length),
        leftBottomCorner + glm::vec3(0, length, length)
    };

    std::vector<unsigned int> indices =
    {
        0, 1, 3,        3, 1, 2,
        0, 1, 4,        4, 1, 5,
        1, 2, 5,        5, 2, 6,
        3, 0, 7,        7, 0, 4,
        2, 3, 6,        6, 3, 7,
        4, 5, 7,        7, 5, 6,
    };

    Mesh* cube = new Mesh(name);

    cube->InitFromData(vertices, indices);
    return cube;
}


Mesh* objects::CreatePyramid(const std::string &name, glm::vec3 leftBottomCorner, float length)
{
    std::vector<VertexFormat> vertices =
    {
        leftBottomCorner,
        leftBottomCorner + glm::vec3(length, 0, 0),
        leftBottomCorner + glm::vec3(length, 0, length), 
        leftBottomCorner + glm::vec3(0, 0, length),
        leftBottomCorner + glm::vec3(length / 2, length, length / 2)
    };

    std::vector<unsigned int> indices =
    {
        0, 1, 3,
        3, 1, 2,
        0, 1, 4,
        1, 2, 4, 
        3, 0, 4,
        2, 3, 4,
    };

    Mesh* pyramid = new Mesh(name);

    pyramid->InitFromData(vertices, indices);
    return pyramid;
}
