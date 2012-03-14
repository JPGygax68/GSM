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
#include <sstream>
#include <stdexcept>
#include <GL/glew.h>

#include "../dll.h"
#include "../types.hpp"
#include "../ifont.hpp"

namespace gsm {

class ISurface;

namespace ogl {

// EXCEPTIONS AND ERROR HANDLING ---------------------------------------------

class Error : public std::runtime_error {
	static const char * get_error_string(GLenum err) {
		std::stringstream ss;
		ss << err << std::ends;
		return ss.str().c_str();
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
		#define OGLREPORT(err, ctx) throw Error(err, ctx)
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

// GENERIC FUNCTIONS ----------------------------------------------------------

template <typename Float>
inline void setArray3(Float *vec, Float x, Float y, Float z) {
	vec[0] = x, vec[1] = y, vec[2] = z;
}

template <typename Float>
inline void setArray4(Float *vec, Float x, Float y, Float z, Float w) {
	vec[0] = x, vec[1] = y, vec[2] = z, vec[3] = w;
}

template <typename Float>
inline void copyArray3(Float *vec, const Float *src) {
	vec[0] = src[0], vec[1] = src[1], vec[2] = src[2];
}

template <typename Float>
inline void copyArray4(Float *vec, const Float *src) {
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
	Float3	_position;
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
	Float4      _ambient;
	Float4      _diffuse;
	Float4      _specular;
};

typedef void * fonthandle_t;

// DRAWING UTILITIES ------------------------------------------------------------------

/** Draw a simple rectangle.
 */
void GSM_API
rectangle(unsigned wb, unsigned hb, int x = 0, int y = 0);

/** Texture must already be bound when calling this method.
    wb, hb:		bitmap width and height
    xr, yr:		origin from top-left of rectangle within texture bitmap (in pixels)
    wr, hr:		width and height of rectangle within texture bitmap (in pixels)
    x, y  :     position of top-left corner on screen
    */
void GSM_API
texturedRectangle(unsigned wb, unsigned hb, int xr, int yr, unsigned wr, unsigned hr, int x = 0, int y = 0);

/** Draws a bevelled, pixel-aligned rectangular frame.
 *  The "colors" parameter specifies the color of each bevel "slope", in the following order:
 *  north, east, south, west. Each color is comprised of 4 float values.
 *  The "bw" parameter specifies the width of the bevel slopes, which are considered to
 *  be *inside* the rectangle (i.e. included in the width and height).
 */
void GSM_API
drawBevelFrame(unsigned w, unsigned h, unsigned bw, const Float4 *colors, int x = 0, int y = 0);

//--- TEXT & FONTS ------------------------------------------------------------

/** Prepares a Font for use in the specified Video Context.
    The function returns a handle to be specified in text rendering calls.
    TODO: distinguish between monitors to take advantage of ClearType
 */
fonthandle_t GSM_API
prepareFont(IFont *font, int vidCtxID);

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
