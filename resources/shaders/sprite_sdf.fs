#version 100

precision mediump float;

// Input vertex attributes (from vertex shader)
varying vec2 fragTexCoord;
varying vec4 fragColor;

// Input uniform values
uniform sampler2D texture0;
uniform vec4 colDiffuse;

uniform float time;

float threshold = 0.5;
float edge = 0.03;
float innerline = 0.1;

vec4 innerline_color = vec4(1.0, 1.0, 1.0, 1.0);

void main()
{
    vec4 texel = texture2D(texture0, fragTexCoord); 
    float r = texel.r;
    float alpha = smoothstep(threshold - edge, threshold + edge, texel.r);
    float outline = smoothstep(threshold + innerline + edge, threshold + innerline - edge, r);
    vec4 color = vec4(1.0, 1.0, 1.0, alpha);
    float t = (sin(time * 1.0 + r * 7.0) + 1.0) * 0.5;
    color.rgb = mix(vec3(1.0, 1.0, 1.0), vec3(1.0, 0.0, 1.0), t);
    color = mix(color, innerline_color, outline);
    color.a = alpha;
    gl_FragColor = color;
}