set(CurrentDirectory ${CMAKE_CURRENT_LIST_DIR})

add_executable(SimpleMeta "")

include(${CMAKE_CURRENT_LIST_DIR}/CMakeFiles.cmake)

target_include_directories(SimpleMeta 
    PUBLIC
    ${CurrentDirectory}
)
Set_Common_TargetCompileOptions(SimpleMeta)
target_link_libraries(SimpleMeta
                      PUBLIC
)

