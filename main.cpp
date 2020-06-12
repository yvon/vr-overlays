#include <QApplication>
#include <QTimer>
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

    new Overlay("twitch", "http://localhost:8080/ui/#/chatoverlays/twitch", 550, 300,
    {{
         {1.0f, 0.0f, 0.0f, 0.3f},
         {0.0f, 1.0f, 0.0f, -0.25f},
         {0.0f, 0.0f, 1.0f, -0.4f}
     }});

    new Overlay("relatives", "http://localhost:8080/ui/#/overlays/relative", 450, 214,
    {{
         {1.0f, 0.0f, 0.0f, -0.3f},
         {0.0f, 1.0f, 0.0f, -0.2f},
         {0.0f, 0.0f, 1.0f, -0.4f}
     }});

    new Overlay("standings", "http://localhost:8080/ui/#/overlays/standings", 450, 300,
    {{
         {1.0f, 0.0f, 0.0f, -0.3f},
         {0.0f, 1.0f, 0.0f, -0.35f},
         {0.0f, 0.0f, 1.0f, -0.4f}
     }});

    QTimer timer;
    timer.setInterval(100);
    timer.callOnTimeout(Overlay::refreshAll);
    timer.start();

    return app.exec();
}
