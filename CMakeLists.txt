cmake_minimum_required(VERSION 3.10)
project(coursework)

set(CMAKE_CXX_STANDARD 17)

# Вказуємо шлях до SFML вручну
set(SFML_DIR "C:/SFML/2.5.0/lib/cmake/SFML")

# Підключаємо SFML
find_package(SFML 2.5.0 REQUIRED graphics window system)

# Додаємо виконуваний файл
add_executable(coursework main.cpp)

# Лінкуємо з бібліотеками SFML
target_link_libraries(coursework sfml-graphics sfml-window sfml-system)