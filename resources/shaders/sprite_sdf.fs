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

float threshold = 0.00000001;

float edge = 0.021;
float innerline = 0.09;
vec4 innerline_color = vec4(0.9, 1.0, 0.6, 0.0);

vec4 colora = vec4(0.0, 0.0, 0.0, 0.8);
vec4 colorb = vec4(0.8, 0.0, 0.9, 0.4);

vec4 sdf0() {
    vec4 texel = texture2D(texture0, fragTexCoord); 
    float r = texel.r;
    float alpha = smoothstep(threshold - edge, threshold + edge, texel.r);
    float outline = smoothstep(threshold + innerline + edge, threshold + innerline - edge, r);
    vec4 color = vec4(1.0, 1.0, 1.0, alpha);
    float t = (sin(time * 10.0 + r * 10.0) * 0.5) + 0.5;
    t *= 0.5;
    //color.rgb = texture2D(texture1_gradient, vec2(t, 0)).rgb;
    color.rgba = mix(colora, colorb, t);
    color = mix(color, innerline_color, outline);
    color.a = alpha * color.a;
    return color;
}

void main()
{
    gl_FragColor = sdf0();
    //gl_FragColor = texture2D(texture1_gradient, fragTexCoord);
}