#version 410

layout(points) in;
layout(triangle_strip, max_vertices = 4) out;

uniform mat4 matrProj;
//uniform mat4 matrModel;
//uniform mat4 matrVisu;
uniform int texnumero;

layout (std140) uniform varsUnif
{
    float tempsDeVieMax;       // temps de vie maximal (en secondes)
    float temps;               // le temps courant dans la simulation (en secondes)
    float dt;                  // intervalle entre chaque affichage (en secondes)
    float gravite;             // gravité utilisée dans le calcul de la position de la particule
    float pointsize;           // taille des points (en pixels)
};

in Attribs {
    vec4 couleur;
    float tempsDeVieRestant;
    float sens; // du vol (partie 3)
    float hauteur; // de la particule dans le repère du monde (partie 3)
} AttribsIn[];

out Attribs {
    vec4 couleur;
    vec2 texCoord;
    float tempsDeVieRestant;
    int estInerte;
} AttribsOut;

// la hauteur minimale en-dessous de laquelle les lutins ne tournent plus (partie 3)
const float hauteurVol = 8.0;

void main()
{
    AttribsOut.estInerte = 1;
    if ( AttribsIn[0].hauteur > hauteurVol ) AttribsOut.estInerte = 0;
    
    vec2 decalage[4];
    decalage[0] = vec2( -0.5 * gl_in[0].gl_PointSize,  0.5 * gl_in[0].gl_PointSize);
    decalage[1] = vec2( -0.5 * gl_in[0].gl_PointSize, -0.5 * gl_in[0].gl_PointSize);
    decalage[2] = vec2(  0.5 * gl_in[0].gl_PointSize,  0.5 * gl_in[0].gl_PointSize);
    decalage[3] = vec2(  0.5 * gl_in[0].gl_PointSize, -0.5 * gl_in[0].gl_PointSize);

    vec2 texture[4];
    texture[0] = vec2(0.0, 1.0);
    texture[1] = vec2(0.0, 0.0);
    texture[2] = vec2(1.0, 1.0);
    texture[3] = vec2(1.0, 0.0);

    for ( int i = 0 ; i < 4 ; ++i ) {

        AttribsOut.texCoord = texture[i];
        vec4 transformPos = gl_in[0].gl_Position;
        // gl_Position = vec4(0.0, decalage[i].x, decalage[i].y, 0.0);
         if( texnumero == 1 ) decalage[i].x *= -AttribsIn[0].sens;

        // assigner la position du point
        gl_Position = matrProj * vec4(transformPos.x + decalage[i].x, transformPos.y + decalage[i].y, transformPos.z, 1.0);

        // assigner la taille des points (en pixels)
        gl_PointSize = gl_in[0].gl_PointSize;

        // assigner la couleur courante
        AttribsOut.couleur =  AttribsIn[0].couleur;
        if (texnumero == 1) AttribsOut.couleur *=  (AttribsIn[0].tempsDeVieRestant / tempsDeVieMax);

        AttribsOut.tempsDeVieRestant = AttribsIn[0].tempsDeVieRestant;
        EmitVertex();

        

    }

}
