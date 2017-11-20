project('TimeGen2FW', 'c')

inc = include_directories('include', '/opt/bcc-2.0.1-gcc/sparc-gaisler-elf/bsp/leon3/include/')

fw = executable('tg2FW','src/main.c', include_directories : inc,
	c_args : ['-msoft-float']
)

mkprom2 = find_program('/opt/mkprom2/mkprom2')
objcopy = find_program('/opt/bcc-2.0.1-gcc/bin/sparc-gaisler-elf-objcopy')
ld = find_program('/opt/bcc-2.0.1-gcc/bin/sparc-gaisler-elf-ld')
gcc = find_program('/opt/bcc-2.0.1-gcc/bin/sparc-gaisler-elf-gcc')
grmon = find_program('/opt/grmon-eval-64-2.0.86/grmon-eval-2.0.86/linux64/bin/grmon')


prom = custom_target('TimeGen2FW.prom',
  input : fw,
  output : 'TimeGen2FW.prom',
  command : [mkprom2, '-duart', '0x80000100', '-baud', '115200', '-spimeas', '-v', '-msoft-float', '-leon3', '-nosram',
  	'-sdram', '16', '-sdrambanks', '2', '-col', '8', '@INPUT@', '-o', '@OUTPUT@' ]
)

flash_payload = custom_target('payload.bin',
  input : prom,
  output : 'payload.bin',
  command : [objcopy, '-O', 'binary', '@INPUT@', '@OUTPUT@'
  ]
)

flash_payload_object = custom_target('payload.o',
  input : flash_payload,
  output : 'payload.o',
  command : [ld, '-r', '-b', 'binary', '@INPUT@', '-o', '@OUTPUT@'
  ]
)

#flasher = custom_target('flasher',
#	input : flash_payload,
#	output : 'flasher',
#	command : [gcc, '-msoft-float', 'src/main.c', '@INPUT@', '-o', '@OUTPUT@'],
#)

flasher = executable('flasher','flasher/main.c', flash_payload_object, include_directories : inc,
	c_args : ['-msoft-float']
)


run_target('load',
  command : [grmon,
  '-uart', '/dev/ttyUSB3', '-ulb',
  '-e', 'load  @0@; run;'.format(flasher.full_path())],
  depends : flasher
)
