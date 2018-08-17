#ifndef NUMERO

#define NUMERO
#include <iostream>

class numero{
private:
	double gene;
public:
	numero(double x=0):gene(x){}
	numero transa (numero n){
		numero filho((gene+n.gene)/2);
		filho.mutacao(0,0);
		return filho;
	}
	void mutacao(int amnt =0, int prob = 0){
		gene+=((double)(rand()%200))/100 - 1;
	}
	double avalia(){
		return gene*(100-gene);
	}
	void set(double x){
		gene=x;
	}
	void debug(){
		std::cout<<gene<<std::endl;
	}
	double get_gene(){
		return gene;
	}
};

#endif