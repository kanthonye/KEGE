#version 450

layout(location = 0) in vec3 in_point;

layout(location = 0) out OutData
{
    vec3 NORMAL;
    vec3 COLOR;
}vs_out;


layout(set = 0, binding = 0) uniform CameraMatrices
{
    mat4 proj;
    mat4 view;
    vec4 position;
    vec4 params;
} camera;

layout( set = 1, binding = 0 ) buffer readonly FaceOrientationBuffer
{
    mat3 orientation[];
};

layout( set = 2, binding = 0 ) buffer readonly InstanceBatchBuffer
{
    mat2x4 instance[];
};

layout( set = 3, binding = 0 ) uniform sampler2D Heightmaps[6];

layout( push_constant ) uniform constants
{
    mat4 transform;
    mat3 rotation;
} model;


void main()
{
    int face_index = int( instance[ gl_InstanceIndex ][1].x );
    float radius = instance[ gl_InstanceIndex ][1].y;

    mat3 scale_matrix = mat3( instance[ gl_InstanceIndex ][0].w );
    vec3 translation = instance[ gl_InstanceIndex ][0].xyz;

    /**
     * because one mesh is use for each face, the vertices need to be rotated to the proper face orientation.
     */
    vec3 face_point = orientation[ face_index ] * in_point;

    vec3 p = (translation + scale_matrix * face_point);

    mat3 iom = inverse( orientation[ face_index ] );

//    vec3 texcoords[4] = vec2[]
//    (
//        vec2(1.0, 1.0, 0.0),
//        vec2(0.0, 1.0, 0.0),
//        vec2(0.0, 0.0, 0.0),
//        vec2(1.0, 0.0, 0.0)
//    );

    vec3 texcoord = iom * translation + scale_matrix * in_point;
    texcoord = (texcoord + 1.f) * 0.5;
    vec4 i = texture( Heightmaps[ face_index ], texcoord.xy );
    float h = mix( 0.0, 70000.0, pow(i.x, 2) );


    vec3 n;
    float xx = p.x * p.x;
    float yy = p.y * p.y;
    float zz = p.z * p.z;
    n.x = p.x * sqrt(1.0 - (yy + zz) / 2.0 + (yy * zz) / 3);
    n.y = p.y * sqrt(1.0 - (zz + xx) / 2.0 + (zz * xx) / 3);
    n.z = p.z * sqrt(1.0 - (xx + yy) / 2.0 + (xx * yy) / 3);
    p = n * (radius + h);

    vec4 position = model.transform * vec4( p, 1.0 );

    gl_Position = camera.proj * camera.view * position;
    gl_Position.y = -gl_Position.y;

    vs_out.NORMAL = n;
    vs_out.COLOR = vec3(1);///vec3( i.x * vec3(0.188, 0.121, 0.107) );
}
