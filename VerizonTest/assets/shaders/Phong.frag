#version 100

#ifdef GL_ES
precision highp float;
#endif

varying vec4 destinationColor;
varying vec3 eyespaceNormal;
varying vec4 eyespacePosition;

varying vec3 lightAttenuation;
varying vec3 pointToLightVector;
varying vec3 lightDirection;

//http://ruh.li/index.html

float directionalLightIntensity(vec3 lightDirection, vec3 normal, vec3 eyeDir)
{
    // the light direction is defined once for all vertices
    // calculate the diffuse intensity
    float diffuseIntensity = max(dot(normal, lightDirection), 0.0);
    
    float specularIntensity = 0.0;
    float cShininess = 100.0;
    float ambientIntensity = 1.0;
    float pointLightIntensity = 1.0;
    
    // clamp the intensity to a range between 0.0 and 1.0. In many examples I have seen this is solved by only computing
    // the specular value if the diffuseIntensity is > 0.0, but using "if"s in shaders is usually very slow!
    diffuseIntensity = clamp(diffuseIntensity, 0.0, 1.0);
    
    // calculate specular intensity (Blinn-Phong formula)
//    vec3 eyeDir = normalize(varEyeDir);
    vec3 halfwayVector = normalize(lightDirection + eyeDir);
    float specTmp = max(dot(normal, halfwayVector), 0.0);
    specularIntensity = pow(specTmp, cShininess);
    
    return ambientIntensity + (diffuseIntensity + specularIntensity) * pointLightIntensity;
}

float pointLightIntensity(vec3 pointToLightVec, vec3 normal, vec3 eyeDir, vec3 lightAtt)
{
    // this can be calculated in the vertex shader. light.position must be defined in view space here...
//    vec3 pointToLightVec = light.position - vec3(modelViewPos);
    
    // get the normalized light direction and the distance to the light at this point
    vec3 lightDirection = normalize(pointToLightVec);
    float lightDist = length(pointToLightVec);
    
    // calculate the diffuse intensity
    float diffuseIntensity = max(dot(normal, lightDirection), 0.0);
    
    float specularIntensity = 0.0;
    float pointLightIntensity = 0.0;
    
    float cShininess = 100.0;
    float ambientIntensity = 1.0;
    
    diffuseIntensity = clamp(diffuseIntensity, 0.0, 1.0);
    
    // point light intensity
    pointLightIntensity = 1.0 / (lightAtt.x + lightAtt.y * lightDist + lightAtt.z * lightDist * lightDist);
    
    // calculate specular intensity (Blinn-Phong formula)
//    vec3 eyeDir = normalize(varEyeDir);
    vec3 halfwayVector = normalize(lightDirection + eyeDir);
    float specTmp = max(dot(normal, halfwayVector), 0.0);
    specularIntensity = pow(specTmp, cShininess);
    
    return ambientIntensity + (diffuseIntensity + specularIntensity) * pointLightIntensity;
}

float spotLightIntensity(vec3 pointToLightVec, vec3 normal, vec3 eyeDir, vec3 lightAtt, vec3 spotDirection)
{
    // this can be calculated in the vertex shader. light.position must be defined in view space here...
//    vec3 pointToLightVector = light.position - vec3(modelViewPos);
    
    // get the normalized light direction and the distance to the light at this point
    vec3 lightDirection = normalize(pointToLightVec);
    float lightDist = length(pointToLightVec);
    
    // calculate the diffuse intensity
    float diffuseIntensity = max(dot(normal, lightDirection), 0.0);
    
    float specularIntensity = 0.0;
    float pointLightIntensity = 0.0;
    float spotCutoffAngle = 0.01;
    float spotExponent = 100.0;
    float cShininess = 100.0;
    float ambientIntensity = 1.0;
    
    diffuseIntensity = clamp(diffuseIntensity, 0.0, 1.0);
    
    // spot light intensity
    // get the angle between the (negative) spotlight direction and direction towards the light
    float spotValue = dot(normalize(-spotDirection), normalize(lightDirection));
    
    // only show light if angle is large enough
    if (spotValue > spotCutoffAngle)
    {
        // adjust concentration of spotlight
        spotValue = pow(spotValue, spotExponent);
        
        // this part of the formula is the same as for point lights
        pointLightIntensity = 1.0 / (lightAtt.x + lightAtt.y * lightDist
                                     + lightAtt.z * lightDist * lightDist);
        pointLightIntensity *= spotValue;
    }
    
    // calculate specular intensity (Blinn-Phong formula)
//    vec3 eyeDir = normalize(varEyeDir);
    vec3 halfwayVector = normalize(lightDirection + eyeDir);
    float specTmp = max(dot(normal, halfwayVector), 0.0);
    specularIntensity = pow(specTmp, cShininess);
    
    return ambientIntensity + (diffuseIntensity + specularIntensity) * pointLightIntensity;
}

void main(void)
{
    vec3 lightDirection = vec3( 0.0, 0.0, -1.0 );
    
//    float dotProduct = directionalLightIntensity(lightDirection, normalize(eyespaceNormal), normalize(eyespacePosition.xyz));
//    float dotProduct = pointLightIntensity(pointToLightVector, normalize(eyespaceNormal), normalize(eyespacePosition.xyz), lightAttenuation);
    float dotProduct = spotLightIntensity(pointToLightVector, normalize(eyespaceNormal), normalize(eyespacePosition.xyz), lightAttenuation, lightDirection);
    vec4 color = vec4(dotProduct, dotProduct, dotProduct, 1.0);
    
    gl_FragColor = color * destinationColor;
}
