target_sources(Serialization
  PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/Attributes.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Attributes.hpp
    ${CMAKE_CURRENT_LIST_DIR}/BinaryStream.cpp
    ${CMAKE_CURRENT_LIST_DIR}/BinaryStream.hpp
    ${CMAKE_CURRENT_LIST_DIR}/JsonSerializers.cpp
    ${CMAKE_CURRENT_LIST_DIR}/JsonSerializers.hpp
    ${CMAKE_CURRENT_LIST_DIR}/MetaSerialization.cpp
    ${CMAKE_CURRENT_LIST_DIR}/MetaSerialization.hpp
    ${CMAKE_CURRENT_LIST_DIR}/SerializationHelpers.hpp
    ${CMAKE_CURRENT_LIST_DIR}/SerializationPolicy.hpp
    ${CMAKE_CURRENT_LIST_DIR}/ContainerSerializationPolicies.hpp
    ${CMAKE_CURRENT_LIST_DIR}/Serializer.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Serializer.hpp
)