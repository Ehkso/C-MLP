#ifndef MLP_H
#define MLP_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
struct mlp {
	char* name;
	int ni;
	int nh;
	int no;
};
struct example {
	double* input;
	double* output;
};
void help();
struct mlp createMLP(char* name, int ni, int nh, int no);
//struct mlp createMLP(char*, int, int, int); // note to self: remember that both ways are usable
struct mlp useMLP(char* name, int ni, int nh, int no);					 
void randomize(struct mlp);
struct example createExample(struct mlp m1, double* inputs, double* outputs);
struct example freeExample(struct example *e);
void parseUCI(struct mlp m1, char* filename);
void trainFromFile(struct mlp, char* filename, int maxEpochs, int numExamples, double learningRate, int kindaAnnealing);
void train(struct mlp, struct example[], int maxEpochs, int numExamples, double learningRate, int kindaAnnealing); // really didn't want to force specifying number of examples but here we are
void testFromFile(struct mlp, char* filename, int numExamples);
void test(struct mlp, double* inputs, double* outputs);
void blindTest(struct mlp, double* inputs);
void forward(struct mlp, struct example);
double backward(struct mlp, struct example);
void updateWeights(struct mlp, double learningRate);
double sigmoid(double); // !!NOTE!! Because of this fn gotta compile with `gcc *.c *.h -lm -o t.exe`, the code `exp([var])` requires -lm
double sigmoid_dx(double);



#endif
