set(CurrentDirectory ${CMAKE_CURRENT_LIST_DIR})

add_library(Serialization "")

include(${CMAKE_CURRENT_LIST_DIR}/CMakeFiles.cmake)

target_include_directories(Serialization 
    PUBLIC
    ${CurrentDirectory}
    ${RapidJsonLocation}
)
Set_Common_TargetCompileOptions(Serialization)
target_link_libraries(Serialization
                      Reflection
)
