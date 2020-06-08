#include <QApplication>
#include <QWebEngineView>
#include <QTimer>
#include <openvr.h>
#include <QOpenGLTexture>
#include <QOpenGLFunctions>

using namespace vr;

const int width = 550;
const int height = 300;

void check_error(int line, EVRInitError error) {if (error != 0) printf("%d: error %s\n", line, VR_GetVRInitErrorAsSymbol(error)); }

QWebEngineView *pView;
VROverlayHandle_t *pHandle;

int main(int argc, char** argv) {
    (void)argc; (void)argv;
    EVRInitError error;
    VR_Init(&error, vr::VRApplication_Overlay);
    check_error(__LINE__, error);

    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QApplication app(argc, argv);

    QWebEngineView view;

    // Transparency
    // https://stackoverflow.com/questions/28183738/transparent-background-in-qwebenginepage
    view.setAttribute(Qt::WA_TranslucentBackground);
    view.setStyleSheet("background:transparent");
    view.page()->setBackgroundColor(Qt::transparent);

    pView = &view;

    view.setUrl(QUrl("http://localhost:8080/ui/#/chatoverlays/twitch"));
    view.resize(width, height);
    view.show();

    // Overlay
    VROverlayHandle_t handle;
    pHandle = &handle;

    VROverlay()->CreateOverlay("image", "image", &handle); /* key has to be unique, name doesn't matter */
    VROverlay()->SetOverlayWidthInMeters(handle, 3);
    VROverlay()->ShowOverlay(handle);

    vr::HmdMatrix34_t transform = {
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 1.0f,
        0.0f, 0.0f, 1.0f, -2.0f
    };
    VROverlay()->SetOverlayTransformAbsolute(handle, TrackingUniverseStanding, &transform);

    QTimer timer;
    timer.setInterval(1000);
    timer.callOnTimeout([] {
        qDebug("Overlay");

        //glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_BLEND);

        // https://stackoverflow.com/questions/55231170/taking-a-screenshot-of-a-web-page-in-pyqt5
        pView->grab().save("test.png", "PNG");
        QImage image = pView->grab().toImage();
        QOpenGLTexture texture(image.mirrored());
        qDebug() << texture.format();
        Texture_t myTexture = {(void*)(uintptr_t)texture.textureId(), TextureType_OpenGL, ColorSpace_Auto};

        VROverlay()->SetOverlayTexture(*pHandle, &myTexture);
        // VROverlay()->SetOverlayFromFile(*pHandle, "C:\\Users\\Yvon\\Documents\\build-twitch-chat-Desktop_Qt_5_15_0_MSVC2019_64bit-Debug\\test.png");
    });
    timer.start();

    return app.exec();
}
