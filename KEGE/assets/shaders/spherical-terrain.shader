
graphics-shader:
{
    shader-stages:
    [
        {vertex:      shaders/spherical-terrain/vert.vert}
        {fragment:    shaders/spherical-terrain/frag.frag}
    ]

    rasterization-state:
    {
        polygon-type:   triangle-list
        polygon-mode:   fill
        cull-mode:      ccw
    }

    name:                   spherical-terrain
}

