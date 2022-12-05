#version 330

layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec2 v_texture;
layout(location = 3) in vec2 v_color;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform mat4 CarModel;
uniform vec3 Color;

out vec3 color; // output catre fragment shader

void main()
{
    // Trimite output-ul catre fragment shader
    color = Color;

    // Calculeaza pozitia masinii in spatiul lume
    vec4 carPosition = Projection * View * CarModel * vec4(1.0);

    // Calculeaza gl_Position (pozitia in spatiul lume a vertexului)
    gl_Position = Projection * View * Model * vec4(v_position, 1.0);

    float scaleFactor = 0.005f;
    float modif = pow(length(carPosition - gl_Position), 2) * scaleFactor;
    
    // Se modifica componenta y conform formulei pentru a realiza curbura 
    gl_Position.y = gl_Position.y - modif;
}
