#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <QuEST.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

void randomNumberGenerator() {

  // load QuEST with certain number of qubits
  printf("Initializing Qubits...\n");
  int numOfQubits = 30;
  QuESTEnv env = createQuESTEnv();
  Qureg qubits = createQureg(numOfQubits, env);
  initZeroState(qubits);
  
  int cbits[numOfQubits];
  int i;
  int answer = 0;

  // apply circuit
  printf("Applying Quantum Circuit...\n");
  for (i = 0; i < numOfQubits; i++) {
    hadamard(qubits, i);
  }

  printf("Measuring Qubits...\n");
  for (i = 0; i < numOfQubits - 1; i++) {
    cbits[i] = measure(qubits, i);
  }
  
  // Used for getting the probability before collapse
  qreal test;
  cbits[numOfQubits - 1] = measureWithStats(qubits, numOfQubits - 1, &test);
  printf("Probability of last qubit's outcome: %f\n", test);
  
  printf("Calculating Final Answer...\n");
  for (i = 0; i < numOfQubits; i++) {
    if (cbits[i] == 1) {
      answer += (int) pow(2, i);
    }
  }
  
  printf("%d\n", answer);
  
  // unload QuEST
  printf("Unloading Qubits...\n");
  destroyQureg(qubits, env); 
  destroyQuESTEnv(env);
}

void quantumTeleportation() {
  // load QuEST with certain number of qubits
  printf("Initializing Qubits...\n");
  int numOfQubits = 3;
  QuESTEnv env = createQuESTEnv();
  Qureg qubits = createQureg(numOfQubits, env);
  initZeroState(qubits);

  int cbits[numOfQubits];
  int i;

  // apply circuit
  printf("Applying Quantum Circuit...\n");
  pauliX(qubits, 0);  // Sets the first qubit to 1.

  // Apply Bell-State
  hadamard(qubits, 1);
  controlledNot(qubits, 1, 2);

  // Perform Quantum Teleportation Gate
  controlledNot(qubits, 0, 1);
  hadamard(qubits, 0);
  controlledNot(qubits, 1, 2);
  controlledPhaseFlip(qubits, 0, 2);

  qreal finalProb;
  for (i = 0; i < numOfQubits; i++) {
    cbits[i] = measureWithStats(qubits, i, &finalProb);
    printf("Collapsed Bit: %d, Probability: %f\n", cbits[i], finalProb);
  }

  // unload QuEST
  printf("Unloading Qubits...\n");
  destroyQureg(qubits, env); 
  destroyQuESTEnv(env);
}

int main(int narg, char *varg[]) {
  quantumTeleportation();
  return 0;
}