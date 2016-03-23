#define BOOST_TEST_MODULE SMT2Parser_Boolector
#include <metaSMT/support/default_visitation_unrolling_limit.hpp>
#include <metaSMT/DirectSolver_Context.hpp>
#include <metaSMT/API/Stack.hpp>
#include <metaSMT/API/Options.hpp>
#include <metaSMT/backend/SMT2.hpp>

using namespace metaSMT;
using namespace metaSMT::solver;
struct Solver_Fixture {
public:
  typedef DirectSolver_Context< SMT2 > ContextType;
  Solver_Fixture() {
    set_option(ctx, "solver_executable", "smt2InputEvaluator_Boolector");
    set_option(ctx, "solver_arguments", "");
  }
  ContextType ctx;
};

#include "test_solver.cpp"
#include "test_QF_BV.cpp"
