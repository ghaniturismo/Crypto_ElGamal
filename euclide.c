/* ______________________________________________
  |                                              |
  | ----------- DM Init-Cryptographie -----------|
  | -- Implémentation de l'algorithme El Gamal --|
  | -- MAZROU Abdelghani ****** BENTOUNES Samy --|
  |______________________________________________|
*/
  
#include <stdio.h>
#include <gmp.h>

//Affichage de "a*u + b*v = p"
void printauplusbv(mpz_t a, mpz_t u, mpz_t b, mpz_t v, mpz_t p) 
{
      mpz_out_str(NULL, 10, a);
      printf(" * (");
      mpz_out_str(NULL, 10, u);
      printf(") + ");
      mpz_out_str(NULL, 10, b);
      printf(" * (");
      mpz_out_str(NULL, 10, v);
      printf(") = ");
      mpz_out_str(NULL, 10, p);
      printf("\n");
}

//Affichage de "b*uRec + (a - b*q) *vRec = p"
void printbuplusamoinsbqv(mpz_t b, mpz_t uRec, mpz_t a, 
        mpz_t q, mpz_t vRec, mpz_t p)
{
  printf("---------------------------------------------\n");
      mpz_out_str(NULL, 10, b);
      printf(" * (");
      mpz_out_str(NULL, 10, uRec);
      printf(") + (");
      mpz_out_str(NULL, 10, a);
      printf(" - ");
      mpz_out_str(NULL, 10, b);
      printf(" * ");
      mpz_out_str(NULL, 10, q);
      printf(" ) * (");
      mpz_out_str(NULL, 10, vRec);
      printf(") = ");
      mpz_out_str(NULL, 10, p);      
      printf("\n");
}


//Affichge de "a= bq + bv + r"
void printdivision(mpz_t a, mpz_t b, mpz_t q, mpz_t r) 
{
      mpz_out_str(NULL, 10, a);
      printf(" = ");
      mpz_out_str(NULL, 10, b);
      printf(" * ");
      mpz_out_str(NULL, 10, q);
      printf(" + ");
      mpz_out_str(NULL, 10, r);
      printf("\n");
}

/*
 * Calcul des coefficients de Bezout u e v tels que a*u + b*v = pgcd de a et b
 */
void bezout(mpz_t a, mpz_t b, mpz_t u, mpz_t v, mpz_t pgcd, mpz_t lastV)
{
  mpz_t pgcd_tmp;
  mpz_inits(pgcd_tmp,NULL);
  mpz_set(pgcd_tmp, pgcd);

  if (!mpz_cmp_ui(b, 0))
    {
      mpz_set(pgcd_tmp, a);
      mpz_set_ui(u, 1);
      mpz_set(v, lastV);
      //printf("---------------------------------------------\n");      
      //printauplusbv(a, u, b, v, pgcd_tmp);
    }
  else
    {
      mpz_t quotient, reste, uRec, vRec, vFoisQuotient;
      mpz_inits(quotient, reste, uRec, vRec, vFoisQuotient,NULL);

      mpz_tdiv_qr(quotient, reste, a, b);
      //printdivision(a, b, quotient, reste);
      bezout(b, reste, uRec, vRec, pgcd_tmp, lastV);      
      //printbuplusamoinsbqv(b, uRec, a, quotient, vRec, pgcd_tmp);
      mpz_set(u, vRec);
      mpz_mul(vFoisQuotient, vRec, quotient);
      mpz_sub(v, uRec, vFoisQuotient);
      //printauplusbv(a, u, b, v, pgcd_tmp);
      mpz_clears(quotient, reste, uRec, vRec, vFoisQuotient, pgcd_tmp, NULL);
    }
}

/* 
 * Fonction euclide
 * Recuperation des coefficients u et v (a partir de a et p) 
 */
void euclide(mpz_t u, mpz_t v, mpz_t a, mpz_t ptmp){
 
    //Initialisation des variables de type mpz_t
    mpz_t zero,x,y,inv_a;
    mpz_inits(inv_a,zero,x,y,NULL);

    //Affectation des valeurs aux variables correcpondantes
    mpz_init_set_ui(zero, 0);
    mpz_set(x, a);
    mpz_set(y, ptmp);

    //Application de la fonction bezout(...) pour recuperer u et v
    bezout(x, y, u, v, ptmp, zero);

    //Initialisation de l inverse modulaire de a
    mpz_init_set_ui(inv_a, 0);

    //Calcul de l inverse modulaire de a
    //affectation de u mod p  a inv_a
    mpz_mod(inv_a,u,ptmp);

      //Affichage de l'inverse modulaire de a
      //gmp_printf("\n\n a^-1 mod p = \n%Zd ^-1\nmod %Zd =\n%Zd \n", a ,ptmp, inv_a);

    //Liberation de la memeoire allouee aux variables
    mpz_clears(x,y,inv_a,NULL);
}