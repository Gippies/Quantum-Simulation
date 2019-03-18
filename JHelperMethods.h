
void loadQuEST(QuESTEnv *env, Qureg *qubits, int numOfQubits);
void unloadQuEST(QuESTEnv *env, Qureg *qubits);
void CTTGate(Qureg qubits, const int targetQubit);
void printAllAmplitudes(Qureg qubits);
void printAllAmplitudesInRange(Qureg qubits, int start, int end);
void measureAllAndPrint(Qureg qubits);
