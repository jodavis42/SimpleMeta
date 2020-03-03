set(CurrentDirectory ${CMAKE_CURRENT_LIST_DIR})

add_library(Reflection "")

include(${CMAKE_CURRENT_LIST_DIR}/CMakeFiles.cmake)

target_include_directories(Reflection 
    PUBLIC
    ${CurrentDirectory}
)
Set_Common_TargetCompileOptions(Reflection)
target_link_libraries(Reflection)

ADD_MSVC_PRECOMPILED_HEADER(Reflection "Precompiled.hpp" "Precompiled.cpp")
