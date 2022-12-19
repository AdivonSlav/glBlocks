#version 460 core

in float isSide;
in vec2 texCoords;
in vec3 normal;
in vec3 fragPos;

out vec4 fragColor;

uniform sampler2D uSampler;
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
const vec4 fogColor = vec4(0.6, 0.8, 1.0, 1.0);
const float fogDensity = 0.00006f;

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

	if (uSunHasSet != 0) // If the sun has set, decrease the ambient and diffuse light to mimic night time
	{
		ambientStrength *= 0.4f;
		diffuseStrength = 0.5f;
	}

	if (ambientStrength < 0.2f)
		ambientStrength = 0.2f;
	else if (ambientStrength > 0.85f)
		ambientStrength = 0.85f;


	vec4 tex = texture2D(uSampler, texCoords);
	vec4 outputLighting = vec4(GetAmbientLight() + (GetDiffuseLight() * 0.7f) + GetSpecularLight(), 1.0f); // We get the output lighting by multiplying all off the calculated types of light

	float fragDistance = gl_FragCoord.z / gl_FragCoord.w; // We get the distance of the fragment to the camera
	float fog = clamp(exp((-fogDensity * fragDistance * fragDistance)), 0.2, 1); // The fog effect is increased exponentially with the distance of the fragment

	gl_FragColor = mix(fogColor, tex * outputLighting, fog); // Linearly interpolates between the fogColor and the texture
}