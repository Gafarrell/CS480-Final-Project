#version 460

in vec4 varyingColor;

in vec3 Normal;
in vec3 varPos;

out vec4 color;

uniform mat4 mv_matrix;
uniform mat4 proj_matrix;
uniform vec4 lightColor;
uniform vec3 lightPos;

void main(void)
{	

	float ambient = 0.05f;

    vec3 normal = normalize(Normal);
    vec3 lightDirection = normalize(lightPos-varPos);


	float specularLight = 0.50f;
	vec3 viewDirection = normalize(-varPos);
	vec3 reflectionDirection = normalize(reflect(-lightDirection, normal));

    float cosTheta = dot(lightDirection, normal);
    float cosPhi = dot(reflectionDirection, viewDirection);

    float diffuse = max(cosTheta,0.0f);
	float specAmount = pow(max(0.0, cosPhi), 8);
	float specular = specAmount * specularLight;


	color = varyingColor  * (diffuse + ambient + specular);
}