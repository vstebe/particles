#version 330
uniform mat4 u_mtxWorld, u_mtxView, u_mtxProjection;


layout(points) in;
layout(triangle_strip, max_vertices=4) out;

in vec4[] vs_out_billColor;

in float[] vs_out_rotation;

out vec4 billColor;
out vec2 texCoord;

mat4 rotationMatrix(vec3 axis, float angle)
{
    axis = normalize(axis);
    float s = sin(angle);
    float c = cos(angle);
    float oc = 1.0 - c;

    return mat4(oc * axis.x * axis.x + c,           oc * axis.x * axis.y - axis.z * s,  oc * axis.z * axis.x + axis.y * s,  0.0,
                oc * axis.x * axis.y + axis.z * s,  oc * axis.y * axis.y + c,           oc * axis.y * axis.z - axis.x * s,  0.0,
                oc * axis.z * axis.x - axis.y * s,  oc * axis.y * axis.z + axis.x * s,  oc * axis.z * axis.z + c,           0.0,
                0.0,                                0.0,                                0.0,                                1.0);
}

void main()
{
    vec4 pos = gl_in[0].gl_Position;

    mat4 rotMat = rotationMatrix(vec3(0,0,1), vs_out_rotation[0]);

    float alpha = 0.1;
    billColor = vs_out_billColor[0];
    gl_Position = u_mtxProjection * (pos + rotMat * vec4(-alpha, -alpha, 0, 0));
    texCoord = vec2(1,1);
    EmitVertex();
    gl_Position = u_mtxProjection * (pos + rotMat * vec4(alpha, -alpha, 0, 0));
    texCoord = vec2(0,1);
    EmitVertex();
    gl_Position = u_mtxProjection * (pos + rotMat * vec4(-alpha, alpha, 0, 0));
    texCoord = vec2(1,0);
    EmitVertex();
    gl_Position = u_mtxProjection * (pos + rotMat * vec4(alpha, alpha, 0, 0));
    texCoord = vec2(0,0);
    EmitVertex();


    EndPrimitive();
}
