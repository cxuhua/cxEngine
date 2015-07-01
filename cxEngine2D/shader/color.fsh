#ifdef GL_ES
precision highp float;
#else
#define highp
#define mediump
#define lowp
#endif
varying mediump vec4 vColor;
void main() {
    gl_FragColor = vColor;
}
