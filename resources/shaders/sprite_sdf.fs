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

float threshold = 0.4;

float edge = 0.42;
float innerline = 0.1;

vec4 colora = vec4(0.2, 0.01, 0.01, 0.4);
vec4 colorb = vec4(0.1, 0.1, 0.01, 0.4);

vec4 colorc = vec4(0.1, 0.1, 0.1, 0.9);

vec4 sdf0() {
    vec4 texel = texture2D(texture0, fragTexCoord); 
    float r = texel.r;
    
    float alpha = smoothstep(threshold, threshold + edge, r);
    
    vec4 color = colora;
    color = mix(color, colorb, alpha);
    color.a = min(alpha, color.a);
    
    //float rr = r + time;
    //float t = sin(rr * 0.1);
    
    //float outline = alpha - smoothstep(threshold - innerline + edge, threshold + innerline + edge, r);
    //color.rgb = mix(color.rgb, colorc.rgb, rr);
    
    return color;
}

void main()
{
    gl_FragColor = sdf0();
    //gl_FragColor = texture2D(texture1_gradient, fragTexCoord);
}