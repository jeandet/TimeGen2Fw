#!/usr/bin/env python3

__author__ = "Alexis Jeandet"
__copyright__ = "Copyright 2018, Laboratory of Plasma Physics"
__credits__ = []
__license__ = "GPLv2"
__version__ = "1.0.0"
__maintainer__ = "Alexis Jeandet"
__email__ = "alexis.jeandet@member.fsf.org"
__status__ = "Development"

import argparse
import subprocess
import os

parser = argparse.ArgumentParser()
parser.add_argument("-p", "--payload", help="Elf file to flash")
parser.add_argument("-t", "--toolchain", help="Path to the toolchain", default='/opt/bcc-2.0.1-gcc/')
parser.add_argument("-o", "--output", help="Output file name", default='/opt/bcc-2.0.1-gcc/')
args = parser.parse_args()


if __name__ == '__main__':
    make_obj_cmd = ['{prefix}/bin/sparc-gaisler-elf-ld'.format(prefix = args.toolchain)]
    make_obj_args = ['-r', '-b', 'binary', args.payload, '-o', 'payload.o']
    subprocess.run(make_obj_cmd + make_obj_args)

    script_path = os.path.dirname(__file__)
    make_exe_cmd = ['{prefix}/bin/sparc-gaisler-elf-gcc'.format(prefix = args.toolchain)]
    make_exe_args = ['-msoft-float',  script_path + '/src/main.c',  script_path + '/../src/peripherals.c', 'payload.o', '-I', script_path + '/../include', '-o', args.output]

    subprocess.run(make_exe_cmd + make_exe_args)
    
