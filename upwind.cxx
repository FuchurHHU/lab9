#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cmath>
//---------------------------------------
using namespace std;
//---------------------------------------
void writeToFile(const double* const u, const string s, const double dx,
                 const double xmin, const int N);
void initialize(double* const u, const double dx, const double xmin,
                const int N);
void upwind(const double* const u0, double* const u1, const double V, double dt, const double dx, const int N);
//---------------------------------------
int main(){

  const double tEnd = 5;
  const double V = 1.0;

  const int N  = 256;
  const double xmin = -10;
  const double xmax =  10;
  const double dx = (xmax-xmin)/(N-1);
  double dt = dx/V *0.5;
  const int Na = 10; // Number of output files up to tEnd
  const int Nk = int(tEnd/Na/dt);

  double* u0 = new double[N];
  double* u1 = new double[N];
  double* h;

  stringstream strm;

  initialize(u0,dx, xmin,N);

  writeToFile(u0, "u_0", dx, xmin, N);

  for(int i=1; i<=Na; i++)
  {
   for(int j=0; j<Nk; j++){

      // Put call to step function here
     upwind(u0,u1,V,dt,dx,N);

      // swap arrays u0 <-> u1,
      // however do not copy values, be more clever ;)
     h  = u1;
     u1 = u0;
     u0 = h;
     //wir hatten das erst in unterfunktion, aber das hat nicht funktioniert, da die pointer erst kopiert und dann nur die kopien ausgetauscht werden. wie die pointer selbst wirklich übergeben werden ist nicht ganz klar(auch götz momentan noch nicht)
     
  }
   strm.str("");
   strm << "u_" << i;
   writeToFile(u0, strm.str(), dx, xmin, N);
  }


  delete[] u0;
  delete[] u1;
  return 0;
}
//-----------------------------------------------

//-----------------------------------------------
void initialize(double* const u, const double dx, const double xmin,
                const int N)
{
   for(int i=0; i<N; i++)
   {
     double x = xmin + i*dx;
     if (fabs(x)<=1.0)
       u[i] = 1;
     else
      u[i] =0;
   }
}
//-----------------------------------------------
void writeToFile(const double* const u, const string s, const double dx,
                 const double xmin, const int N)
{
   ofstream out(s.c_str());
   for(int i=0; i<N; i++){
     double x = xmin + i * dx;
     out << x << "\t" << u[i] << endl;
   }
   out.close();
}
//-----------------------------------------------
void upwind(const double* const u0, double* const u1, const double V, double dt, const double dx, const int N)
{
  u1[0] = -V*dt * u0[0]/dx + u0[0];
  
  for(int i=1; i<N; i++){
    u1[i] = -V*dt * (u0[i]-u0[i-1])/dx + u0[i]; 
  }
}
