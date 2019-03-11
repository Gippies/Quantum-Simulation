from sys import argv
import numpy as np

first_arr = argv[1].split(',')
second_arr = argv[2].split(',')

first_arr = [float(x) for x in first_arr]
second_arr = [float(x) for x in second_arr]

tensor = []
for i in first_arr:
	for j in second_arr:
		tensor.append(i*j)


input_mat = np.matrix(tensor)
print("Tensor:")
print(input_mat)
input_mat = input_mat.T

cnot = np.matrix([
	[1.0, 0.0, 0.0, 0.0],
	[0.0, 1.0, 0.0, 0.0],
	[0.0, 0.0, 0.0, 1.0],
	[0.0, 0.0, 1.0, 0.0]
])

ans = np.matmul(cnot, input_mat)
print("CNOT:")
print(ans.T)