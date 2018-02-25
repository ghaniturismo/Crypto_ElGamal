#include "gmp.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

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

int main(int argc, char * argv[]){
  	int n;
	gmp_randinit_default(generator);    
//msg to encrypt
    mpz_t msg1,msg2;
    mpz_inits(msg1,msg2, NULL);
    generate_number(msg1,16);
    generate_prime_number(msg2,16);
    gmp_printf("msg number :%Zu \n",msg1);
    gmp_printf("msg alea number :%Zu \n",msg2);

}
