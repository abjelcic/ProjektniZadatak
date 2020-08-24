#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include <iostream>
#include <map>
#include <string>
#include <vector>

#include <dune/common/exceptions.hh>
#include <dune/common/parallel/mpihelper.hh>

#include <dune/grid/io/file/gmshreader.hh>
#include <dune/grid/uggrid.hh>

#include <dune/common/parametertreeparser.hh>

#include <dune/alugrid/dgf.hh>
#include <dune/alugrid/grid.hh>
#include <dune/grid/io/file/dgfparser/dgfparser.hh>

#include "bc_extension.hpp"
#include "driver.hpp"


//std::string GmshFile = "src_dir/cavity.msh";
std::string GmshFile = "src_dir/nozzle.msh";
double U0;
double tau;


int main(int argc, char **argv) {

#if !HAVE_SUPERLU
  std::cerr << "Error: These examples work only if SuperLU is available."
            << std::endl;
  exit(1);
#endif
  //Dune::MPIHelper &helper = Dune::MPIHelper::instance(argc, argv);
  Dune::MPIHelper::instance(argc, argv);
  // Initialize Navier Stokes parameter class from file
  Dune::ParameterTree params;

  std::string input_filename("src_dir/params.input");

  std::cout << "Reading input file \"" << input_filename << "\"" << std::endl;
  try {
    Dune::ParameterTreeParser::readINITree(input_filename, params);
  } catch (...) {
    std::cerr << "The configuration file \"cgstokes.ini\" "
                 "could not be read. Exiting..."
              << std::endl;
    exit(1);
  }

  // 1. Konstrukcija mreže. Koristimo  UG Grid i L-shape domenu u 2D
  const int dim = 2;
  //typedef Dune::UGGrid<dim> GridType;
  using GridType = Dune::ALUGrid<dim, dim, Dune::simplex, Dune::conforming>;
  typedef double RF;

  // Pročitaj sve parametre iz ulazne datoteke
  int refinement = params.get<int>("domain.level");
  std::string outf = params.get<std::string>("OutputFile"); // izlazni VTK file
  RF mu = params.get<double>("physics.mu");
  RF rho = params.get<double>("physics.rho");
  RF dt = params.get<double>("time.dt");
  RF tfin = params.get<double>("time.tfin");
  tau = params.get<double>("physics.tau");
  U0 = params.get<double>("physics.U0");

  // Mreža se čita iz grids/lshape.msh datoteke
  GridType *pgrid = Dune::GmshReader<GridType>::read(
      GmshFile, true, true);


  pgrid->globalRefine(refinement);
  typedef GridType::LeafGridView GV;
  const GV &gv = pgrid->leafGridView();

  // 2. Konstrukcija klase paramatara
  // Dune::PDELab::NavierStokesDefaultParameters.
  //
  typedef ZeroFunction<GV, RF, 1> BdryPressure;
  typedef Velocity<GV, RF, 2> BdryVelocity;

  typedef Dune::PDELab::CompositeGridFunction<BdryVelocity, BdryPressure>
      BdrySolution;

  BdryVelocity bdry_velocity(gv);
  BdryPressure bdry_pressure(gv);
  BdrySolution bdry_solution(bdry_velocity, bdry_pressure);

  BCTypeParam bdry_type;

  typedef ZeroFunction<GV, RF, dim> SourceFunction;
  typedef SourceFunction
      NeumannFlux; // artificijelna definicija. Nemamo Neumannovog rubnog uvjeta
  NeumannFlux neumann_flux(gv); // Ne koristi se ali mora biti prisutan
  SourceFunction source_function(gv);

  const bool navier =
      true; // Treba li asemblirati nelinearni konvektivni član ?
  const bool tensor =
      true; // Treba li raditi sa simetriziranim gradijentom ili ne
            // (utječe na interpretaciju rubnih uvjeta)
  typedef Dune::PDELab::NavierStokesDefaultParameters<
      GV, RF, SourceFunction, BCTypeParam, BdrySolution, NeumannFlux, navier,
      tensor>
      LOPParameters;

  LOPParameters parameters(mu, rho, source_function, bdry_type, bdry_solution,
                           neumann_flux);



  // solve problem
  driver<GV, BdrySolution, LOPParameters>(gv, outf, parameters, bdry_solution,
                                          dt, tfin);

  return 0;
}
