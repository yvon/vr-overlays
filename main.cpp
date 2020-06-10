#include <QApplication>
#include <openvr.h>

#include "overlay.h"

void check_error(int line, vr::EVRInitError error) {
    if (error != 0) printf("%d: error %s\n", line, vr::VR_GetVRInitErrorAsSymbol(error));
}

int main(int argc, char** argv) {
    vr::EVRInitError error;
    vr::VR_Init(&error, vr::VRApplication_Overlay);
    check_error(__LINE__, error);

    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication app(argc, argv);

    Overlay twitchChat;

    return app.exec();
}
