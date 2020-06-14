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

    new Overlay("twitch", "http://ir-apps.kutu.ru/twitch-chat/chat.html?channel_id=53731598&bgOpacity=0&textShadow=1&reversedMessages=1&hideMessageAfter=30", 550, 300,
    {{
         {1.0f, 0.0f, 0.0f, 0.02f},
         {0.0f, 1.0f, 0.0f, -0.1f},
         {0.0f, 0.0f, 1.0f, -0.4f}
     }});

    new Overlay("racing", "http://localhost:8185/racing-overlay/?profileName=VR&profileId=181301e0-ae35-11ea-b3df-c9508a03a60b", 1920, 1080,
    {{
         {5.0f, 0.0f, 0.0f, 0.0f},
         {0.0f, 5.0f, 0.0f, -0.1f},
         {0.0f, 0.0f, 5.0f, -0.4f}
     }});

    QTimer timer;
    timer.setInterval(100);
    timer.callOnTimeout(Overlay::refreshAll);
    timer.start();

    return app.exec();
}
