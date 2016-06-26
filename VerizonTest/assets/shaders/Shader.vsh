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
varying float destinationOpacity;
varying float destinationHidden;
//varying mat4 destinationColorTransform;
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
    mat3 normal = mat3_emu(inNormalMatrix);
    
    vec3 eyeNormal = normalize(normal * inNormal.xyz);
    vec3 lightPosition = vec3(0.0, 0.0, 1.0);
    float nDotVP = max(0.0, dot(eyeNormal, normalize(lightPosition)));
    
    destinationColor = inColor * nDotVP;
    destinationTexCoord2D = inTexCoord;
    destinationOpacity = inOpacity;
    destinationHidden = inHidden;
//    destinationColorTransform = inColorTransform;
    gl_Position = (((projection * modelView) * inTransform) * inPosition);
}

