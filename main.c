
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

//Declaration des variables globales

//Variable reprensentant p
mpz_t p_global;

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
           
    //Generation de la cle privée x
    mpz_urandomm(x, state, p);
    mpz_add_ui(x, x, 2);
    
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
void encrypt(mpz_t grand_C,mpz_t grand_B, mpz_t p, mpz_t g, mpz_t grand_X, mpz_t msg_chiffre) {
	
    //Initialisation des variables
	mpz_t r, y;
	mpz_inits(r, y, NULL);
   
    //generation de r aleatoire 
    mpz_urandomm(r, state, p);
    
    //------------FAIRE UNE LISTE POUR DIFFERENCIER CHAQUE R---------//

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
    // D^-1 est apres appel a euclide c'est u.
    euclide(u, v, grand_D, p_global);

    //m = C × D^−1 mod p.
    mpz_mul(msg_dechiffre, grand_C, u);
    mpz_mod(msg_dechiffre, msg_dechiffre, p_global);

    // CLEAN
    mpz_clears(u,v,grand_D,NULL);
}

/*
 *  main
 */
int main(int argc, char * argv[]){
    
    // init random state
    gmp_randinit_mt(state);
    gmp_randseed_ui(state, time(NULL));
    //gmp_randseed_ui(state, 123987);

    mpz_t g,p;
    mpz_t x,grand_X;
    mpz_t grand_B, grand_C;
    mpz_t msg_Entre,msg_Sortie;

    mpz_init(msg_Entre);
    mpz_set_ui(msg_Entre, 12345641);
    mpz_inits(x,grand_X, msg_Sortie,NULL);
    mpz_inits(grand_C, grand_B,NULL);

    // G initialization
    mpz_init(g);
    mpz_set_ui(g, 2);
    gmp_printf("g = %Zd  \n", g);

    // P initialization
    mpz_init(p);
    mpz_set_str(p, P_VAL_HEXA, 16);
    gmp_printf("p en 1024 bit rfc2409 = \n%Zd \n", p);
    
    //affectation de la valeur de p a p_global
    mpz_set(p_global, p); 
        
    //avoir Kp=(p,g,X), Ks=(x) 
  	keyGen(p,g,grand_X,x);
  	encrypt(grand_C, grand_B, p, g, grand_X, msg_Entre);
  	decrypt(msg_Sortie,grand_C, grand_B, x); 
    gmp_printf("\nmsg : \n%Zd\n", msg_Sortie);

    // CLEAN
    mpz_clear(p);
    mpz_clear(x);
    mpz_clear(g);
    mpz_clear(p_global);
    mpz_clear(grand_X);
    mpz_clear(grand_B);
    mpz_clear(grand_C);

return 0;
}