graphics-shader:
{
    name:                   atmospheric-volume
    
    shader-stages:
    [
        {vertex:      shaders/atmosphere/atmosphere-volume.vert}
        {fragment:    shaders/atmosphere/atmosphere-volume.frag}
    ]

    rasterization-state:
    {
        polygon-type:   triangle-list
        polygon-mode:   fill
        cull-mode:      cw
    }
}

