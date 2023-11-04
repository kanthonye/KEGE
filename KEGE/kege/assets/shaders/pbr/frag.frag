#version 450
layout(location = 0) out vec4 frag_Color;

layout(location = 0) in vec3 _normal;
layout(location = 1) in vec2 _texcoord;
layout(location = 2) in vec3 _tangent;
layout(location = 3) in vec4 _color;

void main()
{
    vec3 ls = normalize( vec3( 5.0, 15.0, 10.0 ) );
    float shade = max( dot(_normal, ls), 0.01);
    frag_Color = vec4(shade * _color.rgb, _color.a);
}

