from os import system
system("rm mytsh.out")
for i in range(1, 17):
	cmd = './sdriver.pl -t trace%02d.txt -s ./tsh -a "-p" >> mytsh.out' % i
	print(cmd)
	with open("mytsh.out", "a") as f:
		f.write('./sdriver.pl -t trace%02d.txt -s ./tsh -a "-p"\n' % i)
	system(cmd)
