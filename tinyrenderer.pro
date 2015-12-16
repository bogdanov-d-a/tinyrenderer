CONFIG += c++11

LIBS += -lSDL2

HEADERS += \
    geometry.h \
    model.h \
    our_gl.h \
    tgaimage.h \
    sdlwindow.h \
    shader.h \
    frametile.h \
    myqueue.h \
    myworker.h \
    mymanager.h

SOURCES += \
    geometry.cpp \
    main.cpp \
    model.cpp \
    our_gl.cpp \
    tgaimage.cpp \
    sdlwindow.cpp \
    shader.cpp \
    frametile.cpp \
    myworker.cpp \
    mymanager.cpp
