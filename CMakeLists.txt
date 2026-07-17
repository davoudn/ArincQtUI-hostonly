cmake_minimum_required(VERSION 3.16)
project(ArincQtUI LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets Sql SerialPort)

qt_standard_project_setup()

set(SOURCES
    src/main.cpp
    src/Utils/utils.cpp
    src/Utils/Timer.cpp
    src/Utils/TreeDataModel.cpp
    src/Utils/generaldata.cpp
    src/Utils/Database.cpp
    src/Utils/bitutils.cpp
    src/Utils/event.cpp
    src/Items/baseitem.cpp
    src/Items/Arinc429/ArincData.cpp
    src/Items/Arinc429/arinc_functions.cpp
    src/Items/Arinc429/Equipment.cpp
    src/Items/Arinc429/equipmentsids.cpp
    src/Items/Arinc429/Labels/Label.cpp
    src/Items/Arinc429/Labels/labelfor.cpp
    src/Items/Arinc429/Parameters/BaseParameter.cpp
    src/Items/Arinc429/Parameters/BNRParameter.cpp
    src/Items/Arinc429/Parameters/BCDParameter.cpp
    src/Items/Arinc429/Parameters/DiscreteParameter.cpp
    src/Items/Arinc429/Parameters/FLOATBCDParameter.cpp
    src/Items/Arinc429/Parameters/FLNOParameter.cpp
    src/Items/Arinc429/Parameters/INTParameter.cpp
    src/Items/Arinc429/Parameters/ATCBCDParameter.cpp
    src/Workers/transmitterworker.cpp
    src/Workers/receiverworker.cpp
    src/Widgets/MainWindow.cpp
    src/Widgets/Transmitter.cpp
    src/Widgets/Receiver.cpp
    src/Widgets/Dialog.cpp
    src/Widgets/settingsdialog.cpp
    src/Widgets/CheckBoxArray.cpp
    src/Hardware/DEI1016.cpp
)

set(HEADERS
    src/Utils/types.h
    src/Utils/utils.h
    src/Utils/Timer.h
    src/Utils/TreeDataModel.h
    src/Utils/generaldata.h
    src/Utils/Database.h
    src/Utils/bitutils.h
    src/Utils/event.h
    src/Utils/singleton.h
    src/Utils/PointerVector.h
    src/Utils/spsc_ring_buffer.hpp
    src/Utils/mpmc_ring_buffer.hpp
    src/Items/baseitem.h
    src/Items/Arinc429/ArincData.h
    src/Items/Arinc429/arinc_functions.h
    src/Items/Arinc429/Equipment.h
    src/Items/Arinc429/equipmentsids.h
    src/Items/Arinc429/Labels/Label.h
    src/Items/Arinc429/Labels/labelfor.h
    src/Items/Arinc429/Labels/LabelStatusEditorDelegate.h
    src/Items/Arinc429/Parameters/BaseParameter.h
    src/Items/Arinc429/Parameters/BNRParameter.h
    src/Items/Arinc429/Parameters/BCDParameter.h
    src/Items/Arinc429/Parameters/DiscreteParameter.h
    src/Items/Arinc429/Parameters/FLOATBCDParameter.h
    src/Items/Arinc429/Parameters/FLNOParameter.h
    src/Items/Arinc429/Parameters/INTParameter.h
    src/Items/Arinc429/Parameters/ATCBCDParameter.h
    src/Workers/transmitterworker.h
    src/Workers/receiverworker.h
    src/Workers/DataSourceBase.h
    src/Widgets/MainWindow.h
    src/Widgets/Transmitter.h
    src/Widgets/Receiver.h
    src/Widgets/Dialog.h
    src/Widgets/settingsdialog.h
    src/Widgets/editordelegate.h
    src/Widgets/CheckBoxArray.h
    src/Hardware/DEI1016.h
    src/Hardware/DEI1016RasberryConfigurations.h
)

set(UI_FILES
    src/Widgets/dialog.ui
    src/Widgets/receiver.ui
    src/Widgets/transmitter.ui
    src/Widgets/settingsdialog.ui
    src/Widgets/checkboxarray.ui
)

set(RESOURCE_FILES
    Icons/icons.qrc
)

qt_add_executable(ArincQtUI
    ${SOURCES}
    ${HEADERS}
    ${UI_FILES}
    ${RESOURCE_FILES}
)

target_include_directories(ArincQtUI PRIVATE
    ${CMAKE_SOURCE_DIR}/src/Utils
    ${CMAKE_SOURCE_DIR}/src/Items
    ${CMAKE_SOURCE_DIR}/src/Items/Arinc429
    ${CMAKE_SOURCE_DIR}/src/Items/Arinc429/Labels
    ${CMAKE_SOURCE_DIR}/src/Items/Arinc429/Parameters
    ${CMAKE_SOURCE_DIR}/src/Workers
    ${CMAKE_SOURCE_DIR}/src/Widgets
    ${CMAKE_SOURCE_DIR}/src/Hardware
)

set_target_properties(ArincQtUI PROPERTIES
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE TRUE
)

target_link_libraries(ArincQtUI PRIVATE
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
    Qt6::Sql
    Qt6::SerialPort
)

install(TARGETS ArincQtUI
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)
