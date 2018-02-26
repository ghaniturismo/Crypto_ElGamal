
#include<stdio.h>
#include<gmp.h>

/*---------------------------------------------------------------------*/

//Declaration des variables globales
//Variable representant la derniere valeur de u
mpz_t uFinal;
//Variable representant la derniere valeur de v
mpz_t vFinal;
//Variable reprensentant p
mpz_t p_global;

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

//Calcul des coefficients de Bezout u e v tels que a*u + b*v = pgcd de a et b
void bezout(mpz_t a, mpz_t b, mpz_t u, mpz_t v, mpz_t pgcd, mpz_t lastV)
{
  if (!mpz_cmp_ui(b, 0))
    {
      mpz_set(pgcd, a);
      mpz_set_ui(u, 1);
      mpz_set(v, lastV);
      printf("---------------------------------------------\n");      
      printauplusbv(a, u, b, v, pgcd);
    }
  else
    {
      mpz_t quotient;
      mpz_t reste;
      mpz_t uRec;
      mpz_t vRec;
      mpz_t vFoisQuotient;
      mpz_init(quotient);
      mpz_init(reste);
      mpz_init(uRec);
      mpz_init(vRec);
      mpz_init(vFoisQuotient);
      mpz_tdiv_qr(quotient, reste, a, b);
      printdivision(a, b, quotient, reste);
      bezout(b, reste, uRec, vRec, pgcd, lastV);      
      printbuplusamoinsbqv(b, uRec, a, quotient, vRec, pgcd);
      mpz_set(u, vRec);
      mpz_mul(vFoisQuotient, vRec, quotient);
      mpz_sub(v, uRec, vFoisQuotient);
      printauplusbv(a, u, b, v, pgcd);
      mpz_clear(quotient);
      mpz_clear(reste);
      mpz_clear(uRec);
      mpz_clear(vRec);
      mpz_clear(vFoisQuotient);
    }
}

//Fonction euclide
//Recuperation des coefficients u et v (a partir de a et p) 
//et affectation dans uFinal et vFinal (variables globales)
int euclide(mpz_t a, mpz_t ptmp)
{
 
  //Initialisation des variables de type mpz_t
  mpz_t zero,x,y,u,v,inv_a;
  mpz_init(inv_a);
  mpz_init(zero);
  mpz_init(x);
  mpz_init(y);
  mpz_init(u);
  mpz_init(v);
  //Affectation des valeurs aux variables correcpondantes
  mpz_init_set_ui(zero, 0);
  mpz_set(x, a);
  mpz_set(y, ptmp);

  //Application de la fonction bezout(...) pour recuperer u et v
  bezout(x, y, u, v, ptmp, zero);

  //Initialisation de l inverse modulaire de a
  mpz_init_set_ui(inv_a, 0);
  //Initialisation de uFinal et vFinal
  mpz_init(uFinal);
  mpz_init(vFinal);

  //Affectation des dernieres valeurs de u et v à uFinal et vFinal
  mpz_set(uFinal, u);
  mpz_set(vFinal, v);

  //Calcul de l inverse modulaire de a
  //affectation de u mod p_global a inv_a
  mpz_mod(inv_a,u,p_global);

  //Affichage de l'inverse modulaire de a
  printf("\n\n\n inv_a : ");
  mpz_out_str(NULL, 10,inv_a);

  //Liberation de la memeoire allouee aux variables
  mpz_clear(v);
  mpz_clear(x);
  mpz_clear(y);
  mpz_clear(inv_a);

  return 0;
}