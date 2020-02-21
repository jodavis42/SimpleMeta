set(CurrentDirectory ${CMAKE_CURRENT_LIST_DIR})

add_executable(SimpleMeta "")

include(${CMAKE_CURRENT_LIST_DIR}/CMakeFiles.cmake)

target_include_directories(SimpleMeta 
    PUBLIC
    ${CurrentDirectory}
    ${RapidJsonLocation}
)
Set_Common_TargetCompileOptions(SimpleMeta)
target_link_libraries(SimpleMeta
                      Reflection
                      Serialization
                      UnitTests
)

add_custom_command(TARGET SimpleMeta POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/DataDrivenTests/ $<TARGET_FILE_DIR:SimpleMeta>/DataDrivenTests)
