
graphics-shader:
{
    shader-stages:
    [
        {vertex:      shaders/copier/vert.vert}
        {fragment:    shaders/copier/frag.frag}
    ]

    rasterization-state:
    {
        polygon-type:   triangle-strip
        polygon-mode:   fill
        cull-mode:      ccw
    }

    name:                   color-depth-copier
}

