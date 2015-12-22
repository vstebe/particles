#version 130


// sampler for the RGBA texture used by the billboard
uniform sampler2D u_tex;

// we should use this, instead of gl_FragColor which is deprecated since GL version 3.0
out vec4 out_fragColor;

in vec2 texCoord;

in vec4 billColor;

void main (void)
{
    // TODO !
    vec4 color = texture2D(u_tex, texCoord);

    // This allows us to do some Alpha Testing : any fragment with an alpha lower than 0.5 won't be rendered !
    //if( color.r < 0.5 )
     //   discard;


   // if(billColor == vec3(0,0,0))
      //  discard;

    // TODO !
    out_fragColor = color * billColor;
    //out_fragColor = vec4(texCoord.x, texCoord.y, texCoord.x, texCoord.y);
}
