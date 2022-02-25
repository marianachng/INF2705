#version 410

////////////////////////////////////////////////////////////////////////////////

// Définition des paramètres des sources de lumière
layout (std140) uniform LightSourceParameters
{
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
    vec4 position;      // dans le repère du monde
    vec3 spotDirection; // dans le repère du monde
    float spotExponent;
    float spotAngleOuverture; // ([0.0,90.0] ou 180.0)
    float constantAttenuation;
    float linearAttenuation;
    float quadraticAttenuation;
} LightSource;

// Définition des paramètres des matériaux
layout (std140) uniform MaterialParameters
{
    vec4 emission;
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
    float shininess;
} FrontMaterial;

// Définition des paramètres globaux du modèle de lumière
layout (std140) uniform LightModelParameters
{
    vec4 ambient;       // couleur ambiante
    bool localViewer;   // observateur local ou à l'infini?
    bool twoSide;       // éclairage sur les deux côtés ou un seul?
} LightModel;

////////////////////////////////////////////////////////////////////////////////

uniform int illumination; // on veut calculer l'illumination ?

const bool utiliseBlinn = true;

in Attribs {
    vec4 couleur;
    vec3 lumiDir;
    vec3 obsVec;
    vec3 normale;
} AttribsIn;

out vec4 FragColor;

vec4 lumiereReflexion(in vec3 normale, in vec3 sourceLumiere, in vec3 observateur){
    vec4 couleur = vec4(0);

    // ambiant sourceLumiere
    couleur += FrontMaterial.ambient * LightSource.ambient;

    float produitScalaire = dot(normale, sourceLumiere);
    if(produitScalaire > 0.0){
        // diffuse
        couleur += AttribsIn.couleur * LightSource.diffuse * produitScalaire;

        // speculaire
        float phongSpec = dot(reflect(-sourceLumiere, normale), observateur);
        if(phongSpec > 0) couleur += FrontMaterial.specular * LightSource.specular * pow(phongSpec, FrontMaterial.shininess);
    }

    return couleur;
}

void main( void )
{
    if(illumination == 0){
       FragColor = AttribsIn.couleur;
       return;
    }
    vec3 sourceLumineuse = normalize(AttribsIn.lumiDir);
    vec3 normale = normalize(gl_FrontFacing?AttribsIn.normale:-AttribsIn.normale);
    vec3 observateur = normalize(AttribsIn.obsVec);

    // ambiante modele
    vec4 couleur = FrontMaterial.emission + FrontMaterial.ambient * LightModel.ambient;

    couleur += lumiereReflexion(normale, sourceLumineuse, observateur);

    FragColor = clamp(couleur, 0.0, 1.0);
}
