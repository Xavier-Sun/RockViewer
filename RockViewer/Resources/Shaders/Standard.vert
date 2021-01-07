#version 330 core

layout (location = 0) in vec3 Position;
layout (location = 1) in vec3 Normal;
layout (location = 2) in vec2 Texcoord;
layout (location = 3) in vec3 Tangent;

out vec3 worldPos;
out vec3 worldNormal;
out vec2 uv;

uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;

void main()
{
    gl_Position = Projection * View * Model * vec4(Position, 1.0);
    worldNormal = mat3(transpose(inverse(Model))) * Normal;
    worldPos = vec3(Model * vec4(Position, 1.0));
    uv = Texcoord;
}
