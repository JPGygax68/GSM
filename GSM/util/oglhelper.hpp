/*-----------------------------------------------------------------------------
 * This file is part of the Graphical Session Manager (GSM) project.
 * Copyright 2012 by Gygax Practical Computing, Biel/Bienne, Switzerland.
 *
 * This software is released under the Gnu General Public License.
 * A copy of that license should have been delivered with this source code.
 * If not, you can download it from http://www.gnu.org/licenses/gpl.txt.
 *-----------------------------------------------------------------------------*/

#ifndef __GSM_UTIL_OGLHELPER_HPP
#define __GSM_UTIL_OGLHELPER_HPP

#ifdef _WIN32
#include <windows.h>
#endif
#include <GL/glew.h>

#include <exception>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>

#include "errlog.hpp"

namespace gsm {


// EXCEPTIONS AND ERROR HANDLING ---------------------------------------------

class opengl_error : public std::runtime_error {
	static const char * get_error_string(GLenum err) {
		std::stringstream ss;
		ss << err << std::ends;
		return ss.str().c_str();
	}
public:
	opengl_error(GLenum err, const char * context) :
		std::runtime_error(
			std::string("OpenGL error ") +
			get_error_string(err) + " while executing " + context )
	{
	}
};

#if _DEBUG && (!_NO_OPENGL_CHECKS)

	#ifdef _OGLERROR_THROW
		#define OGLREPORT(err, ctx) throw gsm::opengl_error(err, ctx)
	#else
		#define OGLREPORT(err, ctx) gsm::ErrorLogger::instance()->log(err, ctx)
	#endif

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

// GENERIC FUNCTIONS ------------------------------------------------------------------

template <typename Float>
inline void set_vector4(Float *vec, Float x, Float y, Float z, Float w) {
	vec[0] = x, vec[1] = y, vec[2] = z, vec[3] = w;
}

template <typename Float>
inline void copy_vector4(Float *vec, Float *src) {
	vec[0] = src[0], vec[1] = src[1], vec[2] = src[2], vec[3] = src[3];
}

// CLASSES ----------------------------------------------------------------------------

template <typename Float>
class Vector3 {
public:
	Vector3() {
		_vec[0] = _vec[1] = _vec[2] = 0;
	}
	Vector3(const Float *src) {
		set(src[0], src[1], src[2]);
	}
	Vector3(Float x, Float y, Float z) {
		set(x, y, z);
	}
	/*operator Float * () const {
		return _vec;
	}*/
	const Float * get() const {
		return _vec;
	}
	operator Float * () {
		return _vec;
	}
	void set(Float x, Float y, Float z) {
		_vec[0] = x, _vec[1] = y, _vec[2] = z;
	}

private:
	Float _vec[3];
};

template <typename Float>
class Vector4 {
public:
	Vector4() {
		_vec[0] = _vec[1] = _vec[2] = 0;
		_vec[3] = 1;
	}
	Vector4(Float x, Float y, Float z, Float w = 1) {
		set(x, y, z, w);
	}
	/*operator Float * () const {
		return _vec;
	}*/
	const Float * get() const {
		return _vec;
	}
	operator Float * () {
		return _vec;
	}
	void set(Float x, Float y, Float z, Float w = 1) {
		_vec[0] = x, _vec[1] = y, _vec[2] = z, _vec[3] = w;
	}

private:
	Float _vec[4];
};

template <typename Float>
class Color3: public Vector3<Float> {
public:
	Color3(): Vector3() {}
	Color3(const Float *src): Vector3(src) {}
	Color3(Float r, Float g, Float b): Vector3(r, g, b) {}
};

template <typename Float>
class Color4: public Vector4<Float> {
public:
	Color4() { set(0, 0, 0, 0); }
	Color4(Float r, Float g, Float b, Float a = 1): Vector4(r, g, b, a) {}
	Color4(const Color3<Float> &src) { 
		const Float *values = src.get();
		set(values[0], values[1], values[2], 1);
	}
};

class Positionable {
public:
	Positionable() {
		position();
	}
	Positionable(const Positionable &src) { 
		_position = src._position;
	}

	void position(GLfloat x, GLfloat y, GLfloat z = 0, GLfloat w = 1.0f) {
		_position.set(x, y, z, w);
	}
	const Vector4<GLfloat> & position() const { return _position; }
	Vector4<GLfloat> & position() { 
		return _position; }

private:
	Vector4<GLfloat>	_position;
};

template <class Obj, typename Float>
void set_position(Obj & obj, Float x, Float y, Float z) {
	static_cast<Positionable&>(obj).position(x, y, z);
}

class Light: public Positionable {
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
		set_vector4(_ambient, r, g, b, a);
	}
	const GLfloat * ambient() const { return _ambient; }

	void diffuse(GLfloat r, GLfloat g, GLfloat b, GLfloat a = 1.0f) {
		set_vector4(_diffuse, r, g, b, a);
	}
	const GLfloat * diffuse() const { return _diffuse; }

	void specular(GLfloat r, GLfloat g, GLfloat b, GLfloat a = 1.0f) {
		set_vector4(_specular, r, g, b, a);
	}
	const GLfloat * specular() const { return _specular; }

	void set_parameters(unsigned index) {
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
	GLfloat		_ambient[4];
	GLfloat		_diffuse[4];
	GLfloat		_specular[4];
};

// General utilities --------------------------------------

inline unsigned
next_pow2(unsigned v) {
	unsigned v2;
	for (v2 = 1; v2 < v; v2 <<= 1);
	return v2;
}

// Color management ---------------------------------------

inline Color3<GLfloat> gl_clear_color() {
	Color3<GLfloat> color;
	OGL(glGetFloatv, (GL_COLOR_CLEAR_VALUE, color) );
	return color;
}

// Drawing primitives -------------------------------------

inline void
glRectangle(int x, int y, unsigned w, unsigned h) {
	glBegin(GL_QUADS);
		glVertex2i(  x,   y);
		glVertex2i(  x, y+h);
		glVertex2i(x+w, y+h);
		glVertex2i(x+w,   y);
	glEnd();
}

inline void
glRectanglePow2(int x, int y, unsigned w, unsigned h) {
	w = next_pow2(w); h = next_pow2(h);
	glBegin(GL_QUADS);
		glVertex2i(  x,   y);
		glVertex2i(  x, y+h);
		glVertex2i(x+w, y+h);
		glVertex2i(x+w,   y);
	glEnd();
}

} // ns valueof(GPCUINS)

#endif // __GSM_UTIL_OGLHELPER_HPP
