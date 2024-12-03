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

float threshold = 0.94;

float edge = 0.07;
float innerline = 0.1;

vec4 colora = vec4(0.99, 0.909, 0.69, 0.99);
vec4 colorb = vec4(0.91, 0.66, 0.16, 0.99);

vec4 colorc = vec4(0.1, 0.1, 0.1, 0.9);

vec4 sdf0() {
    vec4 texel = texture2D(texture0, fragTexCoord); 
    float r = texel.r * fragColor.r;
    
    vec4 texel1 = texture2D(texture0, 
        vec2(fragTexCoord.x + texel.g / 1024.0, fragTexCoord.y));
    
    r = r * texel1.r;
    
    float alpha = smoothstep(threshold, threshold + edge, r);
    
    vec4 color = colora;
    color = mix(color, colorb, alpha);
    color.a = min(alpha, color.a);
    
    return color;
}

void main()
{
    gl_FragColor = sdf0();
    //gl_FragColor = texture2D(texture1_gradient, fragTexCoord);
}