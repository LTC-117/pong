cmake_minimum_required(VERSION 3.10)

# Nome do projeto
project(pong VERSION 1.0 LANGUAGES C)

# Define os diretórios
set(SRC_DIR "src")
set(INC_DIR "inc")
set(BUILD_DIR "build")

# Configura os arquivos fonte e de cabeçalho
file(GLOB SOURCES "${SRC_DIR}/*.c")
file(GLOB HEADERS "${INC_DIR}/*.h")

# Localiza o SDL2
find_package(SDL2 REQUIRED)

# Define o executável
add_executable(pong ${SOURCES})

# Inclui o diretório de cabeçalhos
include_directories(${INC_DIR} ${SDL2_INCLUDE_DIRS})

# Linka o SDL2 ao executável
target_link_libraries(pong ${SDL2_LIBRARIES})

# Define o diretório de build para os binários
set_target_properties(pong PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${BUILD_DIR})
