#version 460

uniform sampler2D sp; 

in vec3 color; 
in vec2 tc;

in vec3 Normal;
in vec3 crntPos;

uniform vec4 lightColor;
uniform vec3 lightPos;

uniform bool hasTexture;


out vec4 frag_color; 

void main(void) 
{ 
    vec3 normal = normalize(Normal);
    vec3 lightDirection = normalize(lightPos - crntPos);
    float diffuse = max(dot(normal, lightDirection),0.0f);


    if(hasTexture)
        frag_color = texture(sp, tc) * lightColor * diffuse;
    else 
        frag_color = vec4(color.rgb, 1);
}