#ifndef __GSM_DLL_H
#define __GSM_DLL_H

#ifdef GSM_STATIC
#define GSM_API
#else
#ifdef GSM_EXPORTS
#define GSM_API __declspec(dllexport)
#else
#define GSM_API __declspec(dllimport)
#endif
#endif

#endif // __GSM_DLL_H
