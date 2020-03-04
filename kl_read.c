#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>

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


double rng[1000] = {0};
double prng[1000] = {0};
double kl = 0;

void input_rng()
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
        rng[temp]++;
        i++;
    }

    fclose(fp);
}

void input_prng()
{   
    FILE *fp;
    char * str = NULL;
    size_t len = 0;
    ssize_t read;
 
    fp = fopen("prng.txt", "r");
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
	double num = rng[i]/prng[i];
        kl = kl + rng[i]*log(num);
    }
    return kl;
}

int main()
{	
    double div;
    input_rng();
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
