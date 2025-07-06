#version 460 core
out vec4 FragColor;
  
in vec2 TexCoords;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedoSpec;

uniform sampler2DArray shadowAtlasArray;

uniform vec3 cameraPosition;

struct LightData {
    vec3 position;

    vec3 attenuation;

    vec3 color;

    vec3 direction;
    float pad0;

    int lightType;
    bool castShadows;
	float intensity;
	float cutoff;

	uint lightIndex;
	float pad2;
	float pad3;
	float pad4;
};

layout(std430, binding = 1) buffer LightBuffer {
    LightData lights[];
};

struct ShadowCasterData {
    uint lightIndex;
    int shadowCasterId;
    float pad0;
    float pad1;

    mat4 lightVPMatrix;
};

layout(std430, binding = 2) buffer shadowBuffer {
    ShadowCasterData shadowCasters[];
};

bool GetShadowCasterFromLight(uint lightIndex, out ShadowCasterData result) {
    for (int i = 0; i < shadowCasters.length(); ++i) {
        if (shadowCasters[i].lightIndex == lightIndex) {
            result = shadowCasters[i];
            return true;
        }
    }
    return false;
}

float CalculateShadow(
LightData light, 
ShadowCasterData shadowCaster, 
vec3 worldPos,
vec3 lightDir,
vec3 normal) {
    vec4 projected = shadowCaster.lightVPMatrix * vec4(worldPos, 1.0);
    //projected.xyz /= projected.w;

    projected = projected * 0.5 + 0.5;

    int tileIndex = shadowCaster.shadowCasterId % 16;
    int tileX = tileIndex % 4;
    int tileY = tileIndex / 4;
    int atlasLayer = shadowCaster.shadowCasterId / 16;

    float tileSize = 1.0 / 4.0;

    projected.x = projected.x * tileSize + float(tileX) * tileSize;
    projected.y = projected.y * tileSize + float(tileY) * tileSize;

    //projected.xy = projected.xy * 2.0 - 1.0;

    float texelSize = tileSize / 1024.0; // assuming 1024x1024 shadow map tiles
    float shadow = 0.0;
    int samples = 4;
    float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005); 

    for (int x = -1; x <= 1; ++x) {
        for (int y = -1; y <= 1; ++y) {
            vec2 offset = vec2(x, y) * texelSize;
            float sampleDepth = texture(shadowAtlasArray, vec3(projected.xy + offset, atlasLayer)).r;
            if (projected.z > 1.0) projected.z = 1;
            shadow += projected.z - bias > sampleDepth ? 1.0 : 0.0;
        }
    }

    shadow /= 9.0;

    return shadow;
    //return currentDepth - bias > shadowMapDepth ? 1.0 : 0.0;
    //return shadowMapDepth;
}

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
        ShadowCasterData currentShadowCaster;
        bool found = GetShadowCasterFromLight(currentLight.lightIndex, currentShadowCaster);

        if (!found) {
            continue; // Skip this light if it has no shadow caster
        }

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

            float shadow = CalculateShadow(
            currentLight, 
            currentShadowCaster, 
            worldFragPos,
            lightDir, 
            normal);

            //resultColor = vec3(shadow);
            resultColor += (ambientLight + (1.0f - shadow) * (diffuseLight + specularLight)) * albedo * currentLight.intensity;
        }
	}

	FragColor = vec4(resultColor, 1.0f);
}