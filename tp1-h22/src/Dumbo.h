#ifndef __DUMBO_H__
#define __DUMBO_H__

// les formes
FormeCube *cube = NULL;
FormeQuad *quad = NULL;
FormeSphere *sphere = NULL;
FormeCylindre *cylindre = NULL;
// (partie 1) Vous devez vous servir des quatre fonctions ci-dessous (*sans les modifier*) pour tracer tous les parties des objets.
// affiche un cylindre de rayon 1.0 et de longueur 1.0, dont la base est centrée en (0,0,0)
void afficherCylindre( ) { cylindre->afficher(); }
// affiche une sphère de rayon 1.0, centrée en (0,0,0)
void afficherSphere( ) { sphere->afficher(); }
// affiche un cube d'arête 1.0, centrée en (0,0,0)
void afficherCube( ) { cube->afficher(); }
// affiche un quad d'arête 1
void afficherQuad( ) { quad->afficher(); }

// affiche la position courante du repère (pour débogage)
void afficherRepereCourant( int num = 0 )
{
    glUniformMatrix4fv( locmatrModel, 1, GL_FALSE, matrModel );
    FenetreTP::afficherAxes( 1.5, 3.0 );
}

// partie 1: Dumbo
class Dumbo
{
public:
    Dumbo()
    {
        initVar();
        // créer un Dumbo graphique
        initialiserGraphique();

        // créer quelques autres formes
        cube = new FormeCube( 1.0, true );
        quad = new FormeQuad( 1.0, true );
        sphere = new FormeSphere( 1.0, 8, 8, true );
        cylindre = new FormeCylindre( 1.0, 1.0, 1.0, 16, 1, true );
    }
    ~Dumbo()
    {
        conclureGraphique();
        delete cube;
        delete quad;
        delete sphere;
        delete cylindre;
    }

    void initVar() { position = glm::vec3(0.25*Etat::dimBoite, 0.0, 0.0); taille = 1; angleCorps = 0.0; angleRotation = 70.0; }
    void verifierAngles() // vérifier que les angles ne débordent pas les valeurs permises
    {
        if ( angleRotation > 90.0 ) angleRotation = 90.0; else if ( angleRotation < 45.0 ) angleRotation = 45.0;
    }

    void initialiserGraphique()
    {
        GLint prog = 0; glGetIntegerv( GL_CURRENT_PROGRAM, &prog );
        if ( prog <= 0 )
        {
            std::cerr << "Pas de programme actif!" << std::endl;
            locVertex = locColor = -1;
            return;
        }
        if ( ( locVertex = glGetAttribLocation( prog, "Vertex" ) ) == -1 ) std::cerr << "!!! pas trouvé la \"Location\" de Vertex" << std::endl;
        if ( ( locColor = glGetAttribLocation( prog, "Color" ) ) == -1 ) std::cerr << "!!! pas trouvé la \"Location\" de Color" << std::endl;

        // allouer les objets OpenGL
        glGenVertexArrays( 1, &vao );

        // initialiser le VAO pour la théière
        glBindVertexArray( vao );

        // (partie 2) MODIFICATIONS ICI ...
        // créer le VBO pour les sommets
        glGenBuffers(1, &vboTheiereSommets);
        glBindBuffer(GL_ARRAY_BUFFER, vboTheiereSommets);
        glBufferData(GL_ARRAY_BUFFER, sizeof(gTheiereSommets), gTheiereSommets, GL_STATIC_DRAW); 
        glVertexAttribPointer(locVertex, 3, GL_FLOAT, GL_FALSE, 0, 0);
        glEnableVertexAttribArray(locVertex);

        // créer le VBO la connectivité
        glGenBuffers(1, &vboTheiereConnec);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboTheiereConnec);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(gTheiereConnec), gTheiereConnec, GL_STATIC_DRAW);
        glBindVertexArray(0);
    }

    void conclureGraphique()
    {
        glDeleteBuffers( 1, &vboTheiereSommets );
        glDeleteBuffers( 1, &vboTheiereConnec );
    }

    // (partie 2) Vous modifierez cette fonction pour utiliser les VBO
    // affiche une théière, dont la base est centrée en (0,0,0)
    void afficherTheiere()
    {
        glBindVertexArray( vao );
        glBindBuffer(GL_ARRAY_BUFFER, vboTheiereSommets);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vboTheiereConnec); 
       
        glDrawElements(GL_TRIANGLES, sizeof(gTheiereConnec) / sizeof(GLuint), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }

    // afficher la tête et le corps de l'éléphant.
    // - Le tête est un cube de base : facteurs d'échelle = ( 1.0, 1.0, 1.0 ).
    //   Pour la partie 2, la taille de la théière sera modulée par ( 0.25, 0.25, 0.25 ).
    //   Il vous faudra inverser la théière afin que le bec verseur pointe vers le bas.
    // - Le bas du cube tête est déplacé à l'origine.
    // - À gauche de la tête se trouve le corps, représenté par un cube vertical de taille ( 2.0, 1.0, 1.0 ).
    //   Le bas de la tête correspond au centre du corps selon y
    //   Les deux faces voisines de la tête et du corps sont collées
    void afficherCorps()
    {
        glVertexAttrib3f(locColor, 0.5, 0.5, 0.5); 
        //Afficher la boite
        // afficher le bon modèle
        switch (Etat::modele)
        {
        default:
        case 1: // la tête (cube)
            matrModel.PushMatrix(); {
                matrModel.Translate(0.0, 0.5, 0.0 );
                glUniformMatrix4fv(locmatrModel, 1, GL_FALSE, matrModel);
                afficherCube();
            }matrModel.PopMatrix(); glUniformMatrix4fv(locmatrModel, 1, GL_FALSE, matrModel);
            break;

        case 2: // la théière
            matrModel.PushMatrix(); {
                matrModel.Translate(0.0, 1.0, 0.0);
                matrModel.Rotate(180, 1.0, 0.0, 0.0);
                matrModel.Scale(0.25, 0.25, 0.25);
                glUniformMatrix4fv(locmatrModel, 1, GL_FALSE, matrModel);
                afficherTheiere();
            }matrModel.PopMatrix(); glUniformMatrix4fv(locmatrModel, 1, GL_FALSE, matrModel);
            break;
        }

        // et son corps
        glVertexAttrib3f(locColor, 0.4, 0.4, 0.4);
        matrModel.PushMatrix(); {
            matrModel.Translate(-1.5, 0.0, 0.0);
            matrModel.Scale(2.0, 1.0, 1.0);
            glUniformMatrix4fv(locmatrModel, 1, GL_FALSE, matrModel);
            afficherCube();
        }matrModel.PopMatrix(); glUniformMatrix4fv(locmatrModel, 1, GL_FALSE, matrModel);
    }

    // Afficher les oreilles
    // Une oreille est un quadrilatère de longueur « 3.0 » et de même largeur que la tête.
    // - Elles sont positionnées au 3 / 4 de la hauteur de la tête.
    // - L'angle de rotation des oreilles est donné par la variable angleRotation.
    void afficherOreille()
    {
        // ajouter une ou des transformations afin de tracer des oreilles
        glVertexAttrib3f(locColor, 0.6, 0.6, 0.6);
        matrModel.PushMatrix(); {

            // créer la première oreille
            matrModel.Translate(-0.5, 0.75, 0.5);
            matrModel.Rotate(180, 1.0, 0.0, 0.0);
            matrModel.Rotate(-angleRotation, 1.0, 0.0, 0.0);
            matrModel.Scale(1.0, 3.0, 1.0);
            glUniformMatrix4fv(locmatrModel, 1, GL_FALSE, matrModel);
            afficherQuad();


        }matrModel.PopMatrix(); glUniformMatrix4fv(locmatrModel, 1, GL_FALSE, matrModel);

        matrModel.PushMatrix(); {
            // créer la seconde oreille
            matrModel.Translate(-0.5, 0.75, -0.5);
            matrModel.Rotate(angleRotation, 1.0, 0.0, 0.0);
            matrModel.Rotate(180, 1.0, 0.0, 0.0);
            matrModel.Scale(1.0, 3.0, 1.0);
            glUniformMatrix4fv(locmatrModel, 1, GL_FALSE, matrModel);
            afficherQuad();

        }matrModel.PopMatrix(); glUniformMatrix4fv(locmatrModel, 1, GL_FALSE, matrModel);
    }

    // afficher les pattes
    // Les pattes sont des cylindres de largeur « largMembre » et longueur « longMembre ».
    // Les pattes sont attachées en bas de la boite du corps de l'éléphant à « anglePatte » degrés d'écart.
    void afficherPattes()
    {
        // donner la couleur des pattes
        glVertexAttrib3f(locColor, 0.5, 0.5, 0.5);

        // ajouter une ou des transformations afin de tracer chacune des pattes
        matrModel.PushMatrix();{
            matrModel.Translate( -0.5, -0.5, 0.5 );
            matrModel.Rotate(45, 0.0, 1.0, 0.0);
            matrModel.Rotate(angleRotation, 1.0, 0.0, 0.0);
            matrModel.Scale(largMembre, largMembre, longMembre);
            glUniformMatrix4fv( locmatrModel, 1, GL_FALSE, matrModel );
            afficherCylindre();
        }matrModel.PopMatrix(); glUniformMatrix4fv(locmatrModel, 1, GL_FALSE, matrModel);

        matrModel.PushMatrix(); {
            matrModel.Translate(-0.5, -0.5, -0.5);
            matrModel.Rotate(180 - 45, 0.0, 1.0, 0.0);
            matrModel.Rotate(angleRotation, 1.0, 0.0, 0.0);
            matrModel.Scale(largMembre, largMembre, longMembre);
            glUniformMatrix4fv(locmatrModel, 1, GL_FALSE, matrModel);
            afficherCylindre();
        }matrModel.PopMatrix(); glUniformMatrix4fv(locmatrModel, 1, GL_FALSE, matrModel);

        matrModel.PushMatrix(); {
            matrModel.Translate(-2.5, -0.5, 0.5);
            matrModel.Rotate(-45, 0.0, 1.0, 0.0);
            matrModel.Rotate(angleRotation, 1.0, 0.0, 0.0);
            matrModel.Scale(largMembre, largMembre, longMembre);
            glUniformMatrix4fv(locmatrModel, 1, GL_FALSE, matrModel);
            afficherCylindre();
        }matrModel.PopMatrix(); glUniformMatrix4fv(locmatrModel, 1, GL_FALSE, matrModel);

        matrModel.PushMatrix(); {
            matrModel.Translate(-2.5, -0.5, -0.5);
            matrModel.Rotate(45 - 180, 0.0, 1.0, 0.0);
            matrModel.Rotate(angleRotation, 1.0, 0.0, 0.0);
            matrModel.Scale(largMembre, largMembre, longMembre);
            glUniformMatrix4fv(locmatrModel, 1, GL_FALSE, matrModel);
            afficherCylindre();
        }matrModel.PopMatrix(); glUniformMatrix4fv(locmatrModel, 1, GL_FALSE, matrModel);
    }

    void afficher()
    {
        matrModel.PushMatrix();{ // sauvegarder la transformation courante

            // transformations Dumbo
            matrModel.Translate(position.x, position.y, position.z);
            matrModel.Rotate(angleCorps, 0.0, 1.0, 0.0);
            matrModel.Scale(taille, taille, taille);

            // afficher le corps
            afficherCorps();

            // afficher le chapeau
            afficherOreille();

            // afficher les quatre pattes
            afficherPattes();

        }matrModel.PopMatrix(); glUniformMatrix4fv( locmatrModel, 1, GL_FALSE, matrModel );
        glUniformMatrix4fv( locmatrModel, 1, GL_FALSE, matrModel ); // informer ...
    }

    void calculerPhysique()
    {
        if ( Etat::enmouvement )
        {
            static int sens[6] = { +1, +1, +1, +1, +1, +1 };
            glm::vec3 vitesse( 0.03, 0.02, 0.05 );
            // mouvement en X
            if ( position.x-taille <= -0.5*Etat::dimBoite ) sens[0] = +1;
            else if ( position.x+taille >= 0.5*Etat::dimBoite ) sens[0] = -1;
            position.x += 60*Etat::dt * vitesse.x * sens[0];
            // mouvement en Y
            if ( position.y-taille <= 0.0 ) sens[2] = +1;
            else if ( position.y+taille >= Etat::dimBoite ) sens[2] = -1;
            position.y += 60*Etat::dt * vitesse.y * sens[2];
            // mouvement en Z
            if ( position.z-taille <= -0.5*Etat::dimBoite ) sens[1] = +1;
            else if ( position.z+taille >= 0.5*Etat::dimBoite ) sens[1] = -1;
            position.z += 60*Etat::dt * vitesse.z * sens[1];

            // rotation du corps
            if ( angleCorps > 360.0 ) angleCorps -= 360.0;
            angleCorps += 60*Etat::dt * 0.35;

            // angle des pattes et des oreilles
            if ( angleRotation <= 45.0 ) sens[3] = +1;
            else if ( angleRotation >= 90.0 ) sens[3] = -1;
            angleRotation += 60*Etat::dt * 1.0 * sens[3];

            // taille du corps
            if ( taille <= 0.25 ) sens[5] = +1;
            else if ( taille >= 1.0 ) sens[5] = -1;
            taille += 60*Etat::dt * 0.005 * sens[5];
        }
    }

    // partie 2: utilisation de vbo et vao
    GLuint vao = 0;
    GLuint vboTheiereSommets = 0;
    GLuint vboTheiereConnec = 0;
    GLint locVertex = -1;
    GLint locColor = -1;

    glm::vec3 position;       // position courante de Dumbo
    GLfloat taille;           // facteur d'échelle pour la taille globale du modèle
    GLfloat angleCorps;       // angle de rotation (en degrés) du corps de Dumbo
    GLfloat angleRotation;       // angle de rotation (en degrés) des pattes et des oreilles
    const GLfloat longMembre = 0.7;  // longueur des membres
    const GLfloat largMembre = 0.1;  // largeur des membres
};

#endif
