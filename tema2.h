#pragma once

#include "components/simple_scene.h"
#include "lab_m1/tema2/lab_camera.h"
#include "lab_m1/tema2/transform3D.h"
#include "lab_m1/tema2/objects.h"

#include <vector>
#include <string>
#include <iostream>


namespace m1
{
    class Tema2 : public gfxc::SimpleScene
    {
        public:

            struct ViewportArea
            {
                ViewportArea() : x(0), y(0), width(1), height(1) {}
                ViewportArea(int x, int y, int width, int height)
                    : x(x), y(y), width(width), height(height) {}
                int x;
                int y;
                int width;
                int height;
            };

            Tema2();
            ~Tema2();

            void Init() override;


        private:

            void FrameStart() override;
            void Update(float deltaTimeSeconds) override;
            void FrameEnd() override;

            void RenderMesh(Mesh *mesh, Shader *shader, const glm::mat4 &modelMatrix, const glm::vec3 &color);

            void RenderScene();
            void RenderPlane();
            void RenderRacetrack();
            void RenderCar();
            void RenderTree(glm::vec3 spawnPoint);
            void RenderTrees();
            void RenderBollards(vector<glm::vec3> points);
            void RenderOtherCars();
            bool IsInsideRacetrack(glm::vec3 p);
            bool IsInsideTriangle(glm::vec3 p, glm::vec3 a, glm::vec3 b, glm::vec3 c);
            bool SameSide(glm::vec3 p1, glm::vec3 p2, glm::vec3 a, glm::vec3 b);
            bool CheckCollision();

            void OnInputUpdate(float deltaTime, int mods) override;
            void OnKeyPress(int key, int mods) override;
            void OnKeyRelease(int key, int mods) override;
            void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
            void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
            void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
            void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
            void OnWindowResize(int width, int height) override;

        protected:

            implemented::LabCamera *camera;
            ViewportArea miniViewportArea;

            glm::mat4 projectionMatrix, modelMatrix, rotateMatrix, carMatrix;

            float carLength, trunkLength, crownLength;
            glm::vec3 carScale, trunkScale, crownScale, trackScale;

            vector<VertexFormat> vertices, vExt, vInt;
            vector<unsigned int> indices, iExt, iInt;

            vector<glm::vec3> points, extPoints, intPoints, extPointsAux, intPointsAux;

            int nrOtherCars;
            vector<vector<glm::vec3>> carPoints;
            vector<int> nextPoints;
            vector<int> speeds;
            vector<glm::vec3> distances, colors;
    };
}
