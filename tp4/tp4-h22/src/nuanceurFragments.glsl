#version 410

uniform sampler2D leLutin;
uniform int texnumero;

in Attribs {
    vec4 couleur;
    vec2 texCoord;
    float tempsDeVieRestant;
} AttribsIn;

out vec4 FragColor;

void main( void )
{
    FragColor = AttribsIn.couleur;
    if( texnumero != 0 ) {
        vec2 currentTex = AttribsIn.texCoord;
        if( texnumero == 1 ) {
            const float nlutins = 20.0; // 20 positions de vol dans la texture
            int num = int ( mod ( 12.0 * AttribsIn.tempsDeVieRestant , nlutins ) ); // 12 Hz
            currentTex.s = ( currentTex.s + num ) / nlutins ;
        }

        //else if ( texnumero == 2 ) {
            //mat2 rotation;
            //float theta = 4.0 * AttribsIn.tempsDeVieRestant;
            //rotation[0] = vec2(cos(theta), sin(theta));
            //rotation[1] = vec2(-sin(theta), cos(theta));

            // currentTex -= vec2(0.5); // careful 
            //currentTex = rotation * currentTex;
            // currentTex += vec2(0.5);

        //}

        vec4 texColor = texture(leLutin, currentTex);
        if( texColor.a < 0.1) discard;
        FragColor = vec4(mix( AttribsIn.couleur.rgb, texColor.rgb, 0.6), AttribsIn.couleur.a);
    }
}
