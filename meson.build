project('TimeGen2FW', 'c')

inc = include_directories('include', '/opt/bcc-2.0.1-gcc/sparc-gaisler-elf/bsp/leon3/include/')

fw = executable('tg2FW','src/main.c', 'src/peripherals.c', include_directories : inc,
	c_args : ['-msoft-float']
)

mkprom2 = find_program('mkprom2')
objcopy = find_program('sparc-gaisler-elf-objcopy')
grmon = find_program('grmon')
make_flasher = find_program('flasher/make_flasher.py')

prom = custom_target('TimeGen2FW.prom',
  input : fw,
  output : 'TimeGen2FW.prom',
  command : [mkprom2, '-duart', '0x80000100', '-baud', '115200', '-spimeas', '-v', '-msoft-float', '-nocomp', '-leon3', '-nosram',
  	'-sdram', '16', '-sdrambanks', '1', '-col', '8', '@INPUT@', '-o', '@OUTPUT@' ]
)

srec = custom_target('TimeGen2FW.srec',
  input : fw,
  output : 'TimeGen2FW.srec',
  command : [objcopy, '-O', 'srec', '@INPUT@', '@OUTPUT@'
  ]
)

flash_payload = custom_target('payload.bin',
  input : prom,
  output : 'payload.bin',
  command : [objcopy, '-O', 'binary', '@INPUT@', '@OUTPUT@'
  ]
)

flash_srec = custom_target('prom.srec',
  input : prom,
  output : 'prom.srec',
  command : [objcopy, '-O', 'srec', '@INPUT@', '@OUTPUT@'
  ]
)

flasher = custom_target('flasher',
  input : flash_payload,
  output : 'flasher',
  command : [make_flasher, '--payload', '@INPUT@', '--output', '@OUTPUT@'
  ]
)

run_target('load',
  command : [grmon,
  '-uart', '/dev/ttyUSB2', '-baud', '921600', '-ulb',
  '-e', 'load  @0@; run;'.format(flasher.full_path())],
  depends : flasher
)
