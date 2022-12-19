#version 460

uniform sampler2D sp; 

in vec3 color;
in vec2 tc;
in vec3 Normal;
in vec3 varPos;
uniform vec4 lightColor;
uniform vec3 lightPos;

uniform bool hasTexture;

out vec4 frag_color; 

void main(void) 
{ 
	float ambient = 0.05f;
	float specularLight = 0.50f;

    vec3 normal = normalize(Normal);
    vec3 lightDirection = normalize(lightPos-varPos);
    float cosTheta = dot(lightDirection, normal);


	vec3 reflectionDirection = normalize(reflect(-lightDirection, normal));
	vec3 viewDirection = normalize(-varPos);
    float cosPhi = dot(viewDirection, reflectionDirection);


    float diffuse = max(cosTheta,0.0f);
	float specAmount = pow(max(cosPhi, 0.0f), 6);
	float specular = specAmount * specularLight;

    if(hasTexture)
    frag_color = texture(sp, tc)  * (diffuse + ambient + specular);
    
    else
    frag_color = vec4(color.rgb,1);
}
