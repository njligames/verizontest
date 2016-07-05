#version 100

attribute vec4 inPosition;
attribute vec4 inNormal;
attribute vec4 inColor;
attribute mat4 inTransform;
attribute float inOpacity;
attribute float inHidden;
attribute mat4 inNormalMatrix;

varying vec4 destinationColor;
varying vec3 destinationEyePosition;

uniform mat4 modelView;
uniform mat4 projection;

void main ()
{
    vec4 position = inPosition;
    vec3 normal = inNormal.xyz;
    vec4 color = inColor;
    mat4 transform = inTransform;
    mat3 normalMatrix = mat3(inNormalMatrix);
    
    vec3 lightPosition = vec3(0.0, 0.0, -1.0);
    
    vec3 eyeNormal = normalize(normalMatrix * normal);
    float nDotVP = max(0.0, dot(eyeNormal, normalize(lightPosition)));
    
    destinationColor = (color * nDotVP);
    destinationEyePosition = vec3(modelView[3][0], modelView[3][1], modelView[3][2]);
    
    if(inHidden == 1.0)
        position = vec4(destinationEyePosition, 1.0);
    destinationColor.a = inOpacity;
    gl_Position = (((modelView * projection) * transform) * position);
}
