cmake_minimum_required(VERSION 3.24)
project(VirtualDjSetPro VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 REQUIRED COMPONENTS
    Core
    Gui
    Widgets
    Multimedia
    Network
    WebEngineWidgets
)

qt_standard_project_setup()

set(SOURCES
    src/main.cpp

    src/app/MainWindow.h
    src/app/MainWindow.cpp
    src/app/Style.h
    src/app/Style.cpp

    src/audio/DeckController.h
    src/audio/DeckController.cpp
    src/audio/AutoMixController.h
    src/audio/AutoMixController.cpp

    src/model/TrackItem.h
    src/model/SetPlan.h
    src/model/SetPlan.cpp

    src/services/SettingsService.h
    src/services/SettingsService.cpp
    src/services/OpenAiService.h
    src/services/OpenAiService.cpp
    src/services/YouTubeService.h
    src/services/YouTubeService.cpp

    src/widgets/DeckWidget.h
    src/widgets/DeckWidget.cpp
    src/widgets/MixerWidget.h
    src/widgets/MixerWidget.cpp
    src/widgets/PlaylistWidget.h
    src/widgets/PlaylistWidget.cpp
    src/widgets/SettingsDialog.h
    src/widgets/SettingsDialog.cpp
    src/widgets/YouTubePanel.h
    src/widgets/YouTubePanel.cpp
    src/widgets/SetBuilderPanel.h
    src/widgets/SetBuilderPanel.cpp
)

qt_add_executable(VirtualDjSetPro WIN32 MACOSX_BUNDLE ${SOURCES})

target_include_directories(VirtualDjSetPro PRIVATE src)

target_link_libraries(VirtualDjSetPro PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    Qt6::Multimedia
    Qt6::Network
    Qt6::WebEngineWidgets
)

if(WIN32)
    target_compile_definitions(VirtualDjSetPro PRIVATE NOMINMAX WIN32_LEAN_AND_MEAN)
endif()
