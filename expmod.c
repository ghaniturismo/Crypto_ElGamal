
#include<stdio.h>
#include<gmp.h>

/*
 * Calcule de A ≡ g^a mod p de la question 4
 * stockage du resultat de g^a mod p dans  la variable resultat 
 * initialiser r avant l'appel à la fonction
 */
void expMod(mpz_t resultat, mpz_t g, mpz_t a, mpz_t p){
  
    mpz_t quot, rest, t0, t1, t2, t3, aTmp;
    mpz_inits(quot, rest, t0, t1, t2, t3, aTmp, NULL);
    mpz_tdiv_qr_ui(quot, rest, a, 2);

    mpz_set(aTmp,a);
		

    if(mpz_sgn(a) == 0){
        mpz_set_ui(resultat, 1);
    }else{
        if(mpz_cmp_ui(a, 1)==0){
            mpz_mod(resultat, g, p);
        }else{
            if(mpz_sgn(rest)==0){
                //expmod(g^2 mod p , a/2)
                //g^2
                mpz_mul(t0, g, g);
                //mod p
                mpz_mod(t1, t0, p);
                expMod(resultat, t1,quot, p);
            }else{
                //expmod((g × puissance(g^2 mod p , (a − 1)/2) )mod p)
                //g^2
                mpz_mul(t0, g, g);
                //mod p
                mpz_mod(t1, t0, p);
                //(a-1) / 2
                mpz_sub_ui(aTmp, a, 1);
                mpz_tdiv_q_ui(quot, aTmp, 2);

                expMod(t2, t1, quot, p);

                mpz_mul(t3, t2, g);
                mpz_mod(resultat, t3, p);
            }  
        }  
    }
    mpz_clears(quot, rest, t0, t1, t2, t3, aTmp, NULL);
}