// -*- tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 2 -*-
// vi: set et ts=4 sw=2 sts=2:
#ifndef DUNE_MATH_HH
#define DUNE_MATH_HH

/** \file
 * \brief Some useful basic math stuff
 */

#include <cmath>
#include <complex>
#include <limits>
#include <type_traits>

#include <dune/common/typeutilities.hh>

namespace Dune
{

  /**
     \brief Standard implementation of MathematicalConstants.

     This implementation will work with all built-in floating point
     types. It provides

   * e as exp(1.0)
   * pi as acos(-1.0)

   */
  template< class T >
  struct StandardMathematicalConstants
  {
    /**
     * \brief Euler's number
     */
    static const T e ()
    {
      using std::exp;
      static const T e = exp( T( 1 ) );
      return e;
    }

    /**
     * \brief Archimedes' constant
     */
    static const T pi ()
    {
      using std::acos;
      static const T pi = acos( T( -1 ) );
      return pi;
    }
  };


  /**
     \brief Provides commonly used mathematical constants.

     a struct that is specialized for types repesenting real or complex
     numbers. It provides commonly used mathematical constants with the
     required accuary for the specified type.
   */
  template< class Field >
  struct MathematicalConstants
    : public StandardMathematicalConstants<Field>
  {};


  /** \brief Power method for integer exponents
   *
   * \note Make sure that Mantissa is a non-integer type when using negative exponents!
   */
  template <class Mantissa, class Exponent>
  constexpr Mantissa power(Mantissa m, Exponent p)
  {
    static_assert(std::numeric_limits<Exponent>::is_integer, "Exponent must be an integer type!");

    auto result = Mantissa(1);
    auto absp = (p<0) ? -p : p;   // This is simply abs, but std::abs is not constexpr
    for (Exponent i = Exponent(0); i<absp; i++)
      result *= m;

    if (p<0)
      result = Mantissa(1)/result;

    return result;
  }

  //! Calculates the factorial of m at compile time
  template <int m>
  struct Factorial
  {
    //! factorial stores m!
    enum { factorial = m * Factorial<m-1>::factorial };
  };

  //! end of recursion of factorial via specialization
  template <>
  struct Factorial<0>
  {
    // 0! = 1
    enum { factorial = 1 };
  };


  //! calculate the factorial of n as a constexpr
  // T has to be an integral type
  template<class T>
  constexpr inline static T factorial(const T& n) noexcept
  {
    static_assert(std::numeric_limits<T>::is_integer, "`factorial(n)` has to be called with an integer type.");
    T fac = 1;
    for(T k = 0; k < n; ++k)
      fac *= k+1;
    return fac;
  }

  //! calculate the factorial of n as a constexpr
  template<class T, T n>
  constexpr inline static auto factorial (std::integral_constant<T, n>) noexcept
  {
    return std::integral_constant<T, factorial(n)>{};
  }


  //! calculate the binomial coefficient n over k as a constexpr
  // T has to be an integral type
  template<class T>
  constexpr inline static T binomial (const T& n, const T& k) noexcept
  {
    static_assert(std::numeric_limits<T>::is_integer, "`binomial(n, k)` has to be called with an integer type.");

    if( k < 0 || k > n )
      return 0;

    if (2*k > n)
      return binomial(n, n-k);

    T bin = 1;
    for(auto i = n-k; i < n; ++i)
      bin *= i+1;
    return bin / factorial(k);
  }

  //! calculate the binomial coefficient n over k as a constexpr
  template<class T, T n, T k>
  constexpr inline static auto binomial (std::integral_constant<T, n>, std::integral_constant<T, k>) noexcept
  {
    return std::integral_constant<T, binomial(n, k)>{};
  }

  template<class T, T n>
  constexpr inline static auto binomial (std::integral_constant<T, n>, std::integral_constant<T, n>) noexcept
  {
    return std::integral_constant<T, (n >= 0 ? 1 : 0)>{};
  }


  //! compute conjugate complex of x
  // conjugate complex does nothing for non-complex types
  template<class K>
  inline K conjugateComplex (const K& x)
  {
    return x;
  }

#ifndef DOXYGEN
  // specialization for complex
  template<class K>
  inline std::complex<K> conjugateComplex (const std::complex<K>& c)
  {
    return std::complex<K>(c.real(),-c.imag());
  }
#endif

  //! Return the sign of the value
  template <class T>
  int sign(const T& val)
  {
    return (val < 0 ? -1 : 1);
  }


  namespace Impl {
    // Returns whether a given type behaves like std::complex<>, i.e. whether
    // real() and imag() are defined
    template<class T>
    struct isComplexLike {
      private:
        template<class U>
        static auto test(U* u) -> decltype(u->real(), u->imag(), std::true_type());

        template<class U>
        static auto test(...) -> decltype(std::false_type());

      public:
        static const bool value = decltype(test<T>(0))::value;
    };
  } // namespace Impl
}

#endif // #ifndef DUNE_MATH_HH
