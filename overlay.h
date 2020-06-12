#ifndef OVERLAY_H
#define OVERLAY_H

#include <vector>
#include <QWebEngineView>
#include <QOpenGLTexture>

#include <openvr.h>

class Overlay
{
    static std::vector<Overlay *> instances;

    QWebEngineView *view;
    vr::VROverlayHandle_t *handle;
    QOpenGLTexture *texture = nullptr;
public:
    static void refreshAll();

    Overlay(const char *key, const char *url, int width, int height, vr::HmdMatrix34_t transform);
    void refresh();
};

#endif // OVERLAY_H
