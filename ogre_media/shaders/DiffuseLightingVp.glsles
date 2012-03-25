#version 100
precision highp int;
precision highp float;


// parameters
uniform vec4 lightPosition;
uniform mat4 worldViewProj;
uniform mat4 inverseTransposeWV;

attribute vec4 position;
attribute vec3 normal;
attribute vec2 uv0;

varying vec2 oUv0;

varying vec3 oTSLightDir;
varying vec3 oNormal;

void main()
{	
	gl_Position = worldViewProj * position;
	
	oUv0 = uv0;

	mat3 normalMatrix = mat3(vec3(1.0, 0.0, 0.0), 
				 vec3(0.0, 1.0, 0.0),
				 vec3(0.0, 0.0, 1.0));
                                 
	
	oNormal = normalMatrix * normal;
	vec3 lightDir = lightPosition.xyz - (position * lightPosition.w).xyz;

	oTSLightDir = lightDir;
}
