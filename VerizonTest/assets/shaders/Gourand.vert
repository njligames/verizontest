#version 100

attribute vec4 inPosition;
attribute vec2 inTexCoord;
attribute vec4 inNormal;
attribute vec4 inColor;
attribute mat4 inTransform;
attribute float inOpacity;
attribute float inHidden;
//attribute mat4 inColorTransform;
attribute mat4 inNormalMatrix;

varying vec4 destinationColor;
varying vec2 destinationTexCoord2D;
//varying mat4 destinationColorTransform;
varying vec3 eyespaceNormal;
varying vec4 eyespacePosition;

uniform mat4 modelView;
uniform mat4 projection;

void main ()
{
    mat3 normalMatrix = mat3(inNormalMatrix);
    vec3 normal = inNormal.xyz;
    vec4 position = inPosition;
    vec4 cameraPosition = vec4(modelView[3][0], modelView[3][1], modelView[3][2], modelView[3][3]);
    
    if(inHidden == 1.0)
        position = cameraPosition;
    
    eyespaceNormal = normalize(normalMatrix * normal);
    eyespacePosition = modelView * position;
    
    vec3 lightDirection = vec3( 0.0, 0.0, -1.0 );
    
    // the light direction must be a normalized vector pointing towards the light, defined in view-space
    float dotValue = max(dot(normal, lightDirection), 0.0);
    
    // this color (a varying variable) is already interpolated when reaching the fragment shader and can be directly used
//    varColor = vec3(dotValue);
    
    vec4 color = inColor * dotValue;
    color.a = inOpacity;
    
    destinationColor = color;
    destinationTexCoord2D = inTexCoord;
    
    gl_Position = (((projection * modelView) * inTransform) * position);
}

