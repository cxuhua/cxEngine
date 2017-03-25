

env = Environment()

ROOT = '/root/cxengine'

env.Append(CFLAGS=['-DLINUX'])
env.Append(CFLAGS=['-std=gnu99'])
env.Append(CFLAGS=['-I'+ROOT])
env.Append(CFLAGS=['-I'+ROOT+'/libs/linux64/include'])

env.Append(CCFLAGS=['-DLINUX'])
env.Append(CCFLAGS=['-std=gnu++11'])
env.Append(CCFLAGS=['-I'+ROOT])
env.Append(CCFLAGS=['-I'+ROOT+'/libs/linux64/include'])

env.Append(LIBPATH=[ROOT])
env.Append(LIBPATH=[ROOT+'/libs/linux64/lib'])

env.Library('ext',Glob('ext/*.c'),CCFLAGS=[])

env.Library('kazmath',Glob('math/kazmath/*.c'),CCFLAGS=[])

env.Library('core',Glob('core/*.cpp'),CFLAGS=[])

env.Library('math',Glob('math/*.cpp'),CFLAGS=[])

env.Library('udp',Glob('udp/*.cpp'),CFLAGS=[])

Export('env')


