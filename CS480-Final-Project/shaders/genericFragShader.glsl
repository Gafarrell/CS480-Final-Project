#version 460

uniform sampler2D sp; 

in vec3 color; 
in vec2 tc;

in vec3 Normal;
in vec3 crntPos;

uniform vec4 lightColor;
uniform vec3 lightPos;
uniform vec3 camPos;

uniform bool hasTexture;

out vec4 frag_color; 

void main(void) 
{ 
	float ambient = 0.20f;

    vec3 normal = normalize(Normal);
    vec3 lightDirection = normalize(lightPos - crntPos);
    float diffuse = max(dot(normal, lightDirection),0.0f);

	float specularLight = 0.50f;
	vec3 viewDirection = normalize(camPos - crntPos);
	vec3 reflectionDirection = reflect(-lightDirection, normal);
	float specAmount = pow(max(dot(viewDirection, reflectionDirection), 0.0f), 8);
	float specular = specAmount * specularLight;

    if(hasTexture)
        frag_color = texture(sp, tc) * lightColor * (diffuse + ambient + specular);
    else 
        frag_color = vec4(color.rgb, 1);
}
