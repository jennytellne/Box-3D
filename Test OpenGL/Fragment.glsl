#version 330 core
uniform float time;
uniform mat4 M2;
out vec4 finalcolor;
in vec3 interpolatedColor;
in vec3 interpolatedNormal;
in vec2 st;

void main () {
    vec3 V = vec3(0 ,0 ,1); // in view space
    float n = (2.0);//" shininess " parameter
    vec3 ka = vec3(0.2, 0.2, 0.2); //ambient reflection color
    vec3 Ia = vec3(0.5, 0.5, 0.5);//ambient illumination color
    vec3 kd = vec3(0.7, 0.7, 0.7);//diffuse surface reflection color
    vec3 Id = vec3(1.0, 0.1, 0.6);//diffuse illumination color
    vec3 ks = vec3(1.0, 1.0, 1.0);//specular surface reflection color
    vec3 Is = vec3(1.0, 1.0, 1.0);//specular illumination color
    //This assumes that N, L and V are normalized .
    vec3 L = normalize(vec3(1.0 , 1.0 , 1.0)); //Lightdirection
    vec3 R = 2.0* dot (interpolatedNormal ,L) *interpolatedNormal - L; // Could also have used the function reflect ()
    float dotNL = max ( dot (interpolatedNormal , L) , 0.0) ; // If negative , set to zero
    float dotRV = max ( dot (R , V) , 0.0) ;
    if (dotNL == 0.0) dotRV = 0.0; // Do not show highlight on the dark side
    vec3 shadedcolor = Ia * ka + Id * kd* dotNL + Is*ks*pow(dotRV , n);
    finalcolor = vec4 ( shadedcolor , 1.0);
}
