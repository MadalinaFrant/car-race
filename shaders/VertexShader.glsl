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

    // Compute gl_Position
    gl_Position = Projection * View * Model * vec4(v_position, 1.0);

    float scaleFactor = 0.005f;
    float modif = pow(length(carPosition - gl_Position), 2) * scaleFactor;
    
    gl_Position.y = gl_Position.y - modif;
}
