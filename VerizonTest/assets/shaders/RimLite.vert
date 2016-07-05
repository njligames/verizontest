#version 100

attribute vec4 inPosition;
attribute vec4 inNormal;
attribute vec4 inColor;
attribute mat4 inTransform;
attribute float inOpacity;
attribute float inHidden;
attribute mat4 inNormalMatrix;

//varying vec4 destinationColor;
varying vec3 destinationEyePosition;
varying float destinationOpacity;

varying vec3 world_pos;
varying vec3 world_normal;
//varying vec2 texcoord;

uniform mat4 modelView;
uniform mat4 projection;

void main ()
{
    vec4 position = inPosition;
    vec3 normal = inNormal.xyz;
    vec4 color = inColor;
    mat4 transform = inTransform;
    mat3 normalMatrix = mat3(inNormalMatrix);
    destinationOpacity = inOpacity;
    
    
    
    //convert in world coords
    world_pos = mat3(transform) * position.xyz;//careful here
    world_normal = normalize(mat3(transform) * normal);
//    texcoord = in_texcoord;
    
    destinationEyePosition = vec3(modelView[3][0], modelView[3][1], modelView[3][2]);
    
    if(inHidden == 1.0)
        position = vec4(destinationEyePosition, 1.0);
    
    gl_Position = (((modelView * projection) * transform) * position);
}
