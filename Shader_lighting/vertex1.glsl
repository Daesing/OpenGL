#version 330 core
layout (location = 0) in vec3 vPos;    // Vertex position (object space)
layout (location = 1) in vec3 vNormal; // Vertex normal (object space)

out vec3 FragPos;  // Object position in world space, passed to fragment shader
out vec3 viewPos;  // Camera position in view space, passed to fragment shader
out vec3 Normal;   // Normal in world space, passed to fragment shader

uniform mat4 modelTransform;     // Model transformation matrix
uniform mat4 projectionTransform; // Projection transformation matrix
uniform mat4 viewTransform;      // View transformation matrix
uniform mat4 normalMatrix;       // Normal matrix (inverse transpose of model matrix)

void main()
{
    // Apply model, view, and projection transformations to vertex position
    gl_Position = projectionTransform * viewTransform * modelTransform * vec4(vPos, 1.0);

    // Pass the fragment position (in world space) to the fragment shader
    FragPos = vec3(modelTransform * vec4(vPos, 1.0));

    // Pass the camera position in view space to the fragment shader
    viewPos = vec3(viewTransform * vec4(vPos, 1.0));

    // Transform normal vector to world space (apply normal matrix)
    Normal = mat3(normalMatrix) * vNormal; // Normal transformation

}
