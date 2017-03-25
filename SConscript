

import os

env=Environment(CC='clang',CXX='clang++')

ROOT=os.path.realpath('./')

if ARGUMENTS.get('release', 0):
    env.Append(CFLAGS=['-DNDEBUG'])
    env.Append(CCFLAGS=['-DNDEBUG'])

env.Append(CFLAGS=['-DLINUX'])
env.Append(CFLAGS=['-std=gnu99'])
env.Append(CFLAGS=['-I'+ROOT])
env.Append(CFLAGS=['-I'+ROOT+'/libs/linux64/include'])

env.Append(CCFLAGS=['-DLINUX'])
env.Append(CCFLAGS=['-std=gnu++11','-Wno-format'])
env.Append(CCFLAGS=['-I'+ROOT])
env.Append(CCFLAGS=['-I'+ROOT+'/libs/linux64/include'])

env.Append(LIBPATH=[ROOT])
env.Append(LIBPATH=[ROOT+'/libs/linux64/lib'])

env.Append(LIBS=['stdc++'])
env.Append(LIBS=['pthread'])
env.Append(LIBS=['jansson'])
env.Append(LIBS=['z'])
env.Append(LIBS=['uv'])
env.Append(LIBS=['expat'])

env.Append(OBJS=env.Object(Glob('ext/*.c'),CCFLAGS=[]))

env.Append(OBJS=env.Object(Glob('math/kazmath/*.c'),CCFLAGS=[]))

env.Append(OBJS=env.Object(Glob('core/*.cpp'),CFLAGS=[]))

env.Append(OBJS=env.Object(Glob('math/*.cpp'),CFLAGS=[]))

env.Append(OBJS=env.Object(Glob('udp/*.cpp'),CFLAGS=[]))

Export('env')


