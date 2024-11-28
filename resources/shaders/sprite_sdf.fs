#version 100

precision mediump float;

// Input vertex attributes (from vertex shader)
varying vec2 fragTexCoord;
varying vec4 fragColor;

// Input uniform values
uniform sampler2D texture0;
uniform vec4 colDiffuse;
uniform sampler2D texture1_gradient;

uniform float time;

float threshold = 0.49;

float edge = 0.01;
float innerline = 0.04;

vec4 colora = vec4(1.0, 1.0, 1.0, 1.0);
vec4 colorb = vec4(1.0, 0.0, 1.0, 1.0);
vec4 colorc = vec4(0.7, 0.7, 1.0, 1.0);

vec4 sdf0() {
    vec4 texel = texture2D(texture0, fragTexCoord); 
    float r = texel.r;
    
    float alpha = smoothstep(threshold + edge, threshold + edge, r);
    float outline = alpha - smoothstep(threshold + innerline + edge, threshold + innerline + edge, r);
    
    vec4 color = colora;
    color.a = alpha;
    color.rgb = mix(color.rgb, colorb.rgb, outline);
    float rr = r * 8.0 + time;
    color.rgb = mix(color.rgb, colorc.rgb, (alpha - outline) * sin(rr));
    
    return color;
}

void main()
{
    gl_FragColor = sdf0();
    //gl_FragColor = texture2D(texture1_gradient, fragTexCoord);
}