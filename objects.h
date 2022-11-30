#pragma once

#include <string>

#include "core/gpu/mesh.h"
#include "utils/glm_utils.h"


namespace objects
{
    // Creeaza pista
    Mesh* CreateRaceTrack(const std::string &name);

    // Creeaza masina
    Mesh* CreateCube(const std::string &name);

}
