// -*- tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 2 -*-
// vi: set et ts=4 sw=2 sts=2:
// $Id$
#ifndef DUNE_ITERATORFACADES_HH
#define DUNE_ITERATORFACADES_HH
#include <iterator>
#include "typetraits.hh"

namespace Dune
{
  /*! \defgroup IteratorFacades Iterator facades
     \ingroup Common

     \brief Iterator facades for writing stl conformant iterators.

     With using these facades writing iterators for arbitrary containers becomes much less
     cumbersome as only few functions have to be implemented. All other functions needed by
     the stl are provided by the facades using the Barton-Nackman trick (also known as
     curiously recurring template pattern.

     The following example illustrates how a random access iterator might be written:
     \include dune/common/test/iteratorfacadetest.hh
     \code
     #include<dune/common/iteratorfacades.hh>

     ...

     template<class C, class T>
     class TestIterator : public Dune::BidirectionalIteratorFacade<TestIterator<C,T>,T, T&, int>
     {
      friend class TestIterator<typename Dune::RemoveConst<C>::Type, typename Dune::RemoveConst<T>::Type >;
      friend class TestIterator<const typename Dune::RemoveConst<C>::Type, const typename Dune::RemoveConst<T>::Type >;

     public:

      // Constructors needed by the facade iterators.
      TestIterator(): container_(0), position_(0)
      { }

      TestIterator(C& cont, int pos)
        : container_(&cont), position_(pos)
      {}

      TestIterator(const TestIterator<typename Dune::RemoveConst<C>::Type, typename Dune::RemoveConst<T>::Type >& other)
        : container_(other.container_), position_(other.position_)
      {}


      TestIterator(const TestIterator<const typename Dune::RemoveConst<C>::Type, const typename Dune::RemoveConst<T>::Type >& other)
        : container_(other.container_), position_(other.position_)
      {}

      // Methods needed by the forward iterator
      bool equals(const TestIterator<typename Dune::RemoveConst<C>::Type,typename Dune::RemoveConst<T>::Type>& other) const
      {
        return position_ == other.position_ && container_ == other.container_;
      }


      bool equals(const TestIterator<const typename Dune::RemoveConst<C>::Type,const typename Dune::RemoveConst<T>::Type>& other) const
      {
        return position_ == other.position_ && container_ == other.container_;
      }

      T& dereference() const
      {
        return container_->values_[position_];
      }

      void increment()
      {
     ++position_;
      }

      // Additional function needed by BidirectionalIterator
      void decrement()
     {
        --position_;
      }

      // Additional function needed by RandomAccessIterator
      T& elementAt(int i)const
      {
        return container_->operator[](position_+i);
      }

      void advance(int n)
      {
        position_=position_+n;
      }

      std::ptrdiff_t distanceTo(TestIterator<const typename Dune::RemoveConst<C>::Type,const typename Dune::RemoveConst<T>::Type> other) const
      {
        assert(other.container_==container_);
        return other.position_ - position_;
      }

      std::ptrdiff_t distanceTo(TestIterator<const typename Dune::RemoveConst<C>::Type, typename Dune::RemoveConst<T>::Type> other) const
      {
        assert(other.container_==container_);
        return other.position_ - position_;
      }
     private:
      C *container_;
      size_t position_;
     };

     \endcode
     See dune/common/test/iteratorbase.hh for details.
   */


  /**
   * @file
   * @brief This file implements iterator facade classes for writing stl conformant iterators.
   *
   * With using these facades writing iterators for arbitrary containers becomes much less
   * cumbersome as only few functions have to be implemented. All other functions needed by
   * the stl are provided by the facades using the Barton-Nackman trick (also known as
   * curiously recurring template pattern.
   */

  /** @addtogroup IteratorFacades
   *
   * @{
   */
  /**
   * @brief Base class for stl conformant forward iterators.
   *
   */
  template<class T, class V, class R = V&, class D = std::ptrdiff_t>
  class ForwardIteratorFacade :
    public std::iterator< std::forward_iterator_tag,
        typename RemoveConst<V>::Type,                   // std::iterator needs mutable value type
        D,
        V*,
        R>
  {

  public:
    /**
     * @brief The type of derived iterator.
     *
     * The iterator has to define following
     * functions have to be present:
     *
     * <pre>
     *
     * // Access the value referred to.
     * Reference dereference() const;
     *
     * // Compare for equality with iterator j
     * equals(i);
     *
     * // position the iterator at the next element.
     * void increment()
     * </pre>
     *
     * For an elaborate explanation see the STL Documentation
     * @url{http://www.sgi.com/tech/stl/iterator_traits.html}
     */
    typedef T DerivedType;

    /**
     * @brief The type of value accessed through the iterator.
     */
    typedef V Value;

    /**
     * @brief The pointer to the Value.
     */
    typedef V* Pointer;

    /**
     * @brief The type of the difference between two positions.
     */
    typedef D DifferenceType;

    /**
     * @brief The type of the reference to the values accessed.
     */
    typedef R Reference;

    /** @brief Dereferencing operator. */
    Reference operator*() const
    {
      return static_cast<DerivedType const*>(this)->dereference();
    }

    Pointer operator->() const
    {
      return &(static_cast<const DerivedType *>(this)->dereference());
    }

    /** @brief Preincrement operator. */
    DerivedType& operator++()
    {
      static_cast<DerivedType *>(this)->increment();
      return *static_cast<DerivedType *>(this);
    }

    /** @brief Postincrement operator. */
    DerivedType operator++(int)
    {
      DerivedType tmp(static_cast<DerivedType const&>(*this));
      this->operator++();
      return tmp;
    }
  };

  /**
   * @brief Checks for equality.
   *
   * This operation is only defined if either D2
   * is convertible to D1 or vice versa. If that is
   * not the case the compiler will report an error
   * as EnableIfInterOperable<D1,D2,bool>::type is
   * not defined.
   *
   */
  template<class T1, class V1, class R1, class D,
      class T2, class V2, class R2>
  typename EnableIfInterOperable<T1,T2,bool>::type
  operator==(const ForwardIteratorFacade<T1,V1,R1,D>& lhs,
             const ForwardIteratorFacade<T2,V2,R2,D>& rhs)
  {
    if(Conversion<T2,T1>::exists)
      return static_cast<const T1&>(lhs).equals(static_cast<const T2&>(rhs));
    else
      return static_cast<const T2&>(rhs).equals(static_cast<const T1&>(lhs));
  }

  /**
   * @brief Checks for inequality.
   *
   * This operation is only defined if either D2
   * is convertible to D1 or vice versa. If that is
   * not the case the compiler will report an error
   * as EnableIfInterOperable<D1,D2,bool>::type is
   * not defined.
   *
   */
  template<class T1, class V1, class R1, class D,
      class T2, class V2, class R2>
  typename EnableIfInterOperable<T1,T2,bool>::type
  operator!=(const ForwardIteratorFacade<T1,V1,R1,D>& lhs,
             const ForwardIteratorFacade<T2,V2,R2,D>& rhs)
  {
    if(Conversion<T2,T1>::exists)
      return !static_cast<const T1&>(lhs).equals(static_cast<const T2&>(rhs));
    else
      return !static_cast<const T2&>(rhs).equals(static_cast<const T1&>(lhs));
  }

  /**
   * @brief Facade class for stl conformant bidirectional iterators.
   *
   */
  template<class T, class V, class R = V&, class D = std::ptrdiff_t>
  class BidirectionalIteratorFacade :
    public std::iterator< std::bidirectional_iterator_tag,
        typename RemoveConst<V>::Type,                   // std::iterator needs mutable value type
        D,
        V*,
        R>
  {

  public:
    /**
     * @brief The type of derived iterator.
     *
     * The iterator has to define following
     * functions have to be present:
     *
     * <pre>
     *
     * // Access the value referred to.
     * Reference dereference() const;
     *
     * // Compare for equality with j
     * equals(i);
     *
     * // position the iterator at the next element.
     * void increment()
     *
     * // position the iterator at the previous element.
     * void decrement()
     *
     * </pre>
     *
     * For an elaborate explanation see the STL Documentation
     * @url{http://www.sgi.com/tech/stl/iterator_traits.html}
     */
    typedef T DerivedType;

    /**
     * @brief The type of value accessed through the iterator.
     */
    typedef V Value;

    /**
     * @brief The pointer to the Value.
     */
    typedef V* Pointer;

    /**
     * @brief The type of the difference between two positions.
     */
    typedef D DifferenceType;

    /**
     * @brief The type of the reference to the values accessed.
     */
    typedef R Reference;

    /** @brief Dereferencing operator. */
    Reference operator*() const
    {
      return static_cast<DerivedType const*>(this)->dereference();
    }

    Pointer operator->() const
    {
      return &(static_cast<const DerivedType *>(this)->dereference());
    }

    /** @brief Preincrement operator. */
    DerivedType& operator++()
    {
      static_cast<DerivedType *>(this)->increment();
      return *static_cast<DerivedType *>(this);
    }

    /** @brief Postincrement operator. */
    DerivedType operator++(int)
    {
      DerivedType tmp(static_cast<DerivedType const&>(*this));
      this->operator++();
      return tmp;
    }


    /** @brief Preincrement operator. */
    DerivedType& operator--()
    {
      static_cast<DerivedType *>(this)->decrement();
      return *static_cast<DerivedType *>(this);
    }

    /** @brief Postincrement operator. */
    DerivedType operator--(int)
    {
      DerivedType tmp(static_cast<DerivedType const&>(*this));
      this->operator--();
      return tmp;
    }
  };

  /**
   * @brief Checks for equality.
   *
   * This operation is only defined if either D2
   * is convertible to D1 or vice versa. If that is
   * not the case the compiler will report an error
   * as EnableIfInterOperable<D1,D2,bool>::type is
   * not defined.
   *
   */
  template<class T1, class V1, class R1, class D,
      class T2, class V2, class R2>
  typename EnableIfInterOperable<T1,T2,bool>::type
  operator==(const BidirectionalIteratorFacade<T1,V1,R1,D>& lhs,
             const BidirectionalIteratorFacade<T2,V2,R2,D>& rhs)
  {
    if(Conversion<T2,T1>::exists)
      return static_cast<const T1&>(lhs).equals(static_cast<const T2&>(rhs));
    else
      return static_cast<const T2&>(rhs).equals(static_cast<const T1&>(lhs));
  }

  /**
   * @brief Checks for inequality.
   *
   * This operation is only defined if either D2
   * is convertible to D1 or vice versa. If that is
   * not the case the compiler will report an error
   * as EnableIfInterOperable<D1,D2,bool>::type is
   * not defined.
   *
   */
  template<class T1, class V1, class R1, class D,
      class T2, class V2, class R2>
  typename EnableIfInterOperable<T1,T2,bool>::type
  operator!=(const BidirectionalIteratorFacade<T1,V1,R1,D>& lhs,
             const BidirectionalIteratorFacade<T2,V2,R2,D>& rhs)
  {
    if(Conversion<T2,T1>::exists)
      return !static_cast<const T1&>(lhs).equals(static_cast<const T2&>(rhs));
    else
      return !static_cast<const T2&>(rhs).equals(static_cast<const T1&>(lhs));
  }

  /**
   * @brief Base class for stl conformant forward iterators.
   *
   */
  template<class T, class V, class R = V&, class D = std::ptrdiff_t>
  class RandomAccessIteratorFacade :
    public std::iterator< std::random_access_iterator_tag,
        typename RemoveConst<V>::Type,                   // std::iterator needs mutable value type
        D,
        V*,
        R>
  {

  public:
    /**
     * @brief The type of derived iterator.
     *
     * The iterator has to define following
     * functions have to be present:
     *
     * <pre>
     *
     * // Access the value referred to.
     * Reference dereference() const;
     *
     * // Compare for equality with j
     * equals(i);
     *
     * // position the iterator at the next element.
     * void increment()
     *
     * // position the iterator at the previous element.
     * void decrement()
     *
     * // advance the iterator by a number of positions-
     * void advance(DifferenceType n);
     * // calculate the distance to another iterator.
     * // One should incorporate an assertion wether
     * // the same containers are referenced
     * DifferenceType distanceTo(j) const;
     * </pre>
     *
     * For an elaborate explanation see the STL Documentation
     * @url{http://www.sgi.com/tech/stl/iterator_traits.html}
     */
    typedef T DerivedType;

    /**
     * @brief The type of value accessed through the iterator.
     */
    typedef V Value;

    /**
     * @brief The pointer to the Value.
     */
    typedef V* Pointer;

    /**
     * @brief The type of the difference between two positions.
     */
    typedef D DifferenceType;

    /**
     * @brief The type of the reference to the values accessed.
     */
    typedef R Reference;

    /** @brief Dereferencing operator. */
    Reference operator*() const
    {
      return static_cast<DerivedType const*>(this)->dereference();
    }

    Pointer operator->() const
    {
      return &(static_cast<const DerivedType *>(this)->dereference());
    }

    /**
     * @brief Get the element n positions from the current one.
     * @param n The distance to the element.
     * @return The element at that distance.
     */
    Reference operator[](DifferenceType n) const
    {
      return static_cast<const DerivedType *>(this)->elementAt(n);
    }

    /** @brief Preincrement operator. */
    DerivedType& operator++()
    {
      static_cast<DerivedType *>(this)->increment();
      return *static_cast<DerivedType *>(this);
    }

    /** @brief Postincrement operator. */
    DerivedType operator++(int)
    {
      DerivedType tmp(static_cast<DerivedType const&>(*this));
      this->operator++();
      return tmp;
    }

    DerivedType& operator+=(DifferenceType n)
    {
      static_cast<DerivedType *>(this)->advance(n);
      return *static_cast<DerivedType *>(this);
    }

    DerivedType operator+(DifferenceType n)
    {
      DerivedType tmp(static_cast<DerivedType const&>(*this));
      tmp.advance(n);
      return tmp;
    }


    /** @brief Predecrement operator. */
    DerivedType& operator--()
    {
      static_cast<DerivedType *>(this)->decrement();
      return *static_cast<DerivedType *>(this);
    }

    /** @brief Postdecrement operator. */
    DerivedType operator--(int)
    {
      DerivedType tmp(static_cast<DerivedType const&>(*this));
      this->operator--();
      return tmp;
    }

    DerivedType& operator-=(DifferenceType n)
    {
      static_cast<DerivedType *>(this)->advance(-n);
      return *static_cast<DerivedType *>(this);
    }

    DerivedType operator-(DifferenceType n)
    {
      DerivedType tmp(static_cast<DerivedType const&>(*this));
      tmp.advance(-n);
      return tmp;
    }


  };

  /**
   * @brief Checks for equality.
   *
   * This operation is only defined if either D2
   * is convertible to D1 or vice versa. If that is
   * not the case the compiler will report an error
   * as EnableIfInterOperable<D1,D2,bool>::type is
   * not defined.
   *
   */
  template<class T1, class V1, class R1, class D,
      class T2, class V2, class R2>
  typename EnableIfInterOperable<T1,T2,bool>::type
  operator==(const RandomAccessIteratorFacade<T1,V1,R1,D>& lhs,
             const RandomAccessIteratorFacade<T2,V2,R2,D>& rhs)
  {
    if(Conversion<T2,T1>::exists)
      return static_cast<const T1&>(lhs).equals(static_cast<const T2&>(rhs));
    else
      return static_cast<const T2&>(rhs).equals(static_cast<const T1&>(lhs));
  }

  /**
   * @brief Checks for inequality.
   *
   * This operation is only defined if either D2
   * is convertible to D1 or vice versa. If that is
   * not the case the compiler will report an error
   * as EnableIfInterOperable<D1,D2,bool>::type is
   * not defined.
   *
   */
  template<class T1, class V1, class R1, class D,
      class T2, class V2, class R2>
  typename EnableIfInterOperable<T1,T2,bool>::type
  operator!=(const RandomAccessIteratorFacade<T1,V1,R1,D>& lhs,
             const RandomAccessIteratorFacade<T2,V2,R2,D>& rhs)
  {
    if(Conversion<T2,T1>::exists)
      return !static_cast<const T1&>(lhs).equals(static_cast<const T2&>(rhs));
    else
      return !static_cast<const T2&>(rhs).equals(static_cast<const T1&>(lhs));
  }

  /**
   * @brief Comparison operator.
   *
   * This operation is only defined if either D2
   * is convertible to D1 or vice versa. If that is
   * not the case the compiler will report an error
   * as EnableIfInterOperable<D1,D2,bool>::type is
   * not defined.
   *
   */
  template<class T1, class V1, class R1, class D,
      class T2, class V2, class R2>
  typename EnableIfInterOperable<T1,T2,bool>::type
  operator<(const RandomAccessIteratorFacade<T1,V1,R1,D>& lhs,
            const RandomAccessIteratorFacade<T2,V2,R2,D>& rhs)
  {
    if(Conversion<T2,T1>::exists)
      return static_cast<const T1&>(lhs).distanceTo(static_cast<const T2&>(rhs))>0;
    else
      return static_cast<const T2&>(rhs).distanceTo(static_cast<const T1&>(lhs))<0;
  }


  /**
   * @brief Comparison operator.
   *
   * This operation is only defined if either D2
   * is convertible to D1 or vice versa. If that is
   * not the case the compiler will report an error
   * as EnableIfInterOperable<D1,D2,bool>::type is
   * not defined.
   *
   */
  template<class T1, class V1, class R1, class D,
      class T2, class V2, class R2>
  typename EnableIfInterOperable<T1,T2,bool>::type
  operator<=(const RandomAccessIteratorFacade<T1,V1,R1,D>& lhs,
             const RandomAccessIteratorFacade<T2,V2,R2,D>& rhs)
  {
    if(Conversion<T2,T1>::exists)
      return static_cast<const T1&>(lhs).distanceTo(static_cast<const T2&>(rhs))>=0;
    else
      return static_cast<const T2&>(rhs).distanceTo(static_cast<const T1&>(lhs))<=0;
  }


  /**
   * @brief Comparison operator.
   *
   * This operation is only defined if either D2
   * is convertible to D1 or vice versa. If that is
   * not the case the compiler will report an error
   * as EnableIfInterOperable<D1,D2,bool>::type is
   * not defined.
   *
   */
  template<class T1, class V1, class R1, class D,
      class T2, class V2, class R2>
  typename EnableIfInterOperable<T1,T2,bool>::type
  operator>(const RandomAccessIteratorFacade<T1,V1,R1,D>& lhs,
            const RandomAccessIteratorFacade<T2,V2,R2,D>& rhs)
  {
    if(Conversion<T2,T1>::exists)
      return static_cast<const T1&>(lhs).distanceTo(static_cast<const T2&>(rhs))<0;
    else
      return static_cast<const T2&>(rhs).distanceTo(static_cast<const T1&>(lhs))>0;
  }

  /**
   * @brief Comparison operator.
   *
   * This operation is only defined if either D2
   * is convertible to D1 or vice versa. If that is
   * not the case the compiler will report an error
   * as EnableIfInterOperable<D1,D2,bool>::type is
   * not defined.
   *
   */
  template<class T1, class V1, class R1, class D,
      class T2, class V2, class R2>
  typename EnableIfInterOperable<T1,T2,bool>::type
  operator>=(const RandomAccessIteratorFacade<T1,V1,R1,D>& lhs,
             const RandomAccessIteratorFacade<T2,V2,R2,D>& rhs)
  {
    if(Conversion<T2,T1>::exists)
      return static_cast<const T1&>(lhs).distanceTo(static_cast<const T2&>(rhs))<=0;
    else
      return static_cast<const T2&>(rhs).distanceTo(static_cast<const T1&>(lhs))>=0;
  }

  /**
   * @brief Calculates the difference between two pointers.
   *
   * This operation is only defined if either D2
   * is convertible to D1 or vice versa. If that is
   * not the case the compiler will report an error
   * as EnableIfInterOperable<D1,D2,bool>::type is
   * not defined.
   *
   */
  template<class T1, class V1, class R1, class D,
      class T2, class V2, class R2>
  typename EnableIfInterOperable<T1,T2,D>::type
  operator-(const RandomAccessIteratorFacade<T1,V1,R1,D>& lhs,
            const RandomAccessIteratorFacade<T2,V2,R2,D>& rhs)
  {
    if(Conversion<T2,T1>::exists)
      return -static_cast<const T1&>(lhs).distanceTo(static_cast<const T2&>(rhs));
    else
      return static_cast<const T2&>(rhs).distanceTo(static_cast<const T1&>(lhs));
  }

  /** @} */
}
#endif
