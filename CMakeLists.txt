cmake_minimum_required(VERSION 3.16)

project(AES-XEncrypt VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

set(OPENSSL_ROOT_DIR $ENV{OPENSSL_ROOT_DIR})
set(OPENSSL_INCLUDE_DIR "${OPENSSL_ROOT_DIR}/include")
set(OPENSSL_CRYPTO_LIBRARY "$ENV{OPENSSL_ROOT_DIR}/lib/VC/x64/MD/libcrypto.lib")
set(OPENSSL_SSL_LIBRARY "$ENV{OPENSSL_ROOT_DIR}/lib/VC/x64/MD/libssl.lib")

# Create imported targets manually
add_library(OpenSSL::Crypto UNKNOWN IMPORTED)
set_target_properties(OpenSSL::Crypto PROPERTIES
    IMPORTED_LOCATION "${OPENSSL_CRYPTO_LIBRARY}"
    INTERFACE_INCLUDE_DIRECTORIES "${OPENSSL_INCLUDE_DIR}"
)

add_library(OpenSSL::SSL UNKNOWN IMPORTED)
set_target_properties(OpenSSL::SSL PROPERTIES
    IMPORTED_LOCATION "${OPENSSL_SSL_LIBRARY}"
    INTERFACE_INCLUDE_DIRECTORIES "${OPENSSL_INCLUDE_DIR}"
    INTERFACE_LINK_LIBRARIES "OpenSSL::Crypto"
)

set(PROJECT_SOURCES




)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(AES-XEncrypt
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        header/DirectoryDFS.h header/Encrypted.h header/mainwindow.h
        src/DirectoryDFS.cpp src/Encrypted.cpp src/EncryptedAlg.cpp src/main.cpp src/mainwindow.cpp
        ui/encrypticon.png ui/mainwindow.ui ui/resources.qrc
        header/EncryptionThread.h
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET AES-XEncrypt APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(AES-XEncrypt SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(AES-XEncrypt
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(AES-XEncrypt PRIVATE Qt${QT_VERSION_MAJOR}::Widgets OpenSSL::SSL OpenSSL::Crypto)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.AES-XEncrypt)
endif()
set_target_properties(AES-XEncrypt PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS AES-XEncrypt
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(AES-XEncrypt)
endif()
