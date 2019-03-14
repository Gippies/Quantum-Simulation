#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include <QuEST.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

void loadQuEST(QuESTEnv *env, Qureg *qubits, int numOfQubits) {
  printf("Initializing Qubits...\n");
  *env = createQuESTEnv();
  *qubits = createQureg(numOfQubits, *env);
}

void unloadQuEST(QuESTEnv *env, Qureg *qubits) {
  printf("Unloading Qubits...\n");
  destroyQureg(*qubits, *env); 
  destroyQuESTEnv(*env);
}

void CTTGate(Qureg qubits, const int targetQubit) {
  phaseShift(qubits, targetQubit, -M_PI/4);
}

void printAllAmplitudes(Qureg qubits) {
  int numAmps = getNumAmps(qubits);
  if (numAmps <= 1024) {
    for (int i = 0; i < numAmps; i++) {
      Complex amp = getAmp(qubits, i);
      printf("Amplitude of %d real: %f imaginary: %f\n", i, amp.real, amp.imag);
    }
  }
  else {
    printf("Too many amplitudes to print, skipping...\n");
  }
}

void printAllAmplitudesInRange(Qureg qubits, int start, int end) {
  int numAmps = getNumAmps(qubits);
  if (end <= numAmps) {
    for (int i = start; i < end; i++) {
      Complex amp = getAmp(qubits, i);
      printf("Amplitude of %d real: %f imaginary: %f\n", i, amp.real, amp.imag);
    }
  }
  else {
    printf("End is larger than the number of amplitudes, skipping...\n");
  }
}

void randomNumberGenerator() {

  // load QuEST with certain number of qubits
  int numOfQubits = 10;
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
  randomNumberGenerator();
  return 0;
}