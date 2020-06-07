#include <QApplication>
#include <QWebEngineView>
#include <openvr.h>

using namespace vr;

const int width = 550;
const int height = 300;

void check_error(int line, EVRInitError error) {if (error != 0) printf("%d: error %s\n", line, VR_GetVRInitErrorAsSymbol(error)); }

int main(int argc, char** argv) {
    (void)argc; (void)argv;
    EVRInitError error;
    VR_Init(&error, vr::VRApplication_Overlay);
    check_error(__LINE__, error);

    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication app(argc, argv);

    QWebEngineView view;
    view.setUrl(QUrl("http://localhost:8080/ui/#/chatoverlays/twitch"));
    view.resize(width, height);
    view.show();

    view.page()->setBackgroundColor(Qt::darkGray);

    QImage image(width, height, QImage::Format_ARGB32);

    view.render(&image);

    VROverlayHandle_t handle;
    VROverlay()->CreateOverlay("image", "image", &handle); /* key has to be unique, name doesn't matter */
    VROverlay()->SetOverlayFromFile(handle, "C:\\Users\\Yvon\\Downloads\\sample.png");
    VROverlay()->SetOverlayWidthInMeters(handle, 3);
    VROverlay()->ShowOverlay(handle);

    vr::HmdMatrix34_t transform = {
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f, -2.0f
    };
    VROverlay()->SetOverlayTransformAbsolute(handle, TrackingUniverseStanding, &transform);


    return app.exec();
}
