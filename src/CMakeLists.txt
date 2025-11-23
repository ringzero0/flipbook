file(GLOB SOURCES "*.cpp")

file(GLOB HEADERS "*.h" "*.hpp" "include/*.h" "include/*.hpp")

set(ALL_FILES ${SOURCES} ${HEADERS}
        promptReq.cpp
        promptReq.h)

add_executable(FlipBook ${ALL_FILES})
target_link_libraries(FlipBook PRIVATE ImGui-SFML::ImGui-SFML nfd)


target_link_libraries(FlipBook PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/libs/lua54.lib")


# Copy DLLs needed for runtime on Windows
if(WIN32)
  if (BUILD_SHARED_LIBS)
    add_custom_command(TARGET FlipBook POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy_if_different
          $<TARGET_FILE:ImGui-SFML::ImGui-SFML>
          $<TARGET_FILE:sfml-graphics>
          $<TARGET_FILE:sfml-window>
          $<TARGET_FILE:sfml-system>
          $<TARGET_FILE_DIR:FlipBook>)
  endif()
endif()