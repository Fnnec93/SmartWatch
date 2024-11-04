#ifndef MDW_LOG_H
#define MDW_LOG_H

// log levels definition.
typedef enum 
{
    SYS_LOG_ERROR,
    SYS_LOG_WARNING,
    SYS_LOG_INFO,
    SYS_LOG_DEBUG
} SysLogLevel;

// Uart init
void  mdw_log_load(void);

void mdw_log_open(void);

void mdw_log_close(void);

// Sys log function
void sys_log(SysLogLevel level, const char* pMessage);

#endif // MDW_LOG_H
