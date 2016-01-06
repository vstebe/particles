#version 330

layout (location=0) in vec3   vtx_position;
layout (location=1) in vec3   vtx_normal;
layout (location=2) in vec2   vtx_texCoord;

uniform mat4 u_mtxView, u_mtxProjection;

out vec3 vs_out_position;
out vec3 vs_out_normal;
out vec2 vs_out_texCoord;

void main(void) {
    vec4 pos = u_mtxProjection * u_mtxView * vec4(vtx_position, 1);
    vs_out_position = vtx_position;
    vs_out_normal = vtx_normal;
    vs_out_texCoord = vtx_texCoord;
    gl_Position = pos;
}

// TODO !
