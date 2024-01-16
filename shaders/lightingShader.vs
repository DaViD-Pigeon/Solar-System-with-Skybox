#version 330 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

out vec3 FragPos;
out vec3 Normal;
out vec3 objColor;  // For flat shading
out vec2 TexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main(){
    // Calculate transformed position in world space
    FragPos = vec3(model * vec4(aPos, 1.0));

    // Calculate transformed normal in world space
    Normal = mat3(transpose(inverse(model))) * aNormal;

    // Calculate screen-space position
    gl_Position = projection * view * vec4(FragPos, 1.0);

    // Pass object color and texture coordinates to the fragment shader
    TexCoord = aTexCoord;
}
