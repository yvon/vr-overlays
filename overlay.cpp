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
    vr::VROverlay()->CreateOverlay(key, key, handle); /* key has to be unique, name doesn't matter */
    vr::VROverlay()->SetOverlayWidthInMeters(*handle, 0.2f);
    vr::VROverlay()->ShowOverlay(*handle);
    vr::VROverlay()->SetOverlayTransformAbsolute(*handle, vr::TrackingUniverseSeated, &transform);

    instances.push_back(this);
}

void Overlay::refreshAll() {
    for(Overlay *overlay : instances) {
        overlay->refresh();
    }
}

void Overlay::refresh() {
    delete texture;

    QImage image = view->grab().toImage();
    texture = new QOpenGLTexture(image.mirrored());
    vr::Texture_t myTexture = {(void*)(uintptr_t)texture->textureId(), vr::TextureType_OpenGL, vr::ColorSpace_Auto};
    vr::VROverlay()->SetOverlayTexture(*handle, &myTexture);
}