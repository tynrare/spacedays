#version 100
#extension GL_EXT_gpu_shader4 : enable

precision mediump float;

// Input vertex attributes (from vertex shader)
varying vec2 fragTexCoord;
varying vec4 fragColor;

// Input uniform values
uniform sampler2D texture0;
uniform vec4 colDiffuse;

uniform int parts;

void main()
{
    vec2 uv = fragTexCoord;
    uv.x *= 0.25;
    vec4 color = vec4(0.0, 0.0, 0.0, 0.0);
    for (int i = 0; i < 4; i++) {
        if ((parts & (1 << i)) > 0) {
            color += texture2D(texture0, uv + vec2(float(i) * 0.25, 0.0)) ;
        }
    }
  
    gl_FragColor = color;
}