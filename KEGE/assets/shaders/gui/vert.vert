#version 450

layout(location = 0) in vec4 in_transform;
layout(location = 1) in vec4 in_color;
layout(location = 2) in vec4 in_textile;
layout(location = 3) in vec4 in_bradius;

layout(location = 0) out vec4 _color;
layout(location = 1) out vec4 _radius;
layout(location = 2) out vec2 _texcoord;
layout(location = 3) out mat4x2 _quad;

layout(set = 10, binding = 0) uniform GlobalUniformBlock
{
    mat4 projection;
    mat4 shape;
} global;

void main()
{
    float s = 2.0;
    float w = in_transform.z;
    float h = in_transform.w;
    vec2  t = in_transform.xy;
    mat4x2 rect;

    // important: clamping the radius to make sure its values dont exceed the quads dimensions
    _radius.x = clamp( in_bradius.x, 0.0, max(w,h) );
    _radius.y = clamp( in_bradius.y, 0.0, max(w,h) );
    _radius.z = clamp( in_bradius.z, 0.0, max(w,h) );
    _radius.w = clamp( in_bradius.w, 0.0, max(w,h) );

    // the vertice for the outer rectangule
    rect[0] = vec2( 0, h) + t;
    rect[1] = vec2( w, h) + t;
    rect[2] = vec2( 0, 0) + t;
    rect[3] = vec2( w, 0) + t;

    // the vertice for the inner rectangule
    _quad[0] = rect[0] * s + vec2( in_bradius.x,-in_bradius.x );
    _quad[1] = rect[1] * s + vec2(-in_bradius.y,-in_bradius.y );
    _quad[2] = rect[2] * s + vec2( in_bradius.z, in_bradius.z );
    _quad[3] = rect[3] * s + vec2(-in_bradius.w, in_bradius.w );

    gl_Position = global.projection * vec4(rect[ gl_VertexIndex ].xy, 0.0f, 1.0f);
    _color = in_color;
}
