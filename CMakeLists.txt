cmake_minimum_required(VERSION 3.16)

project(v-directory-encryptor VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

set(PROJECT_SOURCES
        src/main.cpp
        src/mainwindow.cpp
        header/mainwindow.h
        ui/mainwindow.ui
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(v-directory-encryptor
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        header/DirectoryDFS.h header/Encrypted.h src/DirectoryDFS.cpp src/Encrypted.cpp src/EncryptedAlg.cpp
        ui/resources.qrc
        test_files/bibles/bible1/bible.txt test_files/bibles/bible1/encrypticon.png test_files/bibles/bible1/fortnitemap.jpg test_files/bibles/bible2/bible1/bible.txt test_files/bibles/bible2/bible1/encrypticon.png test_files/bibles/bible2/bible1/fortnitemap.jpg test_files/bibles/bible2/bible.txt test_files/bibles/bible2/encrypticon.png test_files/bibles/bible2/fortnitemap.jpg test_files/bibles/bible.txt test_files/bibles/encrypticon.png test_files/bibles/fortnitemap.jpg test_files/bible.txt test_files/encrypticon.png test_files/fortnitemap.jpg
        test_files/bibles/bible1/bible.txt test_files/bibles/bible1/encrypticon.png test_files/bibles/bible1/fortnitemap.jpg test_files/bibles/bible2/bible1/bible.txt test_files/bibles/bible2/bible1/encrypticon.png test_files/bibles/bible2/bible1/fortnitemap.jpg test_files/bibles/bible2/bible.txt test_files/bibles/bible2/encrypticon.png test_files/bibles/bible2/fortnitemap.jpg test_files/bibles/bible.txt test_files/bibles/encrypticon.png test_files/bibles/fortnitemap.jpg test_files/bible.txt test_files/encrypticon.png test_files/fortnitemap.jpg
    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET v-directory-encryptor APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation
else()
    if(ANDROID)
        add_library(v-directory-encryptor SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(v-directory-encryptor
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(v-directory-encryptor PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.v-directory-encryptor)
endif()
set_target_properties(v-directory-encryptor PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS v-directory-encryptor
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(v-directory-encryptor)
endif()
