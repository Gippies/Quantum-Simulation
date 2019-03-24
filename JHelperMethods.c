#include <stdio.h>

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

void swapGate(Qureg qubits, const int qubit1, const int qubit2) {
  controlledNot(qubits, qubit1, qubit2);
  controlledNot(qubits, qubit2, qubit1);
  controlledNot(qubits, qubit1, qubit2);
}

void multiToffoliGate(Qureg qubits, int *controlQubits, const int numControlQubits, const int targetQubit) {
  ComplexMatrix2 u;
  u.r0c0 = (Complex) {.real=0.0, .imag=0.0};
  u.r0c1 = (Complex) {.real=1.0, .imag=0.0};
  u.r1c0 = (Complex) {.real=1.0, .imag=0.0};
  u.r1c1 = (Complex) {.real=0.0, .imag=0.0};
  
  multiControlledUnitary(qubits, controlQubits, numControlQubits, targetQubit, u);
}

void printAllAmplitudes(Qureg qubits) {
  printf("Printing All Amplitudes...\n");
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
  printf("Printing All Amplitudes From %d to %d...\n", start, end);
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

void measureAllAndPrint(Qureg qubits) {
  printf("Measuring Qubits...\n");
  qreal finalProb;
  int measuredValue;
  int numQubits = getNumQubits(qubits);
  for (int i = 0; i < numQubits; i++) {
    measuredValue = measureWithStats(qubits, i, &finalProb);
    printf("Qubit %d: Collapsed Value: %d, Probability: %f\n", i, measuredValue, finalProb);
  }
}

void measureAndPrint(Qureg qubits, int measureQubit) {
  printf("Measuring Qubit %d...\n", measureQubit);
  qreal probability;
  int measuredValue;
  measuredValue = measureWithStats(qubits, measureQubit, &probability);
  printf("Qubit %d: Collapsed Value: %d, Probability: %f\n", measureQubit, measuredValue, probability);
}