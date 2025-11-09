include(FetchContent)
#  URL_MD5 2c4438b3e5b2d81a6e626ecf72bf75be
# SFML
FetchContent_Declare(
  sfml
  URL https://github.com/SFML/SFML/archive/refs/tags/3.0.0.zip

)
add_subdirectory(sfml)

# Dear ImGui
FetchContent_Declare(
  imgui
  URL https://github.com/ocornut/imgui/archive/refs/tags/v1.91.9.zip
)

FetchContent_MakeAvailable(imgui)

# ImGui-SFML
FetchContent_Declare(
  imgui-sfml
  URL https://github.com/SFML/imgui-sfml/archive/refs/tags/v3.0.zip

)
add_subdirectory(imgui-sfml)
