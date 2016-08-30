#version 100

#ifdef GL_ES
precision highp float;
#endif

varying vec2 UV;
varying vec3 Position_worldspace;
varying vec3 Normal_cameraspace;
varying vec3 EyeDirection_cameraspace;
varying vec3 LightDirection_cameraspace;
varying float Vertex_opacity;

// Values that stay constant for the whole mesh.
//uniform sampler2D myTextureSampler;
//uniform mat4 MV;
uniform vec3 LightPosition_worldspace;
uniform vec3 LightColor;
uniform float LightPower;

uniform float MaterialShininess;


vec3 DirectionalLight(in float materialShininess,
                      in vec3 materialAmbientColor,
                      in vec3 materialDiffuseColor,
                      in vec3 materialSpecularColor,
                      in float lightSourcePower,
                      in vec3 lightSourcePosition,
                      in vec3 lightSourceAmbient,
                      in vec3 lightSourceDiffuse,
                      in vec3 lightSourceSpecular)
{
    // Distance to the light
    float distance = length( lightSourcePosition - Position_worldspace );
    
    // Normal of the computed fragment, in camera space
    vec3 n = normalize( Normal_cameraspace );
    // Direction of the light (from the fragment to the light)
    vec3 l = normalize( LightDirection_cameraspace );
    // Cosine of the angle between the normal and the light direction,
    // clamped above 0
    //  - light is at the vertical of the triangle -> 1
    //  - light is perpendicular to the triangle -> 0
    //  - light is behind the triangle -> 0
    float nDotVP = clamp( dot( n, l ), 0.0, 1.0 ); // normal . light direction
    //nDotVP = max(0.0, dot(normal, normalize(vec3(lightSourcePosition))));
    
    // Eye vector (towards the camera)
    vec3 E = normalize(EyeDirection_cameraspace);
    // Direction in which the triangle reflects the light
    vec3 R = reflect(-l, n);
    // Cosine of the angle between the Eye vector and the Reflect vector,
    // clamped to 0
    //  - Looking into the reflection -> 1
    //  - Looking elsewhere -> < 1
    float nDotHV = clamp( dot( n, R ), 0.0, 1.0 ); // normal . light half vector
    //nDotHV = max(0.0, dot(normal, vec3(lightSourceHalfVector)));
    
    vec3 color =
    // Ambient : simulates indirect lighting
    materialAmbientColor * lightSourceAmbient +
    // Diffuse : "color" of the object
    materialDiffuseColor * lightSourceDiffuse * lightSourcePower * nDotVP / (distance * distance) +
    // Specular : reflective highlight, like a mirror
    materialSpecularColor * lightSourceSpecular * lightSourcePower * pow(nDotHV, materialShininess) / (distance * distance);
    
    return color;
}

void main()
{
    // Material properties
//    vec3 MaterialDiffuseColor = texture( myTextureSampler, UV ).rgb;
    vec3 MaterialDiffuseColor = vec3(1.0, 1.0, 1.0);
    vec3 MaterialAmbientColor = vec3(0.0, 0.0, 0.0);
    vec3 MaterialSpecularColor= vec3(1.0, 1.0, 1.0);
    
    vec3 lightSourceAmbient = LightColor;
    vec3 lightSourceDiffuse = LightColor;
    vec3 lightSourceSpecular = LightColor;
    gl_FragColor = vec4(DirectionalLight(MaterialShininess,
                                         MaterialAmbientColor,
                                         MaterialDiffuseColor,
                                         MaterialSpecularColor,
                                         LightPower,
                                         LightPosition_worldspace,
                                         lightSourceAmbient,
                                         lightSourceDiffuse,
                                         lightSourceSpecular),
                        Vertex_opacity);
    
}