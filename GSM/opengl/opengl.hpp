/*-----------------------------------------------------------------------------
 * This file is part of the Graphical Session Manager (GSM) project.
 * Copyright 2012 by Gygax Practical Computing, Biel/Bienne, Switzerland.
 *
 * This software is released under the Gnu General Public License.
 * A copy of that license should have been delivered with this source code.
 * If not, you can download it from http://www.gnu.org/licenses/gpl.txt.
 *-----------------------------------------------------------------------------*/

#ifndef __GSM_OPENGL_OPENGL_HPP
#define __GSM_OPENGL_OPENGL_HPP

#include <string>
// #include <sstream>
#include <stdexcept>

#include "../dll.h"
#include "../types.hpp"
#include "../ifont.hpp"
#include "../util/errlog.hpp"

#ifdef GLBINDINGS_GLEW
#ifdef _WIN32
#define NOMINMAX
#include <Windows.h>
#endif
#endif
#include <gl/gl.h>


namespace gsm {

class ISurface;

namespace ogl {

// EXCEPTIONS AND ERROR HANDLING ---------------------------------------------

class Error : public std::runtime_error {
	static const std::string 
    get_error_string(GLenum err) {
		return std::to_string(err);
	}
public:
	Error(GLenum err, const char * context) :
		std::runtime_error(
			std::string("OpenGL error ") +
			get_error_string(err) + " while executing " + context )
	{
	}
};

#if _DEBUG && (!_NO_OPENGL_CHECKS)

	//#ifdef _OGLERROR_THROW
		#define OGLREPORT(err, ctx) throw gsm::ogl::Error(err, ctx)
	//#else
	//	#define OGLREPORT(err, ctx) gsm::ErrorLogger::instance()->log(err, ctx)
	//#endif

	#define OGLCHECK(ctx) { \
		GLenum err = glGetError(); \
		if (err != GL_NO_ERROR) OGLREPORT(err, ctx); \
	}

	#define	 OGL(func, args) { \
		func args; \
		OGLCHECK(#func "(" #args ")" ); \
	}

	#define	 OGLI(func, args) { \
		func args; \
		int err = glGetError(); \
	}

#else

	#define OGLCHECK(ctx)
	#define OGL(func, args) func args
	#define OGLI(func, args) func args

#endif

// SIMPLE TYPES ---------------------------------------------------------------

typedef GLfloat  Float3[3];
typedef GLfloat  Float4[4];
typedef GLdouble Double3[3];
typedef GLdouble Double4[4];

// COLOR TYPES ----------------------------------------------------------------

template <typename ValueType>
class Color4 {

public:
    static const Color4 
    from3(const ValueType *vec, ValueType opacity = (ValueType)1.0) {
        Color4<ValueType> clr;
        clr[0] = vec[0];
        clr[1] = vec[1];
        clr[2] = vec[2];
        clr[3] = (ValueType) opacity;
        return clr;
    }

    static const Color4
    from4(const ValueType *vec) {
        Color4<ValueType> clr;
        clr[0] = vec[0];
        clr[1] = vec[1];
        clr[2] = vec[2];
        clr[3] = vec[3];
        return clr;
    }

    Color4() { vals[0] = vals[1] = vals[2] = vals[3] = 0; }

    Color4(const Color4 &src) {
        vals[0] = src.vals[0], vals[1] = src.vals[1], vals[2] = src.vals[2], vals[3] = src.vals[3]; }

    operator const ValueType * () const { return vals; }

    operator ValueType * () { return vals; }

    void setOpacity(ValueType opacity) { vals[3] = opacity; }

private:
    ValueType vals[4];
};

typedef Color4<GLfloat> Color4f;

typedef Color4<GLdouble> Color4d;

// GENERIC FUNCTIONS ----------------------------------------------------------

template <typename Float>
inline void
setArray3(Float *vec, Float x, Float y, Float z) {
	vec[0] = x, vec[1] = y, vec[2] = z;
}

template <typename Float>
inline void
setArray4(Float *vec, Float x, Float y, Float z, Float w) {
	vec[0] = x, vec[1] = y, vec[2] = z, vec[3] = w;
}

template <typename Float>
inline void
copyArray3(Float *vec, const Float *src) {
	vec[0] = src[0], vec[1] = src[1], vec[2] = src[2];
}

template <typename Float>
inline void
copyArray4(Float *vec, const Float *src) {
	vec[0] = src[0], vec[1] = src[1], vec[2] = src[2], vec[3] = src[3];
}

// CLASSES --------------------------------------------------------------------

class GSM_API Positionable {
public:
	Positionable() {
		position();
	}
	Positionable(const Positionable &src) { 
		copyArray3<GLfloat>(_position, src._position);
	}

	void position(GLfloat x, GLfloat y, GLfloat z = 0) {
		setArray3(_position, x, y, z);
	}
	const Float3 & position() const { return _position; }
	Float3 & position() { 
		return _position; }

private:
	Float3	_position = { 0 };
};

template <class Obj, typename Float>
void set_position(Obj & obj, Float x, Float y, Float z) {
	static_cast<Positionable&>(obj).position(x, y, z);
}

class GSM_API Light: public Positionable {
public:
	Light() {
		//_index = index_;
		position(0, 0, 0);
		ambient(1, 1, 1);
		diffuse(1, 1, 1);
		specular(1, 1, 1);
	}

	//unsigned index() const { return _index; }

	void ambient(GLfloat r, GLfloat g, GLfloat b, GLfloat a = 1.0f) {
		setArray4(_ambient, r, g, b, a);
	}
	const GLfloat * ambient() const { return _ambient; }

	void diffuse(GLfloat r, GLfloat g, GLfloat b, GLfloat a = 1.0f) {
		setArray4(_diffuse, r, g, b, a);
	}
	const GLfloat * diffuse() const { return _diffuse; }

	void specular(GLfloat r, GLfloat g, GLfloat b, GLfloat a = 1.0f) {
		setArray4(_specular, r, g, b, a);
	}
	const GLfloat * specular() const { return _specular; }

	void setParameters(unsigned index) {
		glEnable(GL_LIGHT0+index);
		OGLCHECK("glEnable(GL_LIGHTx)");
		glLightfv(GL_LIGHT0+index, GL_AMBIENT, _ambient);
		OGLCHECK("glLightfv(,GL_AMBIENT,)");
		glLightfv(GL_LIGHT0+index, GL_DIFFUSE, _diffuse);
		OGLCHECK("glLightfv(,GL_DIFFUSE,)");
		glLightfv(GL_LIGHT0+index, GL_SPECULAR, _specular);
		OGLCHECK("glLightfv(,GL_SPECULAR,)");
	}

	void set_position(unsigned index) {
		glLightfv(GL_LIGHT0+index, GL_POSITION, position() );
		OGLCHECK("glLightfv(,GL_POSITION,)");
	}

	void disable(unsigned index) {
		glDisable(GL_LIGHT0+index);
		OGLCHECK("glDisable(GL_LIGHTx)");
	}

private:
	Float4      _ambient = { 0 };
	Float4      _diffuse = { 0 };
	Float4      _specular = { 0 };
};

typedef void * fonthandle_t;

// DRAWING UTILITIES ------------------------------------------------------------------

/** Draw a simple rectangle.
 */
void GSM_API
rectangle(unsigned wb, unsigned hb, int x = 0, int y = 0);

/** Texture must already be bound when calling this method.
    wb, hb:		Bitmap width and height
    xr, yr:		Origin from top-left of rectangle within texture bitmap (in pixels)
    wr, hr:		Rendered, i.e. on-screen width and height of the rectangle (in pixels)
                If smaller than the bitmap, the bitmap will be truncated; if larger, 
                the bitmap will be repeated.
    x, y  :     Position of top-left corner on screen
	yFlipped:	if true, the bitmap is "upside-down" (bottom pixels first in memory)
    */
void GSM_API
texturedRectangle(unsigned wb, unsigned hb, int xr, int yr, unsigned wr, unsigned hr, int x = 0, int y = 0, 
	bool yFlipped = false);

/** Draws a bevelled, pixel-aligned rectangular frame.
 *  The "colors" parameter specifies the color of each bevel "slope", in the following order:
 *  north, east, south, west. Each color is comprised of 4 float values.
 *  The "bw" parameter specifies the width of the bevel slopes, which are considered to
 *  be *inside* the rectangle (i.e. included in the width and height).
 */
void GSM_API
drawBevelFrame(unsigned w, unsigned h, unsigned bw, const Float4 *colors, int x = 0, int y = 0, GLfloat opacity = 1.0f);

//--- TEXT & FONTS ------------------------------------------------------------

/** Prepares a Font for use in the specified Video Context.
    The function returns a handle to be specified in text rendering calls.
    TODO: distinguish between monitors to take advantage of ClearType
 */
fonthandle_t GSM_API
prepareFont(IFont *font, int vidCtxID, const CharacterList * charlist = nullptr);

/** Saves the current projection matrix and sets up a new projection so that 2D coordinates 
    match directly with the pixel grid, with the top-left corner being at (0, 0) and the 
    bottom-right corner at (width, height).
    DOES NOT clear the MODELVIEW matrix!
 */
void GSM_API
prepareForTextRendering();

/** Restores the previous projection matrix. This is equivalent to doing glPopMatrix()
    with the GL_PROJECTION matrix active, then selecting the GL_MODELVIEW matrix.
 */
void GSM_API
doneWithTextRendering();

/** Renders Unicode text, which may contain line breaks (CR), but no other control
    characters.
    - This routine modifies the transformation matrix by doing pixel-aligned translations.
    - The horizontal and vertical displacements are also added to the dx and dy parameters.
 */
void GSM_API
renderText(fonthandle_t fonthandle, const unicode_t *text, int &dx, int &dy);

/** Use this version if you do not need to track output position advances.
 */
void GSM_API
renderText(fonthandle_t fonthandle, const unicode_t *text);

/** Determine the width of a line of text.
 */
void GSM_API
measureText(fonthandle_t fonthandle, const unicode_t *text, size_t len, int interline, BoundingBox &bbox);

/** Render text, aligning it.
    This routine does not attempt to break lines that do not fit the specified width.
 */
void GSM_API
renderTextAligned(fonthandle_t fonthandle, const unicode_t *text, size_t len, IFont::Alignment align, int interline, unsigned w, int &y);

int GSM_API
calcLeading(fonthandle_t font1, fonthandle_t font2 = NULL, unsigned lineSpacing = 0);

//--- DEBUG -------------------------------------------------------------------

bool  GSM_API
dbg_getFontTexture(fonthandle_t fonthandle, unsigned range_index, GLuint &tex, unsigned &w, unsigned &h);

} // ns opengl
} // ns gsm

#endif // __GSM_OPENGL_OPENGL_HPP
