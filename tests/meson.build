boost_testing = dependency('boost', modules: 'test')

gestion_fichiers_testing = executable('gestion_fichiers_testing', 'gestion_fichiers_testing.cpp',
	include_directories: gestion_fichiers_inc,
	link_with: libgestion_fichiers,
	dependencies: boost_testing)
pipe_testing = executable('pipe_testing', 'pipe_testing.cpp',
	include_directories: [gestion_fichiers_inc, pipe_inc],
	link_with: [libgestion_fichiers, libpipe],
	dependencies: boost_testing)
semaphore_testing = executable('semaphore_testing', 'semaphore_testing.cpp',
	include_directories: semaphore_inc,
	link_with: libsemaphore,
	dependencies: [boost_testing, thread_dep])
thread_testing = executable('thread_testing', 'thread_testing.cpp',
	include_directories: thread_inc,
	link_with: libthread,
	dependencies: [boost_testing, thread_dep])
memory_testing = executable('memory_testing', 'memory_testing.cpp',
	include_directories: memory_inc,
	dependencies: boost_testing)

test('Gestion Fichiers Testing', gestion_fichiers_testing, args: ['-l', 'test_suite'])
test('Pipe Testing',             pipe_testing,             args: ['-l', 'test_suite'])
test('Semaphore Testing',        semaphore_testing,        args: ['-l', 'test_suite'])
test('Thread Testing',           thread_testing,           args: ['-l', 'test_suite'])
test('Memory Testing',           memory_testing,           args: ['-l', 'test_suite'])

# vim: ft=cmake
