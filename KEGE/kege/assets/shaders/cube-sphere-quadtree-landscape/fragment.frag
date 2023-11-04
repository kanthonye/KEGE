#version 450
layout(location = 0) out vec4 frag_Color;

layout(location = 0) in OutData
{
    vec3 NORMAL;
    vec3 COLOR;
};

void main()
{
    vec3 sun_dir = vec3( 0.707, 0.0, 0.707 );

    float diff = max(0.0, dot(sun_dir, NORMAL));
    frag_Color = vec4(COLOR*diff, 1.0);
}

