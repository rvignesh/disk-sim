#ifndef GENZIP_H_INCLUDED
#define GENZIP_H_INCLUDED


//----- Function prototypes -------------------------------------------------
int      zipf(double alpha, int n);  // Returns a Zipf random variable
double   rand_val(int seed);         // Jain's RNG
int*  generate_zipf(int num_values,double alpha, double n,int seed );


#endif // GENZIP_H_INCLUDED
