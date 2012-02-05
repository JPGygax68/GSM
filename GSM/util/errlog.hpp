#ifndef __GSM_UTIL_ERRLOG_HPP
#define __GSM_UTIL_ERRLOG_HPP

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
		unsigned n = log_funcs.size();
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
		unsigned n = log_funcs.size();
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
