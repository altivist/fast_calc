cmake_minimum_required(VERSION 3.14)
project(fast_calc)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# # --- FetchContent для FTXUI ---
include(FetchContent)

FetchContent_Declare(
    ftxui
    GIT_REPOSITORY https://github.com/ArthurSonzogni/FTXUI.git
    GIT_TAG v6.1.9
)

FetchContent_MakeAvailable(ftxui)

include(FetchContent)
FetchContent_Declare(
    tomlplusplus
    GIT_REPOSITORY https://github.com/marzer/tomlplusplus.git
    GIT_TAG v3.4.0
)
FetchContent_MakeAvailable(tomlplusplus)


add_executable(fast_calc
src/main.cpp
src/ui/main_screen.cpp
src/ui/calc_screen.cpp
src/ui/text_screen.cpp
src/ui/history_now_screen.cpp

)

target_link_libraries(fast_calc
  tomlplusplus::tomlplusplus
  PRIVATE ftxui::screen
  PRIVATE ftxui::dom
  PRIVATE ftxui::component
)
