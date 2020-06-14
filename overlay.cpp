#include "overlay.h"

std::vector<class Overlay *> Overlay::instances;

Overlay::Overlay(const char *key, const char *url, int width, int height, vr::HmdMatrix34_t transform)
{
    view = new QWebEngineView();
    handle = new vr::VROverlayHandle_t();

    // Transparency
    // https://stackoverflow.com/questions/28183738/transparent-background-in-qwebenginepage
    view->setAttribute(Qt::WA_TranslucentBackground);
    view->setStyleSheet("background:transparent");
    view->page()->setBackgroundColor(Qt::transparent);

    // Load page
    view->setUrl(QUrl(url));
    view->resize(width, height);
    view->show();

    // Create Overlay
    vr::VROverlay()->CreateOverlay(key, key, handle); // key has to be unique, name doesn't matter
    vr::VROverlay()->SetOverlayWidthInMeters(*handle, 0.2f);
    vr::VROverlay()->ShowOverlay(*handle);
    vr::VROverlay()->SetOverlayTransformAbsolute(*handle, vr::TrackingUniverseSeated, &transform);

    instances.push_back(this);

    // Instanciate the texture. The constructor configure and allocate the texture for us.
    texture = new QOpenGLTexture(captureImage());

    // OpenVR texture description
    vrTexture = new vr::Texture_t({(void*)(uintptr_t)texture->textureId(),
                                   vr::TextureType_OpenGL,
                                   vr::ColorSpace_Auto});
}

void Overlay::refreshAll() {
    for(Overlay *overlay : instances) {
        overlay->refresh();
    }
}

void Overlay::refresh() {
    // Capture new page snapshot
    QImage image = captureImage().convertToFormat(QImage::Format_RGBA8888);

    // Update the texture data
    texture->setData(0, QOpenGLTexture::RGBA, QOpenGLTexture::UInt8, image.constBits());

    // Ask steam VR to redraw the overlay
    vr::VROverlay()->SetOverlayTexture(*handle, vrTexture);
}

QImage Overlay::captureImage() {
    return view->grab().toImage().mirrored();
}
