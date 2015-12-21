#version 330
uniform mat4 u_mtxWorld, u_mtxView, u_mtxProjection;


layout(points) in;
layout(triangle_strip, max_vertices=4) out;

in vec3[] vs_out_billColor;
out vec3 billColor;
out vec2 texCoord;

void main()
{
    vec4 pos = gl_in[0].gl_Position;

    float alpha = 0.1;
    billColor = vs_out_billColor[0];
    gl_Position = u_mtxProjection * (pos + vec4(0, 0, 0, 0));
    texCoord = vec2(1,1);
    EmitVertex();
    gl_Position = u_mtxProjection * (pos + vec4(alpha, 0, 0, 0));
    texCoord = vec2(0,1);
    EmitVertex();
    gl_Position = u_mtxProjection * (pos + vec4(0, alpha, 0, 0));
    texCoord = vec2(1,0);
    EmitVertex();
    gl_Position = u_mtxProjection * (pos + vec4(alpha, alpha, 0, 0));
    texCoord = vec2(0,0);
    EmitVertex();


    EndPrimitive();
    billColor = pos.rgb;
}
