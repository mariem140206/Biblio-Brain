cmake_minimum_required(VERSION 3.15)
project(LibrarySystem)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Dependencies
find_package(Threads REQUIRED)
find_package(OpenSSL REQUIRED)

# Crow
include(FetchContent)
FetchContent_Declare(
        crow
        GIT_REPOSITORY https://github.com/CrowCpp/Crow.git
        GIT_TAG master
)
FetchContent_MakeAvailable(crow)

# Includes
include_directories(
        ${CMAKE_SOURCE_DIR}
        ${CMAKE_SOURCE_DIR}/app
        ${CMAKE_SOURCE_DIR}/utils
)

# Source files (حدد الملفات مباشرة لتجنب اختبارات Crow)
set(SOURCES
        main.cpp
        app/controllers/AuthController.cpp
        app/controllers/BookController.cpp
        app/controllers/BorrowController.cpp
        app/controllers/UserController.cpp
        app/models/Book.cpp
        app/models/Borrow.cpp
        app/models/User.cpp
        app/services/AuthService.cpp
        app/services/DateService.cpp
	app/services/PasswordHasher.cpp
)

# Executable
add_executable(library_system ${SOURCES})

# Link
target_link_libraries(library_system
        PRIVATE
        Crow::Crow
        Threads::Threads
        OpenSSL::SSL
        OpenSSL::Crypto
)

# Windows libs
if(WIN32)
    target_link_libraries(library_system PRIVATE ws2_32 Mswsock Advapi32)
endif()

# Copy resources
file(COPY public DESTINATION ${CMAKE_BINARY_DIR})

