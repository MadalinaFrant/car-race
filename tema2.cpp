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

    float x = camera->GetTargetPosition().x;
    float z = camera->GetTargetPosition().z;
    carPosition = glm::vec3(x, 0.15f, z);

    rotateMatrix = glm::mat4(1);


    fov = RADIANS(60);
    left = -7.5f;
    right = 7.5f;
    bottom = -2.0f;
    top = 2.0f;


    Mesh* racetrack = objects::CreateRaceTrack("racetrack", glm::vec3(1, 0, 0));
    AddMeshToList(racetrack);

    Mesh* car = objects::CreateCube("car", glm::vec3(0.3f, 0.1f, 0.7f));
    AddMeshToList(car);
}


void Tema2::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0.4f, 0.7, 1, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    miniViewportArea = ViewportArea(50, 50, resolution.x / 5.f, resolution.y / 5.f);

    camera->RotateThirdPerson_OX(-M_PI / 6);

    glViewport(0, 0, resolution.x, resolution.y);
    projectionMatrix = glm::perspective(fov, window->props.aspectRatio, 0.01f, 200.0f);
    RenderScene();

    camera->RotateThirdPerson_OX(M_PI / 6);


    camera->RotateThirdPerson_OX(-M_PI / 2);

    glViewport(miniViewportArea.x, miniViewportArea.y, miniViewportArea.width, miniViewportArea.height);
    projectionMatrix = glm::ortho(left, right, bottom, top, 0.01f, 200.0f);
    RenderScene();

    camera->RotateThirdPerson_OX(M_PI / 2);
}


void Tema2::RenderScene()
{
    {
        RenderMesh(meshes["racetrack"], shaders["VertexColor"], glm::mat4(1));
    }

    {
        modelMatrix = glm::mat4(1);
        modelMatrix *= transform3D::Translate(carPosition.x, carPosition.y, carPosition.z);
        modelMatrix *= rotateMatrix;
        modelMatrix *= transform3D::Scale(0.15f, 0.15f, 0.35f);
        RenderMesh(meshes["car"], shaders["VertexColor"], modelMatrix);
    }

    DrawCoordinateSystem(camera->GetViewMatrix(), projectionMatrix);
}


void Tema2::Update(float deltaTimeSeconds)
{
}


void Tema2::FrameEnd()
{
}


void Tema2::RenderMesh(Mesh *mesh, Shader *shader, const glm::mat4 &modelMatrix)
{
    if (!mesh || !shader || !shader->program)
        return;

    // Render an object using the specified shader and the specified position
    shader->Use();
    glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(camera->GetViewMatrix()));
    glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
    glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    mesh->Render();
}


void Tema2::OnInputUpdate(float deltaTime, int mods)
{
    float cameraSpeed = 1.25f;

    if (window->KeyHold(GLFW_KEY_W)) {
        // Translate the camera forward
        camera->MoveForward(cameraSpeed * deltaTime);
        carPosition.x = camera->GetTargetPosition().x;
        carPosition.z = camera->GetTargetPosition().z;
    }

    if (window->KeyHold(GLFW_KEY_A)) {
        // Translate the camera to the left
        camera->RotateThirdPerson_OY(cameraSpeed * deltaTime);
        carPosition.x = camera->GetTargetPosition().x;
        carPosition.z = camera->GetTargetPosition().z;
        rotateMatrix *= transform3D::RotateOY(cameraSpeed * deltaTime);
    }

    if (window->KeyHold(GLFW_KEY_S)) {
        // Translate the camera backward
        camera->MoveForward(-cameraSpeed * deltaTime);
        carPosition.x = camera->GetTargetPosition().x;
        carPosition.z = camera->GetTargetPosition().z;
    }

    if (window->KeyHold(GLFW_KEY_D)) {
        // Translate the camera to the right
        camera->RotateThirdPerson_OY(-cameraSpeed * deltaTime);
        carPosition.x = camera->GetTargetPosition().x;
        carPosition.z = camera->GetTargetPosition().z;
        rotateMatrix *= transform3D::RotateOY(-cameraSpeed * deltaTime);
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
