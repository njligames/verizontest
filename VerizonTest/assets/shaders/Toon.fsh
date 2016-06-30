#version 100

#ifdef GL_ES
precision mediump float;
#endif

varying vec4 destinationColor;
//varying vec2 destinationTexCoord2D;
//varying mat4 destinationColorTransform;
varying vec3 eyespaceNormal;
varying vec4 eyespacePosition;

//uniform sampler2D diffuseTexture2D;

uniform int opacityModifyRGB;

vec4 transformRGB(vec4 color, mat4 matrix)
{
    float tr = color.r*matrix[0][0] + color.g*matrix[1][0] + color.b*matrix[2][0] + matrix[3][0];
    float tg = color.r*matrix[0][1] + color.g*matrix[1][1] + color.b*matrix[2][1] + matrix[3][1];
    float tb = color.r*matrix[0][2] + color.g*matrix[1][2] + color.b*matrix[2][2] + matrix[3][2];
    
    return clamp(vec4(tr, tg, tb, color.a), 0.0, 1.0);
}

void main(void)
{
    vec4 color = destinationColor;
    int modifyRGB = opacityModifyRGB;
    
    highp vec4 lightPosition = vec4(0.5, 0.5, -1.0, 0.0);
    
    highp vec3 lightDirection;
    
    if(lightPosition.w == 0.0)
        lightDirection = normalize(vec3(lightPosition));
    else
        lightDirection = normalize(vec3(lightPosition - eyespacePosition));
    highp vec3 normal = normalize(eyespaceNormal);
    highp vec3 viewDirection = vec3(0.0, 0.0, 1.0);
    highp vec3 halfPlane = normalize(lightDirection + viewDirection);
    
    highp float diffuseFactor = max(0.0, dot(normal, lightDirection));
    highp float specularFactor = max(0.0, dot(normal, halfPlane));
    
    //TODO
    highp float materialSpecularExponent = 1.0;
    
    specularFactor = pow(specularFactor, materialSpecularExponent);
    
    if(diffuseFactor < 0.1)
        diffuseFactor = 0.0;
    else if (diffuseFactor < 0.3)
        diffuseFactor = 0.3;
    else if (diffuseFactor < 0.6)
        diffuseFactor = 0.6;
    else
        diffuseFactor = 1.0;
    
    specularFactor = step(0.5, specularFactor);
    
    
    
    //    vec4 diffuseColor = texture2D(diffuseTexture2D, destinationTexCoord2D);
    
    if (modifyRGB == 1)
    {
        color = vec4(destinationColor.r * destinationColor.a,
                     destinationColor.g * destinationColor.a,
                     destinationColor.b * destinationColor.a,
                     destinationColor.a);
    }
    
    //    color = (color * diffuseColor);
    
    
    //    color = transformRGB(color, destinationColorTransform);
    
    highp vec3 finalColor = color.rgb * diffuseFactor * specularFactor;
    gl_FragColor = vec4(finalColor, 1);
}
