#!/usr/bin/env python3

import numpy as np
import argparse

'''将EMAN2的输出角度保存为lst文件'''
parser = argparse.ArgumentParser(description='Convert EMAN2 text file to lst file')
parser.add_argument('input', metavar='Input_file', help='The text file generated by e2project3d.py')
args = parser.parse_args()

input_file = args.input
file_name = input_file.split('.')[0]

with open(input_file, 'r') as file:
    lines = file.readlines()[3:-1]

psi_angles = []
theta_angles = []
phi_angles = []
for i in lines:
    psi = np.float32(i.split('\t')[1]) # azimuthal
    theta = np.float32(i.split('\t')[2]) # altitude
    phi = np.float32((i.split('\t')[3]).split('\\')[0]) # psi
    psi_angles.append(psi)
    theta_angles.append(theta)
    phi_angles.append(phi)

data = np.stack([theta_angles, psi_angles, phi_angles], axis=1)
np.savetxt(f'{file_name}.lst', data, delimiter='\t', fmt='%3.4f')