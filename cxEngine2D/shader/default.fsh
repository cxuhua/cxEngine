#ifdef GL_ES
precision highp float;
#else
#define highp
#define mediump
#define lowp
#endif

varying mediump vec4 vFragmentColor;
varying mediump vec2 vTexCoord;

uniform sampler2D uTexture0;

const vec3 grayValue = vec3(0.299, 0.587, 0.114);

void main() {
    vec4 color = texture2D(uTexture0, vTexCoord);
    gl_FragColor = vFragmentColor * color;
}
