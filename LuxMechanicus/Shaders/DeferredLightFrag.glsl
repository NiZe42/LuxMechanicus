#version 460 core
out vec4 FragColor;
  
in vec2 TexCoords;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedoSpec;

uniform vec3 cameraPosition;

struct LightData {
    vec3 position;
    vec3 attenuation;
    vec3 color;
    vec3 direction;
	float pad;
    int lightType;
    float intensity;
    float cutoff;
};

layout(std430, binding = 1) buffer LightBuffer {
    LightData lights[];
};

void main( void ) {
	vec3 worldFragPos = texture(gPosition, TexCoords).rgb;
    vec3 normal = texture(gNormal, TexCoords).rgb;
    vec3 albedo = texture(gAlbedoSpec, TexCoords).rgb;
	// There can also be specular component, but for not it is not added.

    vec3 viewDir = normalize(cameraPosition - worldFragPos);

	vec3 resultColor = vec3(0, 0, 0);

	int shininess = 32;
	float ambientStrength = 0.2f;

	for (int i = 0; i < lights.length(); i++) {

		LightData currentLight = lights[i];

		if (currentLight.lightType == 0) {   // SpotLight.

			vec3 lightVec = currentLight.position - worldFragPos;
			vec3 lightDir = normalize(lightVec);

			vec3 spotDir = normalize(currentLight.direction); 
			spotDir = -spotDir;

			float cosCutoff = cos(radians(currentLight.cutoff));
			float lightAndSpotDotProduct = dot(lightDir, spotDir);
			float cutOffIntensity = clamp((lightAndSpotDotProduct - cosCutoff) / (1.0f - cosCutoff), 0.0f, 1.0f); 

			vec3 ambientLight = currentLight.color * ambientStrength;
			vec3 diffuseLight = max(dot(normal, lightDir), 0.0f) * currentLight.color;

			vec3 specularLight = pow(max(dot(viewDir, reflect(-lightDir, normal)), 0.0f), shininess) * currentLight.color;

			resultColor += (ambientLight + diffuseLight + specularLight) * albedo * cutOffIntensity * currentLight.intensity;

		} else if (currentLight.lightType == 1) {    // PointLight.

			vec3 lightVec = currentLight.position - worldFragPos;
			vec3 lightDir = normalize(lightVec);
	    
			vec3 ambientLight = currentLight.color * ambientStrength;
	    
			vec3 diffuseLight = max(dot(normal, lightDir), 0) * currentLight.color;
		
			float attenuation = 1 / (currentLight.attenuation.x 
			+ currentLight.attenuation.y * length(lightVec) + 
			currentLight.attenuation.z * pow(length(lightVec), 2));
        
			vec3 specularLight = pow(max(dot(viewDir, reflect(-lightDir, normal)), 0.0f), shininess) * currentLight.color;

			resultColor += (ambientLight + diffuseLight + specularLight) * albedo * attenuation * currentLight.intensity;

		} else if (currentLight.lightType == 2) {  // Directional Light

            vec3 lightDir = normalize(-currentLight.direction); 

            vec3 ambientLight = currentLight.color * ambientStrength;
            vec3 diffuseLight = max(dot(normal, lightDir), 0.0) * currentLight.color;
            vec3 specularLight = pow(max(dot(viewDir, reflect(-lightDir, normal)), 0.0), shininess) * currentLight.color;

            resultColor += (ambientLight + diffuseLight + specularLight) * albedo * currentLight.intensity;
        }
	}

	FragColor = vec4(resultColor, 1.0f);
}