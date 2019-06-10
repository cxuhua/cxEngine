
////////////default shader///////////////////////////
static cchars DefaultVSH = STRINGIFY(
precision highp float;
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
});
static cchars DefaultFSH = STRINGIFY(
precision highp float;
varying mediump vec4 vFragmentColor;
varying mediump vec2 vTexCoord;
uniform sampler2D uTexture0;
uniform vec4 uColor;
void main() {
    vec4 color = texture2D(uTexture0, vTexCoord);
    gl_FragColor = vFragmentColor * color * uColor;
});
////////////gray shader///////////////////////////
static cchars GrayVSH = STRINGIFY(
precision highp float;
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
});
static cchars GrayFSH = STRINGIFY(
precision highp float;
varying mediump vec4 vFragmentColor;
varying mediump vec2 vTexCoord;
uniform sampler2D uTexture0;
const vec3 grayValue = vec3(0.299, 0.587, 0.114);
uniform vec4 uColor;
void main() {
    vec4 color = texture2D(uTexture0, vTexCoord);
    float vscale = dot(color.rgb, grayValue);
    vec4 gray = vec4(vscale, vscale, vscale, color.a);
    gl_FragColor = uColor * vFragmentColor * mix(color, gray, vFragmentColor.a);
});
////////////color shader///////////////////////////
static cchars ColorVSH = STRINGIFY(
precision highp float;
attribute highp vec3 aPosition;
attribute mediump vec4 aColor;
varying mediump vec4 vColor;
uniform highp mat4 uMP;
void main() {
    gl_Position = uMP * vec4(aPosition,1.0);
    vColor = aColor;
});
static cchars ColorFSH = STRINGIFY(
precision highp float;
varying mediump vec4 vColor;
uniform vec4 uColor;
void main() {
    gl_FragColor = vColor * uColor;
});

