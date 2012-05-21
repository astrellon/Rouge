const vec3 AMBIENT = vec3(0.2, 0.2, 0.2);
const vec3 LIGHT_COLOUR = vec3(1.0f, 0.8f, 0.7f);

uniform vec3 lightDirection;

varying vec3 fragmentNormal;
varying vec3 cameraVector;
varying vec4 objectColour;

void
main()
{
	// initialize diffuse/specular lighting
	vec3 diffuse = vec3(0.0, 0.0, 0.0);
	vec3 specular = vec3(0.0, 0.0, 0.0);

	// normalize the fragment normal and camera direction
	vec3 normal = normalize(fragmentNormal);
	vec3 cameraDir = normalize(cameraVector);

	float diffuseDot = dot(normal, lightDirection);
	diffuse += LIGHT_COLOUR * clamp(diffuseDot, 0.0, 1.0);

	// specular
	//vec3 halfAngle = normalize(cameraDir + lightDirection);
	//vec3 specularColor = min(LIGHT_COLOUR + 0.2, 0.6);
	//float specularDot = dot(normal, halfAngle);
	//specular += specularColor * pow(clamp(specularDot, 0.0, 1.0), 16.0);

	//vec4 sample = vec4(1.0, 0.0, 1.0, 1.0);
	gl_FragColor = vec4(clamp(objectColour.rgb * (diffuse + AMBIENT) + specular, 0.0, 1.0), objectColour.a);
}
