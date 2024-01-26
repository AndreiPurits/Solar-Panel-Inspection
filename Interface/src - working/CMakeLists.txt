cmake_minimum_required(VERSION 3.2)

PROJECT(ERIS)
 
project      (ERIS)
set (CMAKE_PREFIX_PATH "C:/3rdPartyLib/Qt/5.15.2/msvc2019_64")
set_target_properties(${PROJECT} PROPERTIES AUTOMOC TRUE)
set(3rdLib "C:/Users/hit-r/vcpkg/packages" CACHE PATH "Path to 3rdPartyLib")
set(opCVPfad "${3rdLib}/opencv4_x64-windows/include" CACHE PATH "Path to OpenCV dir")
set(opCV_LIBRARY "${3rdLib}/opencv4_x64-windows/lib" CACHE PATH "Path to OpenCV lib")
set(Qt5_DIR "C:/3rdPartyLib/Qt/Qt5.15.2/msvc2019_64/lib/cmake/Qt5")
#find_package (Qt5Widgets REQUIRED HINTS ${qt5Pfad})
find_package(Qt5 COMPONENTS Widgets WinExtras Gui Core REQUIRED)

include_directories (${opCVPfad}) #${qt5Pfad}/../../../include )
#link_directories    (${qt5Pfad}/../.. ${opCV_LIBRARY})
# add_definitions     (${PCL_DEFINITIONS})

set  (CMAKE_AUTOMOC ON)
set  (project_SOURCES main.cpp ERIS.cpp ClickableLabel.cpp oGraphics.cpp oIRBACS.cpp StdAfx.cpp xGlobal.cpp)
set  (project_HEADERS ERIS.h ClickableLabel.h hGlobal.h hGraphics.h hIRBACS.h StdAfx.h ldirbgrab.h)
set  (project_FORMS ERIS.ui second_screen.ui third_screen.ui)


QT5_WRAP_UI  (project_FORMS_HEADERS ${project_FORMS})

ADD_DEFINITIONS (${QT_DEFINITIONS})

ADD_EXECUTABLE  (ERIS WIN32 ${project_SOURCES}
                                ${project_FORMS_HEADERS}
                                ${project_HEADERS_MOC})

target_link_libraries(ERIS 
	Qt5::Widgets Qt5::WinExtras Qt5::Core Qt5::Gui) # kernel32.lib user32.lib gdi32.lib winspool.lib shell32.lib ole32.lib oleaut32.lib uuid.lib comdlg32.lib advapi32.lib comctl32.lib Psapi.lib)#qhullstatic.lib AMR-9.2.lib wsock32.libws2_32.lib
qt5_use_modules(ERIS Widgets)