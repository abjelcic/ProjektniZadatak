#ifndef CG_STOKES_INITIAL_HH
#define CG_STOKES_INITIAL_HH

#include <dune/common/fvector.hh>
#include <cmath>
#include <dune/pdelab/localoperator/stokesparameter.hh>
#include <dune/pdelab/common/function.hh>

//===============================================================
// Define parameter functions f,g,j and \partial\Omega_D/N
//===============================================================
extern std::string GmshFile;
extern double U0;
extern double tau;


// Klasa koja određuje tip granice
class BCTypeParam
{
private:
    double time;
public:
    // Ova klasa daje indekse: DoNothing i VelocityDirichlet i StressNeumann
  typedef Dune::PDELab::StokesBoundaryCondition BC;

  struct Traits
  {
    typedef BC::Type RangeType;
    typedef Dune::PDELab::StokesBoundaryCondition  BoundaryCondition;
  };

  // Domena se po osi x prostire od -1 do 5. Na granici x=5 ne postavljamo nikakve uvjete
  // (= homogeni Neumannovi uvjeti). Na ostatku granice imamo Dirichletove uvjete na brzinu.
  template<typename I>
  inline void evaluate (const I & intersection,
                        const Dune::FieldVector<typename I::ctype, I::coorddimension-1> & coord,
                        BC::Type& y) const
  {
    Dune::FieldVector<typename I::ctype, I::coorddimension>
        xg = intersection.geometry().global( coord );

    auto xx = xg[0];
    auto yy = xg[1];



    if( GmshFile == "src_dir/cavity.msh" )
    {
        if( std::abs(xx-(1.0+1.0/7.0))<1.e-6 )//&& std::abs(yy-(1.0-1.5/7.0))<0.5/7.0  )
            y = BC::DoNothing;
        else
            y = BC::VelocityDirichlet;
    }
    else if( GmshFile == "src_dir/nozzle.msh" )
    {
        if( std::abs(xx-18.268512728)<1.e-6 )
            y = BC::DoNothing;
        else
            y = BC::VelocityDirichlet;
    }
    else
    {
        std::cout << "gmshfile uknown..." << std::endl;
        exit(1);
    }



  }
  template <typename T>
  void setTime(T t){
    time = t;
  }
};


// Inicijalni i Dirichletov  rubni uvjet za brzinu
template<typename GV, typename RF, int dim>
class Velocity :
  public Dune::PDELab::AnalyticGridFunctionBase<
                             Dune::PDELab::AnalyticGridFunctionTraits<GV,RF,dim>,
                             Velocity<GV,RF,dim>
                                               >
{
private:
  RF time;

public:
  typedef Dune::PDELab::AnalyticGridFunctionTraits<GV,RF,dim> Traits;
  typedef Dune::PDELab::AnalyticGridFunctionBase<Traits,Velocity<GV,RF,dim> > BaseT;

  typedef typename Traits::DomainType DomainType;
  typedef typename Traits::RangeType RangeType;

  Velocity(const GV & gv) : BaseT(gv) {
    time = 0.0;
  }

  inline void evaluateGlobal(const DomainType & x, RangeType & y) const
  {

    auto xx = x[0];
    auto yy = x[1];

    y[0] = 0.0;
    y[1] = 0.0;


    auto u0 = (time<tau) ? std::sin(M_PI*time/(2*tau))*std::sin(M_PI*time/(2*tau)) : 1.0;
    u0 *= U0;

    if( GmshFile == "src_dir/cavity.msh" )
    {
        if( std::abs(xx-1.5/7.0)<0.5/7.0 && std::abs(yy-(-1.0/7.0))<1.e-6 )
        {
            auto xxx = xx-1.5/7.0;
            y[1] = u0 * ((0.5/7.0-xxx)/(0.5/0.7))*((0.5/7.0+xxx)/(0.5/0.7));
        }
    }
    else if( GmshFile == "src_dir/nozzle.msh" )
    {
        if( std::abs(xx)<1.e-6 )
        {
            auto yyy = yy;
            y[0] = u0 * ((0.6-yyy)/(0.6))*((0.6+yyy)/(0.6));
        }
    }
    else
    {
        std::cout << "gmshfile uknown..." << std::endl;
        exit(1);
    }




  }

  template <typename T>
  void setTime(T t){
    time = t;
  }

};




// Vektorska funkcija jednaka nuli. Ovdje je koristimo  za tlak (dim_range=1)
// i za Neumannov rubni uvjet (dim_range=dim) koji nije prisutan pa ga stavljamo na nulu.
// Rubni uvjet za tlak jednako tako ne postoji i stoga vrijednost tlaka stavljamo na nulu.
template<typename GV, typename RF, std::size_t dim_range>
class ZeroFunction :
  public Dune::PDELab::AnalyticGridFunctionBase<
                            Dune::PDELab::AnalyticGridFunctionTraits<GV,RF,dim_range>,
                            ZeroFunction<GV,RF,dim_range>
                                               >,
  public Dune::PDELab::InstationaryFunctionDefaults
{
public:
  typedef Dune::PDELab::AnalyticGridFunctionTraits<GV,RF,dim_range> Traits;
  typedef Dune::PDELab::AnalyticGridFunctionBase<Traits, ZeroFunction> BaseT;

  typedef typename Traits::DomainType DomainType;
  typedef typename Traits::RangeType RangeType;

  ZeroFunction(const GV & gv) : BaseT(gv) {}

  inline void evaluateGlobal(const DomainType & x, RangeType & y) const
  {
    y=0;
  }
};



#endif
