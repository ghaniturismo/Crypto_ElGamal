#define _GNU_SOURCE

#include "gmp.h"
#include "main.h"
#include "euclide.h"
#include "expmod.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <limits.h>
#include <time.h>
#include <string.h>

#define TAILLE_MAX 1024
//#define RAND_MAX 32767

gmp_randstate_t state;

//Variable reprensentant p
mpz_t p_global;

/*
 * KeyGen() est la fonction qui génère les clés de Bob, elle prend en entrée/sortie sans les modifier
 * p et g, elle tire au hasard un x, la clé secrète de Bob et calcule sa clé publique correspondante
 * X ≡ g x mod p. Elle renvoie en sortie x et X.
*/
void keyGen(mpz_t p, mpz_t g, mpz_t grand_X, mpz_t x){
           
    // Generation de la cle privée x
    mpz_urandomm(x, state, p);
    mpz_add_ui(x, x, 2);
    
    // calcule du grand X ≡ g^x mod p --> 1eme methode
    expMod(grand_X, g, x, p);

    // calcule du grand X ≡ g^x mod p --> 2eme methode
        // mpz_powm(grand_X, g, x, p);   
        // gmp_printf("===publicKeyOfA grand_X ===\n%Zd\n", grand_X);
}

/*
 * Encrypt() est la fonction qui produit en sortie le couple chiffré (C ≡ m × y mod p, B ≡
 * g^r mod p) en prenant en entrée la clé publique de Bob K p = (p, g, X) et un message m
 * avec y ≡ X^r mod p et r alea.
 */
void encrypt(mpz_t grand_C,mpz_t grand_B, mpz_t p, mpz_t g, mpz_t grand_X, mpz_t msg_chiffre, FILE* file_r) {
	
    // Initialisation des variables
	mpz_t r, y;
    mpz_inits(r, y, NULL);

    char chaine[TAILLE_MAX] = "";
    char c[TAILLE_MAX] = "";

    // generation de r aleatoire 
    mpz_urandomm(r, state, p);

    //mettre le r genere dans une chaine de caractere dans la variable c
    mpz_get_str(c,10,r);
 
    // Ouverture du fichier r.txt
    file_r = fopen("r.txt", "a+");
    int k;
    if (file_r != NULL)
    {
        while (fgets(chaine, TAILLE_MAX, file_r) != NULL) // On lit le fichier tant qu'on ne reçoit pas d'erreur (NULL)
        {
            k = strcmp(chaine,c);
            if(k == 10){ // Les deux chaines sont egaux
                mpz_urandomm(r, state, p);
                mpz_get_str(c,10,r);
                rewind(file_r);//: remet le curseur au début du fichier    
            }
        }
        gmp_fprintf(file_r,"%Zd\n", r); 
    }
    
    // Fermeture du fichier
    fclose(file_r);

	//------Calcul de y (y = X^r mod p)
	expMod(y, grand_X, r, p);
	//gmp_printf("y : \n%Zd", y);

	//------Calcul de grand_B (B = g^r mod p)
	expMod(grand_B, g, r, p);
	//gmp_printf("\nB : \n%Zd", grand_B);

	//------Calcul de grand_C (C ≡ m × y mod p)
	//m * y
	mpz_mul(msg_chiffre, msg_chiffre, y);
	mpz_mod(grand_C, msg_chiffre, p);

    // CLEAN
    mpz_clears(r,y,state, NULL);
}

/*
 * Decrypt() est la fonction de déchiffrement qui prend en entrée (C, B) et la clé secrète de Bob
 * Ks = x et produit en sortie le message m.
 */
void decrypt(mpz_t msg_dechiffre, mpz_t grand_C, mpz_t grand_B, mpz_t x) {
    
    mpz_t u, v, grand_D;
    mpz_inits(u, v,grand_D, NULL);

	//------Calcul de grand_D (D = B^x mod p)
	expMod(grand_D, grand_B, x, p_global);

	//------Calcul de C*D^-1 mod p = m mod p --> d'ou le resultat de m
    // D^-1 on le trouve apres l appel de euclide qu'on trouve le resultat dans u.
    euclide(u, v, grand_D, p_global);

    //m = C × D^−1 mod p.
    mpz_mul(msg_dechiffre, grand_C, u);
    mpz_mod(msg_dechiffre, msg_dechiffre, p_global);

    // CLEAN
    mpz_clears(u,v,grand_D,NULL);
}


/*
 * Méthode renvoyant un int aléatoire compris entre a et b
 * On suppose a<b
 */
int rand_a_b(int a, int b) {
	int res;
	res = rand()%(b-a)+a;
	return res;
}

/*
 *  main
 */
int main(int argc, char * argv[]){
    
    //--------File test.txt----------------/
    FILE* f = NULL;
    char *new_str;
    //char *new_str1;
    asprintf(&new_str,"%s","test.txt");
    f = fopen(new_str, "r");
    remove("test.txt");
    if (f == NULL)
    {
        f = fopen(new_str, "a+");
    }
    else
    {
        fclose(f);
        f = fopen(new_str, "a+");
    }

    //-----File r.txt pour stocke les r generé----------------/
    FILE* file_r = NULL;
    char *new_str2;
    asprintf(&new_str2,"%s","r.txt");
    file_r = fopen(new_str2, "r");
    if (file_r == NULL)
    {
        file_r = fopen(new_str2, "a+");
    }
    else
    {
        fclose(file_r);
        file_r = fopen(new_str2, "a+");
    }

    // Declaration des variables mpz
    mpz_t g,p;
    mpz_t x,grand_X;
    mpz_t grand_B, grand_C;
    mpz_t msg_Entre,msg_Sortie;

    // Initialisation de l'aleatoire (plus fiable en utilisant srand())
    srand(time(NULL));

    // initialisation de la borne minimum aleatoire
    int rand_min = rand();

    // Test des 5 occurences
    // boucle sur i
    int i;
    for( i=0; i<5; i++) {
    	fprintf(f,"\n---------------------------TEST %d---------------------------\n\n\n", i+1);
    	// initialisation de l etat, utile pour le random gmp
    	gmp_randinit_mt(state);
    	gmp_randseed_ui(state, time(NULL));
		mpz_init(msg_Entre);
		int message_int;
		message_int = rand_a_b(rand_min, RAND_MAX); //message aleatoire entre rand_min et RAND_MAX
		mpz_set_ui(msg_Entre, message_int);
				
        //gmp_printf("\nmessage entre  --> %Zd\n",msg_Entre);
		mpz_inits(x,grand_X, msg_Sortie,NULL);
		mpz_inits(grand_C, grand_B,NULL);
		
        // G initialization
		mpz_init(g);
		mpz_set_ui(g, 2);
		
        // P initialization
		mpz_init(p);
		mpz_set_str(p, P_VAL_HEXA, 16);
		
        // Affectation de la valeur de p a p_global
		mpz_set(p_global, p); 
		
        // Avoir Kp=(p,g,X), Ks=(x)
		keyGen(p,g,grand_X,x);
		
        // Ecriture dans le fichier de Kp et Ks
		fprintf(f,"------------------------KeyGen------------------------\n");
		fprintf(f,"--------------------Cle publique Kp--------------------\n");
		gmp_fprintf(f,"p = \n%Zd  \n\n", p);
		gmp_fprintf(f,"g = %Zd  \n\n", g);
		gmp_fprintf(f,"X = \n%Zd  \n\n", grand_X);
		fprintf(f,"---------------------Cle privee Ks---------------------\n");
		gmp_fprintf(f,"x = \n%Zd  \n\n", x);
		fprintf(f,"----------------Chiffrement du message----------------\n");
		gmp_fprintf(f,"message entre  --> %Zd\n\n", msg_Entre);
        
        // Chiffrement
		encrypt(grand_C, grand_B, p, g, grand_X, msg_Entre, file_r);

		fprintf(f,"---------------Dechiffrement du message---------------\n");
        // Dechiffrement
		decrypt(msg_Sortie,grand_C, grand_B, x);

		gmp_fprintf(f,"message sortie --> %Zd  \n\n", msg_Sortie);
		//gmp_printf("message sorite --> %Zd  \n\n", msg_Sortie);
	}


	// Affichage des aléas r dans le fichier test.txt
	char chaine1[TAILLE_MAX] = "";
    fprintf(f, "\n\n\n\n---------------------LISTE DES ALEA R---------------------\n\n");
    if (f != NULL)
    {
        while (fgets(chaine1, TAILLE_MAX, file_r) != NULL) // On lit le fichier tant qu'on ne reçoit pas d'erreur (NULL)
        {
          fprintf(f,"\n%s\n", chaine1); //affichage de chaque ligne du fichier r.txt dans test.txt
        }
    }

    printf("\n\n\n fichier test.txt genere  \n\n\n");
    // Fermeture des fichiers
	fclose(f);
	fclose(file_r);

    // CLEAN variable mpz
    mpz_clears(p,x,g,p_global,grand_X,grand_B,grand_C,NULL);
	return 0;
}