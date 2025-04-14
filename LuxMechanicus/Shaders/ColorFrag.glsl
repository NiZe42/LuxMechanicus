#version 330 core
out vec4 FragColor;
  
in vec2 texCoord;
in vec3 outNormal;
in vec3 cameraPos;
in vec3 worldFragPos;
in mat4 outModelMatrix;

uniform bool hasTexture;
uniform sampler2D ourTexture;

//Point Lights
uniform int pointLightCount;
uniform vec3 pointLightColors[10];
uniform vec3 pointLightPositions[10];
uniform vec3 pointLightAttenuations[10];
uniform float pointLightIntensities[10];

//Spot Lights
uniform int spotLightCount;
uniform vec3 spotLightColors[10];
uniform vec3 spotLightPositions[10];
uniform vec3 spotLightDirections[10];
uniform float spotLightCutoffs[10];
uniform float spotLightIntensities[10];

void main( void ) {
    vec3 viewDir = normalize(cameraPos - worldFragPos);
	
	vec3 textureColor;

	if(hasTexture) {
		textureColor = texture(ourTexture, texCoord).xyz;
	} else {
		textureColor = vec3(1, 1, 1);
	}

	vec3 normal = normalize(outNormal);

	vec3 resultColor = vec3(0, 0, 0);

	int shininess = 32;
	float ambientStrength = 0.2f;

	for(int i = 0; i < pointLightCount; i++) {
	    vec3 lightVec = pointLightPositions[i] - worldFragPos;
	    vec3 lightDir = normalize(lightVec);
	    
	    vec3 ambientLight = pointLightColors[i] * ambientStrength;
	    
		vec3 diffuseLight = max(dot(normal, lightDir), 0) * pointLightColors[i];
		
		float attenuation = 1 / (pointLightAttenuations[i].x 
		+ pointLightAttenuations[i].y * length(lightVec) + 
		pointLightAttenuations[i].z * pow(length(lightVec), 2));
        
		vec3 specularLight = pow(max(dot(viewDir, reflect(-lightDir, normal)), 0.0f), shininess) * pointLightColors[i];

		resultColor += (ambientLight + diffuseLight + specularLight) * textureColor * attenuation * pointLightIntensities[i];
	}

	for(int i = 0; i < spotLightCount; i++) {
		vec3 lightVec = spotLightPositions[i] - worldFragPos;
		vec3 lightDir = normalize(lightVec);

		vec3 spotDir = normalize(spotLightDirections[i]); 
		spotDir = -spotDir;

		float cosCutoff = cos(radians(spotLightCutoffs[i]));
		float lightAndSpotDotProduct = dot(lightDir, spotDir);
		float cutOffIntensity = clamp((lightAndSpotDotProduct - cosCutoff) / (1.0f - cosCutoff), 0.0f, 1.0f); 

		vec3 ambientLight = spotLightColors[i] * ambientStrength;
		vec3 diffuseLight = max(dot(normal, lightDir), 0.0f) * spotLightColors[i];

		vec3 specularLight = pow(max(dot(viewDir, reflect(-lightDir, normal)), 0.0f), shininess) * spotLightColors[i];

		resultColor += (ambientLight + diffuseLight + specularLight) * textureColor * cutOffIntensity * spotLightIntensities[i];
	}

	FragColor = vec4(resultColor, 1.0f);
}