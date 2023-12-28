set(target wordcount)
find_package (Threads REQUIRED)

set(sources
    main.cpp
)


add_executable(${target}
    ${sources}
)

target_link_libraries(${target}
    PRIVATE
    mapreduce
    ${CMAKE_THREAD_LIBS_INIT}
)
