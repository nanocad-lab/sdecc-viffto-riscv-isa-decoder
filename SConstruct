#!/bin/python

import platform
os_info = platform.platform()
print os_info

if os_info.find('Linux') >= 0 or os_info.find('Darwin') >= 0: # Linux or Mac build
    env = Environment()
    env.Append(CPPFLAGS = '-Wall -O3')
    env.Append(CPPPATH = ['src',])
elif os_info.find('Windows') >= 0:
    env = Environment(MSVC_USE_SCRIPT = 'C:\\Program Files (x86)\\Microsoft Visual Studio 14.0\\VC\\bin\\x86_amd64\\vcvarsx86_amd64.bat')
    env.Append(CPPFLAGS = ['/c','/nologo','/W3','/WX-','/O2'])
else:
    print 'unknown platform, exiting'
    exit(1)
sources = [
    'src/main.cc',
    'src/MyDecoder.cc'
]

defaultBuild = env.Program(target = 'riscvdecode', source = sources)
Default(defaultBuild)
