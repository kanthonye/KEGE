#version 450

//layout(set = 2, binding = 0) uniform sampler2D ColorAttachment;
//layout(set = 2, binding = 1) uniform sampler2D DepthAttachment;

layout(location = 0) out vec4 _out_color;
//layout(location = 0) in  vec2 _texcoord;

layout(location = 0) in FragData
{
    vec2  texcoord;
    float OBJECT_RADIUS;
    float ATMOSPHERE_LENGTH;
    int   NUM_OUT_SCATTER;
    int   NUM_IN_SCATTER; // max-altitude
    vec3  OBJECT_CENTER;
    vec3  LIGHT_DIR;
    vec3  ray_origin;
    vec3  ray_dir;
};

// math const
const float PI  = 3.14159265359;
const float MAX = 5000000.0;


float ATMOSPHERE_RADIUS = OBJECT_RADIUS + ATMOSPHERE_LENGTH;
float DENSITY_FALLOFF = 0.5;
float SCATTERING_STRENGTH = 0.005;
vec3  WAVE_LENGTHS = vec3(750.0, 530.0, 400.0);
vec3  SCATTERING_COEFFICIENT = pow(750.0 / WAVE_LENGTHS, vec3(4.0)) * SCATTERING_STRENGTH;




vec2 ray_vs_sphere( vec3 rp, vec3 rd, vec3 sc, float sr )
{
    vec3  m = rp - sc;
    float b = dot( m, rd );
    float c = dot( m, m ) - sr * sr;

    //// Exit if r’s origin outside s (c > 0) and r pointing away from s (b > 0)
    if (c > 0.0f && b > 0.0f) return vec2( MAX );

    float d = b * b - c;

    // A negative discriminant corresponds to ray missing sphere
    if ( d < 0.0 ) return vec2( MAX );

    // Ray now found to intersect sphere, compute smallest t value of intersection
    d = sqrt( d );

    // If near is negative, ray started inside sphere so clamp near to zero
    float near = -b - d;
    float far  = -b + d;

    return vec2( near, far );
}



// Mie
// g : ( -0.75, -0.999 )
//      3 * ( 1 - g^2 )               1 + c^2
// F = ----------------- * -------------------------------
//      8pi * ( 2 + g^2 )     ( 1 + g^2 - 2 * g * c )^(3/2)
float phase_mie( float g, float c, float cc )
{
    float gg = g * g;

    float a = ( 1.0 - gg ) * ( 1.0 + cc );

    float b = 1.0 + gg - 2.0 * g * c;
    b *= sqrt( b );
    b *= 2.0 + gg;

    return ( 3.0 / 8.0 / PI ) * a / b;
}

// Rayleigh
// g : 0
// F = 3/16PI * ( 1 + c^2 )
float phase_ray( float cc )
{
    return ( 3.0 / 16.0 / PI ) * ( 1.0 + cc );
}

float densityAtPoint(vec3 p)
{
    // Get the elevation of the point above the surface of the planet.
    float elevation = length(p - OBJECT_CENTER) - OBJECT_RADIUS;

    // Scale the elevation so that a point directly on the surface of the
    // planet would have a value of 0, and a point directly on the edge of the
    // atmosphere would have a value of 1.
    float elevation_scalar = elevation / ATMOSPHERE_LENGTH;

    // Use some math to determine the density.
    return exp(-elevation_scalar * 10) * (1.0 - elevation_scalar);
}

float opticalDepth(in vec3 ray_origin, in vec3 ray_dir, float ray_length)
{
    float step_length = ray_length / float( NUM_OUT_SCATTER - 1 );
    vec3  step_size = ray_dir * step_length;
    float density_value = 0.0;
    vec3  o = ray_origin;
    for (int i = 0; i < NUM_OUT_SCATTER; i++, o += step_size)
    {
        density_value += densityAtPoint( o ) * step_length;
    }
    return density_value;
}

vec3 in_scatter( vec3 ray_origin, vec3 ray_dir, float ray_length, vec3 sun_dir )
{
    float step_length = ray_length / float( NUM_IN_SCATTER - 1);
    vec3  step_size = ray_dir * step_length;
    vec3  sample_point = ray_origin + step_size;

    vec3 accumulated_light = vec3(0.0);
    for ( int i = 0; i < NUM_IN_SCATTER; i++ )
    {
        float density = densityAtPoint( sample_point );

        vec2 sunray_length = ray_vs_sphere( sample_point, sun_dir, OBJECT_CENTER, ATMOSPHERE_RADIUS );
        //sunray_length.y = min(sunray_length.y, depth);

        float sunray_optical_depth = opticalDepth( sample_point, sun_dir, sunray_length.y );
        float eyeray_optical_depth = opticalDepth( sample_point, -ray_dir, step_length * float(i) );

        vec3 transmission = exp( -(sunray_optical_depth + eyeray_optical_depth) * SCATTERING_COEFFICIENT );

        accumulated_light += step_length * density * transmission * SCATTERING_COEFFICIENT;
        sample_point += step_size;
    }
    return accumulated_light;
}






vec3 getCameraRay(float fieldOfViewY, float aspectRatio, vec2 point)
{
  // compute focal length from given field-of-view
  float focalLength = 1.0 / tan(0.5 * fieldOfViewY * 3.14159265359 / 180.0);
  // compute position in the camera's image plane in range [-1.0, 1.0]
  vec2 pos = 2.0 * (point - 0.5);
  return normalize(vec3(pos.x * aspectRatio, pos.y, -focalLength));
}

float linearizeDepth(float depth, float near, float far)
{
    //return (2.0 * near) / (far + near - original_depth * (far - near));
    return (near * far) / (far + depth * (near - far));
}


void main()
{
    //vec2 texcoord = (gl_FragCoord.xy / vec2(2048.f, 1440.f)) * 2.f - 1.f;

    float near = 0.1;
    float far = 100000.0;
    float aspect_ratio = 1024.0 / 720.0;
    vec4  color_value = vec4(0.0);// = texture( ColorAttachment, texcoord );
    //float depth_value;// = texture( DepthAttachment, texcoord ).r;

    vec2 atmosphere_intersection = ray_vs_sphere( ray_origin, ray_dir, OBJECT_CENTER, ATMOSPHERE_RADIUS );
    if ( atmosphere_intersection.x != atmosphere_intersection.y )
    {
        vec3  I;
        float depth;
        vec3  origin;
        vec3  sundir;
        vec2  surface_intersection;
        float distance_through_atmosphere;

        /*
         when the ray starts inside the sphere the first collision distance atmosphere_intersection.x will be
         a negative value. thus we set it zero. by doing this we get rid of the artifacts (big hole in the atmosphere)
         that shows up with look up into sky
         */
        atmosphere_intersection.x = max( 0.f, atmosphere_intersection.x );

        /*
         we need to test if the surface is interected, if the surface is hit by the ray, then this this will 
         be the furthest distance otherwise the furthest diatnce is the other side of the atmosphere. to do
         this get the minimun distance between the distance to the surface and the distance to the other side
         of the atmosphere sphere.
         */
        surface_intersection = ray_vs_sphere( ray_origin, ray_dir, OBJECT_CENTER, OBJECT_RADIUS );
        atmosphere_intersection.y = min( atmosphere_intersection.y, surface_intersection.x );

        /*
         next we calculate the distance throught the atmosphere by subtracting the nearest and furthest distances
         */
        distance_through_atmosphere = atmosphere_intersection.y - atmosphere_intersection.x;

        /*
         calculate atmospheric scattering at the starting point of the intersection
         */
        sundir = LIGHT_DIR;
        origin = ray_origin + ray_dir * atmosphere_intersection.x;
        I = in_scatter( origin, ray_dir, distance_through_atmosphere, sundir );

        //I = color_value.xyz * (1 - I) + I;
        color_value = vec4( I, dot(vec3(1.f), I) );
    }

    //gl_FragDepth = depth_value;
    _out_color = color_value;
}
