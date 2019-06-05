#define BOOST_TEST_MODULE direct_SWORD
#include <metaSMT/DirectSolver_Context.hpp>
#include <metaSMT/backend/SWORD_Backend.hpp>

using namespace metaSMT::solver;
using namespace metaSMT;
struct Solver_Fixture {
  typedef DirectSolver_Context < SWORD_Backend > ContextType;
  ContextType ctx ;
};

#include "test_solver.cpp"
#include "test_QF_BV.cpp"
#include "test_unsat.cpp"
#include "test_cardinality.cpp"
