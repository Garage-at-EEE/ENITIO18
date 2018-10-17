#!/usr/bin/env python3

import subprocess

python3_command = '/home/pi/Morse-Decoder-GUI/python2Scanner.py'

while True:
    process = subprocess.Popen(python3_command, stdout=subprocess.PIPE)
    output, error = process.communicate()
    list1 = str(output).split('\\n')
    clueNum = list1[1]
    print(clueNum)
