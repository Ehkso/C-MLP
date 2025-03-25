#include "mlp.h"
int main(int argc, char *argv[]){
	if (argc != 1){
		help();
		return 0;
	}
	// These are examples of usage; uncomment one if you want to test it.
	/*
	// XOR
	struct mlp xor = createMLP("xor", 2, 3, 1);
	//struct mlp xor = useMLP("xor", 2, 3, 1);
	struct example e1 = createExample(xor, ((double[]){0, 0}), ((double[]) {0}));
	// Interestingly, this standard C method for determining length of an array works with ints but not doubles
	//printf("%d\n", sizeof(e1.input) / sizeof(e1.input[0]));
	struct example e2 = createExample(xor, ((double[]){0, 1}), ((double[]) {1}));
	struct example e3 = createExample(xor, ((double[]){1, 0}), ((double[]) {1}));
	struct example e4 = createExample(xor, ((double[]){1, 1}), ((double[]) {0}));
	struct example e[] = {e1, e2, e3, e4};
	
	train(xor, e, 10000, 4, 0.25, 0);
	
	blindTest(xor, ((double[]) {0, 0}));
	blindTest(xor, ((double[]) {0, 1}));
	blindTest(xor, ((double[]) {1, 0}));
	blindTest(xor, ((double[]) {1, 1}));
	*/

	/*
	// SINE
	//struct mlp sinMLP = createMLP("sin", 4, 5, 1);
	struct mlp sinMLP = createMLP("sin", 4, 50, 1);
	FILE *fptr;
	// Commented out separately because once you've made the files you only need this to create a new one (but not if just retesting things)
	// 
	fptr = fopen("sinTraining.txt", "w");
	for (int i = 0; i < 400; i++){
		double x1 = ((double)(rand() % 100) / 100.0) * (((rand() % 2) == 1) ? 1 : -1);
		double x2 = ((double)(rand() % 100) / 100.0) * (((rand() % 2) == 1) ? 1 : -1);
		double x3 = ((double)(rand() % 100) / 100.0) * (((rand() % 2) == 1) ? 1 : -1);
		double x4 = ((double)(rand() % 100) / 100.0) * (((rand() % 2) == 1) ? 1 : -1);
		double ans = sin(x1-x2+x3-x4);
		fprintf(fptr, "%lf,%lf,%lf,%lf,%lf\n", ans, x1, x2, x3, x4);
	}
	fclose(fptr);
	fptr = fopen("sinTesting.txt", "w");
	for (int i = 0; i < 100; i++){
		double x1 = ((double)(rand() % 100) / 100.0) * (((rand() % 2) == 1) ? 1 : -1);
		double x2 = ((double)(rand() % 100) / 100.0) * (((rand() % 2) == 1) ? 1 : -1);
		double x3 = ((double)(rand() % 100) / 100.0) * (((rand() % 2) == 1) ? 1 : -1);
		double x4 = ((double)(rand() % 100) / 100.0) * (((rand() % 2) == 1) ? 1 : -1);
		double ans = sin(x1-x2+x3-x4);
		fprintf(fptr, "%lf,%lf,%lf,%lf,%lf\n", ans, x1, x2, x3, x4);
	}
	fclose(fptr);
	// /
	trainFromFile(sinMLP, "sinTraining.txt", 1000, 400, 0.05, 0);
	testFromFile(sinMLP, "sinTesting.txt", 100);
	 */

	/*
	// UCI
	struct mlp uciMLP = createMLP("uci", 16, 100, 26);
	parseUCI(uciMLP, "letter-recognition-training.data");
	parseUCI(uciMLP, "letter-recognition-testing.data");
	//trainFromFile(uciMLP, "parsed-letter-recognition-training.data", 10000, 16000, 0.25, 1);
	testFromFile(uciMLP, "parsed-letter-recognition-testing.data", 4000);
	
	//trainFromFile(uciMLP, "parsed-letter-recognition-training.data", 6000, 16000, 0.25, 1);
	//testFromFile(uciMLP, "parsed-letter-recognition-testing.data", 400);
	//*/
	
	return 0;
}
