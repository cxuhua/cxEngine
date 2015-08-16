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

void main(void)
{
    vec4  waterColor = vec4(0.166, 0.587, 0.788, 1.0);
    float waterSpeed = 0.8;
    float textureStrength = 0.3;
    
    vec2 uv = vTexCoord;
    
	float iGlobalTime = time * waterSpeed;

    vec4 n1 = texture2D(uTexture0, (vec2(uv.x*2.0 + iGlobalTime * 0.027, uv.y*2.0 - iGlobalTime * 0.025)));
    vec4 n2 = texture2D(uTexture0, (vec2(uv.x*5.312 - iGlobalTime * 0.025, uv.y*3.321 + iGlobalTime * 0.023)));
    vec4 n3 = texture2D(uTexture0, (vec2(uv.x*1.312 + iGlobalTime * 0.024, uv.y*2.321 + iGlobalTime * 0.026)));
	
	n1.rgb *= textureStrength;
	n2.rgb *= textureStrength;
	n3.rgb *= textureStrength;
    
	vec4 color = waterColor + (n1+n2+n3);
	
// 2d sun
//    lowp vec2 q = vTexCoord;
//	color.rgb *= (0.6 + (0.5*(q.y)));
	
	gl_FragColor = vec4(color.r, color.g, color.b, 1.0);
}
