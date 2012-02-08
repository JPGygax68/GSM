/*-----------------------------------------------------------------------------
 * This file is part of the Graphical Session Manager (GSM) project.
 * Copyright 2012 by Gygax Practical Computing, Biel/Bienne, Switzerland.
 *
 * This software is released under the Gnu General Public License.
 * A copy of that license should have been delivered with this source code.
 * If not, you can download it from http://www.gnu.org/licenses/gpl.txt.
 *-----------------------------------------------------------------------------*/

#ifndef __GSM_ISURFACE_HPP
#define __GSM_ISURFACE_HPP

#include "util/bitset.hpp"

namespace gsm {
    
class IEvent;
struct Rect;

class ISurface {
public:

    virtual ~ISurface() {}

    enum Attribute {
        SINGLE_BUFFERED,
        SUPPORTS_OPENGL
    };
    typedef bitset<Attribute> Attributes;

    virtual void
    show() = 0;

    /** Must return the width of the usable part of the surface (without the frame if there is one).
     */
    virtual int
    clientWidth() = 0;

    /** Must return the height of the usable part of the surface (without the frame if there is one).
     */
    virtual int
    clientHeight() = 0;

    /** Graphic APIs like OpenGL work within Rendering Contexts. Usually, there is (at least) one
        such context per Surface, which the call select() is responsible for activating. 
        However, graphical resources can sometimes be shared between rendering contexts. In order
        for user code to know when it needs to re-generate a given graphical resource, it must be
        able to identify the currently active "video context" (of which there should be exactly
        one per installed graphics adapter).
        This call returns an opaque ID fulfilling that purpose.
        By definition, the ID must be != 0 to be valid.
     */
    virtual int 
    videoContextID() = 0;

    /** Prepares the surface for use by the graphics API (typically OpenGL).
        The optional monitor parameter makes it possible to completely render a Surface that
        overlaps multiple monitors. To do so, call select() repeatedly, incrementing the value
        of the monitor parameter after each call until the return value is 0.
        The return value of the call identifies the Video Context; 0 means that the specified
        index value was too high.
        The optional rect parameter informs the caller about the rectangle that actually lies
        on the monitor that was selected for rendering.
        In other words, to completely render a Surface, construct a loop like:
            int vidCtxId;
            gsm::Rect rect;
            for (int imon = 0; ((vidCtxId = surf->select(imon, &rect)) != 0; imon ++) {
                // ... render ...
                surf->present(imon);
            }
        OpenGL note: Each successful call to select() may activate a different OpenGL Rendering
            Context. This means that each rendering "pass" initiated by select() must re-
            establish the required OpenGL states.
     */
    virtual int
    select(int monitor = 0, Rect *rect = NULL) = 0;

    virtual void
    setCaption(const char *cap) = 0;

    /** Signals that painting on the surface is done and that it can be presented on the specified
        monitor (see select() above).
        TODO: additional parameter signalling "not ready to render yet, come back later"!
     */
    virtual void
    present(int monitor = 0) = 0;

	/** Sometimes, it is necessary to render to a surface from more than one thread. In such cases,
		OpenGL, and probably other APIs too, require a separate context. This call creates such
		an "extra" context.
	 */
	virtual void *
	createExtraContext() = 0;

	/** Activates an "extra" rendering context created by createExtraContext().
	 */
	virtual void
	selectExtraContext(void *ctx) = 0;

	/** Deletes an "extra" rendering context created by createExtraContext().
	 */
	virtual void
	deleteExtraContext(void *ctx) = 0;
};

} // ns gsm

#endif // __GSM_ISURFACE_HPP
