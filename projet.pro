#Specify this is an app, and not a lib or what
TEMPLATE = app

#Set the name of the app
Debug:TARGET        = opengl_tp_6_1_debug
Release:TARGET      = opengl_tp_6_1

QT = opengl

DESTDIR = $$PWD/bin

app_data.path    = $${DESTDIR}
app_data.files   = $$PWD/data

INSTALLS += app_data

win32:DEPLOY_COMMAND    = windeployqt
macx:DEPLOY_COMMAND     = macdeployqt


Debug:BUILDDIR      = debug
Release:BUILDDIR    = release
#OBJECTS_DIR         = $${BUILDDIR}/.obj
#MOC_DIR             = $${BUILDDIR}/.moc
#RCC_DIR             = $${BUILDDIR}/.rcc
#UI_DIR              = $${BUILDDIR}/.ui


INCLUDEPATH +=  $$PWD/src
INCLUDEPATH += include/

VPATH   =   $$PWD/src
HEADERS +=   src/emetter.h \
    src/particle.h \
    src/particleconfiguration.h \
    src/mainwindow.h \
    src/tpglwindow.h \
    src/particlesrenderer.h \
    src/gpuprogram.h \
    src/utils.h \
    src/bigquad.h

SOURCES +=   src/emetter.cpp \
    src/particle.cpp \
    src/particleconfiguration.cpp \
    src/mainwindow.cpp \
    src/tpglwindow.cpp \
    src/main.cpp \
    src/particlesrenderer.cpp \
    src/gpuprogram.cpp \
    src/utils.cpp \
    src/bigquad.cpp

DISTFILES += \
    data/FS.glsl \
    data/VS.glsl

win32:LIBS      += -lopengl32

#macx:LIBS       += -L$${PWD}/../deps/glew/lib/
#macx:LIBS       += -lGLEW -lGLU
	
#unix!macx:LIBS  += -L$${PWD}/../deps/glew/lib/
#unix!macx:LIBS  += -lGLU



#message(CONFIG: $${CONFIG})
message(Qt version: $$[QT_VERSION])
message(Qt is installed in $$[QT_INSTALL_PREFIX])

FORMS += \
    src/mainwindow.ui
