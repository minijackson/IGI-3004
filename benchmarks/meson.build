gestion_fichiers_benchmark = executable('gestion_fichiers_benchmark', 'gestion_fichiers.cpp',
	include_directories: gestion_fichiers_inc,
	link_with: libgestion_fichiers,
	dependencies: celero_dep)

benchmark('Gestion Fichiers', gestion_fichiers_benchmark, args: ['-t', 'gestion_fichiers_benchmark.csv'])

# vim: ft=cmake
