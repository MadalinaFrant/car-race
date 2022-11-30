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
    camera = new implemented::LabCamera();
    camera->Set(glm::vec3(0, 1, 2.5f), glm::vec3(0, 1, 0), glm::vec3(0, 1, 0));

    carLength = 2;
    carScale = glm::vec3(0.15f, 0.15f, 0.35f);

    rotateMatrix = glm::mat4(1);
    rotateMatrix *= transform3D::RotateOY(M_PI / 1.5f);
    camera->RotateThirdPerson_OY(M_PI / 1.5f);
    camera->MoveForward(5);

    Mesh* plane = new Mesh("plane");
    plane->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "plane50.obj");
    AddMeshToList(plane);

    Mesh* racetrack = objects::CreateRaceTrack("racetrack");
    AddMeshToList(racetrack);

    Mesh* car = objects::CreateCube("car");
    AddMeshToList(car);

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

    glm::ivec2 resolution = window->GetResolution();
    miniViewportArea = ViewportArea(50, 50, resolution.x / 5.f, resolution.y / 5.f);

    camera->RotateThirdPerson_OX(-M_PI / 6);

    glViewport(0, 0, resolution.x, resolution.y);
    projectionMatrix = glm::perspective(RADIANS(60), window->props.aspectRatio, 0.01f, 200.0f);
    RenderScene();

    camera->RotateThirdPerson_OX(M_PI / 6);


    camera->RotateThirdPerson_OX(-M_PI / 2);

    glViewport(miniViewportArea.x, miniViewportArea.y, miniViewportArea.width, miniViewportArea.height);
    projectionMatrix = glm::ortho(-10.0f, 10.0f, -5.0f, 5.0f, 0.01f, 200.0f);
    RenderScene();

    camera->RotateThirdPerson_OX(M_PI / 2);
}


void Tema2::Update(float deltaTimeSeconds)
{
}


void Tema2::FrameEnd()
{
}


void Tema2::RenderScene()
{
    {
        modelMatrix = glm::mat4(1);
        modelMatrix *= transform3D::Scale(5, 5, 5);
        RenderMesh(meshes["plane"], shaders["LabShader"], modelMatrix, glm::vec3(0.1f, 0.45f, 0.1f));
    }

    {
        modelMatrix = glm::mat4(1);
        modelMatrix *= transform3D::Scale(5, 5, 5);
        RenderMesh(meshes["racetrack"], shaders["LabShader"], modelMatrix, glm::vec3(0.15f, 0.15f, 0.15f));
    }

    {
        float x = camera->GetTargetPosition().x;
        float y = (carLength * carScale.y) / 2;
        float z = camera->GetTargetPosition().z;

        modelMatrix = glm::mat4(1);
        modelMatrix *= transform3D::Translate(x, y, z);
        modelMatrix *= rotateMatrix;
        modelMatrix *= transform3D::Scale(carScale.x, carScale.y, carScale.z);
        RenderMesh(meshes["car"], shaders["LabShader"], modelMatrix, glm::vec3(0.3f, 0.1f, 0.7f));

        carMatrix = modelMatrix;
    }

    //DrawCoordinateSystem(camera->GetViewMatrix(), projectionMatrix);
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
    float cameraSpeed = 2.5f;

    if (window->KeyHold(GLFW_KEY_W)) {
        // Miscare in fata a camerei
        camera->MoveForward(cameraSpeed * deltaTime);
    }

    if (window->KeyHold(GLFW_KEY_S)) {
        // Miscare in spate a camerei
        camera->MoveForward(-cameraSpeed * deltaTime);
    }

    if (window->KeyHold(GLFW_KEY_A)) {
        // Rotatie spre stanga a camerei
        camera->RotateThirdPerson_OY(cameraSpeed * deltaTime * 0.5f);
        // Rotatie spre stanga a masinii
        rotateMatrix *= transform3D::RotateOY(cameraSpeed * deltaTime * 0.5f);
    }

    if (window->KeyHold(GLFW_KEY_D)) {
        // Rotatie spre dreapta a camerei
        camera->RotateThirdPerson_OY(-cameraSpeed * deltaTime * 0.5f);
        // Rotatie spre dreapta a camerei
        rotateMatrix *= transform3D::RotateOY(-cameraSpeed * deltaTime * 0.5f);
    }
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
