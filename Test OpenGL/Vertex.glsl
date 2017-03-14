#version 330 core
uniform mat4 MV;
uniform mat4 P;
uniform mat4 M2;
out vec2 st;
out vec3 interpolatedNormal;
layout(location=0)in vec3 Position;
layout(location=1)in vec3 Normal;
layout(location=2) in vec2 TexCoord;
void main() {
    gl_Position = P*MV*M2*vec4(Position, 1.0); // Special , required output
    vec3 transformedNormal = mat3(MV)*Normal;
    interpolatedNormal = normalize(transformedNormal);
    st = TexCoord; // Will also be interpolated across the triangle
    
    
}