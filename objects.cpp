#include "objects.h"

#include <vector>

#include "core/engine.h"
#include "utils/gl_utils.h"


Mesh* objects::CreateRaceTrack(const std::string &name, glm::vec3 color)
{
    std::vector<VertexFormat> vertices =
    {

    };

    std::vector<unsigned int> indices = 
    { 

    };

    Mesh* square = new Mesh(name);

    square->InitFromData(vertices, indices);
    return square;
}

Mesh* objects::CreateCube(const std::string &name, glm::vec3 color)
{

    std::vector<VertexFormat> vertices =
    {
        VertexFormat(glm::vec3(-1, -1,  1), color),
        VertexFormat(glm::vec3( 1, -1,  1), color),
        VertexFormat(glm::vec3(-1,  1,  1), color),
        VertexFormat(glm::vec3( 1,  1,  1), color),
        VertexFormat(glm::vec3(-1, -1, -1), color),
        VertexFormat(glm::vec3( 1, -1, -1), color),
        VertexFormat(glm::vec3(-1,  1, -1), color),
        VertexFormat(glm::vec3( 1,  1, -1), color)
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

