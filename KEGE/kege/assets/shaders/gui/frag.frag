#version 450
layout(location = 0) in vec4 _color;
layout(location = 1) in vec4 _radius;
layout(location = 2) in vec2 _texcoord;
layout(location = 3) in mat4x2 _quad;
layout(location = 0) out vec4 final_color;

float circle( vec2 center, float radius )
{
    float r = length( center - gl_FragCoord.xy ) - radius;
    return 1.0 - smoothstep(0.0, 1.0, r);
}

float ul_corner_quad()
{
    float x,y;
    x = max(_quad[0].x - gl_FragCoord.x, 0.0);
    x = smoothstep(0.0, 1.0, max(x, 0.0));

    y = max(gl_FragCoord.y - _quad[0].y, 0.0);
    y = smoothstep(0.0, 1.0, max(y, 0.0));
    return (x + y - 1.0);
}

float ur_corner_quad()
{
    float x,y;
    x = max(gl_FragCoord.x - _quad[1].x, 0.0);
    x = smoothstep(0.0, 1.0, max(x, 0.0));

    y = max(gl_FragCoord.y - _quad[1].y, 0.0);
    y = smoothstep(0.0, 1.0, max(y, 0.0));
    return (x + y - 1.0);
}

float ll_corner_quad()
{
    float x,y;
    x = max(_quad[2].x - gl_FragCoord.x, 0.0);
    x = smoothstep(0.0, 1.0, max(x, 0.0));

    y = max(_quad[2].y - gl_FragCoord.y, 0.0);
    y = smoothstep(0.0, 1.0, max(y, 0.0));
    return (x + y - 1.0);
}

float lr_corner_quad()
{
    float x,y;
    x = max(gl_FragCoord.x - _quad[3].x, 0.0);
    x = smoothstep(0.0, 1.0, max(x, 0.0));

    y = max(_quad[3].y - gl_FragCoord.y, 0.0);
    y = smoothstep(0.0, 1.0, max(y, 0.0));
    return (x + y - 1.0);
}

void main()
{
    float alpha;
    alpha  = 1.0 - ul_corner_quad() + circle(_quad[0], _radius.x);
    alpha *= 1.0 - ur_corner_quad() + circle(_quad[1], _radius.y);
    alpha *= 1.0 - ll_corner_quad() + circle(_quad[2], _radius.z);
    alpha *= 1.0 - lr_corner_quad() + circle(_quad[3], _radius.w);

    final_color = vec4( _color.rgb, alpha );
}
