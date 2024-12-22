cmake_minimum_required(VERSION 3.16)

project(snake LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_OSX_ARCHITECTURES "arm64")
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror -Wextra")

find_package(Qt6 REQUIRED COMPONENTS Widgets)
find_package(Curses REQUIRED)  # Locate NCurses

# Ensure NCurses is linked explicitly
include_directories(${CURSES_INCLUDE_DIR})
link_directories(${CURSES_LIBRARY_DIR})

add_executable(snake main.cpp DesktopSnake.h DesktopSnake.cc
    ../../../includes/define_snake.h
    ../../../includes/objects_snake.h
    ../../../brick_game/snake/model/snake_model.cc
    ../../../includes/snake_controller.h
    ../../../includes/snake_model.h
    ../../../includes/snake_view.h
    ../../../brick_game/snake/controller/snake_controller.cc
    ../../../gui/cli/snake_view.cc
    DesktopTetris.h DesktopTetris.cc
    ../../../brick_game/tetris/backend_tetris.cc
    ../../../brick_game/tetris/helper_tetris.cc
    ../../../brick_game/tetris/tetris.cc
    ../../../includes/backend_tetris.h
    ../../../includes/cli_tetris.h
    ../../../includes/define_tetris.h
    ../../../includes/helper_tetris.h
    ../../../includes/objects_tetris.h
    ../../../includes/tetris.h
    ../../../gui/cli/cli_tetris.cc
)

# Link both Qt6::Widgets and NCurses
target_link_libraries(snake PRIVATE Qt6::Widgets ${CURSES_LIBRARIES})

include(GNUInstallDirs)
install(TARGETS snake
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
