#ifndef OVERLAY_H
#define OVERLAY_H

#include <QObject>
#include <QTimer>
#include <QWebEngineView>
#include <QOpenGLTexture>

#include <openvr.h>

class Overlay : public QObject
{
    Q_OBJECT
    QWebEngineView *view;
    vr::VROverlayHandle_t *handle;
    QOpenGLTexture *texture = nullptr;
public:
    Overlay(const char *key, const char *url, int width, int height, vr::HmdMatrix34_t transform);
    QTimer *timer;
public slots:
    void RefreshSlot();
};

#endif // OVERLAY_H
