#version 330

in vec3 vtx_position;
in vec4 vtx_color;

uniform mat4 u_mtxWorld, u_mtxView, u_mtxProjection;

out vec4 vs_out_billColor;

void main(void) {

    vs_out_billColor = vtx_color;
    //vec4 pos = u_mtxView * u_mtxWorld * vec4(1,0,1, 1);
    vec4 pos = u_mtxView * u_mtxWorld * vec4(vtx_position, 1);
    gl_Position = pos;
}

// TODO !
