// -*- tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 2 -*-
// vi: set et ts=4 sw=2 sts=2:
#ifndef DUNE_TYPETRAITS_HH
#define DUNE_TYPETRAITS_HH

#include <complex>
#include <type_traits>
#include <utility>
#include <vector>

namespace Dune
{

  namespace Impl
  {
    ///
    /**
     * @internal
     * @brief Helper to make void_t work with gcc versions prior to gcc 5.0 and some clang versions.
     */
    template <class...>
    struct voider
    {
      using type = void;
    };
  }

  //! Is void for all valid input types. The workhorse for C++11 SFINAE-techniques.
  /**
   * \ingroup CxxUtilities
   *
   * Note, since c++17 there is also `std::void_t` that should be preferred. But, due to an issue
   * in the c++ standard, see CWG issue #1980. "Equivalent but not functionally-equivalent redeclarations",
   * and a corresponding failure in some clang compilers, this tool is left here as a workaround.
   * Use it if you want to specialize multiple classes using `void_t`.
   */
  template <class... Types>
  using void_t = typename Impl::voider<Types...>::type;

  /**
   * @file
   * @brief Traits for type conversions and type information.
   * @author Markus Blatt, Christian Engwer
   */
  /** @addtogroup CxxUtilities
   *
   * @{
   */

  /**
   * @brief Just an empty class
   */
  struct Empty {};

  /**
   * @brief Checks whether two types are interoperable.
   *
   * Two types are interoperable if conversions in either directions
   * exists.
   */
  template<class T1, class T2>
  struct IsInteroperable
  {
    enum {
      /**
       * @brief True if either a conversion from T1 to T2 or vice versa
       * exists.
       */
      value = std::is_convertible<T1,T2>::value || std::is_convertible<T2,T1>::value
    };
  };

  /**
   * @brief Enable typedef if two types are interoperable.
   *
   * (also see IsInteroperable)
   */
  template<class T1, class T2, class Type>
  struct EnableIfInterOperable
    : public std::enable_if<IsInteroperable<T1,T2>::value, Type>
  {};

  /**
     \brief template which always yields a false value
     \tparam T Some type.  It should be a type expression involving template
               parameters of the class or function using AlwaysFalse.

     Suppose you have a template class.  You want to document the required
     members of this class in the non-specialized template, but you know that
     actually instantiating the non-specialized template is an error.  You
     can try something like this:
     \code
     template<typename T>
     struct Traits {
       static_assert(false,
                          "Instanciating this non-specialized template is an "
                          "error.  You should use one of the specializations "
                          "instead.");
       //! The type used to frobnicate T
       typedef void FrobnicateType;
     };
     \endcode
     This will trigger static_assert() as soon as the compiler reads the
     definition for the Traits template, since it knows that "false" can
     never become true, no matter what the template parameters of Traits are.
     As a workaround you can use AlwaysFalse: replace <tt>false</tt> by
     <tt>AlwaysFalse<T>::value</tt>, like this:
     \code
     template<typename T>
     struct Traits {
       static_assert(AlwaysFalse<T>::value,
                          "Instanciating this non-specialized template is an "
                          "error.  You should use one of the specializations "
                          "instead.");
       //! The type used to frobnicate T
       typedef void FrobnicateType;
     };
     \endcode
     Since there might be an specialization of AlwaysFalse for template
     parameter T, the compiler cannot trigger static_assert() until the
     type of T is known, that is, until Traits<T> is instantiated.
   */
  template<typename T>
  struct AlwaysFalse {
    //! always a false value
    static const bool value = false;
  };

  /**
     \brief template which always yields a true value
     \tparam T Some type.  It should be a type expression involving template
               parameters of the class or function using AlwaysTrue.

     \note This class exists mostly for consistency with AlwaysFalse.
   */
  template<typename T>
  struct AlwaysTrue {
    //! always a true value
    static const bool value = true;
  };

  //! \brief Whether this type acts as a scalar in the context of
  //!        (hierarchically blocked) containers
  /**
     All types `T` for which `IsNumber<T>::value` is `true` will act as a
     scalar when used with possibly hierarchically blocked containers, such as
     `FieldMatrix`, `FieldVector`, `BCRSMatrix`, `BlockVector`,
     `MultiTypeBlockVector`, etc.  This enables earlier error reporting when
     implementing binary container-scalar operators, such as `=` or `*=`.

     By default is `true` for all arithmetic types (as per
     `std::is_arithmetic`), and for `T=std::complex<U>`, iff
     `IsNumber<U>::value` itself is `true`.

     Should be specialized to `true` for e.g. extended precision types or
     automatic differentiation types, or anything else that might sensibly be
     an element of a matrix or vector.
   */
  template <typename T>
  struct IsNumber
    : public std::integral_constant<bool, std::is_arithmetic<T>::value> {
  };

#ifndef DOXYGEN

  template <typename T>
  struct IsNumber<std::complex<T>>
    : public std::integral_constant<bool, IsNumber<T>::value> {
  };

#endif // DOXYGEN

  //! \brief Whether this type has a value of NaN.
  /**
   * Internally, this is just a forward to `std::is_floating_point<T>`.
   */
  template <typename T>
  struct HasNaN
      : public std::integral_constant<bool, std::is_floating_point<T>::value> {
  };

#ifndef DOXYGEN

  template <typename T>
  struct HasNaN<std::complex<T>>
      : public std::integral_constant<bool, std::is_floating_point<T>::value> {
  };

#endif // DOXYGEN

  //! \brief Whether this type has a value of NaN.
  //! \deprecated has_nan is deprecated, use `Dune::HasNaN` instead
  /**
   * Internally, this is just a forward to `std::is_floating_point<T>`.
   */
  template <typename T>
  struct [[deprecated("Has been renamed to 'HasNaN'.")]] has_nan
    : HasNaN<T> {};

}
#endif
