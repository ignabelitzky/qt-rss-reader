QT += widgets network sql

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    src/core/repositories/feedrepository.cpp \
    src/core/rssfetcher.cpp \
    src/core/rssitem.cpp \
    src/core/rssparser.cpp \
    src/db/databasemanager.cpp \
    src/main.cpp \
    src/models/feedtablemodel.cpp \
    src/models/rssmodel.cpp \
    src/ui/dialogs/addfeeddialog.cpp \
    src/ui/mainwindow.cpp

HEADERS += \
    src/core/repositories/feedrepository.h \
    src/core/rssfetcher.h \
    src/core/rssitem.h \
    src/core/rssparser.h \
    src/db/databasemanager.h \
    src/models/feed.h \
    src/models/feedtablemodel.h \
    src/models/rssmodel.h \
    src/ui/dialogs/addfeeddialog.h \
    src/ui/mainwindow.h

FORMS += \
    src/ui/dialogs/addfeeddialog.ui \
    src/ui/mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources/resources.qrc
