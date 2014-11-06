// -*- tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 2 -*-
// vi: set et ts=4 sw=2 sts=2:
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <dune/common/fvector.hh>
#include <dune/common/fmatrix.hh>
#include <dune/common/dynmatrixev.hh>

#include <algorithm>
#include <complex>

using namespace Dune;

/** \brief Test the eigenvalue code with the Rosser test matrix

   This matrix was a challenge for many matrix eigenvalue
   algorithms. But the Francis QR algorithm, as perfected by
   Wilkinson and implemented in EISPACK, has no trouble with it. The
   matrix is 8-by-8 with integer elements. It has:

 * A double eigenvalue
 * Three nearly equal eigenvalues
 * Dominant eigenvalues of opposite sign
 * A zero eigenvalue
 * A small, nonzero eigenvalue

*/
template<typename ft>
void testRosserMatrix()
{
  // Hack: I want this matrix to be a DynamicMatrix, but currently I cannot
  // initialize such a matrix from initializer lists.  Therefore I take the
  // detour over a FieldMatrix.
  FieldMatrix<ft,8,8> AField = {
    { 611, 196, -192, 407, -8, -52, -49, 29 },
    { 196, 899, 113, -192, -71, -43, -8, -44 },
    { -192, 113, 899, 196, 61, 49, 8, 52 },
    { 407, -192, 196, 611, 8, 44, 59, -23 },
    { -8, -71, 61, 8, 411, -599, 208, 208 },
    { -52, -43, 49, 44, -599, 411, 208, 208 },
    { -49, -8, 8, 59, 208, 208, 99, -911 },
    { 29, -44, 52, -23, 208, 208, -911, 99}
  };

  DynamicMatrix<ft> A(8,8);
  for (int i=0; i<8; i++)
    for (int j=0; j<8; j++)
      A[i][j] = AField[i][j];

  // compute eigenvalues
  DynamicVector<std::complex<ft> > eigenComplex;
  DynamicMatrixHelp::eigenValuesNonSym(A, eigenComplex);

  // test results
  /*
     reference solution computed with octave 3.2

     > format long e
     > eig(rosser())

   */
  std::vector<ft> reference = {
          -1.02004901843000e+03,
          -4.14362871168386e-14,
           9.80486407214362e-02,
           1.00000000000000e+03,
           1.00000000000000e+03,
           1.01990195135928e+03,
           1.02000000000000e+03,
           1.02004901843000e+03
  };

  std::vector<ft> eigenRealParts(8);
  for (int i=0; i<8; i++)
    eigenRealParts[i] = std::real(eigenComplex[i]);

  std::sort(eigenRealParts.begin(), eigenRealParts.end());

  for (int i=0; i<8; i++)
  {
    if (std::fabs(std::imag(eigenComplex[i])) > 1e-10)
      DUNE_THROW(MathError, "Symmetric matrix has complex eigenvalue");

    if( std::fabs(reference[i] - eigenRealParts[i]) > 1e-10 )
      DUNE_THROW(MathError,"error computing eigenvalues");
  }

  std::cout << "Eigenvalues of Rosser matrix: " << eigenComplex << std::endl;
}


int main() try
{
  testRosserMatrix<double>();
  return 0;
} catch (Exception exception)
{
  std::cerr << exception << std::endl;
  return 1;
}
