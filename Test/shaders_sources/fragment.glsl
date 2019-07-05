#version 330 core
#define POINT_LIGHTS_COUNT 4
struct Material {
	sampler2D diffuse;
	sampler2D specular;

	float shininess;
};


struct DirLight {
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct PointLight {
	vec3 position;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
	float linear;
	float quadratic;
};

struct ProjectorLight {
	vec3 position;
	vec3 direction;

	float innerCutOff;
	float outerCutOff;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
	float linear;
	float quadratic;
};



in vec3 norm;
in vec3 fragPos;
in vec2 texCoords;


out vec4 color;

uniform vec3 viewPos;
uniform vec3 viewDir;

uniform Material material;
uniform DirLight dirlight;
uniform PointLight pointLights[POINT_LIGHTS_COUNT];
uniform ProjectorLight projector;


vec3 calcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
	vec3 lightDir = normalize(-light.direction);
	
	float diff = max(dot(normal, lightDir), 0.f);
	
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.f), material.shininess);
	
	vec3 ambient  = light.ambient  * texture(material.diffuse,  texCoords).rgb;
	vec3 diffuse  = light.diffuse  * texture(material.diffuse,  texCoords).rgb * diff;
	vec3 specular = light.specular * texture(material.specular, texCoords).rgb * spec;
		
	return (ambient + diffuse + specular);
}


vec3 calcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
	float dist = length(fragPos - light.position);
	vec3 lightDir = normalize(light.position - fragPos);
	float attenuation = 1.f / (light.constant +
		   light.linear * dist +
		   light.quadratic * dist * dist);
	
	float diff = max(dot(normal, lightDir), 0.0f);
	
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.f), material.shininess);
	
	vec3 ambient  = light.ambient  * texture(material.diffuse,  texCoords).rgb;
	vec3 diffuse  = light.diffuse  * texture(material.diffuse,  texCoords).rgb * diff;
	vec3 specular = light.specular * texture(material.specular, texCoords).rgb * spec;
	
	return (ambient + diffuse + specular) * attenuation;
}	


vec3 calcProjectorLight(ProjectorLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
	float dist = length(fragPos - light.position);
	vec3 lightDir = normalize(light.position - fragPos);
	float attenuation = 1.f / (light.constant +
		   light.linear * dist +
		   light.quadratic * dist * dist);
	
	float diff = max(dot(normal, lightDir), 0.0f);
	
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.f), material.shininess);
	
	float theta = dot(lightDir, normalize(-light.direction));
	float epsilon = light.innerCutOff - light.outerCutOff;
	float intensivity = clamp((theta - light.outerCutOff) / epsilon, 0.0f, 1.0f);

	vec3 ambient  = light.ambient  * texture(material.diffuse,  texCoords).rgb;
	vec3 diffuse  = light.diffuse  * texture(material.diffuse,  texCoords).rgb * diff;
	vec3 specular = light.specular * texture(material.specular, texCoords).rgb * spec;
	
	return (ambient + diffuse + specular) * attenuation * intensivity;
}	


void main()
{
	vec3 result = calcDirLight(dirlight, norm, viewDir);

	for (int i = 0; i < POINT_LIGHTS_COUNT; i++)
		result += calcPointLight(pointLights[i], norm, fragPos, viewDir);

	result += calcProjectorLight(projector, norm, fragPos, viewDir);

	color = vec4(result, 1.f);
}