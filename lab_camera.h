#pragma once

#include "utils/glm_utils.h"
#include "utils/math_utils.h"


namespace implemented
{
    class LabCamera
    {
        public:

            LabCamera()
            {
                position    = glm::vec3(0, 2, 5);
                forward     = glm::vec3(0, 0, -1);
                up          = glm::vec3(0, 1, 0);
                right       = glm::vec3(1, 0, 0);
                distanceToTarget = 2;
            }

            LabCamera(const glm::vec3 &position, const glm::vec3 &center, const glm::vec3 &up)
            {
                Set(position, center, up);
            }

            ~LabCamera()
            { 
            }

            void Set(const glm::vec3 &position, const glm::vec3 &center, const glm::vec3 &up)
            {
                this->position = position;
                forward        = glm::normalize(center - position);
                right          = glm::cross(forward, up);
                this->up       = glm::cross(right, forward);
            }

            void MoveForward(float distance)
            {
                glm::vec3 dir = glm::normalize(glm::vec3(forward.x, 0, forward.z));
                position += dir * distance;
            }

            void TranslateForward(float distance)
            {
                glm::vec3 dir = glm::normalize(forward);
                position += dir * distance;
            }

            void TranslateUpward(float distance)
            {
                glm::vec3 dir = glm::normalize(glm::vec3(0, 1, 0));
                position += dir * distance;
            }

            void TranslateRight(float distance)
            {
                glm::vec3 dir = glm::normalize(glm::vec3(right.x, 0, right.z));
                position += dir * distance;
            }

            void RotateFirstPerson_OX(float angle)
            {
                glm::vec4 newForward = glm::rotate(glm::mat4(1.0f), angle, right) * glm::vec4(forward, 1);
                forward = glm::normalize(glm::vec3(newForward));

                up = glm::normalize(glm::cross(right, forward));
            }

            void RotateFirstPerson_OY(float angle)
            {
                glm::vec4 newForward = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0, 1, 0)) * glm::vec4(forward, 1);
                forward = glm::normalize(glm::vec3(newForward));
                glm::vec4 newRight = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0, 1, 0)) * glm::vec4(right, 1);
                right = glm::normalize(glm::vec3(newRight));

                up = glm::normalize(glm::cross(right, forward));
            }

            void RotateFirstPerson_OZ(float angle)
            {
                glm::vec4 newRight = glm::rotate(glm::mat4(1.0f), angle, forward) * glm::vec4(right, 1);
                right = glm::normalize(glm::vec3(newRight));

                up = glm::normalize(glm::cross(right, forward));
            }

            void RotateThirdPerson_OX(float angle)
            {
                TranslateForward(distanceToTarget);
                RotateFirstPerson_OX(angle);
                TranslateForward(-distanceToTarget);
            }

            void RotateThirdPerson_OY(float angle)
            {
                TranslateForward(distanceToTarget);
                RotateFirstPerson_OY(angle);
                TranslateForward(-distanceToTarget);
            }

            void RotateThirdPerson_OZ(float angle)
            {
                TranslateForward(distanceToTarget);
                RotateFirstPerson_OZ(angle);
                TranslateForward(-distanceToTarget);
            }

            glm::mat4 GetViewMatrix()
            {
                return glm::lookAt(position, position + forward, up);
            }

            glm::vec3 GetTargetPosition()
            {
                return position + forward * distanceToTarget;
            }


        public:

            float distanceToTarget;
            glm::vec3 position;
            glm::vec3 forward;
            glm::vec3 up;
            glm::vec3 right;
    };
}
