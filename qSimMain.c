#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#include <QuEST.h>

#include "JHelperMethods.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

void randomNumberGenerator() {

  // load QuEST with certain number of qubits
  int numOfQubits = 25;
  QuESTEnv env;
  Qureg qubits;
  loadQuEST(&env, &qubits, numOfQubits);
  int cbits[numOfQubits];
  int i;
  int answer = 0;

  // apply circuit
  printf("Applying Quantum Circuit...\n");
  initZeroState(qubits);
  for (i = 0; i < numOfQubits; i++) {
    hadamard(qubits, i);
  }
  printAllAmplitudesInRange(qubits, 0, 4);

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
  
  unloadQuEST(&env, &qubits);
}

void quantumTeleportation() {
  // load QuEST with certain number of qubits
  int numOfQubits = 3;
  QuESTEnv env;
  Qureg qubits;
  loadQuEST(&env, &qubits, numOfQubits);
  int cbits[numOfQubits];
  int i;

  // apply circuit
  printf("Applying Quantum Circuit...\n");
  initZeroState(qubits);
  pauliX(qubits, 0);  // Sets the first qubit to 1.

  // Apply Bell-State
  hadamard(qubits, 1);
  controlledNot(qubits, 1, 2);

  // Perform Quantum Teleportation Gate
  controlledNot(qubits, 0, 1);
  hadamard(qubits, 0);
  controlledNot(qubits, 1, 2);
  controlledPhaseFlip(qubits, 0, 2);

  printf("Measuring Qubits...\n");
  qreal finalProb;
  for (i = 0; i < numOfQubits; i++) {
    cbits[i] = measureWithStats(qubits, i, &finalProb);
    printf("Collapsed Bit: %d, Probability: %f\n", cbits[i], finalProb);
  }

  unloadQuEST(&env, &qubits);
}

void qGateTest() {
  // load QuEST with certain number of qubits
  int numOfQubits = 1;
  QuESTEnv env;
  Qureg qubits;
  loadQuEST(&env, &qubits, numOfQubits);
  int cbit;
  
  // apply circuit
  printf("Applying Quantum Circuit...\n");
  initZeroState(qubits);
  pauliX(qubits, 0);
  tGate(qubits, 0);
  printAllAmplitudes(qubits);
  
  printf("Measuring Qubits...\n");
  qreal finalProb;
  cbit = measureWithStats(qubits, 0, &finalProb);
  printf("Collapsed Bit: %d, Probability: %f\n", cbit, finalProb);
  
  unloadQuEST(&env, &qubits);
}

int main(int narg, char *varg[]) {
  struct timespec start, stop;
  clock_gettime(CLOCK_REALTIME, &start);

  // Insert Desired Function Here:
  randomNumberGenerator();

  clock_gettime(CLOCK_REALTIME, &stop);
  double result = (stop.tv_sec - start.tv_sec) * 1e3 + (stop.tv_nsec - start.tv_nsec) / 1e6;  // Milliseconds
  if (result > 1000.0) {
    result /= 1000.0;  // Convert to seconds.
    printf("Time Elapsed: %f seconds\n", result);
  }
  else {
    printf("Time Elapsed: %f milliseconds\n", result);
  }
  return 0;
}