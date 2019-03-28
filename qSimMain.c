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
  printf("Running 'True' Random Number Generator Algorithm...\n");
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
  printf("Running Quantum Teleportation Algorithm...\n");
  int numOfQubits = 3;
  QuESTEnv env;
  Qureg qubits;
  loadQuEST(&env, &qubits, numOfQubits);

  // apply circuit
  printf("Applying Quantum Circuit...\n");
  initZeroState(qubits);
  pauliX(qubits, 2);  // Sets the first qubit to 1.

  // Apply Bell-State
  hadamard(qubits, 1);
  controlledNot(qubits, 1, 0);

  // Perform Quantum Teleportation Gate
  controlledNot(qubits, 2, 1);
  hadamard(qubits, 2);
  controlledNot(qubits, 1, 0);
  controlledPhaseFlip(qubits, 2, 0);

  measureAllAndPrint(qubits);

  unloadQuEST(&env, &qubits);
}

void qGateTest() {
  printf("Running Quantum Gate Testers...\n");
  int numOfQubits = 4;
  QuESTEnv env;
  Qureg qubits;
  loadQuEST(&env, &qubits, numOfQubits);
  
  // apply circuit
  printf("Applying Quantum Circuit...\n");
  initValueState(qubits, 10);
  
  measureAllAndPrint(qubits);
  
  unloadQuEST(&env, &qubits);
}

void txtExercise4_27() {
  printf("Running Quantum Gate Testers...\n");
  int numOfQubits = 3;
  QuESTEnv env;
  Qureg qubits;
  loadQuEST(&env, &qubits, numOfQubits);
  
  // apply circuit
  printf("Applying Quantum Circuit...\n");
  initValueState(qubits, 1);  // Set to |001>
  printAllAmplitudes(qubits);
  
  controlledNot(qubits, 2, 0);
  controlledNot(qubits, 1, 0);
  multiToffoliGate(qubits, (int []){1, 2}, 2, 0);
  controlledNot(qubits, 0, 1);
  controlledNot(qubits, 0, 2);
  controlledNot(qubits, 0, 1);
  multiToffoliGate(qubits, (int []){0, 1}, 2, 2);
  controlledNot(qubits, 0, 1);
  
  // Should get |111>
  printAllAmplitudes(qubits);
  measureAllAndPrint(qubits);
  
  unloadQuEST(&env, &qubits);
}

void deutschJozsa() {
  // This doesn't seem to be working properly...
  printf("Running Deutsch-Jozsa Algorithm...\n");
  int numOfQubits = 3;
  QuESTEnv env;
  Qureg qubits;
  loadQuEST(&env, &qubits, numOfQubits);
  
  // apply circuit
  printf("Applying Quantum Circuit...\n");
  initZeroState(qubits);
  pauliX(qubits, 0);
  for (int i = 0; i < numOfQubits; i++)
    hadamard(qubits, i);
  
  // Black Box Part Here:
  pauliZ(qubits, 2);
  controlledPhaseFlip(qubits, 1, 0);
  
  // Rest of circuit:
  for (int i = 1; i < numOfQubits; i++)
    hadamard(qubits, i);
  
  // measureAllAndPrint(qubits);
  measureAndPrint(qubits, 0);
  
  unloadQuEST(&env, &qubits);
}

void QFT() {
  printf("Running Quantum Fourier Transform...\n");
  int numOfQubits = 3;
  QuESTEnv env;
  Qureg qubits;
  loadQuEST(&env, &qubits, numOfQubits);
  qreal theta = 0.0;
  int thetaCounter = 2;
  
  // apply circuit
  printf("Applying Quantum Circuit...\n");
  initOneState(qubits);
  
  // This might be upside-down...
  for (int i = 0; i < numOfQubits; i++) {
    hadamard(qubits, i);
    // printf("i: %d\n", i);
    thetaCounter = 2;
    for (int j = i + 1; j < numOfQubits; j++) {
      theta = 2.0 * M_PI / pow(2.0, thetaCounter);
      thetaCounter++;
      // printf("Theta: %f\n", theta);
      controlledPhaseShift(qubits, j, i, theta);
    }
  }
  
  printAllAmplitudes(qubits);
  measureAllAndPrint(qubits);
  // measureAndPrint(qubits, 0);
  
  unloadQuEST(&env, &qubits);
}

void inverseQFT() {
  printf("Running Inverse Quantum Fourier Transform...\n");
  int numOfQubits = 3;
  QuESTEnv env;
  Qureg qubits;
  loadQuEST(&env, &qubits, numOfQubits);
  qreal theta = 0.0;
  int thetaCounter = numOfQubits;
  
  // apply circuit
  printf("Applying Quantum Circuit...\n");
  initOneState(qubits);
  
  // This might be upside-down...
  for (int i = numOfQubits - 1; i >= 0; i--) {
    // printf("i: %d\n", i);
    thetaCounter = numOfQubits - i;
    for (int j = numOfQubits - 1; j > i; j--) {
      // printf("    j: %d\n", j);
      theta = 2.0 * M_PI / pow(2.0, thetaCounter);
      // printf("    Theta Counter: %d\n", thetaCounter);
      thetaCounter--;
      controlledPhaseShift(qubits, j, i, theta);
    }
    hadamard(qubits, i);
  }
  
  printAllAmplitudes(qubits);
  measureAllAndPrint(qubits);
  // measureAndPrint(qubits, 0);
  
  unloadQuEST(&env, &qubits);
}

int main(int narg, char *varg[]) {
  struct timespec start, stop;
  clock_gettime(CLOCK_REALTIME, &start);

  // Insert Desired Function Here:
  inverseQFT();

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