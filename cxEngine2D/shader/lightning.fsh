#ifdef GL_ES
precision highp float;
#else
#define highp
#define mediump
#define lowp
#endif

varying mediump vec4 vFragmentColor;
varying highp vec2 vTexCoord;

uniform sampler2D uTexture0;

uniform float time;

float rand(vec2 n)
{
    return fract(sin(dot(n, vec2(12.9898, 4.1414))) * 43758.5453);
}

float noise(vec2 n)
{
    const vec2 d = vec2(0.0, 1.0);
    vec2 b = floor(n), f = smoothstep(vec2(0.0), vec2(1.0), fract(n));
    return mix(mix(rand(b), rand(b + d.yx), f.x), mix(rand(b + d.xy), rand(b + d.yy), f.x), f.y);
}

float fbm(vec2 n)
{
    float total = 0.0, amplitude = 1.0;
    for (int i = 0; i < 3; i++) {
        total += noise(n) * amplitude;
        n += n;
        amplitude *= 0.5;
    }
    return total;
}

void main(void)
{
    vec4 col = vec4(0,0,0,1);
    vec2 uv = vTexCoord;
    vec2 t = uv * vec2(2.0,1.0) - time*2.0;
    float ycenter = fbm(t)*0.25;
    float diff = abs(uv.y - ycenter-0.25);
    float c1 = 1.0 - mix(0.0,1.0,diff*50.0);
    col = vec4(c1*0.7,c1*.3,c1*.2,1.0)*2.0;
    gl_FragColor = col;
}
