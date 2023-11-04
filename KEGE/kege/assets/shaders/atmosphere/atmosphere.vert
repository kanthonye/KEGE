#version 450

layout(location = 0) out FragData
{
    float OBJECT_RADIUS;
    float ATMOSPHERE_LENGTH;
    int   NUM_OUT_SCATTER;
    int   NUM_IN_SCATTER; // max-altitude
    vec3  OBJECT_CENTER;
    vec3  LIGHT_DIR;
    vec3  ray_origin;
    //vec3  ray_dir;
    vec2  texcoord;

    float aspect_ratio;
    float fov;
    float near;
    float far;
}vs_out;

layout(set = 0, binding = 0) uniform CameraMatrices
{
    mat4 proj;
    mat4 view;
    vec4 position;

    float aspect;
    float fov;
    float near;
    float far;
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


vec2 positions[4] = vec2[]
(
 vec2( 1.0,  1.0),
 vec2(-1.0,  1.0),
 vec2( 1.0, -1.0),
 vec2(-1.0, -1.0)
);

void main()
{
    gl_Position = vec4( positions[ gl_VertexIndex ], 0.0, 1.0 );

    //vec4 position = object_matrices.transform * vec4( positions[ gl_VertexIndex ], 0.0, 1.0 );

    vs_out.texcoord = (positions[ gl_VertexIndex ] + 1.0f) * 0.5;
    vs_out.OBJECT_CENTER = vec3(camera.view * vec4(object_matrices.transform[3].xyz, 1.0));
    vs_out.OBJECT_CENTER.y = -vs_out.OBJECT_CENTER.y;
    vs_out.ATMOSPHERE_LENGTH = 80000;//atmosphere.data.max_altitude;
    vs_out.OBJECT_RADIUS = atmosphere.data.radius;
    vs_out.NUM_OUT_SCATTER = 20;
    vs_out.NUM_IN_SCATTER = 40;
    vs_out.ray_origin = vec3( 0.0, 0.0, 0.0 );


    vec3 sun_position = vec3(0, 0, 100000);
    sun_position = (camera.view * vec4(sun_position, 0)).xyz;
    sun_position.y = -sun_position.y;
    vec3 sun_direction = sun_position - vs_out.OBJECT_CENTER;
    vs_out.LIGHT_DIR = normalize( sun_direction );

    vs_out.aspect_ratio = 1024.0 / 720.0;//camera.aspect;
    vs_out.near = 0.1;//camera.near;
    vs_out.far = 10000000.0;;//camera.far;
    vs_out.fov = 65;
}



