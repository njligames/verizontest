#version 100

#ifdef GL_ES
precision highp float;
#endif

attribute vec4 inPosition;
attribute vec2 inTexCoord;
attribute vec4 inNormal;
attribute mat4 inTransform;
attribute float inOpacity;
attribute float inHidden;
attribute mat4 inNormalMatrix;

varying vec2 UV;
varying vec3 Position_worldspace;
varying vec3 Normal_cameraspace;
varying vec3 EyeDirection_cameraspace;
varying vec3 LightDirection_cameraspace;
varying float Vertex_opacity;

uniform mat4 modelView;
uniform mat4 projection;
uniform vec3 LightPosition_worldspace;

void main ()
{
    mat3 normalMatrix = mat3(inNormalMatrix);
    float hidden = inHidden;
    
    Vertex_opacity = inOpacity;
    if(hidden == 1.0)
        Vertex_opacity = 0.0;
    
    
    mat4 M = inTransform;
    mat4 V = modelView;
    vec3 vertexPosition_modelspace = inPosition.xyz;
    vec3 vertexNormal_modelspace = inNormal.xyz;
    vec2 vertexUV = inTexCoord;
    
    
    // Output position of the vertex, in clip space : MVP * position
//    gl_Position =  MVP * vec4(vertexPosition_modelspace,1);
    gl_Position = (((modelView * projection) * inTransform) * inPosition);
    
    // Position of the vertex, in worldspace : M * position
    Position_worldspace = (M * vec4(vertexPosition_modelspace,1)).xyz;
    
    // Vector that goes from the vertex to the camera, in camera space.
    // In camera space, the camera is at the origin (0,0,0).
    vec3 vertexPosition_cameraspace = ( V * M * vec4(vertexPosition_modelspace,1)).xyz;
    EyeDirection_cameraspace = vec3(0,0,0) - vertexPosition_cameraspace;
    
    // Vector that goes from the vertex to the light, in camera space. M is ommited because it's identity.
    vec3 LightPosition_cameraspace = ( V * vec4(LightPosition_worldspace,1)).xyz;
    LightDirection_cameraspace = LightPosition_cameraspace + EyeDirection_cameraspace;
    
    // Normal of the the vertex, in camera space
    Normal_cameraspace = ( V * M * vec4(vertexNormal_modelspace,0)).xyz; // Only correct if ModelMatrix does not scale the model ! Use its inverse transpose if not.
    
    // UV of the vertex. No special space for this one.
    UV = vertexUV;
}
