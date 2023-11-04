
graphics-shader:
{
    shader-stages:
    [
        {vertex:      shaders/cube-sphere-quadtree-landscape/vertex.vert}
        {fragment:    shaders/cube-sphere-quadtree-landscape/fragment.frag}
    ]

    rasterization-state:
    {
        polygon-type:   triangle-list
        polygon-mode:   line
        cull-mode:      ccw
    }

    name:               cube-sphere-quadtree-planet
}

