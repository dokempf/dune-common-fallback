add_subdirectory(test)

dune_add_library("dunecommon"
  exceptions.cc
  parametertree.cc
  parametertreeparser.cc
  )

#install headers
install(FILES
        boundschecking.hh
        classname.hh
        densevector.hh
        dotproduct.hh
        exceptions.hh
        ftraits.hh
        fvector.hh
        genericiterator.hh
        iteratorfacades.hh
        math.hh
        matvectraits.hh
        parametertree.hh
        parametertreeparser.hh
        power.hh
        promotiontraits.hh
        typetraits.hh
        typeutilities.hh
        unused.hh
DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/dune/common)
