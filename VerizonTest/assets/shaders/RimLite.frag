#version 100

#ifdef GL_ES
precision mediump float;
#endif

//uniform vec3 light_position;
//const vec3 lightPosition = vec3(0.0, 0.0, -1.0);

//uniform sampler2D texture1;

//can pass them as uniforms
const vec3 DiffuseLight = vec3(0.15, 0.05, 0.0);
const vec3 RimColor  = vec3(0.2, 0.2, 0.2);

//higher gamma to get a darker image
const float gamma = 1.0/0.6;

varying vec3 destinationEyePosition;
varying float destinationOpacity;
varying vec3 world_pos;
varying vec3 world_normal;
//varying vec2 texcoord;

void main()
{
    
    vec3 lightPosition = vec3(0.0, -10.0, -1.0);
    //vec3 tex1 = texture(texture1, texcoord).rgb;
    
    
    //get light an view directions
    vec3 L = normalize( lightPosition - world_pos);
    vec3 V = normalize( destinationEyePosition - world_pos);
    
    //diffuse lighting
    vec3 diffuse = DiffuseLight * max(0.0, dot(L, world_normal));
    
    //rim lighting
    float rim = 1.0 - max(dot(V, world_normal), 0.0);
    rim = smoothstep(0.6, 1.0, rim);
    vec3 finalRim = RimColor * vec3(rim, rim, rim);
    
    //get all lights and texture
    vec3 finalColor = finalRim + diffuse;// + tex1;
    
    vec4 finalColorGamma = vec4(pow(finalColor.r, gamma),
                                pow(finalColor.g, gamma),
                                pow(finalColor.b, gamma),
                                1.0);
    
    gl_FragColor = finalColorGamma;
}

