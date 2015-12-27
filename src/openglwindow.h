#ifndef __OPENGLWINDOW_H__
#define __OPENGLWINDOW_H__

#include <QtGui/QWindow>

QT_BEGIN_NAMESPACE
class QPainter;
class QOpenGLContext;
class QOpenGLPaintDevice;
QT_END_NAMESPACE


class OpenGLWindow : public QWindow, protected QOpenGLFunctions_3_3_Core
{
    Q_OBJECT
public:
    explicit OpenGLWindow(QWindow *parent = 0);
    ~OpenGLWindow();

    virtual void render(QPainter *painter);
    virtual void render();
    virtual void update() = 0;

    virtual void initialize();

    void setAnimating(bool animating);

public slots:
    void renderLater();
    void renderNow();

protected:
    bool event(QEvent *event) Q_DECL_OVERRIDE;

    void exposeEvent(QExposeEvent *event) Q_DECL_OVERRIDE;

private:
    bool                    m_update_pending;
    bool                    m_animating;

    QOpenGLContext*         m_context;
    QOpenGLPaintDevice*     m_device;
};

#endif //__OPENGLWINDOW_H__
