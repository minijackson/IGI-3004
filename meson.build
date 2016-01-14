project('Système d\'exploitation', 'cpp', license: 'MIT')

add_global_arguments(['-std=c++14', '-fdiagnostics-color=always'], language: 'cpp')

thread_dep = dependency('threads')

subdir('./src/gestion-fichiers')
subdir('./src/pipe')
subdir('./src/procutils')

subdir('./src/TD1')
subdir('./src/TD2')
subdir('./src/TD3')
subdir('./src/TD4')

# vim: ft=cmake
