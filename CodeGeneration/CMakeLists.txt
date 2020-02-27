set(CurrentDirectory ${CMAKE_CURRENT_LIST_DIR})

add_executable(CodeGeneration "")

include(${CMAKE_CURRENT_LIST_DIR}/CMakeFiles.cmake)

target_include_directories(CodeGeneration 
    PUBLIC
    ${CurrentDirectory}
)
Set_Common_TargetCompileOptions(CodeGeneration)
target_link_libraries(CodeGeneration
                      Reflection
                      Serialization
)

add_definitions(-DGENERATION_FOLDER="${ReflectionLocation}")
