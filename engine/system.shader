
////////////default shader///////////////////////////
static cxStr *DefaultVSH = cxStr::UTF8(STRINGIFY(
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
}));

static cxStr *DefaultFSH = cxStr::UTF8(STRINGIFY(
precision highp float;
varying mediump vec4 vFragmentColor;
varying mediump vec2 vTexCoord;
uniform sampler2D uTexture0;
void main() {
    vec4 color = texture2D(uTexture0, vTexCoord);
    gl_FragColor = vFragmentColor * color;
}));

////////////gray shader///////////////////////////
static cxStr *GrayVSH = cxStr::UTF8(STRINGIFY(
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
}));

static cxStr *GrayFSH = cxStr::UTF8(STRINGIFY(
precision highp float;
varying mediump vec4 vFragmentColor;
varying mediump vec2 vTexCoord;
uniform sampler2D uTexture0;
const vec3 grayValue = vec3(0.299, 0.587, 0.114);
void main() {
    vec4 color = texture2D(uTexture0, vTexCoord);
    float vscale = dot(color.rgb, grayValue);
    vec4 gray = vec4(vscale, vscale, vscale, color.a);
    gl_FragColor = vFragmentColor * mix(color, gray, vFragmentColor.a);
}));

////////////color shader///////////////////////////
static cxStr *ColorVSH = cxStr::UTF8(STRINGIFY(
precision highp float;
attribute highp vec3 aPosition;
attribute mediump vec4 aColor;
varying mediump vec4 vColor;
uniform highp mat4 uMP;
uniform highp mat4 uMV;
mat4 mvp = uMP * uMV;
void main() {
    gl_Position = mvp * vec4(aPosition,1.0);
    vColor = aColor;
}));

static cxStr *ColorFSH = cxStr::UTF8(STRINGIFY(
precision highp float;
varying mediump vec4 vColor;
void main() {
    gl_FragColor = vColor;
}));

