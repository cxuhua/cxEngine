#ifdef GL_ES
precision highp float;
#else
#define highp
#define mediump
#define lowp
#endif
attribute highp vec3 aPosition;
attribute mediump vec2 aTexcoord;
attribute mediump vec4 aColor;
varying mediump vec4 vFragmentColor;
varying mediump vec2 vTexCoord;
uniform highp mat4 uMP;
void main() {
    vFragmentColor = aColor;
    vTexCoord = aTexcoord;
    gl_Position = uMP * vec4(aPosition,1.0);
}
