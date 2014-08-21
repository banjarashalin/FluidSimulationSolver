#-------------------------------------------------
#
# Project created by Shalin K Banjara on 2011-11-07
#
# Fluid Solver based on Smooth Particle Hydrodynamcis
#
# MSc Computer Animation and Visual Effects 2010-2011
#
# Bournemouth University
#
#-------------------------------------------------

#   Create a QT application with QTCore module, QTGui module and QtOpenGL module.
QT += core gui opengl xml

#   Set the config variable.
#   The qt part of the CONFIG line tells qmake that the application is built using Qt.
#   This means that qmake will link against the Qt libraries when linking and add in the neccesary include paths for compiling.
CONFIG += qt
#   The warn_on part of the CONFIG line tells qmake that it should set the compiler flags so that warnings are output.
CONFIG += warn_on
#   The release part of the CONFIG line tells qmake that the application must be built as a release application.
#   The debug part of the CONFIG line tells qmake that the application must be built as a debugging application.
#   Since release version of an application doesn't contain any debugging symbols or other debuggin information.
#CONFIG += debug
#   The console part of the CONFIG line tells the target is a Win32 console application (app only). The proper include paths, compiler flags and libraries will automatically be added to the project.
CONFIG += console

#   The app part in the TEMPLATE line tells qmake to create a Makefile for building applications.
TEMPLATE += app

#   Set the target file name.
TARGET = bin/SPHFluidSolver

#   Set the destination directory where the target file is saved.
DESTDIR=./

#   This OBJECTS_DIR variable specifies the directory where all intermediate objects should be placed.
OBJECTS_DIR=./obj/
MOC_DIR=./moc/


#   This variable specifies the #include directories which should be searched when compiling the project
INCLUDEPATH += $$(HOME)/NGL/include/ ./include/ ./pluginxml/

#   This variable contains the list of all directories to look in to resolve dependencies. This will be used when crawling through included files.
DEPENDPATH+=$$(HOME)/NGL/include/


#   Add the source files to the project file.
SOURCES += \
    src/MainWindow.cpp \
    src/main.cpp \
    src/GLWindow.cpp \
    pluginxml/pugixml.cpp \
    src/Particle.cpp \
    src/FluidParticle.cpp \
    src/SimulationParameters.cpp \
    src/Render.cpp \
    src/EnvironmentAndRigidBody.cpp \
    src/Integrate.cpp \
    src/SPHSolver.cpp \
    src/SpatialHashing.cpp \
    src/Export.cpp



#   Add the header files to the project file.
HEADERS  += \
    include/MainWindow.h \
    include/GLWindow.h \
    pluginxml/pugixml.hpp \
    pluginxml/pugiconfig.hpp \
    include/Particle.h \
    include/FluidParticle.h \
    include/SimulationParameters.h \
    include/Render.h \
    include/EnvironmentAndRigidBody.h \
    include/Integrate.h \
    include/SPHSolver.h \
    include/SpatialHashing.h \
    include/Export.h


#   Adding the other files which are not cpp or h file.
OTHER_FILES +=   \
    SolverParameters/Settings.xml \
    shaders/Basic.vs \
    shaders/Basic.fs \
    shaders/Colour.vs \
    shaders/Colour.fs


UI_HEADERS_DIR=ui
FORMS += ui/MainWindow.ui



#   qmake adds the values of this variable as compiler C preprocessor macros (-D option).
DEFINES+=USING_QT_CREATOR

#   This variable contains the C++ compiler flags that are used when building a project.
#   The value of this variable is typically handled by qmake or qmake.conf and rarely needs to be modified.
#   The flags specific to debug and release modes can be adjusted by modifying the QMAKE_CXXFLAGS_DEBUG and QMAKE_CXXFLAGS_RELEASE variables, respectively.

QMAKE_CXXFLAGS+= -msse -msse2 -msse3
QMAKE_CXXFLAGS +=  -march=native

#   Include OpenMP library files
QMAKE_CXXFLAGS += -fopenmp
QMAKE_LFLAGS *= -fopenmp

#   This variable contains a list of libraries to be linked into the project.
#   You can use the Unix -l (library) and -L (library path) flags and qmake will do the correct thing with these libraries on Windows (namely this means passing the full path of the library to the linker).
#   The only limitation to this is the library must exist, for qmake to find which directory a -l lib lives in.
LIBS +=  -L /$(HOME)/NGL/lib -l NGL


linux-g++-64 {
    DEFINES += LINUX
    LIBS+= -lGLEW
}

win32: {
        DEFINES+=USING_GLEW
        INCLUDEPATH+=-I c:/boost_1_44_0
        INCLUDEPATH+= -I C:/NGL/Support/glew
        LIBS+= -L C:/NGL/lib -l NGL

        LIBS+= -lmingw32
}

































