#include "gmp.h"
#include "main.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <limits.h>

#define WORD_BIT (sizeof(int) * CHAR_BIT)


gmp_randstate_t generator;

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
    
    int result; // var for store result
    int multiply; //base multiply
    int mask; //mask for define bits state
    int i; //loop counter
    
    result = 1;
    multiply = base;
    mask = 1;
    
    for (i = 0; i < WORD_BIT; ++i) {
        if (exponent & mask) { //if bit is set
            result = (result * multiply) % modulo;         
        }
        multiply = (multiply * multiply) % modulo;
        mask <<= 1; //shift mask bit
    }
    
    return result;
}
/*
    main
*/

int main(int argc, char * argv[]){
    
    mpz_t g;
    mpz_t p;

    // G initialization
    mpz_init(g);
    mpz_set_ui(g, 2);
    gmp_printf("g = %Zd  \n", g);

    // P initialization
    mpz_init(p);
    mpz_set_str(p, P_VAL_HEXA, 16);
    gmp_printf("p en 1024 bit rfc2409 = %Zd \n", p);

    gmp_printf("paire(p,g) = ( %Zd ,  %Zd )\n", p,g);

  	int n;
	gmp_randinit_default(generator);    
//msg to encrypt
    mpz_t msg1,msg2;
    mpz_inits(msg1,msg2, NULL);
    generate_number(msg1,16);
    generate_prime_number(msg2,16);
    gmp_printf("msg number :%Zu \n",msg1);
    gmp_printf("msg alea number :%Zu \n",msg2);

int a;
a = expmod(432, 18, 765);
printf("expmod(19,1024,62) = %d\n", a);

return 0;
}
