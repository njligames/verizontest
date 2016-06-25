#version 100
attribute mat4 inTransform;
attribute mat4 inColorTransform;

attribute vec4 inPosition;
attribute vec4 inColor;
//attribute vec4 inNormal;
//attribute vec2 inTexCoord;
attribute float inOpacity;
attribute float inHidden;

varying vec4 destinationColor;
//varying vec4 destinationNormal;
//varying vec2 destinationTexCoord2D;
varying float destinationOpacity;
varying float destinationHidden;
varying mat4 destinationColorTransform;
uniform mat4 modelView;
uniform mat4 projection;
void main ()
{
    destinationColor = inColor;
//    destinationNormal = inNormal;
//    destinationTexCoord2D = inTexCoord;
    destinationOpacity = inOpacity;
    destinationHidden = inHidden;
    destinationColorTransform = inColorTransform;
    
    /*mat4 t = inTransform;
    t = mat4(1.0, 0.0, 0.0, 0.0,
             0.0, 1.0, 0.0, 0.0,
             0.0, 0.0, 1.0, 0.0,
             0.0, 0.0, 30.0, 1.0);*/
    
//    gl_Position = (((projection * modelView) * t) * inPosition);
    gl_Position = (((projection * modelView) * inTransform) * inPosition);
}

