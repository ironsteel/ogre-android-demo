#version 100
precision mediump int;
precision mediump float;


// parameters
uniform vec4 lightPosition;
uniform mat4 worldViewProj;
uniform mat4 inverseTransposeWV;

attribute vec4 position;
attribute vec3 normal;
attribute vec2 uv0;

varying vec2 oUv0;

varying vec4 lightPos;
varying vec3 oNormal;
varying vec4 oPosition;

void main()
{	
	gl_Position = worldViewProj * position;
	
	oUv0 = uv0;

	oNormal =  normal;
        
        oPosition = position;
	
	lightPos = lightPosition;
}
