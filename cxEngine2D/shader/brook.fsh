#ifdef GL_ES
precision highp float;
#else
#define highp
#define mediump
#define lowp
#endif

#define TAU 6.28318530718
#define MAX_ITER 5

varying mediump vec4 vFragmentColor;
varying highp vec2 vTexCoord;

uniform sampler2D uTexture0;

uniform float time;

void main(void)
{
    float gtime = time * .5+23.0;
    vec2 uv = vTexCoord;
    vec2 p = mod(uv*TAU, TAU)-250.0;
    vec2 i = vec2(p);
    float c = 1.0;
    float inten = .005;
    for (int n = 0; n < MAX_ITER; n++){
        float t = gtime * (1.0 - (3.5 / float(n+1)));
        i = p + vec2(cos(t - i.x) + sin(t + i.y), sin(t - i.y) + cos(t + i.x));
        c += 1.0/length(vec2(p.x / (sin(i.x+t)/inten),p.y / (cos(i.y+t)/inten)));
    }
    c /= float(MAX_ITER);
    c = 1.17-pow(c, 1.4);
    vec3 colour = vec3(pow(abs(c), 8.0));
    colour = clamp(colour + vec3(0.0, 0.35, 0.5), 0.0, 1.0);
    gl_FragColor = vec4(colour, 1.0);
}
