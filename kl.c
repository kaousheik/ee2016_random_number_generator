#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

/* Period parameters */  
#define N 624
#define M 397
#define MATRIX_A 0x9908b0dfUL   /* constant vector a */
#define UPPER_MASK 0x80000000UL /* most significant w-r bits */
#define LOWER_MASK 0x7fffffffUL /* least significant r bits */

static unsigned long mt[N]; /* the array for the state vector  */
static int mti=N+1; /* mti==N+1 means mt[N] is not initialized */

#define MAXCHAR 8

char look_up[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};

int index_of(char in) {
    for (int i = 0; i < 16; i++) {
        if (in == look_up[i])
            return i;
    }
    return -1;
}

unsigned long to_dec(char in[]) {
    unsigned long out = 0;
    for (int i = 0; i < 8; i++) {
        // printf("%d\n", index_of(in[i]));
        out += index_of(in[i]) * pow(16, 7 - i);
    }
    return out;
}
/* initializes mt[N] with a seed */
void init_genrand(unsigned long s)
{
    mt[0]= s & 0xffffffffUL;
    for (mti=1; mti<N; mti++) {
        mt[mti] = 
	    (1812433253UL * (mt[mti-1] ^ (mt[mti-1] >> 30)) + mti); 
        /* See Knuth TAOCP Vol2. 3rd Ed. P.106 for multiplier. */
        /* In the previous versions, MSBs of the seed affect   */
        /* only MSBs of the array mt[].                        */
        /* 2002/01/09 modified by Makoto Matsumoto             */
        mt[mti] &= 0xffffffffUL;
        /* for >32 bit machines */
    }
}

/* initialize by an array with array-length */
/* init_key is the array for initializing keys */
/* key_length is its length */
/* slight change for C++, 2004/2/26 */
void init_by_array(unsigned long init_key[], int key_length)
{
    int i, j, k;
    init_genrand(19650218UL);
    i=1; j=0;
    k = (N>key_length ? N : key_length);
    for (; k; k--) {
        mt[i] = (mt[i] ^ ((mt[i-1] ^ (mt[i-1] >> 30)) * 1664525UL))
          + init_key[j] + j; /* non linear */
        mt[i] &= 0xffffffffUL; /* for WORDSIZE > 32 machines */
        i++; j++;
        if (i>=N) { mt[0] = mt[N-1]; i=1; }
        if (j>=key_length) j=0;
    }
    for (k=N-1; k; k--) {
        mt[i] = (mt[i] ^ ((mt[i-1] ^ (mt[i-1] >> 30)) * 1566083941UL))
          - i; /* non linear */
        mt[i] &= 0xffffffffUL; /* for WORDSIZE > 32 machines */
        i++;
        if (i>=N) { mt[0] = mt[N-1]; i=1; }
    }

    mt[0] = 0x80000000UL; /* MSB is 1; assuring non-zero initial array */ 
}

/* generates a random number on [0,0xffffffff]-interval */
unsigned long genrand_int32(void)
{
    unsigned long y;
    static unsigned long mag01[2]={0x0UL, MATRIX_A};
    /* mag01[x] = x * MATRIX_A  for x=0,1 */

    if (mti >= N) { /* generate N words at one time */
        int kk;

        if (mti == N+1)   /* if init_genrand() has not been called, */
            init_genrand(5489UL); /* a default initial seed is used */

        for (kk=0;kk<N-M;kk++) {
            y = (mt[kk]&UPPER_MASK)|(mt[kk+1]&LOWER_MASK);
            mt[kk] = mt[kk+M] ^ (y >> 1) ^ mag01[y & 0x1UL];
        }
        for (;kk<N-1;kk++) {
            y = (mt[kk]&UPPER_MASK)|(mt[kk+1]&LOWER_MASK);
            mt[kk] = mt[kk+(M-N)] ^ (y >> 1) ^ mag01[y & 0x1UL];
        }
        y = (mt[N-1]&UPPER_MASK)|(mt[0]&LOWER_MASK);
        mt[N-1] = mt[M-1] ^ (y >> 1) ^ mag01[y & 0x1UL];

        mti = 0;
    }
  
    y = mt[mti++];

    /* Tempering */
    y ^= (y >> 11);
    y ^= (y << 7) & 0x9d2c5680UL;
    y ^= (y << 15) & 0xefc60000UL;
    y ^= (y >> 18);

    return y;
}



double rng[1000] ;
double prng[1000];
double kl = 0;
void input_rng()
{	
    double a;
    for (int i = 0; i < 10000; i++)
    {
	a = genrand_int32()/429497;
        int temp = a / 10;
        rng[temp]++;
    }
}
void input_prng()
{	
    FILE *fp;
    char * str = NULL;
    size_t len = 0;
    ssize_t read;
 
    fp = fopen("rng.txt", "r");
    if (fp == NULL){
        printf("Could not open file");
    }

    int i = 0;
    double a;

    while (((read = getline(&str, &len, fp)) != -1) && i < 10000) {
	    a = to_dec(str)/429497;
        int temp = a / 10;
        prng[temp]++;
        i++;
    }

    fclose(fp);
}

double calc()
{
    for (int i = 0; i < 1000; i++)
    {
	double num = prng[i]/rng[i];
        kl = kl + prng[i]*log(num);
    }
    return kl;
}
int main()
{	
    unsigned long init[4]={0x123, 0x234, 0x345, 0x456}, length=4;
    init_by_array(init, length);
    double div;
    input_rng();
init_by_array(init, length);
    input_prng();
    for (int i = 0; i < 1000; i++)
    {
        rng[i] = rng[i]/10000 ;
        prng[i] = prng[i]/10000 ;
    }
    div = calc();
    for (int i=0;i<1000;i++){
	printf("%lf", rng[i]);
	printf("\t");
	printf("%lf", prng[i]);
	printf("\n");
    }
    printf("The divergence between the two distributions is %lf", div);
    return 0;
}
