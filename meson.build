project('TimeGen2FW', 'c')

inc = include_directories('include', '/opt/bcc-2.0.1-gcc/sparc-gaisler-elf/bsp/leon3/include/')

fw = executable('tg2FW','src/main.c', include_directories : inc,
	c_args : ['-msoft-float']
)

mkprom2 = find_program('/opt/mkprom2/mkprom2')
grmon = find_program('/opt/grmon-eval-64-2.0.87/grmon-eval-2.0.87/linux64/bin/grmon')

prom = custom_target('TimeGen2FW.prom',
  input : fw,
  output : 'TimeGen2FW.prom',
  command : [mkprom2, '-duart', '0x80000100', '-baud', '115200', '-spimeas', '-v', '-msoft-float', '-leon3', '-nosram',
  	'-sdram', '16', '-sdrambanks', '2', '-col', '8', '@INPUT@', '-o', '@OUTPUT@' ]
)

run_target('load',
  command : [grmon,
  '-uart', '/dev/ttyUSB3', '-ulb',
  '-e', 'spim flash select 17; spim flash load  @0@; run;'.format(prom.full_path())],
  depends : prom
)
