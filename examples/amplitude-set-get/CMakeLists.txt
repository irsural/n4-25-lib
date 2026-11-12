find_package(Threads REQUIRED)

add_executable(
  amplitude-set-get
  main.cpp
  actions.cpp
  actions.h
  ts_queue.h
  utils.h
)
target_link_libraries(amplitude-set-get PRIVATE n4_25::n4_25 Threads::Threads)
