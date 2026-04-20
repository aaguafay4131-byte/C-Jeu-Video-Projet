cmake_minimum_required(VERSION 3.16)
project(DinoGame LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Path to SFML - UPDATE THIS IF NEEDED
set(SFML_ROOT "C:/dev/SFML")

# Define the executable with ALL source files
add_executable(DinoGame 
    src/main.cpp
    src/Dino.cpp
    src/GroundObstacle.cpp
    src/Game.cpp
    src/Bird.cpp
)

# SFML Include Directories - Use PUBLIC so headers can find SFML
target_include_directories(DinoGame PUBLIC ${SFML_ROOT}/include)

# SFML Library Directories
target_link_directories(DinoGame PRIVATE ${SFML_ROOT}/lib)

# Link SFML Libraries (SFML 2.x - no -3 suffix)
target_link_libraries(DinoGame PRIVATE
    sfml-graphics
    sfml-window
    sfml-system
)

# Copy SFML DLLs to build folder
add_custom_command(TARGET DinoGame POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${SFML_ROOT}/bin/sfml-graphics-2.dll"
    "${SFML_ROOT}/bin/sfml-window-2.dll"
    "${SFML_ROOT}/bin/sfml-system-2.dll"
    $<TARGET_FILE_DIR:DinoGame>
)

# Copy font.ttf to build folder
add_custom_command(TARGET DinoGame POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${CMAKE_SOURCE_DIR}/font.ttf"
    $<TARGET_FILE_DIR:DinoGame>
)

add_custom_command(TARGET DinoGame POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_SOURCE_DIR}/assets"
    "$<TARGET_FILE_DIR:DinoGame>/assets"
)