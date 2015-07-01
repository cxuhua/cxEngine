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
void main() {
    vec4 color = texture2D(uTexture0, vTexCoord);
    gl_FragColor = vFragmentColor * color;
}
