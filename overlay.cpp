#include <QTimer>
#include <QOpenGLTexture>

#include "overlay.h"

Overlay::Overlay()
{
    view = new QWebEngineView();
    handle = new vr::VROverlayHandle_t();

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
    vr::VROverlay()->CreateOverlay("image", "image", handle); /* key has to be unique, name doesn't matter */
    vr::VROverlay()->SetOverlayWidthInMeters(*handle, 0.2);
    vr::VROverlay()->ShowOverlay(*handle);

    vr::HmdMatrix34_t transform = {
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, -0.25f, // Height
        0.0f, -0.4f, 1.0f, -0.3f // Depth
    };
    vr::VROverlay()->SetOverlayTransformAbsolute(*handle, vr::TrackingUniverseSeated, &transform);
    qDebug("hello");

    timer = new QTimer(this);
    timer->setInterval(1000 / 30); // Apprx. 30FPS
    connect(timer, SIGNAL(timeout()), this, SLOT(RefreshSlot()));

    timer->start();
}

void Overlay::RefreshSlot() {
    qDebug("hello");
    QImage image = view->grab().toImage();
    QOpenGLTexture texture(image.mirrored());
    vr::Texture_t myTexture = {(void*)(uintptr_t)texture.textureId(), vr::TextureType_OpenGL, vr::ColorSpace_Auto};

    vr::VROverlay()->SetOverlayTexture(*handle, &myTexture);
}
