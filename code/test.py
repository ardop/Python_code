import subprocess

p = subprocess.Popen('sudo ./test', shell=True, stdout=subprocess.PIPE, stderr=subprocess.STDOUT)
for line in p.stdout.readlines():
    print line,
