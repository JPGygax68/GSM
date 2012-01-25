#include <exception>
#include <iostream>
#include <cassert>
#include <GL/glew.h>
#include <GL/GLU.h>
#include <GSM/util/oglhelper.hpp>
#include <GSM/gsm.hpp>
#include <GSM/ifontprov.hpp>
#include <GSM/isurface.hpp>
#include <GSM/idisplay.hpp>
#include <GSM/opengl/opengl.hpp>

class MyWindow: public gsm::IDisplay {
public:

    MyWindow(int x, int y, int w, int h, const char *caption = NULL) {
        using namespace gsm;
        closed = false;
        surf = sessionManager()->openWindow(x, y, w, h, caption, this, ISurface::SUPPORTS_OPENGL);
        surf->show();
    }

    virtual void
    onInit() {
        using namespace gsm;
        glClearColor(0.2f, 0.5f, 1, 1);
        myFont = fontProvider()->getFont(IFont::ANY, "Arial", 16);
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
    onPaint(gsm::ICanvas *cnv, int vidCtxID) {
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
            int vidCtxID = surf->select();
            doPaint(vidCtxID);
            surf->present();
        }
    }

private:

    void
    doPaint(int vidCtxID) {
        using namespace gsm;

        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

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

        glLoadIdentity();
        ogl::fonthandle_t fh = ogl::prepareFont(myFont, 1);
        int x = 100, y = 50;
        ogl::renderText(fh, x, y, L"Hello World!");
    }

    gsm::ISurface *surf;
    bool closed;
    gsm::IFont *myFont;
};

int
main(int argc, char *argv[])
{
    using namespace gsm;

    try {
        gsm::init();
        ISessionManager *sm = static_cast<ISessionManager*>( findComponent("SessionManager") );

        MyWindow win1(10, 10, 800, 600);
        MyWindow win2(30, 200, 800, 600, "Window 2");

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
