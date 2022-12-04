#include "lab_m1/tema2/tema2.h"

using namespace std;
using namespace m1;


Tema2::Tema2()
{
}


Tema2::~Tema2()
{
}


void Tema2::Init()
{

    points = 
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

    camera = new implemented::LabCamera();
    camera->Set(glm::vec3(0, 1, 2.5f), glm::vec3(0, 1, 0), glm::vec3(0, 1, 0));

    trackScale = glm::vec3(5);

    carLength = 1;
    carScale = glm::vec3(0.3f, 0.4f, 0.5f);

    trunkLength = 0.2f;
    trunkScale = glm::vec3(1, 2, 1);

    crownLength = 0.75f;
    crownScale = glm::vec3(1, 2, 1);

    rotateMatrix = glm::mat4(1);
    
    camera->MoveForward(7.5f);
    camera->RotateThirdPerson_OY(M_PI / 3);
    rotateMatrix *= transform3D::RotateOY(M_PI / 3);

    Mesh* plane = new Mesh("plane");
    plane->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "plane50.obj");
    AddMeshToList(plane);

    objects::GenPoints(points, &extPoints, &intPoints, glm::vec3(0.5f), glm::vec3(0.3f));


    Mesh* racetrack = objects::CreateRaceTrack("racetrack", 
                                            extPoints, intPoints, &vertices, &indices);
    AddMeshToList(racetrack);

    Mesh* car = objects::CreateCube("car", glm::vec3(-carLength / 2, 0, 0), carLength);
    AddMeshToList(car);

    Mesh* trunk = objects::CreateCube("trunk", glm::vec3(0, 0, 0), trunkLength);
    AddMeshToList(trunk);

    Mesh* crown = objects::CreatePyramid("crown", glm::vec3(0, 0, 0), crownLength);
    AddMeshToList(crown);

    {
        Shader *shader = new Shader("LabShader");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "tema2", "shaders", "VertexShader.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "tema2", "shaders", "FragmentShader.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }
}


void Tema2::FrameStart()
{
    glClearColor(0.4f, 0.7, 1, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}


void Tema2::Update(float deltaTimeSeconds)
{
    glm::ivec2 resolution = window->GetResolution();
    miniViewportArea = ViewportArea(50, 50, resolution.x / 5.f, resolution.y / 5.f);

    camera->RotateThirdPerson_OX(-M_PI / 6);

    glViewport(0, 0, resolution.x, resolution.y);
    projectionMatrix = glm::perspective(RADIANS(60), window->props.aspectRatio, 0.01f, 200.0f);
    RenderScene();

    camera->RotateThirdPerson_OX(M_PI / 6);


    camera->RotateThirdPerson_OX(-M_PI / 2);
    camera->up = glm::vec3(1, 0, 0);

    glViewport(miniViewportArea.x, miniViewportArea.y, miniViewportArea.width, miniViewportArea.height);
    projectionMatrix = glm::ortho(-10.0f, 10.0f, -5.0f, 5.0f, 0.01f, 200.0f);
    RenderScene();

    camera->RotateThirdPerson_OX(M_PI / 2);
}


void Tema2::FrameEnd()
{
}


void Tema2::RenderScene()
{
    {
        modelMatrix = glm::mat4(1);
        modelMatrix *= transform3D::Scale(trackScale.x, trackScale.y, trackScale.z);
        RenderMesh(meshes["plane"], shaders["LabShader"], modelMatrix, glm::vec3(0.1f, 0.45f, 0.1f));
    }

    {
        modelMatrix = glm::mat4(1);
        modelMatrix *= transform3D::Scale(trackScale.x, trackScale.y, trackScale.z);
        RenderMesh(meshes["racetrack"], shaders["LabShader"], modelMatrix, glm::vec3(0.15f, 0.15f, 0.15f));
    }

    {
        RenderTrees();
    }

    {
        glm::vec3 carPosition = camera->GetTargetPosition();
        carPosition.y = 0;

        modelMatrix = glm::mat4(1);
        modelMatrix *= transform3D::Translate(carPosition.x, carPosition.y, carPosition.z);
        modelMatrix *= rotateMatrix;
        modelMatrix *= transform3D::Scale(carScale.x, carScale.y, carScale.z);
        RenderMesh(meshes["car"], shaders["LabShader"], modelMatrix, glm::vec3(0.3f, 0.1f, 0.7f));

        carMatrix = modelMatrix;
    }

    {
        RenderOtherCars();
    }

    //DrawCoordinateSystem(camera->GetViewMatrix(), projectionMatrix);
}


void Tema2::RenderTrees()
{
    for (glm::vec3 spawnPoint : extPoints) {
        RenderTree(spawnPoint * trackScale);
    }

    for (glm::vec3 spawnPoint : intPoints) {
        RenderTree(spawnPoint * trackScale);
    }
}


void Tema2::RenderTree(glm::vec3 spawnPoint)
{
    modelMatrix = glm::mat4(1);
    modelMatrix *= transform3D::Translate(spawnPoint.x, spawnPoint.y, spawnPoint.z);
    modelMatrix *= transform3D::Scale(trunkScale.x, trunkScale.y, trunkScale.z);
    RenderMesh(meshes["trunk"], shaders["LabShader"], modelMatrix, glm::vec3(0.3f, 0.2f, 0));

    modelMatrix = glm::mat4(1);
    modelMatrix *= transform3D::Translate(spawnPoint.x, spawnPoint.y, spawnPoint.z);
    glm::vec3 toCenter;
    toCenter.x = trunkLength * trunkScale.x / 2 - crownLength * crownScale.x / 2;
    toCenter.y = trunkLength * trunkScale.y;
    toCenter.z = trunkLength * trunkScale.z / 2 - crownLength * crownScale.z / 2;
    modelMatrix *= transform3D::Translate(toCenter.x, toCenter.y, toCenter.z);
    modelMatrix *= transform3D::Scale(crownScale.x, crownScale.y, crownScale.z);
    RenderMesh(meshes["crown"], shaders["LabShader"], modelMatrix, glm::vec3(0.1f, 0.3f, 0.1f));
}


void Tema2::RenderOtherCars()
{
    vector<glm::vec3> carPoints1, carPoints2;

    objects::GenPoints(points, &carPoints1, &carPoints2, glm::vec3(0.2f), glm::vec3(0.1f));
    
    nextPoints.push_back(0);
    nextPoints.push_back(0);

    float K = 500;
    carPoints.push_back(objects::GenMorePoints(carPoints1, K));
    carPoints.push_back(objects::GenMorePoints(carPoints2, K));

    for (int i = 0; i < 2; i++) {

        int nextPoint = nextPoints[i];

        glm::vec3 carPosition = carPoints[i][nextPoint] * trackScale;
        carPosition.y = 0;

        int currPoint = nextPoint;
        nextPoint++;
        nextPoints[i]++;
        if (nextPoint == carPoints[i].size()) {
            nextPoint = 0;
            nextPoints[i] = 0;
        }

        float x1 = carPoints[i][currPoint].x * trackScale.x;
        float z1 = carPoints[i][currPoint].z * trackScale.z;
        float x2 = carPoints[i][nextPoint].x * trackScale.x;
        float z2 = carPoints[i][nextPoint].z * trackScale.z;

        float yRot = atan((x2 - x1) / (z2 - z1));

        modelMatrix = glm::mat4(1);
        modelMatrix *= transform3D::Translate(carPosition.x, carPosition.y, carPosition.z);
        modelMatrix *= transform3D::RotateOY(yRot);
        modelMatrix *= transform3D::Scale(carScale.x, carScale.y, carScale.z);
        RenderMesh(meshes["car"], shaders["LabShader"], modelMatrix, glm::vec3(0, 1, 1));

    }
}


void Tema2::RenderMesh(Mesh *mesh, Shader *shader, const glm::mat4 &modelMatrix, const glm::vec3 &color)
{
    if (!mesh || !shader || !shader->program)
        return;

    // Render an object using the specified shader and the specified position
    shader->Use();
    glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));
    glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
    glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    int car_location = glGetUniformLocation(shader->program, "CarModel");
    glUniformMatrix4fv(car_location, 1, GL_FALSE, glm::value_ptr(carMatrix));

    int color_location = glGetUniformLocation(shader->program, "Color");
    glUniform3fv(color_location, 1, glm::value_ptr(color));

    mesh->Render();
}


void Tema2::OnInputUpdate(float deltaTime, int mods)
{
    float cameraSpeed = 3.5f;

    if (window->KeyHold(GLFW_KEY_SPACE)) { // accelerare
        cameraSpeed *= 2;
    }

    // copie camera (pentru a afla urmatoarea miscare fara a o executa)
    implemented::LabCamera* c = new implemented::LabCamera();
    c->position = camera->position;
    c->forward = camera->forward;
    c->up = camera->up;
    c->right = camera->right;


    if (window->KeyHold(GLFW_KEY_W)) {
        // Miscare in fata a camerei
        c->MoveForward(cameraSpeed * deltaTime);

        if (IsInsideRacetrack(c->GetTargetPosition())) {
            camera->MoveForward(cameraSpeed * deltaTime);
        }
    }

    if (window->KeyHold(GLFW_KEY_S)) {
        // Miscare in spate a camerei
        c->MoveForward(-cameraSpeed * deltaTime);

        if (IsInsideRacetrack(c->GetTargetPosition())) {
            camera->MoveForward(-cameraSpeed * deltaTime);
        }
    }

    if (window->KeyHold(GLFW_KEY_A)) {
        // Rotatie spre stanga a camerei
        camera->RotateThirdPerson_OY(cameraSpeed * deltaTime * 0.25f);
        // Rotatie spre stanga a masinii
        rotateMatrix *= transform3D::RotateOY(cameraSpeed * deltaTime * 0.25f);
    }

    if (window->KeyHold(GLFW_KEY_D)) {
        // Rotatie spre dreapta a camerei
        camera->RotateThirdPerson_OY(-cameraSpeed * deltaTime * 0.25f);
        // Rotatie spre dreapta a camerei
        rotateMatrix *= transform3D::RotateOY(-cameraSpeed * deltaTime * 0.25f);
    }
}


bool Tema2::IsInsideRacetrack(glm::vec3 p)
{
    for (int i = 0; i < indices.size() - 2; i += 3) {
        glm::vec3 a = vertices[indices[i]].position * trackScale;
        glm::vec3 b = vertices[indices[i + 1]].position * trackScale;
        glm::vec3 c = vertices[indices[i + 2]].position * trackScale;

        if (IsInsideTriangle(p, a, b, c)) {
            return true;
        }
    }

    return false;
}


bool Tema2::IsInsideTriangle(glm::vec3 p, glm::vec3 a, glm::vec3 b, glm::vec3 c)
{
    return SameSide(p, a, b, c) && SameSide(p, b, a, c) && SameSide(p, c, a, b);
}


bool Tema2::SameSide(glm::vec3 p1, glm::vec3 p2, glm::vec3 a, glm::vec3 b)
{
    return glm::dot(glm::cross(b - a, p1 - a), glm::cross(b - a, p2 - a)) >= 0;
}


void Tema2::OnKeyPress(int key, int mods)
{
}


void Tema2::OnKeyRelease(int key, int mods)
{
}


void Tema2::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
}


void Tema2::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
}


void Tema2::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
}


void Tema2::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Tema2::OnWindowResize(int width, int height)
{
}
