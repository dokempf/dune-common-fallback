// -*- tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 2 -*-
// vi: set et ts=4 sw=2 sts=2:
#ifndef DUNE_COMMON_TYPEUTILITIES_HH
#define DUNE_COMMON_TYPEUTILITIES_HH

#include <cstddef>
#include <type_traits>
#include <tuple>


namespace Dune {

  /**
   * \file
   * \brief Utilities for type computations, constraining overloads, ...
   * \author Carsten Gräser
   */

  /**
   * \brief Helper class for tagging priorities.
   *
   * \ingroup TypeUtilities
   *
   * When using multiple overloads of a function
   * where some are removed from the overload set
   * via SFINAE, the remaining overloads may be ambiguous.
   * A prototypic example would be a default overload
   * that should be used if the others do not apply.
   *
   * By adding additional arguments of type PriorityTag<k>
   * with increasing priority k to all overloads and calling
   * the method with PriorityTag<m> where m is larger or equal
   * to the maximal used priority, those can be made unambiguous.
   *
   * In this case the matching overload with highest priority
   * will be used. This is achieved by the fact that PriorityTag<k>
   * derives from all types PriorityTag<i> with i less than k.
   *
   * \tparam priority The priority of this tag.
   */
  template<std::size_t priority>
  struct PriorityTag : public PriorityTag<priority-1>
  {
    static constexpr std::size_t value = priority;
  };

  /**
   * \brief Helper class for tagging priorities.
   *
   * \ingroup TypeUtilities
   *
   * PriorityTag<0> does not derive from any
   * other PriorityTag.
   */
  template<>
  struct PriorityTag<0>
  {
    static constexpr std::size_t value = 0;
  };



} // namespace Dune



#endif // DUNE_COMMON_TYPEUTILITIES_HH
