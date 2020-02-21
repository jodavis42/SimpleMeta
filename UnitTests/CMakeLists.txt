set(CurrentDirectory ${CMAKE_CURRENT_LIST_DIR})

add_library(UnitTests "")

include(${CMAKE_CURRENT_LIST_DIR}/CMakeFiles.cmake)

target_include_directories(UnitTests 
    PUBLIC
    ${CurrentDirectory}
    ${RapidJsonLocation}
)
Set_Common_TargetCompileOptions(UnitTests)
target_link_libraries(UnitTests
                      Reflection
                      Serialization
)

add_custom_command(TARGET UnitTests POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/DataDrivenTests/ $<TARGET_FILE_DIR:UnitTests>/DataDrivenTests)
