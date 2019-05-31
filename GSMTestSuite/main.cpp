/*-----------------------------------------------------------------------------
 * This file is part of the Graphical Session Manager (GSM) project.
 * Copyright 2012 by Gygax Practical Computing, Biel/Bienne, Switzerland.
 *
 * This software is released under the Gnu General Public License.
 * A copy of that license should have been delivered with this source code.
 * If not, you can download it from http://www.gnu.org/licenses/gpl.txt.
 *-----------------------------------------------------------------------------*/

#ifdef WIN32
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>
#endif
#include <gl/gl.h>
#include <gl/glu.h>
#include <exception>
#include <iostream>
#include <cassert>
#include <set>
#include <GSM/util/text.hpp>
#include <GSM/gsm.hpp>
#include <GSM/ifontprov.hpp>
#include <GSM/isurface.hpp>
#include <GSM/idisplay.hpp>
#include <GSM/iptrmovevt.hpp>
#include <GSM/iptrbtnevt.hpp>
#include <GSM/ikeybevt.hpp>
#include <GSM/opengl/opengl.hpp>

// This allows us to include just gl/gl.h instead of having to resort to an extension loader
#define GL_MULTISAMPLE 0x809D

using namespace gsm;

class MyDisplay: public IDisplay {
public:

    MyDisplay(int x, int y, int w, int h, const char *caption = NULL) {
        closed = false;
        surf = sessionManager()->openWindow(x, y, w, h, caption, this, ISurface::SUPPORTS_OPENGL);
        surf->show();
        mouseX = mouseY = 0;
        typed_text = L"Type here:";
        shift = ctrl = alt = false;
    }

    MyDisplay(int screen_num) {
        closed = false;
        surf = sessionManager()->openScreen(screen_num, this, ISurface::SUPPORTS_OPENGL);
        surf->show();
        mouseX = mouseY = 0;
        typed_text = L"Type here:";
        shift = ctrl = alt = false;
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
        if (evt->isCharacter()) {
            if (isprint(evt->unicode())) {
                typed_text += evt->unicode();
            }
            // BACKSPACE
            else if (evt->unicode() == 8) {
                if (typed_text.size() > 0) typed_text.pop_back();
            }
        }
        else if (evt->down()) {
            down_keys.insert( evt->keyCode() );
        }
        else if (evt->up()) {
            down_keys.erase( evt->keyCode() );
        }
        return false; 
    }

    virtual bool
    onPointerMotionEvent(IPointerMotionEvent *evt) {
        mouseX = evt->position().x;
        mouseY = evt->position().y;
        return false; }

    virtual bool
    onPointerButtonEvent(IPointerButtonEvent *evt) {
        if (evt->doubleClick()) {
            clickX = evt->position().x;
            clickY = evt->position().y;
        }
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

        ogl::fonthandle_t hdrfh = ogl::prepareFont(headerFont, vidCtxID, & CharacterList::LATIN1() );
        ogl::fonthandle_t txtfh = ogl::prepareFont(textFont, vidCtxID, & CharacterList::LATIN1() );

        int x = 0, y = 0;

        // Last click position
        ogl::rectangle(2, 2, clickX-1, clickY-2);

        // Pointer position
        glTranslatef(50, 50, 0); x += 50; y += 50;
        glColor3f(1.0f,1.0f,1.0f);
		ogl::prepareForTextRendering();
        ogl::renderText(txtfh, format(L"%d, %d", mouseX, mouseY).c_str());
		ogl::doneWithTextRendering();
        glTranslatef(0, 20, 0); y += 20;

        ogl::rectangle(200, 100, 0);
        glTranslatef(0, 150, 0); y += 120;

        glColor3f(1.0f, 1.0f, 1.0f);
        ogl::renderText(hdrfh, L"Hello World!");
        glTranslatef(0, (GLfloat) ogl::calcLeading(hdrfh, txtfh), 0);
        const unicode_t * text = 
            L"Que j'aime à faire apprendre un nombre utile aux sages\n"
            L"Immortel, Archimède";
        ogl::renderTextAligned(txtfh, text, 0, IFont::LEFT    , 0, 400, y);
        ogl::renderTextAligned(txtfh, text, 0, IFont::CENTERED, 2, 400, y);
        ogl::renderTextAligned(txtfh, text, 0, IFont::RIGHT   , 4, 400, y);

        BoundingBox bbox = getTextBounds(textFont, text);
        bbox.inflate(6, 6);
        glColor3f(1.0f, 0.4f, 0.3f);
        ogl::rectangle(bbox.width(), bbox.height());
        glTranslatef(0, (GLfloat)-bbox.yMin, 0); y += - bbox.yMin;
        glColor3f(1.0f, 1.0f, 1.0f);
        ogl::renderTextAligned(txtfh, text, 0, IFont::CENTERED, 2, bbox.width(), y);
        glTranslatef(0, (GLfloat)(bbox.height() + 10), 0); y += bbox.height() + 10;

        // Keys
        std::wstring key_list;
        for (std::set<int>::const_iterator it = down_keys.begin(); it != down_keys.end(); it ++) {
            key_list += format(L"%d ", *it);
        }
        ogl::renderText(txtfh, key_list.c_str());
        glTranslatef(0, 20, 0); y += 20;

        // Metakeys
        ISessionManager::MetaKeys meta_keys = sessionManager()->getMetaKeyStates();
        std::wstring metakey_list;
        if ((meta_keys.test(ISessionManager::SHIFT)   ) != 0) metakey_list += L"SHIFT    ";
        if ((meta_keys.test(ISessionManager::ALT  )   ) != 0) metakey_list += L"ALT      ";
        if ((meta_keys.test(ISessionManager::CONTROL) ) != 0) metakey_list += L"CONTROL  ";
        if ((meta_keys.test(ISessionManager::CAPSLOCK)) != 0) metakey_list += L"CAPSLOCK ";
        if ((meta_keys.test(ISessionManager::NUMLOCK) ) != 0) metakey_list += L"NUMLOCK  ";
        ogl::renderText(txtfh, metakey_list.c_str());

        // Typed text
        glTranslatef(0, 20, 0);
        ogl::renderText(txtfh, typed_text.c_str());

        showFirstFontTexture(hdrfh);
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
    int mouseX, mouseY;
    int clickX, clickY;
    std::wstring typed_text;
    bool shift, ctrl, alt;
    std::set<int> down_keys;
};

int
main(int argc, char *argv[])
{
    try {
        init();
        ISessionManager *sm = sessionManager();

        MyDisplay win1(0);
        MyDisplay win2(30, 200, 800, 600, "Window 2");

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
