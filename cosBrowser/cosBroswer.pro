QT       += core gui sql concurrent

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
DEFINES += _WIN32_WINNT=0x0A00 # <timezoneapi.h> 就会暴露出 GetDynamicTimeZoneInformation 的声明。
QMAKE_PROJECT_DEPTH = 0
THIRD_DIR = $$PWD/../

win32:CONFIG(release, debug|release): LIBS += -L$$THIRD_DIR/third/cos/libs/x64/ -lcossdk
else:win32:CONFIG(debug, debug|release): LIBS += -L$$THIRD_DIR/third/cos/libs/x64/ -lcossdkd
else:unix: LIBS += -L$$THIRD_DIR/third/cos/libs/x64/ -lcossdk


win32:CONFIG(release, debug|release): LIBS += -L$$THIRD_DIR/third/cos/third_party/lib/x64/poco/ -lPocoFoundation
else:win32:CONFIG(debug, debug|release): LIBS += -L$$THIRD_DIR/third/cos/third_party/lib/x64/poco/ -lPocoFoundation
else:unix: LIBS += -L$$THIRD_DIR/third/cos/third_party/lib/x64/poco/ -lPocoFoundation


INCLUDEPATH += $$THIRD_DIR/third/cos/include \
    $$THIRD_DIR/third/cos/third_party/include
DEPENDPATH += $$THIRD_DIR/third/cos/include \
    $$THIRD_DIR/third/cos/third_party/include

INCLUDEPATH += $$THIRD_DIR/third/spdlog/include


SOURCES += \
    main.cpp \
    src/bend/dao/cloud/daocloudscos.cpp \
    src/bend/dao/cloud/daocloudsmock.cpp \
    src/bend/dao/configs/version.cpp \
    src/bend/dao/configs/versioncmd.cpp \
    src/bend/dao/configs/versionjson.cpp \
    src/bend/dao/db/daologininfosqlite.cpp \
    src/bend/gateway.cpp \
    src/bend/manager/managercloud.cpp \
    src/bend/manager/managerdatabase.cpp \
    src/config/baseexception.cpp \
    src/fend/uicommond/basedialog.cpp \
    src/fend/uicommond/breadcrumdwidget.cpp \
    src/fend/uicommond/comboline.cpp \
    src/fend/uicommond/mylineedit.cpp \
    src/fend/uicommond/pageflipwidget.cpp \
    src/fend/uicommond/uimessagebox.cpp \
    src/fend/uicommond/uiprogressbarwidget.cpp \
    src/fend/uicommond/uipushbutton.cpp \
    src/fend/uicommond/uiqoswidget.cpp \
    src/fend/uidelegates/bucketdelegate.cpp \
    src/fend/uidelegates/tableitemdelegate.cpp \
    src/fend/uilogin/logindialog.cpp \
    src/fend/uimain/bucketsListwidget.cpp \
    src/fend/uimain/bucketstablewidget.cpp \
    src/fend/uimain/createbucketdialog.cpp \
    src/fend/uimain/objectswidget.cpp \
    src/fend/uimain/toolbarwidget.cpp \
    src/fend/uimain/uimain.cpp \
    src/fend/uitransfer/uidownload.cpp \
    src/fend/uitransfer/uitransfer.cpp \
    src/fend/uitransfer/uiupload.cpp \
    src/helper/byteshelper.cpp \
    src/helper/dbsqlite.cpp \
    src/helper/filehelper.cpp \
    src/middle/managerglobal.cpp \
    src/middle/managermodels.cpp \
    src/middle/models/mybucket.cpp \
    src/middle/signals/managersignals.cpp \
    src/plugins/managerplugin.cpp

HEADERS += \
    src/bend/dao/cloud/daoclouds.h \
    src/bend/dao/cloud/daocloudscos.h \
    src/bend/dao/cloud/daocloudsmock.h \
    src/bend/dao/configs/version.h \
    src/bend/dao/configs/versioncmd.h \
    src/bend/dao/configs/versionjson.h \
    src/bend/dao/db/daologininfo.h \
    src/bend/dao/db/daologininfosqlite.h \
    src/bend/gateway.h \
    src/bend/manager/managercloud.h \
    src/bend/manager/managerdatabase.h \
    src/config/apis.h \
    src/config/baseexception.h \
    src/config/errorcode.h \
    src/config/global.h \
    src/fend/uicommond/basedialog.h \
    src/fend/uicommond/comboline.h \
    src/fend/uicommond/uimessagebox.h \
    src/fend/uicommond/uiprogressbarwidget.h \
    src/fend/uicommond/uipushbutton.h \
    src/fend/uicommond/uiqoswidget.h \
    src/fend/uidelegates/tableitemdelegate.h \
    src/fend/uimain/bucketsListwidget.h \
    src/fend/uimain/bucketstablewidget.h \
    src/fend/uimain/createbucketdialog.h \
    src/fend/uitransfer/uidownload.h \
    src/fend/uitransfer/uitransfer.h \
    src/fend/uitransfer/uiupload.h \
    src/helper/byteshelper.h \
    src/middle/managerglobal.h \
    src/middle/managermodels.h \
    src/middle/models/dbmodels.h \
    src/fend/uicommond/breadcrumdwidget.h \
    src/fend/uicommond/mylineedit.h \
    src/fend/uicommond/pageflipwidget.h \
    src/fend/uidelegates/bucketdelegate.h \
    src/fend/uilogin/logindialog.h \
    src/fend/uimain/objectswidget.h \
    src/fend/uimain/toolbarwidget.h \
    src/fend/uimain/uimain.h \
    src/helper/dbsqlite.h \
    src/helper/filehelper.h \
    src/middle/models/dbmodels.h \
    src/middle/models/mybucket.h \
    src/middle/signals/managersignals.h \
    src/plugins/managerplugin.h

FORMS += \
    src/fend/uicommond/basedialog.ui \
    src/fend/uicommond/breadcrumdwidget.ui \
    src/fend/uicommond/pageflipwidget.ui \
    src/fend/uicommond/uimessagebox.ui \
    src/fend/uicommond/uiprogressbarwidget.ui \
    src/fend/uicommond/uiqoswidget.ui \
    src/fend/uilogin/logindialog.ui \
    src/fend/uimain/bucketsListwidget.ui \
    src/fend/uimain/bucketstablewidget.ui \
    src/fend/uimain/createbucketdialog.ui \
    src/fend/uimain/objectswidget.ui \
    src/fend/uimain/toolbarwidget.ui \
    src/fend/uimain/uimain.ui \
    src/fend/uitransfer/uidownload.ui \
    src/fend/uitransfer/uitransfer.ui \
    src/fend/uitransfer/uiupload.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resource.qrc

DISTFILES += \
    static/image/Delete-1.png \
    static/image/Delete-2.png \
    static/image/Maximize-1.png \
    static/image/Maximize-2.png \
    static/image/Maximize-3.png \
    static/image/Maximize-4.png \
    static/image/Minimize-1.png \
    static/image/Minimize-2.png \
    static/image/Quit-1.png \
    static/image/Quit-2.png \
    static/image/Search.png \
    static/image/Trans-1.png \
    static/image/Trans-2.png \
    static/image/bucket.png \
    static/image/close1.png \
    static/image/close2.png \
    static/image/dir.png \
    static/image/file.png \
    static/image/icontt.ico \
    static/image/icontt.png \
    static/image/right.png \
    static/image/upload.png \
    static/image/download.png \
    static/qss/default.qss \
    static/testing/buckets1.json \
    static/testing/buckets2.json \
    static/version/config_default.json
