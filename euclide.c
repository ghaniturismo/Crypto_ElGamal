

#include<stdio.h>
#include<gmp.h>

/*---------------------------------------------------------------------*/


mpz_t uFinal;
mpz_t vFinal;
mpz_t p_global;

/*
  Affiche au + bv = p
 */

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

/*---------------------------------------------------------------------*/

/*
  Affiche au + bv = p
 */

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

/*---------------------------------------------------------------------*/

/*
  Affiche b * uRec + (a - b*q) vRec = p
 */

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

/*---------------------------------------------------------------------*/

/*
  Calcule u et v tels que au + bv = pgcd, ou pgcd est le pgcd de a et b. 
  Toutes les variables doivent etre initialisees.
 */

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

/*---------------------------------------------------------------------*/



/*---------------------------------------------------------------------*/

int euclide(mpz_t a, mpz_t ptmp)
{
  
  mpz_t zero,x,y,u,v,inv_a;

  mpz_init(inv_a);

  mpz_init(zero);
  mpz_init(x);
  mpz_init(y);
  mpz_init(u);
  mpz_init(v);
  
  mpz_init_set_ui(zero, 0);
  mpz_set(x, a);
  mpz_set(y, ptmp);
  //mpz_init_set_ui(x, mpz_get_ui(a));
  //mpz_init_set_ui(y, mpz_get_ui(p));

  bezout(x, y, u, v, ptmp, zero);

  mpz_init_set_ui(inv_a, 0);
  
//*******
  mpz_init(uFinal);
  mpz_init(vFinal);

  //Affectation des derniers valeur de u et v
  mpz_set(uFinal, u);
  mpz_set(vFinal, v);

  mpz_mod(inv_a,u,p_global);

  printf("\n\n\n inv_a : ");
  mpz_out_str(NULL, 10,inv_a);

  mpz_clear(u);
  mpz_clear(v);
  mpz_clear(x);
  mpz_clear(y);

  mpz_clear(inv_a);

  return 0;
}

int main(int argc, char const *argv[])
{

  //Initialisation des variables mpz
  mpz_t a;
  mpz_t p;
  mpz_inits(a,p,p_global,NULL);
  //Affectation des valeurs
  mpz_init_set_ui(a, 13);
  mpz_init_set_ui(p, 60);
  mpz_init_set_ui(p_global, 60);
  euclide(a,p);

  //mpz_t uFinal;
  //mpz_t vFinal;

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
  return 0;
}