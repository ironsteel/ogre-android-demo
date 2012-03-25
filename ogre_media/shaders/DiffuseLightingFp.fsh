#version 100
precision mediump int;
precision mediump float;

uniform vec4 lightDiffuse;

uniform sampler2D textureMap;

varying vec2 oUv0;

varying vec4 lightPos;
varying vec3 oNormal;
varying vec4 oPosition;


void main()
{
        vec3 lightDir = normalize(lightPos.xyz -  (oPosition * lightPos.w).xyz);
        vec3 texColor = texture2D(textureMap, oUv0).xyz;
        gl_FragColor = vec4(texColor, 1.0) * dot(normalize(oNormal), lightDir) * lightDiffuse;
}
