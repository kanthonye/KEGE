
graphics-shader:
{
    shader-stages:
    [
        {vertex:      shaders/atmosphere/atmosphere.vert}
        {fragment:    shaders/atmosphere/atmosphere.frag}
    ]

    rasterization-state:
    {
        polygon-type:   triangle-strip
        polygon-mode:   fill
        cull-mode:      ccw
    }

    name:                   post-process-atmosphere
}

