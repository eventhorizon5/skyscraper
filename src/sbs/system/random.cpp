/*
    This random number generator originally appeared in "Toward a Universal
    Random Number Generator" by George Marsaglia and Arif Zaman.
    Florida State University Report: FSU-SCRI-87-50 (1987)

    It was later modified by F. James and published in "A Review of Pseudo-
    random Number Generators"

    THIS IS THE BEST KNOWN RANDOM NUMBER GENERATOR AVAILABLE.
        (However, a newly discovered technique can yield
          a period of 10^600. But that is still in the development stage.)

    It passes ALL of the tests for random number generators and has a period
    of 2^144, is completely portable (gives bit identical results on all
    machines with at least 24-bit mantissas in the floating point
    representation).

    The algorithm is a combination of a Fibonacci sequence (with lags of 97
    and 33, and operation "subtraction plus one, modulo one") and an
    "arithmetic sequence" (using subtraction).
*/

/*
    This C++ language version was written by Andrew Zabolotny, and was
    based on the C language version that was written by Jim Butler,
    which in turn was based on a FORTRAN program posted by David LaSalle
    of Florida State University.
*/

#include "globals.h"
#include "random.h"
#include <stdio.h>
#include <time.h>

namespace SBS {

RandomGen::RandomGen()
{
	Initialize();
}

RandomGen::RandomGen(unsigned int iSeed)
{
	//initialize with given seed
	Initialize(iSeed);
}

void RandomGen::Initialize()
{
	//initialize generator using time() as seed
	Initialize((unsigned int)time(0));
}

void RandomGen::Initialize(unsigned int iSeed)
{
	//select sequence number out of 942,438,978
	unsigned int ij = iSeed % 31329UL;
	unsigned int kl = (iSeed / 31329UL) % 30082UL;
	InitRANMAR(ij, kl);
}

void RandomGen::InitRANMAR(unsigned int ij, unsigned int kl)
{
	/*
		This is the initialization routine for the random number generator RANMAR()
		NOTE: The seed variables can have values between:    0 <= IJ <= 31328
													 0 <= KL <= 30081
		The random number sequences created by these two seeds are of sufficient
		length to complete an entire calculation with. For example, if several
		different groups are working on different parts of the same calculation,
		each group could be assigned its own IJ seed. This would leave each group
		with 30000 choices for the second seed. That is to say, this random
		number generator can create 900 million different subsequences -- with
		each subsequence having a length of approximately 10^30.
	*/

	int i, j, k, l, m;
	float s, t;

	i = (ij / 177) % 177 + 2;
	j = ij % 177 + 2;
	k = (kl / 169) % 178 + 1;
	l = kl % 169;

	for (int ii = 1; ii <= 97; ii++)
	{
		s = 0.0;
		t = 0.5;
		for (int jj = 1; jj <= 24; jj++)
		{
			m = (((i * j) % 179) * k) % 179;
			i = j;
			j = k;
			k = m;
			l = (53 * l + 1) % 169;
			if ((l * m) % 64 >= 32)
				s += t;
			t *= 0.5;
		}
		u[ii] = s;
	}

	c = 362436.0 / 16777216.0;
	cd = 7654321.0 / 16777216.0;
	cm = 16777213.0 / 16777216.0;

	i97 = 97;
	j97 = 33;
}

float RandomGen::RANMAR()
{
	float uni;			/* the random number itself */

	uni = u [i97] - u [j97];	/* difference between two [0..1] numbers */
	if (uni < 0.0)
		uni += 1.0;
	u [i97] = uni;
	i97--;			/* i97 ptr decrements and wraps around */
	if (i97 == 0)
		i97 = 97;
	j97--;			/* j97 ptr decrements likewise */
	if (j97 == 0)
		j97 = 97;
	c -= cd;			/* finally, condition with c-decrement */
	if (c < 0.0)
		c += cm;		/* cm > cd we hope! (only way c always >0) */
	uni -= c;
	if (uni < 0.0)
		uni += 1.0;

	return (uni);			/* return the random number */
}

float RandomGen::Get()
{
	//get a floating point random number less than 1
	return RANMAR();
}

unsigned int RandomGen::Get(unsigned int iLimit)
{
	//get a random number less than iLimit
	return int(Get() * iLimit);
}

bool RandomGen::SelfTest ()
{
	/*
		Use IJ = 1802 & KL = 9373 to test the random number generator. The
		subroutine RANMAR should be used to generate 20000 random numbers.
		Then display the next six random numbers generated multiplied by 4096*4096
		If the random number generator is working properly, the random numbers
		should be:
			  6533892.0  14220222.0  7275067.0
			  6172232.0  8354498.0   10633180.0
	*/
	InitRANMAR(1802, 9373);
	for (int i = 0; i < 20000; i++)
		RANMAR();
	if ((RANMAR() * 4096 * 4096 != 6533892.0)
		|| (RANMAR() * 4096 * 4096 != 14220222.0)
		|| (RANMAR() * 4096 * 4096 != 7275067.0)
		|| (RANMAR() * 4096 * 4096 != 6172232.0)
		|| (RANMAR() * 4096 * 4096 != 8354498.0)
		|| (RANMAR() * 4096 * 4096 != 10633180.0))
	{
		puts("WARNING: The random number generator is not working properly!\n");
		return false;
	}
	return true;
}

}
