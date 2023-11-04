graphics-shader:
{
    shader-stages:
    [
        {vertex:      shaders/debug/triangle.vert}
        {fragment:    shaders/debug/triangle.frag}
    ]

    rasterization-state:
    {
        polygon-type:   triangle-list
        polygon-mode:   fill
        cull-mode:      ccw
    }

    name:                   debug-triangle
}

