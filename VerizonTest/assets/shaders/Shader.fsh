#version 100

#ifdef GL_ES
precision mediump float;
#endif

varying vec4 destinationColor;
//varying vec2 destinationTexCoord2D;
//varying mat4 destinationColorTransform;

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
    
    gl_FragColor = color;
}
