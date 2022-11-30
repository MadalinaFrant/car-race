#version 330

// Input
// Get vertex attributes from each location
layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec2 v_texture;
layout(location = 3) in vec2 v_color;

// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform mat4 CarModel;
uniform vec3 Color;

// Output
// Output values to fragment shader
out vec3 color;

void main()
{
    // Send output to fragment shader
    color = Color;

    vec4 carPosition = Projection * View * CarModel * vec4(1.0);

    float scaleFactor = 0.02f;
    float modif = pow(length(vec3(carPosition.x, carPosition.y, carPosition.z) - v_position), 2) 
                * scaleFactor;

    float posX = v_position.x;
    float posY = v_position.y - modif;
    float posZ = v_position.z;

    // Compute gl_Position
    gl_Position = Projection * View * Model * vec4(posX, posY, posZ, 1.0);
}
