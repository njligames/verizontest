#version 100

attribute vec4 inPosition;
attribute vec4 inNormal;
attribute vec4 inColor;
attribute mat4 inTransform;
attribute float inOpacity;
attribute float inHidden;
attribute mat4 inNormalMatrix;

uniform mat4 modelView;
uniform mat4 projection;

varying vec3 world_pos;
varying vec3 world_normal;
//varying vec2 texcoord;
varying vec3 eye_position;

void main ()
{
//    mat4 normalMatrix = inNormalMatrix;
//    vec4 color = inColor;
//    float opacity = inOpacity;
    eye_position = vec3(modelView[3][0], modelView[3][1], modelView[3][2]);
    vec4 position = inPosition;
    
    world_pos = mat3(inTransform) * inPosition.xyz;
    world_normal = normalize(mat3(inTransform) * inNormal.xyz);
//    texcoord = inTexCoord;
    
    if(inHidden == 1.0)
    {
        position = vec4(modelView[3][0], modelView[3][1], modelView[3][2], modelView[3][3]);
    }
    
    gl_Position = (((modelView * projection) * inTransform) * position);
}

