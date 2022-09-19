#version 330 core
#ifdef GL_ES
precision mediump float;
#endif

#define TWO_PI 6.28318530718

uniform float fBrightness;
uniform vec2 uResolution;

vec3 hsb2rgb(in vec3 c)
{
    vec3 rgb = clamp(abs(mod(c.x * 6.0 + vec3(0.0, 4.0, 2.0), 6.0) - 3.0) - 1.0, 0.0, 1.0);
    rgb = rgb * rgb * (3.0 - 2.0 * rgb);
    return c.z * mix(vec3(1.0), rgb, c.y);
}

void main()
{
    // TODO: 内边距设置还需要考虑其他方向
    vec2 st = (2.0 * gl_FragCoord.xy - uResolution.xy) / min(uResolution.y, uResolution.x);
    float radius = length(st);
    if (radius <= 1.0)
    {
        float angle = atan(st.y, st.x);
        vec3 color = hsb2rgb(vec3(angle / TWO_PI, radius, fBrightness));

        gl_FragColor = vec4(color, 1.0);
    }
    // else
    // {
    //     gl_FragColor = vec4(0.0);
    // }
}
