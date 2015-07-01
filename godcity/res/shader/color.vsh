#ifdef GL_ES
precision highp float;
#else
#define highp
#define mediump
#define lowp
#endif
attribute highp vec3 aPosition;
attribute mediump vec4 aColor;
varying mediump vec4 vColor;
uniform highp mat4 uMP;
uniform highp mat4 uMV;
mat4 mvp = uMP * uMV;
void main() {
    gl_Position = mvp * vec4(aPosition,1.0);
    vColor = aColor;
}
