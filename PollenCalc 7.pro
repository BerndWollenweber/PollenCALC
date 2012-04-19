TEMPLATE = app
TARGET = PollenCalc
QT += core
HEADERS += editor.h \
    mainwindow.h \
    PM/show.h \
    PM/alnfile.h \
    inputform.h
SOURCES += editor.cpp \
    mainwindow.cpp \
    main.cpp \
    PM/show.cpp \
    PM/alnfile.cpp \
    inputform.cpp
FORMS += \ 
    inputform.ui
RESOURCES += mdieditor.qrc


##static { # everything below takes effect with CONFIG += static
#    CONFIG += static
##    CONFIG += staticlib # this is needed if you create a static library, not a static executable
#    DEFINES += STATIC
#    message("~~~ static build ~~~") # this is for information, that the static build is done
##    mac: TARGET = $$join(TARGET,,,_static) #this adds an _static in the end, so you can seperate static build from non static build
#    win32: TARGET = $$join(TARGET,,,s) #this adds an s in the end, so you can seperate static build from non static build
##}
