#version 410
layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

uniform mat4 matrModel;
uniform mat4 matrVisu;
uniform mat4 matrProj;

in Attribs {
    vec4 couleur;
} AttribsIn[];

out Attribs {
    vec4 couleur;
    vec3 lumiDir;
    vec3 obsVec;
    vec3 normale;
} AttribsOut;

const vec3 lumiDir = vec3( 0, 0, 1 );
const vec3 obsVec = vec3( 0, 0, 1 );

void main(){
    vec4 arete1 = gl_in[1].gl_Position - gl_in[0].gl_Position;
    vec4 arete2 = gl_in[2].gl_Position - gl_in[0].gl_Position;
    vec3 normale = cross(arete1.xyz, arete2.xyz);
    for ( int i = 0 ; i < gl_in.length() ; ++i ){
        gl_Position = matrProj * gl_in[i].gl_Position;
        AttribsOut.couleur = AttribsIn[i].couleur;
        AttribsOut.lumiDir = lumiDir;
        AttribsOut.obsVec = obsVec;
        AttribsOut.normale = normale;
        EmitVertex();
    }
}