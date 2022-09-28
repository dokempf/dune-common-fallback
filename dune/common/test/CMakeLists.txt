include(DuneCMakeCompat)
include(DuneInstance)

dune_add_test(SOURCES fvectortest.cc
              LINK_LIBRARIES dunecommon
              LABELS quick)

dune_add_test(SOURCES parametertreetest.cc
              LINK_LIBRARIES dunecommon
              LABELS quick)
