#ifdef GL_ES
precision mediump float;
#else
#define highp
#define mediump
#define lowp
#endif

#ifndef SIMPLE
//#ifndef MEDIUM
#define LIGHTMAP
//#endif // MEDIUM
#define REFLECTION
#endif // SIMPLE
#ifdef FOAM
#ifndef SIMPLE
#define USE_FOAM
#endif // SIMPLE
#endif // FOAM

uniform lowp sampler2D normal0;
#ifdef USE_FOAM
	uniform lowp sampler2D foam;
#endif

varying vec4 v_wave;
varying highp vec2 v_bumpUv1;
#ifdef USE_FOAM
varying highp vec2 v_foamUv;
varying float v_foamPower;
#endif
varying vec3 v_darkColor;
varying vec3 v_lightColor;

#ifdef LIGHTMAP
uniform lowp sampler2D lightmap;
varying vec2 v_worldPos;
#endif

void main()
{
    vec4 normalMapValue = texture2D(normal0, v_bumpUv1.xy);
    gl_FragColor =  vec4(mix(v_lightColor, v_darkColor, (normalMapValue.x * v_wave.y) + (normalMapValue.y * (1.0 - v_wave.y))), v_wave.x)
	#ifdef REFLECTION
	+ vec4(pow(((normalMapValue.z * v_wave.y) + (normalMapValue.w * (1.0 - v_wave.y))) * v_wave.z, v_wave.w));
	#else
	;
	#endif
	#ifdef USE_FOAM
	gl_FragColor = mix(gl_FragColor, vec4(0.95, 0.95, 0.95, gl_FragColor.a), min(1.0, texture2D(foam, v_foamUv).r * v_foamPower))
	#ifdef LIGHTMAP
	 * (texture2D(lightmap, v_worldPos) * 1.3);
	#else
	;
	#endif // LIGHTMAP
	#endif // USE_FOAM
}
