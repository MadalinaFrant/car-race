#version 330

in vec3 color; // primeste culoarea de la vertex shader

layout(location = 0) out vec4 out_color; // output

void main()
{
    out_color = vec4(color, 1); // scrie culoarea la output
}
