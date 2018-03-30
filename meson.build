project('Système d\'exploitation', 'cpp', default_options : ['cpp_std=c++14'], license: 'MIT')

#add_global_arguments(['-fdiagnostics-color=always', '-Wno-c++98-compat', '-Wno-c++98-compat-pedantic', '-Wno-padded', '-Wno-shadow'], language: 'cpp')

thread_dep = dependency('threads')

subdir('./src/gestion-fichiers')
subdir('./src/pipe')
subdir('./src/procutils')
subdir('./src/semaphore')
subdir('./src/thread')
subdir('./src/memory')

subdir('./src/TD1')
subdir('./src/TD2')
subdir('./src/TD3')
subdir('./src/TD4')
subdir('./src/TD5')

celero_dep = dependency('celero', fallback: ['celero', 'celero_dep'])

subdir('./tests')
subdir('./benchmarks')

# vim: ft=cmake
