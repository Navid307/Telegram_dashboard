# CMake generated Testfile for 
# Source directory: /home/navid/Github/raspberry_pi_monorepo/dashboard/tests
# Build directory: /home/navid/Github/raspberry_pi_monorepo/dashboard/build/tests
# 
# This file includes the relevant testing commands required for 
# testing this directory and lists subdirectories to be tested as well.
add_test(dashboard_tests "/home/navid/Github/raspberry_pi_monorepo/dashboard/build/tests/dashboard_tests")
set_tests_properties(dashboard_tests PROPERTIES  _BACKTRACE_TRIPLES "/home/navid/Github/raspberry_pi_monorepo/dashboard/tests/CMakeLists.txt;25;add_test;/home/navid/Github/raspberry_pi_monorepo/dashboard/tests/CMakeLists.txt;0;")
subdirs("googletest")
