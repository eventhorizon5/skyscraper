/* $Id$ */

#ifndef _SBS_RANDOM_H
#define _SBS_RANDOM_H

namespace SBS {

class SBSIMPEXP RandomGen
{
public:
	RandomGen();
	RandomGen(unsigned int iSeed);
	void Initialize();
	void Initialize(unsigned int iSeed);
	float Get();
	unsigned int Get(unsigned int iLimit);
	bool SelfTest(); //perform self test

private:
	int i97, j97;
	float u[98];
	float c, cd, cm;
	void InitRANMAR(unsigned int ij, unsigned int kl); //initialize
	float RANMAR(); //get next number in sequence
};

}

#endif
