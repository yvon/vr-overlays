#ifndef OVERLAY_H
#define OVERLAY_H

#include <vector>
#include <QWebEngineView>
#include <QOpenGLTexture>

#include <openvr.h>

class Overlay
{
    static std::vector<Overlay *> instances;

    // Web browser
    QWebEngineView *view;

    // OpenGL overlay texture
    QOpenGLTexture *texture = nullptr;

    // OpenVR objects
    vr::VROverlayHandle_t *handle;
    vr::Texture_t *vrTexture = nullptr;

    int width;
    int height;
public:
    const char *key;
    static void refreshAll();

    Overlay(const char *key, const char *url, int width, int height, vr::HmdMatrix34_t transform);
    void refresh();
};

#endif // OVERLAY_H
