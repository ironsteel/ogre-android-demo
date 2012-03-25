#version 100
precision highp int;
precision highp float;

uniform vec4 lightDiffuse;

uniform sampler2D textureMap;

varying vec2 oUv0;
varying vec3 oNormal;
varying vec3 oTSLightDir;


void main()
{
        vec3 lightVec = normalize(oTSLightDir).xyz;
        
        // Calculate dot product
        vec3 texColor = texture2D(textureMap, oUv0).xyz;
        gl_FragColor = vec4(texColor, 1.0) * lightDiffuse * dot(normalize(oNormal), lightVec);
        
}
