
#include "gmp.h"
#include "main.h"
#include "euclide.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <limits.h>
#include <time.h>

#define WORD_BIT (sizeof(int) * CHAR_BIT)

gmp_randstate_t state;
gmp_randstate_t generator;

//Declaration des variables globales
//Variable representant la derniere valeur de u
mpz_t uFinal;
//Variable representant la derniere valeur de v
mpz_t vFinal;
//Variable reprensentant p
mpz_t p_global;

//Variables recuperant les sorties des fonctions encrypt() et decrypt()
mpz_t grand_B, grand_C, grand_D;

/*
 * Calcule de A ≡ g^a mod p de la question 4
 * stockage du resultat de g^a mod p dans  la variable res 
 * initialiser r avant l'appel à la fonction
 */
void expMod(mpz_t res, mpz_t g, mpz_t a, mpz_t p){
  
    mpz_t quotient, rest, r0, r1, r2, r3, aTmp;
    mpz_inits(quotient, rest, r0, r1, r2, r3, aTmp, (void *)NULL);
    mpz_tdiv_qr_ui(quotient, rest, a, 2);

    mpz_set(aTmp,a);

    if(mpz_sgn(a) == 0){
        mpz_set_ui(res, 1);
    }else{
        if(mpz_cmp_ui(a, 1)==0){
            mpz_mod(res, g, p);
        }else{
            if(mpz_sgn(rest)==0){
                //expmod(g^2 mod p , a/2)
                //g^2
                mpz_mul(r0, g, g);
                //mod p
                mpz_mod(r1, r0, p);
                //expMod(r0, p, g, 2);
                expMod(res, r1,quotient, p);
            }else{
                //expmod((g × puissance(g^2 mod p , (a − 1)/2) )mod p)
                //g^2
                mpz_mul(r0, g, g);
                //mod p
                mpz_mod(r1, r0, p);
                //(a-1) / 2
                mpz_sub_ui(aTmp, a, 1);
                mpz_tdiv_q_ui(quotient, aTmp, 2);

                expMod(r2, r1, quotient, p);

                mpz_mul(r3, r2, g);
                mpz_mod(res, r3, p);
            }  
        }  
    }
}

/*
 *KeyGen() est la fonction qui génère les clés de Bob, elle prend en entrée/sortie sans les modifier
 *p et g, elle tire au hasard un x, la clé secrète de Bob et calcule sa clé publique correspondante
 *X ≡ g x mod p. Elle renvoie en sortie x et X.
*/
void keyGen(mpz_t p, mpz_t g, mpz_t grand_X, mpz_t x){
    gmp_randstate_t state;
    // init random state
    gmp_randinit_mt(state);
    gmp_randseed_ui(state, time(NULL));
    //gmp_randseed_ui(state, 123987);
    //Generation de x la cle privée 
    mpz_urandomm(x, state, p);
    //mpz_add_ui(x, x, 2);
    // calcule du grand X ≡ g^x mod p --> 1eme methode
    expMod(grand_X, g, x, p);
    gmp_printf("===publicKeyOfA grand_X ===\n%Zd\n", grand_X);
    // calcule du grand X ≡ g^x mod p --> 2eme methode
    // mpz_powm(grand_X, g, x, p);   
    // gmp_printf("===publicKeyOfA grand_X ===\n%Zd\n", grand_X);
}

/**
 * Encrypt() est la fonction qui produit en sortie le couple chiffré (C ≡ m × y mod p, B ≡
 * g^r mod p) en prenant en entrée la clé publique de Bob K p = (p, g, X) et un message m
 * avec y ≡ X^r mod p et r alea.
 */
void encrypt(mpz_t p, mpz_t g, mpz_t grand_X, mpz_t message) {
	//Initialisation des variables
	mpz_t y, r;// grand_B, grand_C;
	mpz_inits(r, y, grand_B, grand_C, NULL);
	//variable temporaire utile pour le calcul de grand_C
	mpz_t tmp;
	mpz_init(tmp);

	//------Calcul de r aleatoire
	gmp_randstate_t state;
    // init random state
    gmp_randinit_mt(state);
    gmp_randseed_ui(state, time(NULL));
    //generation de r aleatoire 
    mpz_urandomm(r, state, p);
    //gmp_printf("r encrypt : %Zd", r);
    
    //------------FAIRE UNE LISTE POUR DIFFERENCIER CHAQUE R---------//

	//------Calcul de y (y = X^r mod p)
	expMod(y, grand_X, r, p);
	//gmp_printf("y : \n%Zd", y);

	//------Calcul de grand_B (B = g^r mod p)
	expMod(grand_B, g, r, p);
	gmp_printf("\nB : \n%Zd", grand_B);

	//------Calcul de grand_C (C ≡ m × y mod p)
	//m * y
	mpz_mul(tmp, message, y);
	mpz_mod(grand_C, tmp, p);
	//gmp_printf("\nC : \n%Zd", grand_C);
}


/*
 *Decrypt() est la fonction de déchiffrement qui prend en entrée (C, B) et la clé secrète de Bob
 	K s = x et produit en sortie le message m.
 */
void decrypt(mpz_t grand_C, mpz_t grand_B, mpz_t x, mpz_t p) {
	//initialisation des variables locales
	mpz_t c_invd; //C * D^-1
	mpz_init(c_invd);

	//variable temporaire utile pour le calcul de C * D^-1
	mpz_t tmp;
	mpz_t tmp2;
	mpz_t tmp3;
	mpz_init(tmp);
	mpz_init(tmp2);
	mpz_init(tmp3);

	//initialisation de grand_D
	mpz_init(grand_D);

	//------Calcul de grand_D (D = B^x mod p)
	expMod(grand_D, grand_B, x, p);

	//------Calcul de C*D^-1 mod p
	mpz_set(tmp, grand_D);
	mpz_set(tmp3, p);
	int i;
	i = euclide(tmp, tmp3);
	//mpz_set_ui(tmp, euclide(grand_D, p));//calcul de D^-1 
	mpz_mul(tmp2, grand_C, tmp);//calcul de C*D^-1
	gmp_printf("\ntmp2 : \n%Zd\n", tmp2);
	mpz_mod(c_invd, tmp2, p);//calcul de C*D^-1 mod p

	gmp_printf("\nc_invd : \n%Zd\n", c_invd);

	/* RESTE A FAIRE
		Bob retrouve ainsi m car
	∗ D = B^x ≡ g^xr ≡ (g^x)^r ≡ X^r ≡ y mod p et
	∗ C × (D)^−1 ≡ m × y × (y)^−1 ≡ m mod p
	*/
}

/*
    main
*/
int main(int argc, char * argv[]){
    
    mpz_t g,p;
    mpz_t x,grand_X,y1;

    mpz_inits(x,grand_X, y1,NULL);

    // G initialization
    mpz_init(g);
    mpz_set_ui(g, 2);
    gmp_printf("g = %Zd  \n", g);

    // P initialization
    mpz_init(p);
    mpz_set_str(p, P_VAL_HEXA, 16);
    gmp_printf("p en 1024 bit rfc2409 = \n%Zd \n", p);
    
    //************************
    mpz_set(p_global, p); //affectation de la valeur de p a p_global
    
    //avoir Kp=(p,g,X), Ks=(x) 
    
    mpz_t mes;
    mpz_init(mes);
    mpz_set_ui(mes, 12345641);
  	keyGen(p,g,grand_X,x);
  	encrypt(p, g, grand_X, mes);
  	decrypt(grand_C, grand_B, x, p); 

  



//*********************************************************************************//
  //Initialisation des variables mpz
  /*mpz_t a;
  mpz_t p1;
  mpz_inits(a,p1,p_global,NULL);
  //Affectation des valeurs
  mpz_init_set_ui(a, 13);
  mpz_init_set_ui(p1, 60);
  mpz_init_set_ui(p_global, 60);
  euclide(a,p1);*/

  // printf("\n\n\n*****lalalalalalalalalalalalalalala*****\n");
  // printf("\n\n\nUFINAL : ");
  // mpz_out_str(NULL, 10, uFinal);
  // printf("\n\n\nVFINAL : ");
  // mpz_out_str(NULL, 10, vFinal);
  // printf("\n\n\n*****lalalalalalalalalalalalalalala*****\n");

//*********************************************************************************//
 
 

  //Test d affichage des deux p
  //gmp_printf("\np : %Zd\n", p);
  //gmp_printf("\np_global : %Zd\n", p_global);  

 // mpz_clear(a);
  mpz_clear(p);
  mpz_clear(uFinal);
  mpz_clear(vFinal);
  mpz_clear(x);
  mpz_clear(g);
  mpz_clear(p_global);

  mpz_clear(grand_X);
  mpz_clear(grand_B);
  mpz_clear(grand_C);
  mpz_clear(grand_D);

return 0;
}