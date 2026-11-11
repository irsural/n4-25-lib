find_package(Threads REQUIRED)

add_executable(amplitude-set-get main.cpp utils.h)
target_link_libraries(amplitude-set-get PRIVATE n4_25::n4_25 Threads::Threads)
