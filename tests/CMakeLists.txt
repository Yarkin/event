add_compile_options(-Wno-unused-parameter)

aux_source_directory(./ xevent_test_dir)
add_executable(xevent_test ${xevent_test_dir} ../src/subscriber.cpp ../src/broker.cpp ../src/publisher.cpp)

target_link_libraries(xevent_test PRIVATE gtest gtest_main)