/* 
 * File:   logging.h
 * Author: student
 *
 * Created on 25. října 2016, 12:30
 */

#ifndef LOGGING_H
#define	LOGGING_H

#define LOG_DEBUG   3
#define LOG_INFO    2
#define LOG_WARNING 1
#define LOG_ERROR   0

#ifdef	__cplusplus
extern "C" {
#endif
    // ... znamena ze fce akceptuje promenny pocet parametru
    void log_message(int level, const char * format, ...);


#ifdef	__cplusplus
}
#endif

#endif	/* LOGGING_H */

