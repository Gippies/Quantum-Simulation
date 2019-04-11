
void loadQuEST(QuESTEnv *env, Qureg *qubits, int numOfQubits);
void unloadQuEST(QuESTEnv *env, Qureg *qubits);
void initOneState(Qureg qubits);
void initValueState(Qureg qubits, int value);
void CTTGate(Qureg qubits, const int targetQubit);
void swapGate(Qureg qubits, const int qubit1, const int qubit2);
void swapAll(Qureg qubits);
void swapAllInRange(Qureg qubits, int start, int end);
void multiToffoliGate(Qureg qubits, int *controlQubits, const int numControlQubits, const int targetQubit);
void QFTCircuit(Qureg qubits, int start, int end);
void inverseQFTCircuit(Qureg qubits, int start, int end);
void printAllAmplitudes(Qureg qubits);
void printAllAmplitudesInRange(Qureg qubits, int start, int end);
void measureAllAndPrint(Qureg qubits);
void measureAndPrintInRange(Qureg qubits, int start, int end);
void measureAndPrint(Qureg qubits, int measureQubit);
