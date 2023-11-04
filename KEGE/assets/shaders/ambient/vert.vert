#version 450

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec3 in_normal;
layout(location = 2) in vec2 in_texcoord;
layout(location = 3) in vec3 in_tangent;
layout(location = 4) in vec4 in_color;

layout(set = 0, binding = 0) uniform GlobalUniformBlock
{
    mat4 proj;
    mat4 view;
    mat4 projview;
} global;

layout( push_constant ) uniform constants
{
    mat4 transform;
    mat3 rotation;
} model;

void main()
{
    vec4 position =  model.transform * vec4( in_position, 1.0 );
    gl_Position = global.proj * global.view * position;
}
