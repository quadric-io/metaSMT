include(CMakeParseArguments)

include_directories(../src ${metaSMT_INCLUDES})

add_library(boost_test boost_test.cpp)
link_libraries(boost_test metaSMT)

find_program(AWK awk)

function (add_all_tests prog cfile)
  cmake_parse_arguments(opts "" "" "ENVIRONMENT;ARGS" ${ARGN})
  execute_process( COMMAND
    ${AWK} -f search_tests.awk ${cfile}
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    OUTPUT_VARIABLE test_cases
  )

  string(REPLACE "\n" ";" test_cases "${test_cases}")

  foreach(test  ${test_cases})
    #message(STATUS "${prog}/${test} ${prog} -t ${test}")
    add_test("${prog}/${test}" ${prog} -t ${test} ${opts_ARGS})
    set_tests_properties ("${prog}/${test}" PROPERTIES TIMEOUT ${TIMEOUT})
    if(opts_ENVIRONMENT)
      set_tests_properties("${prog}/${test}" PROPERTIES ENVIRONMENT ${opts_ENVIRONMENT})
    endif()
  endforeach(test)
endfunction(add_all_tests)

function (add_test_executable name source )
  cmake_parse_arguments(req "" "" "REQUIRES;PROPERTIES;ARGS;LIBRARIES;ENVIRONMENT" ${ARGN})

  set(req_all 1)

  foreach( req ${req_REQUIRES} )
    if(NOT ${req} )
      set(req_all 0)
      # message("UNSATISFIED DEPENDENCY: " ${req} " of " ${name})
    endif(NOT ${req} )
  endforeach( req )

  if(req_all)
    add_executable(${name} ${source})
    add_all_tests(${name} ${source} ARGS ${req_ARGS} ENVIRONMENT ${req_ENVIRONMENT})
    if(req_LIBRARIES)
      target_link_libraries(${name} metaSMT ${req_LIBRARIES})
    endif()
    if(req_PROPERTIES)
      set_target_properties(${name} PROPERTIES ${req_PROPERTIES})
    endif()
  endif(req_all)
endfunction(add_test_executable)

add_test_executable( result_wrapper test_result_wrapper.cpp)

add_test_executable( direct_SWORD direct_SWORD2.cpp REQUIRES SWORD_FOUND )

add_test_executable( direct_CUDD direct_CUDD.cpp REQUIRES CUDD_FOUND )
add_test_executable( direct_BitBlast_CUDD direct_BitBlast_CUDD.cpp
  REQUIRES CUDD_FOUND)
add_test_executable(direct_CUDD_Distributed direct_CUDD_Distributed.cpp 
  REQUIRES CUDD_FOUND)

add_test_executable( direct_Boolector direct_Boolector.cpp
  REQUIRES Boolector_FOUND )

add_test_executable( SMT2Parser_Boolector SMT2Parser_Boolector.cpp
  REQUIRES Z3_FOUND Boolector_FOUND
  LIBRARIES ${Boost_IOSTREAMS_LIBRARY} 
  )

add_test_executable( SMT2Parser_SMT2 SMT2Parser_SMT2.cpp
  REQUIRES Z3_FOUND
  LIBRARIES ${Boost_IOSTREAMS_LIBRARY} 
  )

add_test_executable( SMT2Parser_STP SMT2Parser_STP.cpp
  REQUIRES Z3_FOUND STP_FOUND
  LIBRARIES ${Boost_IOSTREAMS_LIBRARY} 
  )

add_test_executable( SMT2Parser_Z3_Backend SMT2Parser_Z3_Backend.cpp
  REQUIRES Z3_FOUND
  LIBRARIES ${Boost_IOSTREAMS_LIBRARY} 
  )

add_test_executable( direct_Z3 direct_Z3.cpp REQUIRES Z3_FOUND )

add_test_executable( direct_STP direct_STP.cpp REQUIRES STP_FOUND )

add_test_executable( direct_CVC4 direct_CVC4.cpp REQUIRES CVC4_FOUND )

add_test_executable( direct_MiniSAT direct_MiniSAT.cpp
  REQUIRES MiniSat_FOUND
  PROPERTIES COMPILE_FLAGS "${MiniSat_CXXFLAGS}"
  )

add_test_executable( direct_PicoSAT direct_PicoSAT.cpp
  REQUIRES PicoSAT_FOUND
)

add_test_executable( direct_Lingeling direct_Lingeling.cpp
  REQUIRES Lingeling_FOUND
)

add_test_executable( direct_Aiger_MiniSAT direct_Aiger_MiniSAT.cpp
  REQUIRES MiniSat_FOUND Aiger_FOUND
  PROPERTIES COMPILE_FLAGS "${MiniSat_CXXFLAGS}"
  )

add_test_executable( direct_Aiger_PicoSAT direct_Aiger_PicoSAT.cpp
  REQUIRES PicoSAT_FOUND Aiger_FOUND )


if(metaSMT_USE_SMT2)
  find_program(Z3_EXECUTABLE z3)


  if (Z3_EXECUTABLE)
    execute_process(
      COMMAND echo "(check-sat) "
      COMMAND ${Z3_EXECUTABLE} -smt2 -in
      OUTPUT_VARIABLE _Z3_OUTPUT
      )
    if( _Z3_OUTPUT STREQUAL "sat\n")
      set( Z3_IS_INTERACTIVE 1)
    else()
      set( Z3_IS_INTERACTIVE 0)
    endif()
  endif()

  add_test_executable( direct_SMT2 direct_SMT2.cpp
    ARGS "--log_level=nothing"
    REQUIRES Z3_EXECUTABLE Z3_IS_INTERACTIVE
    LIBRARIES ${Boost_IOSTREAMS_LIBRARY}
    ENVIRONMENT "Z3_EXECUTABLE=${Z3_EXECUTABLE}"
  )

endif(metaSMT_USE_SMT2)

if(metaSMT_USE_ExprSolver)
  add_test_executable( direct_ExprSolver_Boolector direct_ExprSolver_Boolector.cpp
    REQUIRES Boolector_FOUND
    LIBRARIES ${Boost_IOSTREAMS_LIBRARY})
  add_test_executable( direct_ExprSolver_Z3 direct_ExprSolver_Z3.cpp
    REQUIRES Z3_FOUND
    LIBRARIES ${Boost_IOSTREAMS_LIBRARY})
  add_test_executable( direct_ExprSolver_SMT2 direct_ExprSolver_SMT2.cpp
    ARGS "--log_level=nothing"
    REQUIRES Z3_EXECUTABLE Z3_IS_INTERACTIVE
    LIBRARIES ${Boost_IOSTREAMS_LIBRARY}
    ENVIRONMENT "Z3_EXECUTABLE=${Z3_EXECUTABLE}"
  )
endif(metaSMT_USE_ExprSolver)
# vim: ts=2 sw=2 et
