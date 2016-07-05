#version 100

#ifdef GL_ES
precision mediump float;
#endif

varying vec4 destinationColor;
varying vec3 destinationEyePosition;

void main(void)
{
    vec4 color = destinationColor;
    vec3 eyePosition = destinationEyePosition;
    gl_FragColor = color;
}
