#version 410
layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

in Attribs {
    vec4 couleur;
} AttribsIn[];

out Attribs {
    vec4 couleur;
} AttribsOut;

void main(){
    for ( int i = 0 ; i < gl_in.length() ; ++i ){
        gl_Position = gl_in[i].gl_Position;
        AttribsOut.couleur = AttribsIn[i].couleur;
        EmitVertex();
    }
}