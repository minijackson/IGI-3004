gestion_fichiers_benchmark = executable('gestion_fichiers_benchmark', 'gestion_fichiers.cpp',
	include_directories: gestion_fichiers_inc,
	link_with: libgestion_fichiers,
	dependencies: celero_dep)
pipe_benchmark = executable('pipe_benchmark', 'pipe.cpp',
	include_directories: [pipe_inc, gestion_fichiers_inc],
	link_with: [libpipe, libgestion_fichiers],
	dependencies: [celero_dep, thread_dep])

benchmark('Gestion Fichiers', gestion_fichiers_benchmark, args: ['-t', 'gestion_fichiers_benchmark.csv'])
benchmark('Pipe',             pipe_benchmark,             args: ['-t', 'pipe_benchmark.csv'])

# vim: ft=cmake
