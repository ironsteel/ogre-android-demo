precision mediump float;

uniform vec3  BrickColor, MortarColor;
uniform vec2  BrickSize;
uniform vec2  BrickPct;

varying vec2  MCposition;
varying float LightIntensity;


void main()
{
    vec3  color;
    vec2  position, useBrick;
    
    position = MCposition / BrickSize;

    if (fract(position.y * 0.5) > 0.5)
        position.x += 0.5;

    position = fract(position);

    useBrick = step(position, BrickPct);

    //color  = mix(MortarColor, BrickColor, useBrick.x * useBrick.y);
    color = vec3(0.0, 1.0, 0.0);
    //color *= LightIntensity;
    gl_FragColor = vec4(color, 1.0);
}
