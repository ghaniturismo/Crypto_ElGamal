
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

//generate a number of exactly k_bit
void generate_number(mpz_t alea, unsigned int nbr_bit)
{
    mpz_t k;
    mpz_init(k);
    mpz_ui_pow_ui(k,2,nbr_bit-1);
    mpz_urandomb(alea, generator, nbr_bit-1);
    mpz_add(alea,alea,k);
    mpz_clear(k);
}

void generate_prime_number(mpz_t alea, unsigned int nbr_bit)
{
    int prime;
    mpz_t k;
    mpz_init(k);
    mpz_ui_pow_ui(k,2,nbr_bit-1);
    do
    {
        mpz_urandomb(alea, generator, nbr_bit-1);
        mpz_add(alea,alea,k);
        prime = mpz_probab_prime_p(alea,10);
    }while(prime==0);
    mpz_clear(k);
}

/*
 * Calcule de A ≡ g^a mod p de la question 4
 * stockage du resultat de g^a mod p dans  la variable res 
 * initialiser r avant l'appel à la fonction
 */
void expMod(mpz_t res, mpz_t p, mpz_t g, mpz_t a){
  
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
                expMod(res, p, r1, quotient);
            }else{
                //expmod((g × puissance(g^2 mod p , (a − 1)/2) )mod p)
                //g^2
                mpz_mul(r0, g, g);
                //mod p
                mpz_mod(r1, r0, p);
                //(a-1) / 2
                mpz_sub_ui(aTmp, a, 1);
                mpz_tdiv_q_ui(quotient, aTmp, 2);

                expMod(r2, p, r1, quotient);

                mpz_mul(r3, r2, g);
                mpz_mod(res, r3, p);
            }  
        }  
    }
}

/*
    main
*/

int main(int argc, char * argv[]){
    
    mpz_t g,p;
    mpz_t x,y,y1;

     // init random state
    gmp_randinit_mt(state);
    gmp_randseed_ui(state, time(NULL));

    mpz_inits(x,y,y1,NULL);

    // G initialization
    mpz_init(g);
    mpz_set_ui(g, 2);
    gmp_printf("g = %Zd  \n", g);

    // P initialization
    mpz_init(p);
    mpz_set_str(p, P_VAL_HEXA, 16);
    gmp_printf("p en 1024 bit rfc2409 = \n%Zd \n", p);
    
    //Generation de x la cle privée 
    mpz_urandomm(x, state, p);
    gmp_printf("x = \n%Zd  \n", x);

    // calcule de la cle public 1eme methode
    expMod(y, p, g, x);
    gmp_printf("===publicKeyOfA y ===\n%Zd\n", y);

    // calcule de la cle public 2eme methode
    mpz_powm(y1, g, x, p);   
    gmp_printf("===publicKeyOfA y1 ===\n%Zd\n", y1);



//---------------------------------------------------------------------------------//
  	// int n;
   //  gmp_randinit_default(generator);    
   //  //msg to encrypt
   //  mpz_t msg1,msg2;
   //  mpz_inits(msg1,msg2, NULL);
   //  generate_number(msg1,16);
   //  generate_prime_number(msg2,16);
   //  gmp_printf("msg number :%Zu \n",msg1);
   //  gmp_printf("msg alea number :%Zu \n",msg2);
//---------------------------------------------------------------------------------//

  mpz_t res,v,m,p10;
  mpz_inits(res,v, m, p10,NULL);
  mpz_set_ui(v, 19);
  mpz_set_ui(m, 152457);
  mpz_set_ui(p10, 62);
  
  expMod(res, p10, v, m);
  printf("Mr expmod = \n");
  mpz_out_str(NULL, 10, res);
  printf("\n");

//*********************************************************************************//
  //Initialisation des variables mpz
  mpz_t a;
  mpz_t p1;
  mpz_inits(a,p1,p_global,NULL);
  //Affectation des valeurs
  mpz_init_set_ui(a, 13);
  mpz_init_set_ui(p1, 60);
  mpz_init_set_ui(p_global, 60);
  euclide(a,p1);

  printf("\n\n\n*****lalalalalalalalalalalalalalala*****\n");
  printf("\n\n\nUFINAL : ");
  mpz_out_str(NULL, 10, uFinal);
  printf("\n\n\nVFINAL : ");
  mpz_out_str(NULL, 10, vFinal);
  printf("\n\n\n*****lalalalalalalalalalalalalalala*****\n");

  mpz_clear(a);
  mpz_clear(p);
  mpz_clear(uFinal);
  mpz_clear(vFinal);
  mpz_clear(x);
  mpz_clear(g);
//*********************************************************************************//
return 0;
}