#version 130


// sampler for the RGBA texture used by the billboard
uniform sampler2D u_tex;

// we should use this, instead of gl_FragColor which is deprecated since GL version 3.0
out vec4 out_fragColor;

in vec3 vs_out_position;
in vec3 vs_out_normal;
in vec2 vs_out_texCoord;

void main (void)
{
    // TODO !
    vec4 color = texture(u_tex, vs_out_texCoord);
    vec3 L = -normalize(vs_out_position);
    vec3 N = vs_out_normal;



    float NL = dot(N, L);

    float diffuse = clamp(NL + 0.2, 0, 1);

    // TODO !
    out_fragColor = color;
    //out_fragColor = vec4(texCoord.x, texCoord.y, texCoord.x, texCoord.y);
    //out_fragColor = vec4(vs_out_texCoord, 1, 1);
}
