#include <cstdlib>
#include <ctime>
#include <sys/time.h>
#include <sys/resource.h>
#include <iomanip>
#include <iostream>
#include "timeSurvey.h"
using namespace std;


static clock_t ct;
static struct rusage rp;
static struct timeval tp;
static struct timezone tzp;

double CTime()
{        
  clock_t time;

  time = clock();

  if(time==-1) return( (double) time);
  else return ( (double) time / (double) CLOCKS_PER_SEC );
}

void initializeTime()
{
  ct = clock();
  getrusage(RUSAGE_SELF, &rp);
  gettimeofday(&tp,&tzp);
}

double clockTime()
{ 
  
  struct timeval tk;
  struct timezone tzp;
  double daytime;
  
  gettimeofday(&tk, &tzp);
  
  daytime=(tk.tv_usec-tp.tv_usec)/1e6+tk.tv_sec-tp.tv_sec;
  
  return(daytime);
}

double CPUTime()
{ 
  
  struct rusage rk;
  double cputime;
  
  getrusage(RUSAGE_SELF, &rk);
  
  cputime = (rk.ru_utime.tv_usec-rp.ru_utime.tv_usec)/1e6;
  cputime += rk.ru_utime.tv_sec-rp.ru_utime.tv_sec;
  
  return(cputime);
}

void printTime()
{ 
  
  clock_t time;
  struct rusage rk;
  struct timeval tk;
  double stdtime, cputime,daytime;
  
  time = clock();
  getrusage(RUSAGE_SELF, &rk);
  gettimeofday(&tk, &tzp);
  
  stdtime = (double) (time - ct) / (double)  CLOCKS_PER_SEC ;

   cputime = ( rk.ru_utime.tv_usec - rp.ru_utime.tv_usec ) / 1.0e6 ;
  cputime += rk.ru_utime.tv_sec - rp.ru_utime.tv_sec;
  
  daytime = ( tk.tv_usec - tp.tv_usec ) / 1.0e6 + tk.tv_sec - tp.tv_sec ;
  
  cout << "-->Standard time: " << stdtime << endl;
  cout << setprecision(10) <<  "-->CPU time: " << cputime << endl;
  cout << "-->Day time: " << daytime << endl;
  
}
