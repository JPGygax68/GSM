#include <exception>
#include <iostream>
#include <cassert>
#include <GL/glew.h>
#include <GL/GLU.h>
#include <GSM/util/oglhelper.hpp>
#include <GSM/gsm.hpp>
#include <GSM/isurface.hpp>
#include <GSM/idisplay.hpp>

class MyWindow: public gsm::IDisplay {
public:

    MyWindow(gsm::ISessionManager *sm, int x, int y, int w, int h, const char *caption = NULL) {
        using namespace gsm;
        closed = false;
        surf = sm->openWindow(x, y, w, h, caption, this, ISurface::SUPPORTS_OPENGL);
        surf->show();
    }

    virtual void
    onInit() {
        glClearColor(0.2f, 0.5f, 1, 1);
    }

    virtual void
    onResize(int w, int h) {
        OGL(glViewport, (0, 0, w, h));
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        OGL(glOrtho, (0, w, h, 0, -1, 1));
        // Calculate The Aspect Ratio Of The Window
        //gluPerspective(60.0f, (GLfloat)w/(GLfloat)h, 0.1f, 100.0f);
        glMatrixMode(GL_MODELVIEW);
    }

    virtual bool
    onPaint(gsm::ICanvas *cnv) {
        assert(!closed);
        //doPaint();
        return true;
    }

    virtual bool
    onKeyboardEvent(gsm::IKeyboardEvent *evt) {
        return false; }

    virtual bool
    onPointerMotionEvent(gsm::IPointerMotionEvent *evt) {
        return false; }

    virtual bool
    onPointerButtonEvent(gsm::IPointerButtonEvent *evt) {
        return false; }

    virtual bool
    onClose() {
        closed = true;
        return true; }

    void
    paint() {
        if (!closed) {
            surf->select();
            doPaint();
            surf->present();
        }
    }

private:

    void
    doPaint() {
        OGL(glClear, (GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT));

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        glTranslatef(100, 100, 0);

        glColor3f(1.0f,1.0f,1.0f);
        glBegin(GL_QUADS);
            glVertex3f(   0,   0,   0);
            glVertex3f( 200,   0,   0);
            glVertex3f( 200, 100,   0);
            glVertex3f(   0, 100,   0);
        OGL(glEnd, ());
    }

    gsm::ISurface *surf;
    bool closed;
};

int
main(int argc, char *argv[])
{
    using namespace gsm;

    try {
        gsm::init();
        ISessionManager *sm = static_cast<ISessionManager*>( findComponent("SessionManager") );

        MyWindow win1(sm, 10, 10, 800, 600);
        MyWindow win2(sm, 30, 200, 800, 600, "Window 2");

        while (true) {
            win1.paint();
            win2.paint();
            sm->fetchNextEvent();
            (void) sm->getEvent();
            if (sm->mustQuit() )
                break;
        }

        return 0;
    }
    catch(std::exception &e) {
        std::cerr << e.what() << std::endl;
    }

    return 1; // error
}
