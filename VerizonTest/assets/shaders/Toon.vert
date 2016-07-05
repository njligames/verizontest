#version 100
attribute vec4 inPosition;
//attribute vec2 inTexCoord;
attribute vec4 inNormal;
attribute vec4 inColor;
attribute mat4 inTransform;
attribute float inOpacity;
attribute float inHidden;
//attribute mat4 inColorTransform;
attribute mat4 inNormalMatrix;

varying vec4 destinationColor;
//varying vec2 destinationTexCoord2D;
//varying mat4 destinationColorTransform;
varying vec3 eyespaceNormal;
varying vec4 eyespacePosition;

uniform mat4 modelView;
uniform mat4 projection;

mat3 mat3_emu(mat4 m4) {
    return mat3(
                m4[0][0], m4[0][1], m4[0][2],
                m4[1][0], m4[1][1], m4[1][2],
                m4[2][0], m4[2][1], m4[2][2]);
}

void main ()
{
    mat3 normalMatrix = mat3_emu(inNormalMatrix);
    vec3 normal = inNormal.xyz;
    vec4 position = inPosition;
    
    if(inHidden == 1.0)
    {
        position = vec4(modelView[3][0], modelView[3][1], modelView[3][2], modelView[3][3]);
    }
    
    eyespaceNormal = normalMatrix * normal;
    eyespacePosition = modelView * position;
    
    vec4 color = inColor;
    color.a = inOpacity;
    
    destinationColor = color;
    gl_Position = (((projection * modelView) * inTransform) * position);
}

