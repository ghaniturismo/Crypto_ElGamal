/* ______________________________________________
  |                                              |
  | ----------- DM Init-Cryptographie -----------|
  | -- Implémentation de l'algorithme El Gamal --|
  | -- MAZROU Abdelghani ****** BENTOUNES Samy --|
  |______________________________________________|
*/
#define _GNU_SOURCE

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

gmp_randstate_t state;

/*
 * KeyGen() est la fonction qui génère les clés de Bob, elle prend en entrée/sortie sans les modifier
 * p et g, elle tire au hasard un x, la clé secrète de Bob et calcule sa clé publique correspondante
 * X ≡ g x mod p. Elle renvoie en sortie x et X.
*/
void keyGen(mpz_t p, mpz_t g, mpz_t grand_X, mpz_t x){

    mpz_t tmp;
    mpz_init(tmp);
    // tirer x au hasard 2 et p-2
    mpz_sub_ui(tmp, p, 2);

    gmp_randinit_default(state);

    gmp_randseed_ui(state, time(NULL)*7);
    mpz_urandomm(x, state, tmp);
    mpz_add_ui(x, x, 2);

    // calcule du grand X ≡ g^x mod p --> 1eme methode
    expMod(grand_X, g, x, p);
    
    // calcule du grand X ≡ g^x mod p --> 2eme methode
    // mpz_powm(grand_X, g, x, p);
}

/*
 * Encrypt() est la fonction qui produit en sortie le couple chiffré (C ≡ m × y mod p, B ≡
 * g^r mod p) en prenant en entrée la clé publique de Bob K p = (p, g, X) et un message m
 * avec y ≡ X^r mod p et r alea.
 */
void encrypt(mpz_t grand_C,mpz_t grand_B, mpz_t p, mpz_t g, mpz_t grand_X, mpz_t msg_chiffre, FILE* file_r) {
	
    mpz_t tmp, r, y;

    mpz_inits(tmp, r, y, NULL);

    // tirer r au hasard 2 et p-2
    mpz_sub_ui(tmp, p, 2);

    gmp_randinit_default(state);

    gmp_randseed_ui(state, time(NULL)*7);
    mpz_urandomm(r, state, tmp);
    mpz_add_ui(r, r, 2);

    char chaine[TAILLE_MAX] = "";
    char c[TAILLE_MAX] = "";

    // Mettre le r genere dans une chaine de caractere dans la variable c
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

	//------Calcul de grand_B (B = g^r mod p)
	expMod(grand_B, g, r, p);

	//------Calcul de grand_C (C ≡ m × y mod p)
	// m * y
	mpz_mul(grand_C, msg_chiffre, y);
	mpz_mod(grand_C, grand_C, p);

    // CLEAN
    mpz_clears(r,y, NULL);
}

/*
 * Decrypt() est la fonction de déchiffrement qui prend en entrée (C, B) et la clé secrète de Bob
 * Ks = x et produit en sortie le message m.
 */
void decrypt(mpz_t msg_dechiffre, mpz_t grand_C, mpz_t grand_B, mpz_t x, mpz_t p) {
    
    mpz_t u, v, grand_D;
    mpz_inits(u, v,grand_D, NULL);

	//------Calcul de grand_D (D = B^x mod p)
	expMod(grand_D, grand_B, x, p);

	//------Calcul de C*D^-1 mod p = m mod p --> d'ou le resultat de m
    // D^-1 on le trouve apres l appel de euclide qu'on trouve le resultat dans u.
    euclide(u, v, grand_D, p);

    //m = C × D^−1 mod p.
    mpz_mul(msg_dechiffre, grand_C, u);
    mpz_mod(msg_dechiffre, msg_dechiffre, p);

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
 * *********************************************** main ************************************************
 */
int main(int argc, char * argv[]){
    
    //--------File test.txt----------------/
    FILE* f = NULL;
    char *new_str;
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

    mpz_t m,m1,m2,m1m2;
    mpz_t grand_B_m1, grand_C_m1;
    mpz_t grand_B_m2, grand_C_m2;
    mpz_t b, c;
    
    mpz_inits(m,m1,m2,m1m2,grand_B_m1,grand_C_m1,grand_B_m2,grand_C_m2,b,c,NULL);

    mpz_set_ui(m1, 10102010);
    mpz_set_ui(m2, 20201030);
    
    // Initialisation de l'aleatoire (plus fiable en utilisant srand())
    srand(time(NULL));

    // initialisation de la borne minimum aleatoire
    int rand_min = rand();

    // ---------------------------------- Test des 5 occurences ----------------------------------
    int i;
    for( i=0; i<5; i++) {
    	fprintf(f,"\n--------------------------------TEST %d--------------------------------\n\n\n", i+1);
    	// initialisation de l etat, utile pour le random gmp
    	gmp_randinit_mt(state);
    	gmp_randseed_ui(state, time(NULL));
		mpz_init(msg_Entre);
		
        int message_int;
		message_int = rand_a_b(rand_min, RAND_MAX); //message aleatoire entre rand_min et RAND_MAX
		mpz_set_ui(msg_Entre, message_int);
		mpz_inits(x,grand_X, msg_Sortie,NULL);
		mpz_inits(grand_C, grand_B,NULL);
		
        // G initialization
		mpz_init(g);
		mpz_set_ui(g, 2);
		
        // P initialization
		mpz_init(p);
		mpz_set_str(p, P_VAL_HEXA, 16);

        // Avoir Kp=(p,g,X) et Ks=(x)
		keyGen(p,g,grand_X,x);
		
        // Ecriture dans le fichier test.txt de Kp et Ks.
		fprintf(f,"\n--------------------------------KeyGen---------------------------------\n");
		fprintf(f,"-----------------------------Cle publique Kp-----------------------------\n");
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
		decrypt(msg_Sortie,grand_C, grand_B, x,p);

		gmp_fprintf(f,"message sortie --> %Zd  \n\n", msg_Sortie);
	}

    fprintf(f,"\n*************************** QUESTION 6 ***************************************\n");
    keyGen(p,g,grand_X,x);
    gmp_fprintf(f, "m1 = %Zd \n", m1);
    gmp_fprintf(f, "m2 = %Zd \n", m2);

    // Chiffrement de m1 et m2
    encrypt(grand_C_m1, grand_B_m1, p, g, grand_X, m1, file_r);
    encrypt(grand_C_m2, grand_B_m2, p, g, grand_X, m2, file_r);

    // C = C1 × C2 mod p.
    mpz_mul(c, grand_C_m1, grand_C_m2);
    mpz_mod(c, c, p);
    
    // B = B1 × B2 mod p.
    mpz_mul(b, grand_B_m1, grand_B_m2);
    mpz_mod(b, b, p);

    // Dechiffrement
    decrypt(m,c, b, x,p);
    
    // m1m2 = m1 * m2
    mpz_mul(m1m2, m1, m2);
    // m mod p
    mpz_mod(m1m2, m1m2, p);

    // Verification de la propriété homomorphique d'ElGamal
    // Test si le message decrept de (m) et le meme avec m1m2 = m1*m2 mod p
    if (mpz_cmp(m1m2,m)==0)
    {
        gmp_fprintf(f,"\n (m , m1m2) sont egaux \n\n");            
        gmp_fprintf(f,"\n (m = %Zd , m1m2 = %Zd) egalite demontre \n",m,m1m2);
    }

	// Affichage des aléas r dans le fichier test.txt
	char chaine1[TAILLE_MAX] = "";
    fprintf(f, "\n\n\n\n---------------------LISTE DES ALEA R---------------------\n\n");
    if (f != NULL)
    {
        while (fgets(chaine1, TAILLE_MAX, file_r) != NULL) // On lit le fichier tant qu'on ne reçoit pas d'erreur (NULL)
        {
          fprintf(f,"\n%s\n", chaine1); //ecriture de chaque ligne du fichier r.txt dans test.txt
        }
    }

    printf("\n\n\n OK ! fichier test.txt genere  \n\n\n");
    
    // Fermeture des fichiers
	fclose(f);
	fclose(file_r);
    // Suppression du fichier r.txt
    //remove("r.txt");

    // CLEAN variable mpz
    mpz_clears(p,x,g,grand_X,grand_B,grand_C,m,m1,m2,m1m2,grand_B_m1,grand_C_m1,grand_B_m2,grand_C_m2,b,c,NULL);

	return 0;
}