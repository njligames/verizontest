#version 100

#ifdef GL_ES
precision mediump float;
#endif

varying vec4 destinationColor;
varying vec3 eyespaceNormal;
varying vec4 eyespacePosition;

void main(void)
{
    highp vec4 lightPosition = vec4(0.0, 10.0, 0.0, 1.0);
    
    
    highp vec3 materialAmbientColor = vec3(1.0, 1.0, 1.0);
    highp vec3 materialDiffuseColor = vec3(1.0, 1.0, 1.0);
    highp vec3 materialSpecularColor = vec3(1.0, 1.0, 1.0);
    highp float materialSpecularExponent = 1.0;
    
    highp vec3 lightDirection;

    if(lightPosition.w == 0.0)
        lightDirection = normalize(vec3(lightPosition));
    else
        lightDirection = normalize(vec3(lightPosition - eyespacePosition));
    
    highp vec3 normal = normalize(eyespaceNormal);
    highp vec3 viewDirection = vec3(0.0, 0.0, -1.0);
    highp vec3 halfPlane = normalize(lightDirection + viewDirection);
//
    highp float diffuseFactor = max(0.0, dot(normal, lightDirection));
    highp float specularFactor = max(0.0, dot(normal, halfPlane));
//
//    
    
//
    specularFactor = pow(specularFactor, materialSpecularExponent);
    
    highp vec3 color = materialAmbientColor + diffuseFactor *
                        materialDiffuseColor + specularFactor *
                        materialSpecularColor;
    
    gl_FragColor = vec4(color, 1) * destinationColor;
//
//    if(diffuseFactor < 0.1)
//        diffuseFactor = 0.0;
//    else if (diffuseFactor < 0.3)
//        diffuseFactor = 0.3;
//    else if (diffuseFactor < 0.6)
//        diffuseFactor = 0.6;
//    else
//        diffuseFactor = 1.0;
//    
//    specularFactor = step(0.5, specularFactor);
//    
//    highp vec3 finalColor = color.rgb * diffuseFactor * specularFactor;
//    gl_FragColor = vec4(finalColor, 1);
}
