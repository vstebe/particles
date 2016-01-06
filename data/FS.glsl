#version 130


// sampler for the RGBA texture used by the billboard
uniform sampler2D u_tex;

// we should use this, instead of gl_FragColor which is deprecated since GL version 3.0
out vec4 out_fragColor;

in vec2 texCoord;

in vec4 billColor;

void main (void)
{
    vec4 color = texture(u_tex, texCoord);
    out_fragColor = color * billColor;
}
