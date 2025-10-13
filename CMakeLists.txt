cmake_minimum_required(VERSION 3.14)
project(fast_calc)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# # --- FetchContent для FTXUI ---
include(FetchContent)
include(FetchContent)

FetchContent_Declare(
    ftxui
    GIT_REPOSITORY https://github.com/ArthurSonzogni/FTXUI.git
    GIT_TAG v6.1.9
)

FetchContent_MakeAvailable(ftxui)
FetchContent_MakeAvailable(ftxui)

add_executable(fast_calc src/main.cpp)
target_link_libraries(fast_calc
  PRIVATE ftxui::screen
  PRIVATE ftxui::dom
  PRIVATE ftxui::component
)
