#version 100

#ifdef GL_ES
precision mediump float;
#endif

uniform vec3 light_position;
varying vec3 eye_position;

//uniform sampler2D texture1;

//can pass them as uniforms
const vec3 DiffuseLight = vec3(0.15, 0.05, 0.0);
const vec3 RimColor  = vec3(0.2, 0.2, 0.2);

//higher gamma to get a darker image
const float gamma = 1.0/0.6;

varying vec3 world_pos;
varying vec3 world_normal;
//varying vec2 texcoord;

varying vec4 destinationColor;

void main(){
    
//    vec3 tex1 = texture(texture1, texcoord).rgb;
    vec3 tex1 = vec3(1.0);
    
    //get light an view directions
    vec3 L = normalize( light_position - world_pos);
    vec3 V = normalize( eye_position - world_pos);
    
    //diffuse lighting
    vec3 diffuse = DiffuseLight * max(0.0, dot(L,world_normal));
    
    //rim lighting
    float rim = 1.0 - max(dot(V, world_normal), 0.0);
    rim = smoothstep(0.6, 1.0, rim);
    vec3 finalRim = RimColor * vec3(rim, rim, rim);
    
    //get all lights and texture
    vec3 finalColor = finalRim + diffuse + tex1;
    
    vec3 finalColorGamma = vec3(pow(finalColor.r, gamma),
                                pow(finalColor.g, gamma),
                                pow(finalColor.b, gamma));
    
    gl_FragColor = vec4(finalColorGamma, 1.0);
}
