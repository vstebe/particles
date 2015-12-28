#version 330


layout(triangle_strip) in;
layout(triangle_strip, max_vertices=1) out;



out vec4 billColor;

void main()
{
    vec4 pos = gl_in[0].gl_Position;
    EmitVertex();
    EndPrimitive();
}
