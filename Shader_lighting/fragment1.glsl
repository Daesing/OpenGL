#version 330 core
in vec3 FragPos;  // Fragment position
in vec3 Normal;   // Normal vector at the fragment
in vec3 viewPos;  // Camera position (viewer's position)

out vec4 FragColor; // Final color of the fragment
uniform vec3 lightPos;  // Light source position
uniform vec3 objectColor;  // Color of the object
uniform vec3 lightColor;  // Color of the light source

void main() {
    float ambientLight = 0.3;  // Intensity of ambient light
    vec3 ambient = ambientLight * lightColor;  // Ambient lighting color

    vec3 normalVector = normalize(Normal);  // Normalize the normal vector
    vec3 lightDir = normalize(lightPos - FragPos);  // Direction from fragment to light

    float diffuseLight = max(dot(normalVector, lightDir), 0.0);  // Diffuse lighting intensity
    vec3 diffuse = diffuseLight * lightColor;  // Diffuse lighting color

    int shininess = 128;  // Shininess factor (controls specular highlight sharpness)

    vec3 viewDir = normalize(viewPos - FragPos);  // Direction from fragment to camera (observer)
    vec3 reflectDir = reflect(-lightDir, normalVector);  // Reflection direction

    float specularLight = max(dot(viewDir, reflectDir), 0.0);  // Specular lighting intensity
    specularLight = pow(specularLight, shininess);  // Apply shininess exponent

    vec3 specular = specularLight * lightColor;  // Specular lighting color

    // Final color = ambient + diffuse + specular, modulated by the object's color
    vec3 result = (ambient + diffuse + specular) * objectColor;

    FragColor = vec4(result, 1.0);  // Set final color with alpha = 1.0 (opaque)
}
