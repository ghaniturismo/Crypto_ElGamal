
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


int expmod(int base, int exponent, int modulo) {

    if (base < 0 || exponent < 0 || modulo <= 0) {
        return 0; //error when invalid args
    }
    
    int result; // var pour stocké le resultat
    int multiply; //base de multiplication
    int mask; //mask pour definir les etats des bits.
    int i;
    
    result = 1;
    multiply = base;
    mask = 1;
    
    for (i = 0; i < WORD_BIT; ++i) {
        if (exponent & mask) { //test si le bit est a 1 
            printf("mask = %d\n",mask);
            result = (result * multiply) % modulo;         
        }
        multiply = (multiply * multiply) % modulo;
        mask <<= 1; //decalage d'un bit
    }
    
    return result;
}
/*
    main
*/

int main(int argc, char * argv[]){
    
    mpz_t g,p;
    mpz_t x,y;

     // init random state
    gmp_randinit_mt(state);
    gmp_randseed_ui(state, time(NULL));

    mpz_inits(x,y,NULL);

    // G initialization
    mpz_init(g);
    mpz_set_ui(g, 2);
    gmp_printf("g = %Zd  \n", g);

    // P initialization
    mpz_init(p);
    mpz_set_str(p, P_VAL_HEXA, 16);
    gmp_printf("p en 1024 bit rfc2409 = \n%Zd \n", p);

    gmp_printf("paire(p,g) = ( %Zd ,  %Zd )\n", p,g);

    mpz_urandomm(x, state, p);
    gmp_printf("x = \n%Zd  \n", x);

    mpz_powm(y, g, x, p);  // calcule de la cle public 2eme methode 
    gmp_printf("===publicKeyOfA===\n%Zd\n", y);


//---------------------------------------------------------------------------------//
  	int n;
	gmp_randinit_default(generator);    
    //msg to encrypt
    mpz_t msg1,msg2;
    mpz_inits(msg1,msg2, NULL);
    generate_number(msg1,16);
    generate_prime_number(msg2,16);
    gmp_printf("msg number :%Zu \n",msg1);
    gmp_printf("msg alea number :%Zu \n",msg2);
//---------------------------------------------------------------------------------//

int a1;
a1 = expmod(432, 5056, 765);
printf("expmod(19,1024,62) = %d\n", a1);

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
//*********************************************************************************//
return 0;
}
