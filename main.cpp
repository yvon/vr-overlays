#include <QApplication>
#include <QWebEngineView>
#include <QTimer>
#include <openvr.h>
#include <QOpenGLTexture>

using namespace vr;

const int width = 550;
const int height = 300;

void check_error(int line, EVRInitError error) {if (error != 0) printf("%d: error %s\n", line, VR_GetVRInitErrorAsSymbol(error)); }

QWebEngineView *view;
VROverlayHandle_t *handle;

int main(int argc, char** argv) {
    (void)argc; (void)argv;
    EVRInitError error;
    VR_Init(&error, vr::VRApplication_Overlay);
    check_error(__LINE__, error);

    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QApplication app(argc, argv);

    view = new QWebEngineView();

    // Transparency
    // https://stackoverflow.com/questions/28183738/transparent-background-in-qwebenginepage
    view->setAttribute(Qt::WA_TranslucentBackground);
    view->setStyleSheet("background:transparent");
    view->page()->setBackgroundColor(Qt::transparent);

    // Load page
    view->setUrl(QUrl("http://localhost:8080/ui/#/chatoverlays/twitch"));
    view->resize(width, height);
    view->show();

    // Create Overlay
    handle = new VROverlayHandle_t();

    VROverlay()->CreateOverlay("image", "image", handle); /* key has to be unique, name doesn't matter */
    VROverlay()->SetOverlayWidthInMeters(*handle, 0.2);
    VROverlay()->ShowOverlay(*handle);

    vr::HmdMatrix34_t transform = {
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, -0.25f, // Height
        0.0f, -0.4f, 1.0f, -0.3f // Depth
    };
    VROverlay()->SetOverlayTransformAbsolute(*handle, TrackingUniverseSeated, &transform);

    QTimer timer;
    timer.setInterval(1000 / 30); // Apprx. 30FPS
    timer.callOnTimeout([] {
        QImage image = view->grab().toImage();
        QOpenGLTexture texture(image.mirrored());
        Texture_t myTexture = {(void*)(uintptr_t)texture.textureId(), TextureType_OpenGL, ColorSpace_Auto};

        VROverlay()->SetOverlayTexture(*handle, &myTexture);
    });
    timer.start();

    return app.exec();
}
