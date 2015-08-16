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
uniform float onepixel;
uniform vec2 range;
const float ea = 0.1;
const float lw = 12.0;
const float sw = 3.0;
float dis = distance(vec2(0.5,0.5),vTexCoord);
float alpha(vec2 range) {
    float min = range.x /2.0;
    float max = range.y /2.0;
    if(dis > max){
        return 0.0;
    }
    float p1 = max - onepixel*sw;
    float a1 = smoothstep(max,p1,dis);
    if(dis > p1 && dis <= max){
        return a1;
    }
    float p2 = p1  - onepixel*lw;
    float a2 = smoothstep(p2,p1,dis);
    if(dis > p2 && dis <= p1){
        return a2 + ea;
    }
    float p3 = min;
    if(dis > p3 && dis <= p2){
        return ea;
    }
    if(range.x == 0.0){
        return 0.0;
    }
    float p4 = min - onepixel*lw;
    float a4 = smoothstep(p3,p4,dis);
    if(dis > p4 && dis <= p3){
        return a4 + ea;
    }
    float p5 = p4 - onepixel*sw;
    float a5 = smoothstep(p5,p4,dis);
    if(dis > p5 && dis <= p4){
        return a5;
    }
    return 0.0;
}

void main(){
    float a = alpha(range);
    gl_FragColor = vec4(vFragmentColor.rgb,vFragmentColor.a * a);
}



