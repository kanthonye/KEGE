#version 450

layout(location = 0) in vec3 in_point;

layout(set = 0, binding = 0) uniform GlobalUniformBlock
{
    mat4 proj;
    mat4 view;
    vec4 position;
} global;


layout( set = 1, binding = 0 ) buffer InstanceBuffer
{
    mat2x4 instance[];
};

layout( push_constant ) uniform constants
{
    mat4 transform;
    mat3 rotation;
} model;

layout(location = 0) out vec3 _color;

void main()
{
    mat3 scale_matrix = mat3( instance[ gl_InstanceIndex ][0].w );
    vec3 translation = instance[ gl_InstanceIndex ][0].xyz;
    vec3 point = translation + scale_matrix * in_point;

    vec3 n = normalize( point );
    vec3 p = n * instance[ gl_InstanceIndex ][1].x;

    vec4 position = model.transform * vec4( p, 1.0 );
    vec3 normal = model.rotation * n;
    gl_Position = global.proj * global.view * position;
    gl_Position.y = -gl_Position.y;

    _color = vec3(0.0,1.0,1.0);
}
