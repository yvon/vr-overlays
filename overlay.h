#ifndef OVERLAY_H
#define OVERLAY_H

#include <QWebEngineView>
#include <QObject>

#include <openvr.h>

class Overlay : public QObject
{
    Q_OBJECT

    const int width = 550;
    const int height = 300;

    QWebEngineView *view;
    vr::VROverlayHandle_t *handle;

public:
    Overlay();
    QTimer *timer;
public slots:
    void RefreshSlot();
};

#endif // OVERLAY_H
