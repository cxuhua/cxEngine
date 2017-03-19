

env = Environment()

ROOT = '/root/cxengine'

env.Append(CCFLAGS=['-DLINUX'])
env.Append(CCFLAGS=['-DNDEBUG'])
env.Append(CCFLAGS=['-I'+ROOT])
env.Append(CCFLAGS=['-I'+ROOT+'/libs/linux64/include'])

env.Append(LIBPATH=[ROOT])
env.Append(LIBPATH=[ROOT+'/libs/linux64/lib'])

Export('env')

Library('libext.a',Glob('ext/*.c'),CFLAGS=[env['CCFLAGS'],'-std=gnu99'])

Library('libkazmath.a',Glob('math/kazmath/*.c'),CFLAGS=[env['CCFLAGS'],'-std=gnu99'])

Library('libcore.a',Glob('core/*.cpp'),CCFLAGS=[env['CCFLAGS'],'-std=gnu++11'])

Library('libmath.a',Glob('math/*.cpp'),CCFLAGS=[env['CCFLAGS'],'-std=gnu++11'])

Library('libudp.a',Glob('udp/*.cpp'),CCFLAGS=[env['CCFLAGS'],'-std=gnu++11'])



