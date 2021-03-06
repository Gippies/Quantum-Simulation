#include <stdio.h>
#include <math.h>

#include <QuEST.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

void loadQuEST(QuESTEnv *env, Qureg *qubits, int numOfQubits) {
  printf("Initializing %d Qubits...\n", numOfQubits);
  *env = createQuESTEnv();
  *qubits = createQureg(numOfQubits, *env);
}

void unloadQuEST(QuESTEnv *env, Qureg *qubits) {
  printf("Unloading Qubits...\n");
  destroyQureg(*qubits, *env); 
  destroyQuESTEnv(*env);
}

void initOneState(Qureg qubits) {
  initZeroState(qubits);
  int numQubits = getNumQubits(qubits);
  for (int i = 0; i < numQubits; i++) {
    pauliX(qubits, i);
  }
}

void initValueState(Qureg qubits, int value) {
  // Inits Qubits to a specified value in binary.
  initZeroState(qubits);
  if (value != 0) {
    int numQubits = getNumQubits(qubits);
    if (value > (int) pow(2, numQubits) - 1 || value < 0) {
      printf("ERROR ERROR, init value out of range. Keeping qubits as 0...\n");
      return;
    }
    printf("Setting Qubits Value to %d...\n", value);
    for (int i = 0; i < numQubits && value > 0; i++) {
      if (value % 2 == 1)
        pauliX(qubits, i);
      value /= 2;
    }
    printf("Resuming Circuit...\n");
  }
}

void CTTGate(Qureg qubits, const int targetQubit) {
  phaseShift(qubits, targetQubit, -M_PI/4);
}

void swapGate(Qureg qubits, const int qubit1, const int qubit2) {
  controlledNot(qubits, qubit1, qubit2);
  controlledNot(qubits, qubit2, qubit1);
  controlledNot(qubits, qubit1, qubit2);
}

void swapAll(Qureg qubits) {
  int numQubits = getNumQubits(qubits);
  for (int i = 0; i < numQubits / 2; i++) {
    swapGate(qubits, i, (numQubits - 1) - i);
  }
}

void swapAllInRange(Qureg qubits, int start, int end) {
  int lastSwap = (start + end) / 2;
  for (int i = start; i < lastSwap; i++) {
    swapGate(qubits, i, (end - 1) - i);
  }
}

void multiToffoliGate(Qureg qubits, int *controlQubits, const int numControlQubits, const int targetQubit) {
  ComplexMatrix2 u;
  u.r0c0 = (Complex) {.real=0.0, .imag=0.0};
  u.r0c1 = (Complex) {.real=1.0, .imag=0.0};
  u.r1c0 = (Complex) {.real=1.0, .imag=0.0};
  u.r1c1 = (Complex) {.real=0.0, .imag=0.0};
  
  multiControlledUnitary(qubits, controlQubits, numControlQubits, targetQubit, u);
}

void QFTCircuit(Qureg qubits, int start, int end) {
  qreal theta;
  int thetaCounter;
  for (int i = end - 1; i >= start; i--) {
    hadamard(qubits, i);
    thetaCounter = 2;
    for (int j = i - 1; j >= start; j--) {
      theta = 2.0 * M_PI / pow(2.0, thetaCounter);
      thetaCounter++;
      controlledPhaseShift(qubits, j, i, theta);
    }
  }
  swapAllInRange(qubits, start, end);
}

void inverseQFTCircuit(Qureg qubits, int start, int end) {
  swapAllInRange(qubits, start, end);
  qreal theta;
  int thetaCounter;
  for (int i = start; i < end; i++) {
    thetaCounter = i + 1;
    for (int j = start; j < i; j++) {
      theta = -(2.0 * M_PI / pow(2.0, thetaCounter));
      thetaCounter--;
      controlledPhaseShift(qubits, j, i, theta);
    }
    hadamard(qubits, i);
  }
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
  // NOTE: As each qubit is measured, it collapses and thus, the probabilities change for all
  // quantumly entangled qubits!! Use printAllAmplitudes before this method to determine amplitudes
  // without collapse.
  printf("Measuring Qubits...\n");
  qreal finalProb;
  int measuredValue;
  int numQubits = getNumQubits(qubits);
  for (int i = numQubits - 1; i >= 0; i--) {
    measuredValue = measureWithStats(qubits, i, &finalProb);
    printf("Qubit %d: Collapsed Value: %d, Probability: %f\n", i, measuredValue, finalProb);
  }
}

void measureAndPrintInRange(Qureg qubits, int start, int end) {
  printf("Measuring Qubits...\n");
  qreal finalProb;
  int measuredValue;
  for (int i = end - 1; i >= start; i--) {
    measuredValue = measureWithStats(qubits, i, &finalProb);
    printf("Qubit %d: Collapsed Value: %d, Probability: %f\n", i, measuredValue, finalProb);
  }
}

int measureAndPrintInRangeWithReturn(Qureg qubits, int start, int end) {
  printf("Measuring Qubits...\n");
  qreal finalProb;
  int measuredValue, finalValue = 0;
  for (int i = end - 1; i >= start; i--) {
    measuredValue = measureWithStats(qubits, i, &finalProb);
    printf("Qubit %d: Collapsed Value: %d, Probability: %f\n", i, measuredValue, finalProb);
    if (measuredValue == 1)
      finalValue += pow(2, i - start);
  }
  printf("Final Value From Qubits: %d\n", finalValue);
  return finalValue;
}

void measureAndPrint(Qureg qubits, int measureQubit) {
  printf("Measuring Qubit %d...\n", measureQubit);
  qreal probability;
  int measuredValue;
  measuredValue = measureWithStats(qubits, measureQubit, &probability);
  printf("Qubit %d: Collapsed Value: %d, Probability: %f\n", measureQubit, measuredValue, probability);
}