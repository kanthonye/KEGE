#version 450

layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec2 vertex_texcoord;

layout(location = 0) out FragData
{
    vec2  texcoord;
    float OBJECT_RADIUS;
    float ATMOSPHERE_LENGTH;
    int   NUM_OUT_SCATTER;
    int   NUM_IN_SCATTER; // max-altitude
    vec3  OBJECT_CENTER;
    vec3  LIGHT_DIR;
    vec3  ray_origin;
    vec3  ray_dir;
}vs_out;

layout(set = 0, binding = 0) uniform CameraMatrices
{
    mat4 proj;
    mat4 view;
    vec4 position;
    vec4 params;
} camera;

struct AtmosphereParameters
{
    float radius;
    float max_altitude;
    int   num_out_scatter;
    int   num_in_scatter; // max-altitude
};

layout(set = 1, binding = 0) uniform AtmosphereData
{
    AtmosphereParameters data;
} atmosphere;

layout( push_constant ) uniform ObjectMatrices
{
    mat4  transform;
    mat3  rotation;
} object_matrices;

void main()
{
    vec4 position = object_matrices.transform * vec4( vertex_position, 1.0 );
    gl_Position = camera.proj * camera.view * position;
    gl_Position.y = -gl_Position.y;

    vs_out.ATMOSPHERE_LENGTH = atmosphere.data.max_altitude;
    vs_out.OBJECT_CENTER = object_matrices.transform[3].xyz;
    vs_out.OBJECT_RADIUS = atmosphere.data.radius;
    vs_out.NUM_OUT_SCATTER = 8;
    vs_out.NUM_IN_SCATTER = 20;
    vs_out.ray_origin = camera.position.xyz;
    vs_out.ray_dir = normalize( vec3(position.xyz - camera.position.xyz) );


    vec3 sun_position = vec3(0, 0, 100000);
//    sun_position = (camera.view * vec4(sun_position, 0)).xyz;
//    sun_position.y = -sun_position.y;
    vec3 sun_direction = sun_position - vs_out.OBJECT_CENTER;
    vs_out.LIGHT_DIR = normalize( sun_direction );
    vs_out.texcoord = vertex_texcoord;
}
