cmake_minimum_required(VERSION 3.5)

project(FilesystemSimulation VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets LinguistTools)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets LinguistTools)

set(TS_FILES FilesystemSimulation_de_DE.ts)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        ${TS_FILES}
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(FilesystemSimulation
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        TreeItem.cpp
        TreeModel.cpp
        TreeModel.h
        TreeItem.h

            BlockWidget.h BlockWidget.cpp
            ./Backend/MBR/MBR.cpp
            ./Backend/MBR/MBR.h

            ./Backend/InterfaceForMBR/FileSystemInterface.cpp
            ./Backend/InterfaceForMBR/FileSystemInterface.h
            ./Backend/InterfaceForMBR/FileInformationInterface.cpp
            ./Backend/InterfaceForMBR/FileInformationInterface.h
            ./Backend/InterfaceForMBR/FolderInformationInterface.cpp
            ./Backend/InterfaceForMBR/FolderInformationInterface.h

            ./Backend/INodeStructure/Data/DataBlock.h
            ./Backend/INodeStructure/Data/DataBlock.cpp
            ./Backend/INodeStructure/Data/SingleIndirect.h
            ./Backend/INodeStructure/Data/SingleIndirect.cpp
            ./Backend/INodeStructure/Data/DoubleIndirect.h
            ./Backend/INodeStructure/Data/DoubleIndirect.cpp
            ./Backend/INodeStructure/Data/TripleIndirect.h
            ./Backend/INodeStructure/Data/TripleIndirect.cpp
            ./Backend/INodeStructure/Data/IndirectBlock.h
            ./Backend/INodeStructure/Data/IndirectBlock.cpp
            ./Backend/INodeStructure/Data/DataHandler.h
            ./Backend/INodeStructure/Data/DataHandler.cpp

            ./Backend/INodeStructure/INodeInterfaces/INodeAdapter.h
            ./Backend/INodeStructure/INodeInterfaces/INodeAdapter.cpp
            ./Backend/INodeStructure/INodeInterfaces/INodeFileInformation.h
            ./Backend/INodeStructure/INodeInterfaces/INodeFileInformation.cpp
            ./Backend/INodeStructure/INodeInterfaces/INodeFolderInformation.h
            ./Backend/INodeStructure/INodeInterfaces/INodeFolderInformation.cpp

            ./Backend/INodeStructure/INodeMetadata.h
            ./Backend/INodeStructure/INodeMetadata.cpp
            ./Backend/INodeStructure/INodeType.h
            ./Backend/INodeStructure/INode.cpp
            ./Backend/INodeStructure/INode.h
            ./Backend/INodeStructure/INodeSystem.h
            ./Backend/INodeStructure/INodeSystem.cpp

            ./Backend/BSFatStructure/BsCluster.cpp
            ./Backend/BSFatStructure/BsCluster.h
            ./Backend/BSFatStructure/BsDtf.cpp
            ./Backend/BSFatStructure/BsDtf.h
            ./Backend/BSFatStructure/BsFat.cpp
            ./Backend/BSFatStructure/BsFat.h
            ./Backend/BSFatStructure/BsFile.cpp
            ./Backend/BSFatStructure/BsFile.h
            ./Backend/BSFatStructure/FileAllocationTableRow.cpp
            ./Backend/BSFatStructure/FileAllocationTableRow.h
            ./Backend/BSFatStructure/BsFatAdapter/BsFatAdapter.cpp
            ./Backend/BSFatStructure/BsFatAdapter/BsFatAdapter.h
            ./Backend/BSFatStructure/BlockState.h
            ./helpers/PathHelper.cpp
            ./helpers/PathHelper.h
            addfile.h addfile.cpp addfile.ui
            ./helpers/datetimehelper.h
            ./helpers/datetimehelper.cpp
            ./Backend/BSFatStructure/FatFileInformation.cpp
            ./Backend/BSFatStructure/FatFileInformation.h
            ./Backend/BSFatStructure/FatFolderInformation.cpp
            ./Backend/BSFatStructure/FatFolderInformation.h
            ./Backend/BSFatStructure/FileAllocationTableRow.cpp
            ./Backend/BSFatStructure/FileAllocationTableRow.h
            addfolder.h addfolder.cpp addfolder.ui
            fragmentationoverview.h fragmentationoverview.cpp fragmentationoverview.ui
            toast.h toast.cpp
            globalerrormessagehandler.h globalerrormessagehandler.cpp
            deletedialog.h deletedialog.cpp deletedialog.ui
            copydialog.h copydialog.cpp copydialog.ui
            Backend/CDROMStructure/cdromadapter.h Backend/CDROMStructure/cdromadapter.cpp

    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET FilesysSim_ui APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation

    qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
else()
    if(ANDROID)
        add_library(FilesystemSimulation SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(FilesystemSimulation
            ${PROJECT_SOURCES}
        )
    endif()

    qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
endif()

target_link_libraries(FilesystemSimulation PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.FilesysSim_ui)
endif()
set_target_properties(FilesystemSimulation PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS FilesystemSimulation
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(FilesystemSimulation)
endif()
