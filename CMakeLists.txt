cmake_minimum_required(VERSION 3.10)
project(coursework)

set(CMAKE_CXX_STANDARD 17)

# Вказуємо шлях до SFML вручну
set(SFML_DIR "/lib/cmake/SFML")

# Підключаємо SFML
find_package(SFML 2.5.0 REQUIRED graphics window system)

# Додаємо виконуваний файл
add_executable(coursework main.cpp)

# Лінкуємо з бібліотеками SFML
target_link_libraries(coursework sfml-graphics sfml-window sfml-system)

file(COPY images/ DESTINATION ${CMAKE_BINARY_DIR}/images)
file(COPY testsfiles/ DESTINATION ${CMAKE_BINARY_DIR}/testsfiles)
file(COPY fonts/ DESTINATION ${CMAKE_BINARY_DIR}/fonts)
file(COPY menu.txt DESTINATION ${CMAKE_BINARY_DIR}/)