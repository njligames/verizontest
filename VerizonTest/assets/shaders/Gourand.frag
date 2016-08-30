#version 100

#ifdef GL_ES
precision mediump float;
#endif

varying vec4 destinationColor;
varying vec3 eyespaceNormal;
varying vec4 eyespacePosition;

void main(void)
{
    gl_FragColor = destinationColor;
}
