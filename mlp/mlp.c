#include "mlp.h"
void help(){
	printf("Welcome to the MLP program. Here's a demo of how to use it:\n\n\
	// You can use useMLP with the same arguments if you're reusing a previously created perceptron.\n\
	struct mlp yourMLP = createMLP([name of MLP], [number of inputs], [number of hidden units], [number of output units]);\n\n\
	// Create your examples and put them in an array to train the perceptron on them\n\
	struct example yourFirstExample = createExample([array of inputs], [array of outputs]);\n\
	[...]\n\
	struct example yourNthExample = createExample([array of inputs], [array of outputs]);\n\
	struct example exampleArray[] = {yourFirstExample, [...], yourNthExample};\n\
	// Define the training conditions\n\
	train(yourMLP, exampleArray, [number of epochs], [number of examples], [learning rate (0 <= lr <= 1)], [(kinda) simulated annealing (0 or 1)]);\n\
	\n\
	// Testing:\n\
	test(yourMLP, [array of inputs]);\n\
	// You can add as many of these as you'd like, they just get appended to the testlog.txt file.\n\nThat's it - for further help just refer to the header file, it'll be more up-to-date than this help function anyway.\n");
}
// Creates a new MLP and randomizes it to initialize
struct mlp createMLP(char* name, int ni, int nh, int no){
	struct mlp m1;
	m1.name = name;
	m1.ni = ni;
	m1.nh = nh;
	m1.no = no;
	randomize(m1);
	return m1;
}
// Attempts to find a preexisting MLP to use, and calls to create one if not found
struct mlp useMLP(char* name, int ni, int nh, int no){
	FILE *fptr;
	char f[strlen(name) + 5]; strcpy(f, name); strcat(f, ".txt");
	fptr = fopen(f, "r");
	if (fptr == NULL){
		printf("Could not find a perceptron file with that name, creating one.\n");
		return createMLP(name, ni, nh, no);
	}
	struct mlp m1;
	m1.name = name;
	m1.ni = 0;
	m1.nh = 0;
	m1.no = 0;
	int maxstrlen = 200; // Should be all we need for this
	char buf[maxstrlen];
	fgets(buf, maxstrlen, fptr);
	while (strncmp(buf, "W1", 2) != 0){
		if (strncmp(buf, "NI", 2) == 0){
			char* token = strtok(buf, " ");
			token = strtok(NULL, "\n");
			//printf("m1.ni: '%s'\n", token);
			m1.ni = atoi(token);
		}
		if (strncmp(buf, "NH", 2) == 0){
			char* token = strtok(buf, " ");
			token = strtok(NULL, "\n");
			//printf("m1.nh: '%s'\n", token);
			m1.nh = atoi(token);
		}
		if (strncmp(buf, "NO", 2) == 0){
			char* token = strtok(buf, " ");
			token = strtok(NULL, "\n");
			//printf("m1.ni: '%s'\n", token);
			m1.no = atoi(token);
		}
		fgets(buf, maxstrlen, fptr);
	}
	fclose(fptr);
	if (m1.ni != ni || m1.nh != nh || m1.no != no){
		printf("Could not find a perceptron file with that information, creating one.\n");
		return createMLP(name, ni, nh, no);
	}
	return m1;
}
void randomize(struct mlp m1){
	srand(time(NULL));
	int iv = 20;
	FILE *fptr;
	char buf[strlen(m1.name) + 5];
	strcpy(buf, m1.name);
	strcat(buf, ".txt");

	fptr = fopen(buf, "w");

	fprintf(fptr, "test\n");
	
	fprintf(fptr, "NI: ");
	fprintf(fptr, "%d\n", m1.ni);

	fprintf(fptr, "NH: ");
	fprintf(fptr, "%d\n", m1.nh);

	fprintf(fptr, "NO: ");
	fprintf(fptr, "%d\n", m1.no);

	fprintf(fptr, "W1: \n");
	for (int i = 0; i < m1.ni; i++){
		for (int j = 0; j < m1.nh; j++){
			fprintf(fptr, "%lf\t", (double) (rand() % iv)+1);
		}
		fprintf(fptr, "\n");
	}
	fprintf(fptr, "W2: \n");
	for (int i = 0; i < m1.nh; i++){
		for (int j = 0; j < m1.no; j++){
			fprintf(fptr, "%lf\t", (double) (rand() % iv)+1);
		}
		fprintf(fptr, "\n");
	}

	/*
	fprintf(fptr, "Z1: \n");
	for (int i = 0; i < m1.ni; i++){
		fprintf(fptr, "%lf\t", 0.0);
	}
	fprintf(fptr, "\n");
	fprintf(fptr, "Z2: \n");
	for (int i = 0; i < m1.nh; i++){
		fprintf(fptr, "%lf\t", 0.0);
	}
	*/
	fprintf(fptr, "H: \n");
	for (int i = 0; i < m1.nh; i++){
		fprintf(fptr, "%lf\t", 0.0);
	}
	fprintf(fptr, "\n");

	fprintf(fptr, "O: \n");
	for (int i = 0; i < m1.no; i++){
		fprintf(fptr, "%lf\t", 0.0);
	}
	fprintf(fptr, "\n");

	fprintf(fptr, "dW1: \n");
	for (int i = 0; i < m1.ni; i++){
		for (int j = 0; j < m1.nh; j++){
			fprintf(fptr, "%lf\t", 0.0);
		}
		fprintf(fptr, "\n");
	}

	fprintf(fptr, "dW2: \n");
	for (int i = 0; i < m1.nh; i++){
		for (int j = 0; j < m1.no; j++){
			fprintf(fptr, "%lf\t", 0.0);
		}
		fprintf(fptr, "\n");
	}
	fprintf(fptr, "fin");
	fprintf(fptr, "\n");
	fclose(fptr);
}

struct example createExample(struct mlp m1, double* inputs, double* outputs){
	struct example e;
	e.input = (double*)malloc(m1.ni * sizeof(double));
	e.output = (double*)malloc(m1.no * sizeof(double));
	if (e.input == NULL || e.output == NULL){ printf("malloc failed to allocate for the example"); exit(EXIT_FAILURE); }
	for (int i = 0; i < m1.ni; i++){
		e.input[i] = inputs[i];
	}
	for (int i = 0; i < m1.no; i++){
		e.output[i] = outputs[i];
	}
	//e.input = inputs;
	//e.output = outputs;
	return e;
}
struct example freeExample(struct example *e){
	free(e->input);
	free(e->output);
}

void parseUCI(struct mlp m1, char* filename){
	FILE *fptr;
	FILE *parsed;
	int maxstrlen = 100;
	char buf[maxstrlen];
	strcpy(buf, "parsed-");
	strcat(buf, filename);
	fptr = fopen(filename, "r");
	parsed = fopen(buf, "w");
	if (fptr == NULL){ printf("Error reading the file.\n"); return; }
	fgets(buf, maxstrlen, fptr);
	while (!feof(fptr)){
		char* token = strtok(buf, ",");
		int letternum = token[0];
		for (int i = 0; i < 26; i++){
			if (i != 0){ fprintf(parsed, ","); }
			if (i == letternum-65){
				fprintf(parsed, "1");
			} else{
				fprintf(parsed, "0");
			}
		}
		for (int i = 0; i < m1.ni; i++){
			token = strtok(NULL, ",");
			fprintf(parsed, ",");
			fprintf(parsed, token);
		}
		fgets(buf, maxstrlen, fptr);
	}
	fclose(fptr);
	fclose(parsed);
	return;
}
void trainFromFile(struct mlp m1, char* filename, int maxEpochs, int numExamples, double learningRate, int kindaAnneal){
	FILE *fptr;
	int maxstrlen = (m1.ni+m1.no) * 200;
	char buf[maxstrlen];
	strcpy(buf, filename);
	printf("Getting %d trainer example(s) from '%s'\n", numExamples, buf);

	fptr = fopen(buf, "r"); if (fptr == NULL){ printf("Could not find '%s'.\n", buf); return;}
	struct example examples[numExamples];
	double inputs[m1.ni];
	double outputs[m1.no];
	int examplenum = 0;
	fgets(buf, maxstrlen, fptr);
	while (!feof(fptr) && examplenum < numExamples){
		for (int i = 0; i < m1.ni; i++){ inputs[i] = 0.0; }
		for (int i = 0; i < m1.no; i++){ outputs[i] = 0.0; }
		//printf("%s", buf);
		char* token = strtok(buf, ",");
		for (int i = 0; i < m1.no; i++){
			outputs[i] = strtod(token, NULL);
			// printf("o%lf ", outputs[i]);
			token = strtok(NULL, ",");
		}
		//printf("%s ", token);
		for (int i = 0; i < m1.ni; i++){
			inputs[i] = strtod(token, NULL);
			//printf("i%lf ", inputs[i]);
			token = strtok(NULL, ",");
		}
		//printf("\n");

		examples[examplenum++] = createExample(m1, inputs, outputs);
		fgets(buf, maxstrlen, fptr);
	}
	fclose(fptr);
	//examples[200] = createExample(m1, ((double[]){0,0,0,0}), ((double[]){0}));
	/*
	for (int i = 0; i < numExamples; i++){
		printf("%4d: ", i);
		for (int j = 0; j < m1.ni; j++){
			printf("%lf ", examples[i].input[j]);
		}
		printf("-> ");
		for (int j = 0; j < m1.no; j++){
			printf("%lf ", examples[i].output[j]);
		}
		printf("\n");
	}
	*/
	//printf("examplenum %d\n", examplenum);
	if (numExamples == examplenum){
		train(m1, examples, maxEpochs, numExamples, learningRate, kindaAnneal);
	} else{
		printf("Error: Example number provided greater than number of examples in %s - will use the %d found.\n", filename, examplenum);
		train(m1, examples, maxEpochs, examplenum, learningRate, kindaAnneal);
	}
	for (int i = 0; i < examplenum; i++){
		freeExample(&examples[i]);
	}
	printf("All examples freed.\n");
	return;
}
void train(struct mlp m1, struct example e[], int maxEpochs, int numExamples, double learningRate, int kindaAnneal){
	double error = 0;
	// To ask for early stoppage if error sits at 0 for a while
	// Using this feature causes it to "segfault" even though it performs its job properly, can set this to 1 to disable asking just for peace of mind
	int alreadyAsked = 0;
	// Simulated Annealing stuff:
	// Track the last n error rates to make sure that it is in fact improving
	double olderror = 0;
	int lastn = 200; // Changing this will also affect the "aggressiveness" of the annealing as it causes it to adjust more/less frequently
	double acceptable = 0.15; // Acceptable "error rate" at which we can lower the "temperature"
	printf("Training %s (%d Hidden Units)...\nEpochs: %d\n(Initial) Learning Rate: %lf\nSimulated Annealing?: %d\n", m1.name, m1.nh, maxEpochs, learningRate, kindaAnneal);
	FILE *fptr;
	fptr = fopen("errorlog.txt", "w");
	fprintf(fptr, "Epoch,Error,Learning Rate\n");
	fclose(fptr);
	for (int i = 0; i < maxEpochs; i++){
		for (int j = 0; j < numExamples; j++){
			forward(m1, e[j]);
			error += (backward(m1, e[j])) / numExamples;
			//if (j == 0){ error = backward(m1, e[j]); }
			if (((i + j) % 100 != 0) || 0){
				updateWeights(m1, learningRate);
				//printf("epoch %d: example %d complete - error %lf\n", i+1, j, error);
			}
		}
		//error /= numExamples;
	
		for (int n = 0; n < 10; n++){
			if (((i+1) == (maxEpochs * n) / 10)){
				printf("epoch %d error %lf lr %lf\n", i+1, error, learningRate);
			}
		}
		fptr = fopen("errorlog.txt", "a");
		fprintf(fptr, "%d,%lf,%lf\n", i+1, error, learningRate);
		fclose(fptr);

		olderror += error;
		if ((i % lastn) == 0){
			olderror /= lastn;
			// Early stopper
			if (!alreadyAsked && olderror < 0.000001){
				char* decision = "yes";
				printf("The error has been 0 for at least the last %d epochs, stop now? y/n: \n", lastn);
				//fgets(decision, 100, stdin);
				if (strncmp(decision, "y", 1) == 0){
					printf("Exited on epoch %d\n", i+1);
					i = maxEpochs+1;
				} else { printf("Continuing...\n"); }
				alreadyAsked = 1;
			}
			// A dirty cross of adaptive lr and simulated annealing
			if (kindaAnneal){
				// Increase the temperature if it seems to be stuck in a local minimum
				if ((fabs(olderror - error) < 0.005) && (error > acceptable)){
					learningRate += 0.05;
				} else{
					learningRate -= 0.01;
				}
			}
			olderror = 0;
			if (learningRate < 0.01){ learningRate = 0.01; }
			if (learningRate > 1){ learningRate = 1; }
		}
		error = 0;
	}
	return;
}
void testFromFile(struct mlp m1, char* filename, int numExamples){
	FILE *fptr;
	int maxstrlen = m1.ni * 200;
	char buf[maxstrlen];
	strcpy(buf, filename);
	printf("Getting %d tester example(s) from '%s'\n", numExamples, buf);

	fptr = fopen(buf, "r"); if (fptr == NULL){ printf("Could not find '%s'.\n", buf); return;}
	struct example examples[numExamples];
	double inputs[m1.ni];
	double outputs[m1.no];
	int examplenum = 0;
	fgets(buf, maxstrlen, fptr);
	while (!feof(fptr) && examplenum < numExamples){
		for (int i = 0; i < m1.ni; i++){ inputs[i] = 0.0; }
		for (int i = 0; i < m1.no; i++){ outputs[i] = 0.0; }
		char* token = strtok(buf, ",");
		for (int i = 0; i < m1.no; i++){
			outputs[i] = strtod(token, NULL);
			token = strtok(NULL, ",");
		}
		for (int i = 0; i < m1.ni; i++){
			inputs[i] = strtod(token, NULL);
			token = strtok(NULL, ",");
		}

		examples[examplenum++] = createExample(m1, inputs, outputs);
		fgets(buf, maxstrlen, fptr);
	}
	fclose(fptr);

	/*
	for (int i = 0; i < numExamples; i++){
		printf("%4d: ", i);
		for (int j = 0; j < m1.ni; j++){
			printf("%lf ", examples[i].input[j]);
		}
		printf("-> ");
		for (int j = 0; j < m1.no; j++){
			printf("%lf ", examples[i].output[j]);
		}
		printf("\n");
	}
	*/
	fptr = fopen("testlog.txt", "w"); fclose(fptr); // wipes the test file, can do this here since all tests happen in this loop here
	for (int i = 0; i <  numExamples; i++){
		test(m1, examples[i].input, examples[i].output);
	}

	for (int i = 0; i < numExamples; i++){
		freeExample(&examples[i]);
	}
	printf("All examples freed.\n");
	return;
	printf("Calculated accuracy of testlog.txt: ");
	int total = 0;
	int linecount = 0; // numExamples suddenly started printing 1409297664 (intmax?) for some reason
	
	fptr = fopen("testlog.txt", "r");
	maxstrlen = m1.nh * 20;
	fgets(buf, maxstrlen, fptr);
	while (!feof(fptr)){
		linecount++;	
		char* token = strtok(buf, " ");
		while (strncmp(token, "Error", 5) != 0){
			token = strtok(NULL, " ");
		}
		token = strtok(NULL, " ");
		
		if (strtod(token, NULL) < 0.1){
			total++;
		}
		//printf("%s", token);
		fgets(buf, maxstrlen, fptr);
	}
	fclose(fptr);
	double perc = ((double) total) / linecount;
	printf("%.2lfpc - %d correct out of %d\n", perc, total, linecount);

	return;
}
void test(struct mlp m1, double* inputs, double* outputs){
	struct example test = createExample(m1, inputs, ((double[]) {0}));
	forward(m1, test);

	FILE *fptr;
	char f[strlen(m1.name) + 5]; strcpy(f, m1.name); strcat(f, ".txt");
	fptr = fopen(f, "r");
	if (fptr == NULL){ printf("Error reading the file.\n"); return; }
	FILE *results;
	results = fopen("testlog.txt", "a");
	double o[m1.no];
	
	int maxstrlen = m1.nh * 200; // theoretically this should cover the longest line
	char buf[maxstrlen];
	while (!feof(fptr)){
		fgets(buf, maxstrlen, fptr);
		if (strncmp(buf, "O", 1) == 0){
			fgets(buf, maxstrlen, fptr);
			char* token = strtok(buf, "\t");
			for (int i = 0; i < m1.no; i++){
				o[i] = strtod(token, NULL);
				token = strtok(NULL, "\t");
			}
			//fprintf(results, "o[] ="); for (int i = 0; i < m1.no; i++){ fprintf(results, " %lf", o[i]); } fprintf(results, "\n");
		}
	} 
	fclose(fptr);
	//int inplen = sizeof(inputs) / sizeof(inputs[0]);
	//printf("%d\n", inplen);
	fprintf(results, "Test: (");
	
	for (int i = 0; i < m1.ni; i++){
		fprintf(results, "%10lf", inputs[i]);
		if (i != m1.ni - 1){
			fprintf(results, ", ");
		}
	}
	fprintf(results, ") -> (");
	double totalError = 0;
	double termError;
	int success = 1;
	for (int i = 0; i < m1.no; i++){
		termError = fabs(outputs[i] - o[i]);
		if (termError > 0.01){ success = 0; }
		//fprintf(results, "%lf", round(o[i]));
		if (termError < 0.000001){
			fprintf(results, "%10lf", o[i]);
		} else { fprintf(results, "%10lf [!%10lf]", o[i], outputs[i]);}
		totalError += termError * termError;
		if (i != m1.no -1){
			fprintf(results, ", ");
		}
	}
	if (m1.no == 1){
		fprintf(results, ") - Total Error:%10lf\n", (totalError / m1.no));
	}else {
		fprintf(results, ") - Total Error: %d\n", 1-success);
	}
	fclose(results);
	return;
}
void blindTest(struct mlp m1, double* inputs){
	struct example test = createExample(m1, inputs, ((double[]) {0}));
	forward(m1, test);

	FILE *fptr;
	char f[strlen(m1.name) + 5]; strcpy(f, m1.name); strcat(f, ".txt");
	fptr = fopen(f, "r");
	if (fptr == NULL){ printf("Error reading the file.\n"); return; }
	FILE *results;
	results = fopen("testlog.txt", "a");
	double o[m1.no];
	
	int maxstrlen = m1.nh * 200; // theoretically this should cover the longest line
	char buf[maxstrlen];
	while (!feof(fptr)){
		fgets(buf, maxstrlen, fptr);
		if (strncmp(buf, "O", 1) == 0){
			fgets(buf, maxstrlen, fptr);
			char* token = strtok(buf, "\t");
			for (int i = 0; i < m1.no; i++){
				o[i] = strtod(token, NULL);
				token = strtok(NULL, "\t");
			}
			fprintf(results, "o[] ="); for (int i = 0; i < m1.no; i++){ fprintf(results, " %lf", o[i]); } fprintf(results, "\n");
		}
	} 
	fclose(fptr);
	//int inplen = sizeof(inputs) / sizeof(inputs[0]);
	//printf("%d\n", inplen);
	fprintf(results, "Test: (");
	
	for (int i = 0; i < m1.ni; i++){
		fprintf(results, "%lf", inputs[i]);
		if (i != m1.ni - 1){
			fprintf(results, ", ");
		}
	}
	fprintf(results, ") -> (");
	for (int i = 0; i < m1.no; i++){
		fprintf(results, "%lf", round(o[i]));
		if (i != m1.no -1){
			fprintf(results, ", ");
		}
	}
	fprintf(results, ")\n");
	fclose(results);
	return;
}
// Processes the example inputs and stores the output
void forward(struct mlp m1, struct example e){
	// Reads mlp file to get the weights
	FILE *fptr;
	char f[strlen(m1.name) + 5]; strcpy(f, m1.name); strcat(f, ".txt");
	fptr = fopen(f, "r");
	if (fptr == NULL){ printf("Error reading the file.\n"); return; }
	FILE *copy;
	copy = fopen("copy.txt", "w");

	double w1[m1.ni][m1.nh];
	double w2[m1.nh][m1.no];
	double hu[m1.nh]; for (int i = 0; i < m1.nh; i++){ hu[i] = 0; }
	double out[m1.no]; for (int i = 0; i < m1.no; i++){ out[i] = 0; }

	// "Revises" the file to put in the values of H and O (creates a copy with same lines except for modifications and then replaces the original)
	int maxstrlen = m1.nh * 200; // theoretically this should cover the longest line
	char buf[maxstrlen];
	fgets(buf, maxstrlen, fptr);
	while (!feof(fptr)){
		if (strncmp(buf, "W1", 2) == 0){
			fprintf(copy, "%s", buf);
			for (int i = 0; i < m1.ni; i++){
				fgets(buf, maxstrlen, fptr);
				fprintf(copy, "%s", buf);
				char* token = strtok(buf, "\t"); // note to self: use strtok_r if ever wanna multithread
				for (int j = 0; j < m1.nh; j++){
					w1[i][j] = strtod(token, NULL);
					//printf("%lf ", w1[i][j]);
					token = strtok(NULL, "\t");
				}
				//printf("\n");
			}
		} else if (strncmp(buf, "W2", 2) == 0){
			fprintf(copy, buf);
			for (int i = 0; i < m1.nh; i++){
				fgets(buf, maxstrlen, fptr);
				fprintf(copy, "%s", buf);
				char* token = strtok(buf, "\t");
				for (int j = 0; j < m1.no; j++){
					w2[i][j] = strtod(token, NULL);
					//printf("%lf ", w2[i][j]);
					token = strtok(NULL, "\t");
				}
				//printf("\n");
			}
			//printf("weights collected\n");
			// By holding all these calculations in here at the end of the W2 collection we guarantee that these calculations are performed between the collection of all weights and the writing of new values to the hidden/output layers
			// This lets us do all the work in this one file access instead of using one read to get the weights, calculating the results, and then reopening the file plus its copy in order to rewrite it

			// Calculate hidden unit values
			//printf("new example\n");
			for (int i = 0; i < m1.ni; i++){
				for (int j = 0; j < m1.nh; j++){
					//printf("hu[%d] (%lf) += e.input[%d] (%lf) * w1\n", j, hu[j], i, e.input[i]);
					hu[j] += e.input[i] * w1[i][j];//w1
				}
			}
			// Interestingly when I swap i,j to make each upper unit get all its inputs from layer below
			// (instead of each unit passing its out to each upper unit) it does this cool thing where
			// it bypasses my epoch limit and just continues forever, I don't know why or how it manages to do that
			
			for (int i = 0; i < m1.nh; i++){ hu[i] = sigmoid(hu[i]); } // can replace this line depending on how you want to handle the units
			// Calculate output layer values
			for (int i = 0; i < m1.nh; i++){
				for (int j = 0; j < m1.no; j++){
					//printf("buf[%d] (%lf) += hu[%d] (%lf) * w2\n", j, buf[j], i, hu[i]);
					out[j] += hu[i] * w2[i][j];//w2
				}
			}
			for (int i = 0; i < m1.no; i++){ out[i] = sigmoid(out[i]); } // same as above
			// /*
			if (m1.no > 1){
				// if multiple outputs pick the highest as decision by setting to 1
				int max = 0;
				for (int i = 0; i < m1.no; i++){
					if (out[i] > out[max]){ max = i; }
				}
				out[max] = 1.0;
			}
			// */
			// end of calculations
		} else if (strncmp(buf, "H", 1) == 0){
			fprintf(copy, "%s", buf);
			fgets(buf, maxstrlen, fptr);
			for (int i = 0; i < m1.nh; i++){
				fprintf(copy, "%lf\t", hu[i]);
			}
			fprintf(copy, "\n");
		} else if (strncmp(buf, "O", 1) == 0){
			fprintf(copy, "%s", buf);
			fgets(buf, maxstrlen, fptr);
			for (int i = 0; i < m1.no; i++){
				fprintf(copy, "%lf\t", out[i]);
			}
			fprintf(copy, "\n");
		} else {
			fprintf(copy, "%s", buf);
		}
		fgets(buf, maxstrlen, fptr);
	}
	fclose(copy);
	fclose(fptr);
	remove(f);
	rename("copy.txt", f);
	return;
}
double backward(struct mlp m1, struct example e){
	// Collects values of hidden / output units + weights for calculations of layer n-1 (which is why we don't need W1)
	FILE *fptr;
	char f[strlen(m1.name) + 5]; strcpy(f, m1.name); strcat(f, ".txt");
	fptr = fopen(f, "r");
	if (fptr == NULL){ printf("Error reading the file.\n"); return 1; }
	FILE *copy;
	copy = fopen("copy.txt", "w");

	//double w1[m1.ni][m1.nh];
	double w2[m1.nh][m1.no];
	double h[m1.nh];
	double o[m1.no];
	
		
	// Rewrites file to have new dW1/dW2
	int maxstrlen = m1.nh * 200; // theoretically this should cover the longest line
	char buf[maxstrlen];
	fgets(buf, maxstrlen, fptr);
	double dW1[m1.ni][m1.nh]; for (int i=0; i<m1.ni; i++){for (int j=0; j<m1.nh; j++){ dW1[i][j] = 0; }}
	double dW2[m1.nh][m1.no]; for (int i=0; i<m1.nh; i++){for (int j=0; j<m1.no; j++){ dW2[i][j] = 0; }}
	double error = 0.0;
	int success = 1;
	while (!feof(fptr)){
		if (strncmp(buf, "W2", 2) == 0){
			fprintf(copy, buf);
			for (int i = 0; i < m1.nh; i++){
				fgets(buf, maxstrlen, fptr);
				fprintf(copy, buf);
				char* token = strtok(buf, "\t");
				for (int j = 0; j < m1.no; j++){
					w2[i][j] = strtod(token, NULL);
					token = strtok(NULL, "\t");
				}
			}
		}
		else if (strncmp(buf, "H", 1) == 0){
			fprintf(copy, buf);
			fgets(buf, maxstrlen, fptr);
			fprintf(copy, buf);
			char* token = strtok(buf, "\t");
			for (int i = 0; i < m1.nh; i++){
				h[i] = strtod(token, NULL);
				token = strtok(NULL, "\t");
			}
			//printf("h[] ="); for (int i = 0; i < m1.nh; i++){ printf(" %lf", h[i]); } printf("\n"); 
		}
		else if (strncmp(buf, "O", 1) == 0){
			fprintf(copy, buf);
			fgets(buf, maxstrlen, fptr);
			fprintf(copy, buf);
			char* token = strtok(buf, "\t");
			for (int i = 0; i < m1.no; i++){
				o[i] = strtod(token, NULL);
				token = strtok(NULL, "\t");
			}
			//printf("o[] ="); for (int i = 0; i < m1.no; i++){ printf(" %lf", o[i]); } printf("\n");

			// Necessary information collected, computing error
			//
			double tErr = 0.0;
			double outD[m1.no]; for (int i = 0; i < m1.no; i++){ outD[i] = 0; }
			double hD[m1.nh]; for (int i = 0; i < m1.nh; i++){ hD[i] = 0; }
			// Calculate error and \deltaO
			for (int i = 0; i < m1.no; i++){
				tErr = e.output[i] - o[i];
				//printf("tErr %lf = e.out[%d] %lf - o[%d] %lf |", tErr, i, e.output[i], i, o[i]);
				if (m1.no > 1){
					if (fabs(tErr) < 0.001){
						tErr = 0;
						//printf("tErr %lf = e.out[%d] %lf - o[%d] %lf |", tErr, i, e.output[i], i, o[i]);
					} else{
						tErr = (tErr < 0) ? -1 : 1;
						success = 0;
						//printf("tErr %lf = e.out[%d] %lf - o[%d] %lf |", tErr, i, e.output[i], i, o[i]);
					}
				}
				outD[i] = tErr * sigmoid_dx(e.output[i]);
				//printf("outDi %d %lf\n", i, outD[i]);
				error += tErr * tErr; // Squaring the error (for MSE)
				//printf("tErr^2 = %lf\n", tErr*tErr);
			}
			//printf("example error: %lf returnin err/no: %lf\n", error, (error / m1.no));
			// Calculate dW2 and \deltaH
			for (int i = 0; i < m1.no; i++){
				for (int j = 0; j < m1.nh; j++){
					dW2[j][i] = outD[i] * h[j];
					//printf("dw2 %d %d outD %d %lf h %d %lf\n", j, i, i, outD[i], j, h[j]);
					hD[j] = outD[i] * w2[j][i];
					//printf("hD %d outD %d %lf w2 %d %d %lf\n", j, i, outD[i], j, i, w2[j][i]);
				}
			}
			// Calculate dW1
			for (int i = 0; i < m1.nh; i++){
				tErr = hD[i] * sigmoid_dx(h[i]);
				//printf("tErr: hD %d %lf sigmdx (h[%d]) (%lf) (%lf)\n", i, hD[i], i, h[i], sigmoid_dx(h[i]));
				for (int j = 0; j < m1.ni; j++){
					dW1[j][i] = tErr * h[j];
					//printf("dw1 %d %d %lf terr %lf h %d %lf\n", j, i, dW1[j][i], tErr, j, h[j]);
				}
			}
			// Calculations complete
		}
		else if (strncmp(buf, "dW1", 3) == 0){
			fprintf(copy, "%s", buf);
			for (int i = 0; i < m1.ni; i++){
				fgets(buf, maxstrlen, fptr);
				for (int j = 0; j < m1.nh; j++){
					fprintf(copy, "%lf\t", dW1[i][j]);
					//fprintf(copy, "%lf\t", 0.0);
				}
				fprintf(copy, "\n");
			}
		} else if (strncmp(buf, "dW2", 3) == 0){
			fprintf(copy, "%s", buf);
			for (int i = 0; i < m1.nh; i++){
				fgets(buf, maxstrlen, fptr);
				for (int j = 0; j < m1.no; j++){
					fprintf(copy, "%lf\t", dW2[i][j]);
				}
				fprintf(copy, "\n");
			}
		} else {
			fprintf(copy, "%s", buf);
		}
		fgets(buf, maxstrlen, fptr);
	}
	fclose(copy);
	fclose(fptr);
	remove(f);
	rename("copy.txt", f);
	if (m1.no == 1){
		// Mean Error (leading to MSE)
		return error / m1.no;
	} else{ return 1 - success; }
}
void updateWeights(struct mlp m1, double lr){
	// Reads file to collect the weights
	FILE *fptr;
	char f[strlen(m1.name) + 5]; strcpy(f, m1.name); strcat(f, ".txt");
	fptr = fopen(f, "r");
	if (fptr == NULL){ printf("Error reading the file.\n"); return; }
	double w1[m1.ni][m1.nh];
	double w2[m1.nh][m1.no];
	double dW1[m1.ni][m1.nh];
	double dW2[m1.nh][m1.no];
	
	int maxstrlen = m1.nh * 200;
	char buf[maxstrlen];
	while (!feof(fptr)){
		fgets(buf, maxstrlen, fptr);
		if (strncmp(buf, "W1", 2) == 0){
			for (int i = 0; i < m1.ni; i++){
				fgets(buf, maxstrlen, fptr);
				char* token = strtok(buf, "\t");
				for (int j = 0; j < m1.nh; j++){
					w1[i][j] = strtod(token, NULL);
					token = strtok(NULL, "\t");
				}
			}
		}
		if (strncmp(buf, "W2", 2) == 0){
			for (int i = 0; i < m1.nh; i++){
				fgets(buf, maxstrlen, fptr);
				char* token = strtok(buf, "\t");
				for (int j = 0; j < m1.no; j++){
					w2[i][j] = strtod(token, NULL);
					token = strtok(NULL, "\t");
				}
			}
		}
		if (strncmp(buf, "dW1", 3) == 0){
			for (int i = 0; i < m1.ni; i++){
				fgets(buf, maxstrlen, fptr);
				char* token = strtok(buf, "\t");
				for (int j = 0; j < m1.nh; j++){
					dW1[i][j] = strtod(token, NULL);
					token = strtok(NULL, "\t");
				}
			}
		}
		if (strncmp(buf, "dW2", 3) == 0){
			for (int i = 0; i < m1.nh; i++){
				fgets(buf, maxstrlen, fptr);
				char* token = strtok(buf, "\t");
				for (int j = 0; j < m1.no; j++){
					dW2[i][j] = strtod(token, NULL);
					token = strtok(NULL, "\t");
				}
			}
		}
	} 
	fclose(fptr);

	// Update weights for W1
	for (int i = 0; i < m1.ni; i++){
		for (int j = 0; j < m1.nh; j++){
			w1[i][j] += dW1[i][j] * lr;
			dW1[i][j] = 0.0;
		}
	}
	// Update weights for W2
	// I think this loop is causing the problem but I can't tell any more since it became a spurious crash
	for (int i = 0; i < m1.nh; i++){
		for (int j = 0; j < m1.no; j++){
			w2[i][j] += dW2[i][j] * lr;
			//printf("w2 %d %d %lf\n", i, j, w2[i][j]); printf("dw2 %d %d %lf\n", i, j, dW2[i][j]);
			dW2[i][j] = 0.0;
		}
	}
	// Rewrites file with new W1/W2
	FILE *copy;
	copy = fopen("copy.txt", "w");
	fptr = fopen(f, "r");
	fgets(buf, maxstrlen, fptr);
	while (!feof(fptr)){
		if (strncmp(buf, "W1", 2) == 0){
			fprintf(copy, "%s", buf);
			for (int i = 0; i < m1.ni; i++){
				fgets(buf, maxstrlen, fptr);
				for (int j = 0; j < m1.nh; j++){
					fprintf(copy, "%lf\t", w1[i][j]);
				}
				fprintf(copy, "\n");
			}
		} else if (strncmp(buf, "W2", 2) == 0){
			fprintf(copy, "%s", buf);
			for (int i = 0; i < m1.nh; i++){
				fgets(buf, maxstrlen, fptr);
				for (int j = 0; j < m1.no; j++){
					fprintf(copy, "%lf\t", w2[i][j]);
				}
				fprintf(copy, "\n");
			}
		} else if (strncmp(buf, "dW1", 3) == 0){
			fprintf(copy, "%s", buf);
			for (int i = 0; i < m1.ni; i++){
				fgets(buf, maxstrlen, fptr);
				for (int j = 0; j < m1.nh; j++){
					fprintf(copy, "%lf\t", dW1[i][j]);
				}
				fprintf(copy, "\n");
			}
		} else if (strncmp(buf, "dW2", 3) == 0){
			fprintf(copy, "%s", buf);
			for (int i = 0; i < m1.nh; i++){
				fgets(buf, maxstrlen, fptr);
				for (int j = 0; j < m1.no; j++){
					fprintf(copy, "%lf\t", dW2[i][j]);
				}
				fprintf(copy, "\n");
			}
		} else {
			fprintf(copy, "%s", buf);
		}
		fgets(buf, maxstrlen, fptr);
	}
	fclose(copy);
	fclose(fptr);
	remove(f);
	rename("copy.txt", f);
	return;
}
double sigmoid(double x){
	return 1.0 / (1.0 + exp(-x));
}
double sigmoid_dx(double x){
	return sigmoid(x) * (1 - sigmoid(x));
}
