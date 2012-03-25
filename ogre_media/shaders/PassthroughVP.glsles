#version 100

precision mediump int;
precision mediump float;

uniform mat4 worldViewProj;
attribute vec4 position;
attribute vec2 uv0;

varying vec2 oUv0;


void main()																					
{																							
	//Transform the vertex (ModelViewProj matrix)											
	gl_Position = worldViewProj * position;
	oUv0 = uv0;
}
