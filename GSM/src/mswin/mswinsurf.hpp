/*-----------------------------------------------------------------------------
 * This file is part of the Graphical Session Manager (GSM) project.
 * Copyright 2012 by Gygax Practical Computing, Biel/Bienne, Switzerland.
 *
 * This software is released under the Gnu General Public License.
 * A copy of that license should have been delivered with this source code.
 * If not, you can download it from http://www.gnu.org/licenses/gpl.txt.
 *-----------------------------------------------------------------------------*/

#ifndef __GSM_MSWINSURF_HPP
#define __GSM_MSWINSURF_HPP

#define WINDOWS_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>

#include <vector>
#include "../../isurface.hpp"


namespace gsm {

    class IDisplay;

    class MSWinSurface: public ISurface {
    public:
        virtual ~MSWinSurface();

    public: // ISurface
        virtual void show();

        virtual int clientWidth();
        virtual int clientHeight();

        virtual int
        select(int monitor = 0, Rect *rect = NULL);

        virtual void setCaption(const char *cap);

        virtual void present(int monitor = 0);

        /** Returns the main Video Context ID.
            Note that a Surface can spread across several monitors, which may not be
            connected to the same graphics adapter, in which case several Video
            Contexts are required to fully render the Surface.
         */
        virtual int videoContextID() { return vidctx_id; }

		virtual void * createExtraContext();

		virtual void selectExtraContext(void *ctx);

		virtual void deleteExtraContext(void *ctx);

    public:
        HWND windowHandle() const { return hWnd; }
        void setWindowHandle(HWND hWnd_) { hWnd = hWnd_; }

        IDisplay *display() const { 
            return disp; }

        HGLRC openGLContext() const { 
            return hGLRC; }

        void setOpenGLContext(HGLRC hRC, bool is_external = false) { 
            hGLRC = hRC; 
            externalRC = is_external;
        }

        void setVideoContextID(int id) { 
            vidctx_id = id; }

    protected:
        MSWinSurface(IDisplay *disp);
    
    private:
        void bindGraphicsResources();

        IDisplay                            *disp;
        HWND                                hWnd;
        HGLRC                               hGLRC;
        bool                                externalRC;
        int                                 vidctx_id;
        std::vector<HGLRC>                  extra_contexts;
        friend class MSWinSessionManager;
    };

} // ns gsm

#endif // __GSM_MSWINSURF_HPP
