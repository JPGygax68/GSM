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

using namespace gsm;

class MyWindow: public IDisplay {
public:

    MyWindow(int x, int y, int w, int h, const char *caption = NULL) {
        closed = false;
        surf = sessionManager()->openWindow(x, y, w, h, caption, this, ISurface::SUPPORTS_OPENGL);
        surf->show();
    }

    virtual void
    onInit() {
        OGL(glClearColor, (0.2f, 0.5f, 1, 1));
        OGL(glEnable, (GL_TEXTURE_2D));
        OGL(glTexEnvi, (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE));
        OGL(glEnable, (GL_BLEND));
        OGL(glBlendFunc, (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA) );
        OGL(glEnable, (GL_CULL_FACE));
        headerFont = fontProvider()->getFont(IFont::ANY, "Arial", 48);
        textFont = fontProvider()->getFont(IFont::ANY, "Arial", 16);
    }

    virtual void
    onResize(int w, int h) {
        if (closed) return;
        OGL(glViewport, (0, 0, w, h));
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        OGL(glOrtho, (0, w, h, 0, -1, 1));
        // Calculate The Aspect Ratio Of The Window
        //gluPerspective(60.0f, (GLfloat)w/(GLfloat)h, 0.1f, 100.0f);
        glMatrixMode(GL_MODELVIEW);
    }

    virtual bool
    onPaint(ICanvas *cnv, int vidCtxID) {
        assert(!closed);
        //doPaint();
        return true;
    }

    virtual bool
    onKeyboardEvent(IKeyboardEvent *evt) {
        return false; }

    virtual bool
    onPointerMotionEvent(IPointerMotionEvent *evt) {
        return false; }

    virtual bool
    onPointerButtonEvent(IPointerButtonEvent *evt) {
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

        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

        OGL(glMatrixMode, (GL_MODELVIEW));
        OGL(glLoadIdentity, ());

        glTranslatef(100, 100, 0);

        glDisable(GL_TEXTURE_2D);
        glColor3f(1.0f,1.0f,1.0f);
        glBegin(GL_QUADS);
            glVertex3f(   0,   0,   0);
            glVertex3f(   0, 100,   0);
            glVertex3f( 200, 100,   0);
            glVertex3f( 200,   0,   0);
        OGL(glEnd, ());
        glEnable(GL_TEXTURE_2D);

        ogl::fonthandle_t hdrfh = ogl::prepareFont(headerFont, vidCtxID);
        ogl::fonthandle_t txtfh = ogl::prepareFont(textFont, vidCtxID);

        int x = 0, y = 0;
        glLoadIdentity();
        glTranslatef(100, 250, 0); x += 100, y += 250;
        ogl::renderText(hdrfh, L"Hello World!");
        glTranslatef(0, (GLfloat) ogl::calcLeading(hdrfh, txtfh), 0);
        const unicode_t * text = 
            L"Que j'aime à faire apprendre un nombre utile aux sages\n"
            L"Immortel, Archimède";
        ogl::renderTextAligned(txtfh, text, 0, IFont::LEFT    , 0, 400, y);
        ogl::renderTextAligned(txtfh, text, 0, IFont::CENTERED, 2, 400, y);
        ogl::renderTextAligned(txtfh, text, 0, IFont::RIGHT   , 4, 400, y);

        BoundingBox bbox;
        ogl::measureText(txtfh, text, 0, 4, bbox);
        unsigned w = (unsigned) (bbox.xMax - bbox.xMin + 2);
        unsigned h = (unsigned) (bbox.yMax - bbox.yMin);
        glDisable(GL_TEXTURE_2D);
        glColor3f(1.0f, 0.4f, 0.3f);
        ogl::texturedRectangle(w, h, 0, 0, w, h, 0, 0);
        OGL(glEnable, (GL_TEXTURE_2D));
        glTranslatef(0, (GLfloat)-bbox.yMin, 0); y += - bbox.yMin;
        glColor3f(1.0f, 1.0f, 1.0f);
        ogl::renderTextAligned(txtfh, text, 0, IFont::CENTERED, 2, w, y);

        //showFirstFontTexture(hdrfh);
    }

    void
    showFirstFontTexture(ogl::fonthandle_t fh)
    {
        OGL(glDisable, (GL_MULTISAMPLE));
        //glTranslatef((GLfloat)x, (GLfloat)y, 0);
        GLuint tex;
        unsigned w, h;
        ogl::dbg_getFontTexture(fh, 0, tex, w, h);
        glDisable(GL_TEXTURE_2D);
        glColor3f(1.0f,0.4f,0.3f);
        ogl::texturedRectangle(w, h, 0, 0, w, h, 0, 0);
        OGL(glEnable, (GL_TEXTURE_2D));
        OGL(glColor3f, (1.0f,1.0f,1.0f));
        OGL(glBindTexture, (GL_TEXTURE_2D, tex) );
        ogl::texturedRectangle(w, h, 0, 0, w, h, 0, 0);
    }

    ISurface *surf;
    bool closed;
    IFont *headerFont;
    IFont *textFont;
};

int
main(int argc, char *argv[])
{
    try {
        init();
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
