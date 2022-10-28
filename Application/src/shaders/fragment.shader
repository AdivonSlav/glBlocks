#version 460 core

in float isSide;
in vec2 texCoords;
flat in uint type;
in vec3 normal;
in vec3 fragPos;

out vec4 fragColor;

uniform sampler2D uSampler;
uniform int uAtlasSize;
uniform vec3 uLightPos;
uniform vec3 uCamPos;
uniform float uLightLevel;
uniform int uSunHasSet;

vec3 lightColor = vec3(1.0f, 1.0f, 1.0f);
float ambientStrength = 0.9f;
float diffuseStrength = 1.0f;
float specularStrength = 0.5f;
vec3 lightDirection = normalize(uLightPos - fragPos);
vec3 normalizedNormal = normalize(normal);

vec2 GetScaledCoords()
{
	vec2 offsets;

	float texColumn = type % uAtlasSize;
	offsets.x = texColumn / uAtlasSize;

	float texRow = floor(type / uAtlasSize);
	offsets.y = texRow / uAtlasSize;

	return (texCoords / uAtlasSize) + offsets;
}

// Gets an ambient light value based on a constant
vec3 GetAmbientLight()
{
	return lightColor * ambientStrength;
}

// Gets the light based on the angle between the light position and the normal of the fragment
vec3 GetDiffuseLight()
{
	float diff = max(dot(normalizedNormal, lightDirection), 0.0f);

	return diff * lightColor * diffuseStrength;
}

vec3 GetSpecularLight()
{
	vec3 viewDirection = normalize(uCamPos - fragPos);
	vec3 reflectDirection = reflect(-lightDirection, normalizedNormal);

	float specular = pow(max(dot(viewDirection, reflectDirection), 0.0f), 32);

	return specular * lightColor * specularStrength;
}

void main()
{
	ambientStrength = uLightLevel;

	if (uSunHasSet != 0)
	{
		ambientStrength *= 0.4f;
		diffuseStrength = 0.5f;
	}

	if (ambientStrength < 0.2f)
		ambientStrength = 0.2f;
	else if (ambientStrength > 0.85f)
		ambientStrength = 0.85f;


	vec2 scaledTexCoords = GetScaledCoords();
	vec4 tex = texture2D(uSampler, scaledTexCoords);

	vec4 outputLighting = vec4(GetAmbientLight() + (GetDiffuseLight() * 0.7f) + GetSpecularLight(), 1.0f);

	gl_FragColor = tex * outputLighting;
}