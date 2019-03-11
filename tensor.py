from sys import argv

first_arr = argv[1].split(',')
second_arr = argv[2].split(',')

first_arr = [float(x) for x in first_arr]
second_arr = [float(x) for x in second_arr]

answer = []
for i in first_arr:
	for j in second_arr:
		answer.append(i*j)

print(answer)
