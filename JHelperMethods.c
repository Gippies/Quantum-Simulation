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
