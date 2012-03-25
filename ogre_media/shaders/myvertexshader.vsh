attribute vec4 position;
attribute vec4 normal; 

uniform mat4 MVmatrix; 
uniform mat4 MVPmatrix; 
uniform mat4 Normalmatrix; 

uniform vec3 LightPosition;

const float SpecularContribution = 0.3;
const float DiffuseContribution  = 1.0 - SpecularContribution;

varying float LightIntensity;  //changed
varying vec2  MCposition; //changed

void main()
{
    vec3 ecPosition = vec3(MVmatrix * position);
 //   vec3 newnormal  = vec3(normal); //this has been introduced 
//    mat3 newNormalmatrix = mat3(Normalmatrix);// this has been introuced
    vec3 tnorm      = vec3(normalize(Normalmatrix * normal)); //this has changed
    vec3 lightVec   = normalize(LightPosition - ecPosition);
    vec3 reflectVec = reflect(-lightVec, tnorm);
    vec3 viewVec    = normalize(-ecPosition);
    float diffuse   = max(dot(lightVec, tnorm), 0.0);
    float spec      = 0.0;

    if (diffuse > 0.0)
    {
        spec = max(dot(reflectVec, viewVec), 0.0);
        spec = pow(spec, 16.0);
    }

    LightIntensity  = DiffuseContribution * diffuse * 2.0 + SpecularContribution * spec + 0.1;
    //LightIntensity  = DiffuseContribution + SpecularContribution * spec + 0.1;

    MCposition      = position.xy;
    gl_Position     = MVPmatrix * position;
    
}
