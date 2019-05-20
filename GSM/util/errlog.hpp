/*-----------------------------------------------------------------------------
 * This file is part of the Graphical Session Manager (GSM) project.
 * Copyright 2012 by Gygax Practical Computing, Biel/Bienne, Switzerland.
 *
 * This software is released under the Gnu General Public License.
 * A copy of that license should have been delivered with this source code.
 * If not, you can download it from http://www.gnu.org/licenses/gpl.txt.
 *-----------------------------------------------------------------------------*/

#ifndef __GSM_UTIL_ERRLOG_HPP
#define __GSM_UTIL_ERRLOG_HPP

#define WINDOWS_LEAN_AND_MEAN
#define NOMINMAX
#include <Windows.h>

#define NOMINMAX
#include <Windows.h>

#include <algorithm>
#include <GL/glew.h>


namespace gsm {

class ErrorLogger {

public:

	typedef void (*log_func)(GLenum err, const char *ctx);

	static inline /*EXPORT*/ ErrorLogger * instance() {
		static ErrorLogger instance;
		return &instance;
	}

	void switch_debugger_channel(bool state) {
		debugger = state;
	}

	void register_log_func(log_func func) {
		log_funcs.push_back(func);
	}

	void remove_log_func(log_func func) {
		log_funcs.erase( std::find(log_funcs.begin(), log_funcs.end(), func) );
	}

	void log(GLenum err, const char *ctx) {
		if (debugger) {
			std::stringstream ss;
			ss << "OpenGL error " << err << " while executing " << ctx << std::endl;
			OutputDebugStringA( ss.str().c_str() );
		}
		for (log_func_list::iterator it = log_funcs.begin(); it != log_funcs.end(); it++) 
			(*it)(err, ctx);
	}

private:

	typedef std::vector<log_func>	log_func_list;

	ErrorLogger() {
		debugger = true;
	}

	bool							debugger;

	log_func_list					log_funcs;
};

} // ns gsm

#endif // __GSM_UTIL_ERRLOG_HPP
