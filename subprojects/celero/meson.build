project('celero', 'cpp', default_options : ['cpp_std=c++14'], license: 'Apache-2.0')

thread_dep = dependency('threads')

target_h = ['celero/include/celero/Archive.h', 'celero/include/celero/Benchmark.h', 'celero/include/celero/Callbacks.h', 'celero/include/celero/Celero.h', 'celero/include/celero/CommandLine.h', 'celero/include/celero/Console.h', 'celero/include/celero/Distribution.h', 'celero/include/celero/Executor.h', 'celero/include/celero/Export.h', 'celero/include/celero/Factory.h', 'celero/include/celero/FileReader.h', 'celero/include/celero/GenericFactory.h', 'celero/include/celero/JUnit.h', 'celero/include/celero/Pimpl.h', 'celero/include/celero/PimplImpl.h', 'celero/include/celero/Print.h', 'celero/include/celero/Experiment.h', 'celero/include/celero/Result.h', 'celero/include/celero/ResultTable.h', 'celero/include/celero/Statistics.h', 'celero/include/celero/TestFixture.h', 'celero/include/celero/ThreadLocal.h', 'celero/include/celero/ThreadTestFixture.h', 'celero/include/celero/TestVector.h', 'celero/include/celero/Timer.h', 'celero/include/celero/Utilities.h']
target_src = ['celero/src/Archive.cpp', 'celero/src/Benchmark.cpp', 'celero/src/Callbacks.cpp', 'celero/src/Celero.cpp', 'celero/src/Console.cpp', 'celero/src/Distribution.cpp', 'celero/src/Executor.cpp', 'celero/src/JUnit.cpp', 'celero/src/Print.cpp', 'celero/src/Experiment.cpp', 'celero/src/Result.cpp', 'celero/src/ResultTable.cpp', 'celero/src/Statistics.cpp', 'celero/src/TestVector.cpp', 'celero/src/TestFixture.cpp', 'celero/src/ThreadTestFixture.cpp', 'celero/src/Timer.cpp', 'celero/src/Utilities.cpp']
target_inc = include_directories('celero/include')

celero_lib = shared_library('celero', target_src, include_directories: target_inc, dependencies: thread_dep)

celero_dep = declare_dependency(include_directories: target_inc, link_with: celero_lib)

# vim: ft=cmake
