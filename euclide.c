

#include<stdio.h>
#include<gmp.h>

/*---------------------------------------------------------------------*/


mpz_t uFinal;
mpz_t vFinal;
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

/*
  Implementation de la fonction phi definie dans le sujet 
  phi(a, b, 0) = b et phi(a, b, n) = phi(a + b, b, n-1).
  Tous les parametres doivent etre initialises.
*/
/*
void phi(mpz_t a, mpz_t b, mpz_t n, mpz_t res)
{
  if (!mpz_cmp_ui(n, 0))
    {
      mpz_set(res, b);
    }
  else
    {
      mpz_t aPlusB;
      mpz_t nMoinsUn;
      mpz_init(aPlusB);
      mpz_init(nMoinsUn);
      mpz_add(aPlusB, a, b);
      mpz_sub_ui(nMoinsUn, n, 1);
      phi(aPlusB, a, nMoinsUn, res);
      mpz_clear(aPlusB);
      mpz_clear(nMoinsUn);
    }
}*/


/*---------------------------------------------------------------------*/

/*
  Place dans res le n-eme nombes de Fibonacci.
  Tous les parametres doivent etre initialises.
*/

/*void fibo(mpz_t n, mpz_t res)
{
  mpz_t zero;
  mpz_t un;
  mpz_init_set_ui(zero, 0);
  mpz_init_set_ui(un, 1);
  phi(un, zero, n, res);
  mpz_clear(zero);
  mpz_clear(un);
}*/

/*---------------------------------------------------------------------*/

/*int main()
{
  mpz_t zero;
  mpz_t u;
  mpz_t v;
  mpz_t p;
  mpz_t cent;
  mpz_t centUn;
  mpz_t Fcent;
  mpz_t FcentUn;
  mpz_init(zero);
  mpz_init(u);
  mpz_init(v);
  mpz_init(p);
  mpz_init_set_ui(zero, 0);
  mpz_init_set_ui(cent, 100);
  mpz_init_set_ui(centUn, 101) ;

  mpz_init_set_ui(p, 1);

  mpz_init(Fcent);
  mpz_init(FcentUn);
  fibo(cent, Fcent);
  fibo(centUn, FcentUn);
  bezout(FcentUn, Fcent, u, v, p, zero);

  mpz_clear(u);
  mpz_clear(v);
  mpz_clear(p);
  mpz_clear(cent);
  mpz_clear(centUn);
  mpz_clear(Fcent);
  mpz_clear(FcentUn);
  return 0;
}*/
int euclide(mpz_t a, mpz_t p)
{
  mpz_t zero;
  mpz_t x;
  mpz_t y;
  mpz_t u;
  mpz_t v;
  mpz_init(zero);
  mpz_init(x);
  mpz_init(y);
  mpz_init(u);
  mpz_init(v);
  mpz_init_set_ui(zero, 0);
  mpz_init_set_ui(x, mpz_get_ui(a));
  mpz_init_set_ui(y, mpz_get_ui(p));

  bezout(x, y, u, v, p, zero);

  //uFinal = u;
  //vFinal = v;
  /*printf("\n\n\n*****lalalalalalalalalalalalalalala*****\n");
  printf("\n\n\nUFINAL : ");
  mpz_out_str(NULL, 10, uFinal);
  printf("\n\n\nVFINAL : ");
  mpz_out_str(NULL, 10, vFinal);
  printf("\n\n\n*****lalalalalalalalalalalalalalala*****\n");*/
  mpz_clear(u);
  mpz_clear(v);
  mpz_clear(x);
  mpz_clear(y);
  return 0;
}

int main(int argc, char const *argv[])
{
  mpz_t a;
  mpz_t p;
  mpz_init(a);
  mpz_init(p);
  mpz_init_set_ui(a, 17);
  mpz_init_set_ui(p, 13) ;
  euclide(a,p);
  mpz_clear(a);
  mpz_clear(p);
  mpz_clear(uFinal);
  mpz_clear(vFinal);
  return 0;
}