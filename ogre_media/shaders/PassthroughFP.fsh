#version 100

precision mediump int;
precision mediump float;
uniform sampler2D textureMap;


varying vec2 oUv0;



/*
  Basic ambient lighting fragment program for GLSL ES
*/
void main()
{
	vec3 texColor = texture2D(textureMap, oUv0).xyz;
	gl_FragColor = vec4(texColor, 1.0);
}
