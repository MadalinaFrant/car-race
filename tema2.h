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

            void OnInputUpdate(float deltaTime, int mods) override;
            void OnKeyPress(int key, int mods) override;
            void OnKeyRelease(int key, int mods) override;
            void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
            void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
            void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
            void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
            void OnWindowResize(int width, int height) override;

            void RenderScene();

        protected:
        
            implemented::LabCamera *camera;

            float carLength;
            glm::vec3 carScale;

            glm::mat4 projectionMatrix, rotateMatrix, modelMatrix, carMatrix;

            ViewportArea miniViewportArea;
    };
}
