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
  pauliX(qubits, 2);  // Sets the last qubit to 1.

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
  int numOfQubits = 6;
  QuESTEnv env;
  Qureg qubits;
  loadQuEST(&env, &qubits, numOfQubits);
  
  // apply circuit
  printf("Applying Quantum Circuit...\n");
  initValueState(qubits, 7);
  
  inverseQFTCircuit(qubits, 0, numOfQubits);

  measureAllAndPrint(qubits);
  
  unloadQuEST(&env, &qubits);
}

void txtExercise4_27() {
  printf("Running Custom Partial Cycle Permutation ...\n");
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
  initValueState(qubits, 2);
  
  for (int i = numOfQubits - 1; i >= 0; i--) {
    hadamard(qubits, i);
    thetaCounter = 2;
    for (int j = i - 1; j >= 0; j--) {
      theta = 2.0 * M_PI / pow(2.0, thetaCounter);
      thetaCounter++;
      controlledPhaseShift(qubits, j, i, theta);
    }
  }
  swapAll(qubits);
  
  printAllAmplitudes(qubits);
  measureAllAndPrint(qubits);
  
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
  initValueState(qubits, 2);
    
  // Apply Regular QFT First...
  for (int i = numOfQubits - 1; i >= 0; i--) {
    hadamard(qubits, i);
    thetaCounter = 2;
    for (int j = i - 1; j >= 0; j--) {
      theta = 2.0 * M_PI / pow(2.0, thetaCounter);
      thetaCounter++;
      controlledPhaseShift(qubits, j, i, theta);
    }
  }
  swapAll(qubits);
  
  printAllAmplitudes(qubits);
  
  swapAll(qubits);
  // Apply Inverse QFT and see if we get the initial value...
  for (int i = 0; i < numOfQubits; i++) {
    thetaCounter = i + 1;
    for (int j = 0; j < i; j++) {
      theta = -(2.0 * M_PI / pow(2.0, thetaCounter));
      thetaCounter--;
      controlledPhaseShift(qubits, j, i, theta);
    }
    hadamard(qubits, i);
  }
  
  printAllAmplitudes(qubits);
  measureAllAndPrint(qubits);
  
  unloadQuEST(&env, &qubits);
}

void phaseEstimation() {
  printf("Running Phase Estimation...\n");
  int numOfQubits = 8;
  int endOfBottomReg = 4;  // This doesn't depend on numOfQubits, but the top register does depend on being able to accurately hold the phase shift that's being estimated, so the top should be big enough to store whatever phi is.
  qreal phi = 2.0;  // This is what we're trying to estimate, it should be able to fit in the top register.
  qreal theta;
  QuESTEnv env;
  Qureg qubits;
  loadQuEST(&env, &qubits, numOfQubits);
  
  // apply circuit
  printf("Applying Quantum Circuit...\n");
  initValueState(qubits, 2);
  
  QFTCircuit(qubits, 0, endOfBottomReg);
  
  for (int i = endOfBottomReg; i < numOfQubits; i++)
    hadamard(qubits, i);
  
  // controlled-U operation...
  for (int i = endOfBottomReg; i < numOfQubits; i++) {
    theta = 2.0 * M_PI * pow(2, i - endOfBottomReg) * phi;
    for (int j = 0; j < endOfBottomReg; j++) {
      controlledPhaseShift(qubits, i, j, theta);
    }
  }
  
  inverseQFTCircuit(qubits, endOfBottomReg, numOfQubits);

  measureAndPrintInRange(qubits, endOfBottomReg, numOfQubits);
  unloadQuEST(&env, &qubits);
}

void orderFinding() {
  printf("Running Order Finding Algorithm...\n");
  int numOfQubits = 8;
  int endOfBottomReg = 4;
  qreal theta;
  qreal phi = 2.0;
  qreal r = 6.0;  // The value we're trying to find.
  QuESTEnv env;
  Qureg qubits;
  loadQuEST(&env, &qubits, numOfQubits);
  
  // apply circuit
  printf("Applying Quantum Circuit...\n");
  initZeroState(qubits);
  for (int i = 0; i < endOfBottomReg; i++)
    pauliX(qubits, i);
  
  for (int i = endOfBottomReg; i < numOfQubits; i++)
    hadamard(qubits, i);
  
  // controlled-U operation... (this would change depending on what x^j mod N is)
  // Assume x = 5, N = 9, j is superposition of top register. Then final r should be 6.
  for (int i = endOfBottomReg; i < numOfQubits; i++) {
    // You would need a 3rd quantum register to find x^j mod N, this below is a shortcut
    // that uses r (even though that's what we're trying to find) to demonstrate the similarity to the phase estimation
    // algorithm. It's also probably wrong btw.
    theta = 2.0 * M_PI * pow(2, i - endOfBottomReg) * phi / r;
    for (int j = 0; j < endOfBottomReg; j++) {
      controlledPhaseShift(qubits, i, j, theta);
    }
  }
  
  inverseQFTCircuit(qubits, endOfBottomReg, numOfQubits);
  
  // printAllAmplitudes(qubits);
  int answer = measureAndPrintInRangeWithReturn(qubits, endOfBottomReg, numOfQubits);
  answer /= phi;
  printf("Calculated Value of r: %d\n", answer);
  unloadQuEST(&env, &qubits);
}

void grover() {
  printf("Running Grover's Algorithm...\n");
  int numOfQubits = 2;
  QuESTEnv env;
  Qureg qubits;
  loadQuEST(&env, &qubits, numOfQubits);
  
  // apply circuit
  printf("Applying Quantum Circuit...\n");
  initZeroState(qubits);
  
  for (int i = 0; i < numOfQubits; i++) {
    hadamard(qubits, i);
  }
    
  // This is the Oracle:
  controlledPhaseFlip(qubits, 1, 0);  // It should result in 3 (or 11 in binary).
  // printAllAmplitudes(qubits);
  
  // This is the Diffusion Operation:
  for (int i = 0; i < numOfQubits; i++) {
    hadamard(qubits, i);
  }
  controlledPhaseFlip(qubits, 1, 0);
  
  pauliX(qubits, 1);
  controlledPhaseFlip(qubits, 1, 0);
  pauliX(qubits, 1);
  
  pauliX(qubits, 0);
  controlledPhaseFlip(qubits, 0, 1);
  pauliX(qubits, 0);
  
  for (int i = 0; i < numOfQubits; i++) {
    hadamard(qubits, i);
  }
  
  printAllAmplitudes(qubits);
  measureAllAndPrint(qubits);
  unloadQuEST(&env, &qubits);
}

int main(int narg, char *varg[]) {
  struct timespec start, stop;
  clock_gettime(CLOCK_REALTIME, &start);

  // Insert Desired Function Here:
  grover();

  clock_gettime(CLOCK_REALTIME, &stop);
  double result = (double) (stop.tv_sec - start.tv_sec) * 1e3 + (double) (stop.tv_nsec - start.tv_nsec) / 1e6;  // Milliseconds
  if (result > 1000.0) {
    result /= 1000.0;  // Convert to seconds.
    printf("Time Elapsed: %f seconds\n", result);
  }
  else {
    printf("Time Elapsed: %f milliseconds\n", result);
  }
  return 0;
}