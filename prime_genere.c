#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <gmp.h>

/*----------------renvoie l tq 2^l*t=x avec t impair----------------------*/

int last_one(mpz_t x){
	mpz_t r,q, x_temp;
	mpz_inits(r, q,x_temp, NULL);
	mpz_set(x_temp, x);
	int res=0;
	mpz_cdiv_r_ui(r, x_temp, 2);
	if(mpz_cmp_ui(r,0)!=0){
		return 0;
	}
	while(mpz_cmp_ui(r,0)==0){
		mpz_cdiv_q_ui(x_temp,x_temp, 2);
		mpz_cdiv_r_ui(r, x_temp, 2);
		res++;
	}
	mpz_clears(x_temp, r,q,NULL);
	return res;
}
/*-------------------------------------------------------------------------*/
/*----------------renvoie un entier aleatoire x tq x in [a,b[-------------------*/

void rand_a_b(mpz_t res, mpz_t a, mpz_t b, gmp_randstate_t graine){
	mpz_t temp;
	mpz_init(temp);
	mpz_sub(temp, b, a);
    mpz_urandomm(res, graine, temp);
    mpz_add(res, res, a);
    mpz_clear(temp);
}
/*-------------------------------------------------------------------------------*/

/*renvoie 0 si n est composé en base a et 1 si il est probablement premier en base a*/

int TestMiller(mpz_t n, mpz_t a){
	int power;
	int j;
	mpz_t temp1, temp2, deux, b,t;
	mpz_inits(temp1, temp2, deux,b,t, NULL);
	mpz_add_ui(deux, deux, 2);
	mpz_sub_ui(temp1, n,1);           //temp1=n-1
	power=last_one(temp1);
	mpz_pow_ui(temp2, deux, power);   // temp2=2^power
	mpz_cdiv_q(t, temp1, temp2);     //t=n-1/2^power la partie impair de n-1
	mpz_powm(b, a, t, n); // b=a^t mod n
	if(mpz_cmp_ui(b, 1)==0){
		mpz_clears(temp1, temp2, deux,b,t, NULL);
		return 1;
	}
	for(j=0; j<power; j++){
		if(mpz_cmp(b, temp1)==0){
			mpz_clears(temp1, temp2, deux,b,t, NULL);
			return 1;
		}
		if(mpz_cmp_ui(b, 1)==0){
			mpz_clears(temp1, temp2, deux,b,t, NULL);
			return 0;
		}
		mpz_powm_ui(b, b,2, n);
	}
	mpz_clears(temp1, temp2, deux,b,t, NULL);
	return 0;
}
/*----------------------------------------------------------------*/
/*-----------applique le test de miller k fois sur des temoisn aleatoire-------------*/
/*----------renvoie 0 si n est compse 1 si il est probablement premier---------------------*/
int miller(mpz_t n, int k){
	int i, j;
	gmp_randstate_t graine;
	gmp_randinit_mt(graine);
	mpz_t alea, temp, deux, temp2;
	mpz_inits(alea, temp,deux, temp2, NULL);
	mpz_set_ui(deux, 2);
	mpz_sub_ui(temp2, n, 1);
	if(mpz_cmp_ui(n, 2)==0){
		gmp_randclear(graine);
		mpz_clears(alea, temp,deux, temp2, NULL);
		return 1;
	}
	mpz_cdiv_r_ui(temp, n, 2);
	if(mpz_cmp_ui(temp, 0)==0){
		gmp_randclear(graine);
		mpz_clears(alea, temp,deux, temp2, NULL);
		return 0;
	}
	for(i=0; i<k; i++){
		rand_a_b(alea, deux, temp2, graine);
		if(TestMiller(n, alea)==0){
			gmp_randclear(graine);
			mpz_clears(alea, temp,deux, temp2, NULL);
			return 0;
		}
	}
	gmp_randclear(graine);
	mpz_clears(alea, temp,deux, temp2, NULL);
	return 1;
}

/*----------------------------------------------------------------------------------------*/
/*genere un entier aleatoire de k bit*/

void alea_k(mpz_t res, int k, gmp_randstate_t graine){
	mpz_t a,b, dix;
	mpz_inits(a,b, dix, NULL);
	mpz_set_ui(dix, 10);
	mpz_pow_ui(a, dix, k-1);
	mpz_pow_ui(b, dix, k);
	rand_a_b(res, a,b, graine); 
	mpz_clears(a, b, dix, NULL);	
}

void genere_premier(mpz_t res, int k, int t){
	gmp_randstate_t graine;
	gmp_randinit_mt(graine);
	mpz_t n;
	mpz_init(n);
	alea_k(n,k, graine);
	while(miller(n,t)!=1){
		alea_k(n,k, graine);
	}
	mpz_set(res,n);
	mpz_clear(n);
	gmp_randclear(graine);
}
		
int main(){
	int k;
	mpz_t p;
	mpz_init(p);
	printf("Entrez la taille du nombre premier desire\n");
	scanf("%d", &k);
	genere_premier(p, k, 10);
	gmp_printf("%Zd est probablement premier\n", p);
	return 0;
}
