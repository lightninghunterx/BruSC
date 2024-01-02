/*
 *******************************************************************
 *
 * $Source: /sc/CvsTree/sc/gen/h/lib/lib_tty.h,v $
 *
 * Copyright (c) 1998
 * BRUKER BIOSPIN GMBH
 * D-76287 Rheinstetten, Germany
 *
 * All Rights Reserved
 *
 * $Id: lib_tty.h,v 1.41 2016/01/29 15:42:32 ah Exp $
 *
 *******************************************************************
 */

#ifndef LIB_TTY_H			/* prevent multiple includes */
#define LIB_TTY_H

/*:=INFO=:**************************************************************
 *
 * Header : lib_tty.h
 *
 * Description : interface to functions handling the communication with
 *		 a tty device
 *
 * Following functions and variables are available :
 * - tty_config()	: configure tty device
 * - tty_open()		: open tty device
 * - tty_set_timeout()	: set timeout on tty device
 * - tty_lock()		: lock tty device
 * - tty_unlock()	: unlock tty device
 * - tty_close()	: close tty device
 * - tty_waitbyte()	: wait until at least one byte can be read
 * - tty_writebyte()	: write one byte to tty device
 * - tty_readbyte()	: read one byte from tty device
 * - tty_name()		: return full name of tty device (incl. host)
 * - tty_errmsg()	: returns error message 
 * - tty_helptxt()	: returns help text 
 * - tty_errcode	: contains error code
 *
 * Usage of function calls :
 *   all functions return 0 when okay, -1 on error:
 *
 * Used libraries: libcb.a, util.a
 *
 * Notes: following h-files must be included before:
 *	  - /u/gen/h/machine.h
 *
 *::=info=:*************************************************************/

/*** include necessary header files ***/

#include <machine.h>
#include <limits.h>     /* to know PATH_MAX (Unix) */
#include <unistd.h>     /* to know PATH_MAX (Win32) */
#include <stdio.h>     /* to know FILE */

/*** forward declarations ***/

struct termios;

/*** definitions needed for making DLL's on Windows-NT */

#ifdef DLL_INTERFACE
#   undef DLL_INTERFACE
#endif
#if defined(DLL_LINKAGE) && !defined(CPROTO)
#   ifdef lib_tty_INTERNAL
#      define DLL_INTERFACE __declspec(dllexport)
#   else
#      define DLL_INTERFACE __declspec(dllimport)
#   endif
#else
#   define DLL_INTERFACE
#endif

/*** definitions needed for using lib_tty within C++ code */

#ifdef __cplusplus
extern "C" {
#endif

/** Definitions */
/*@{*/

/*** general tty definitions ***/

#define TTY_DEFAULT_TIMEOUT	20	/* seconds */
#define TTY_INITIAL_FDVAL	-1

#define TTY_HL			64	/* length of host string, same as INSTRUM */
#define TTY_NL			64	/* length of name string */
#define TTY_PL			PATH_MAX /* length of path string */
#define TTY_EML			1024	/* length of error message */
#define TTY_STTYL		1024	/* length of stty string */

/*** specific tty device definitions ***/

	/*** part of pathname to configuration file : ***/
#define TTY_CONFIG_DIR	 	"rs232_device"
#define SCM_CONFIG_FILE 	TTY_CONFIG_DIR "/scm"
#define BACS_CONFIG_FILE 	TTY_CONFIG_DIR "/bacs"
#define BPSU_CONFIG_FILE 	TTY_CONFIG_DIR "/bpsu"
#define TEMP_CONFIG_FILE 	TTY_CONFIG_DIR "/temp"
#define GTU_CONFIG_FILE 	TTY_CONFIG_DIR "/tg"
#define BARPRN_CONFIG_FILE 	TTY_CONFIG_DIR "/barcode_prn"
#define MAS_CONFIG_FILE 	TTY_CONFIG_DIR "/mas"
#define PHYS_CONFIG_FILE 	TTY_CONFIG_DIR "/phys"
#define PREAMP_CONFIG_FILE 	TTY_CONFIG_DIR "/preamp"
#define MOTCTRL_CONFIG_FILE 	TTY_CONFIG_DIR "/motctrl"
#define LOCK_CONFIG_FILE	TTY_CONFIG_DIR "/lock"
#define SWISH_CONFIG_FILE	TTY_CONFIG_DIR "/swish"
#define ROBOT_CONFIG_FILE       TTY_CONFIG_DIR "/robot"
#define INJECTOR_CONFIG_FILE    TTY_CONFIG_DIR "/injector"

	/*** default devices for different units : ***/
#define SCM_DEFAULT_TTYDEV 	"/dev/tty14"
#define BACS_DEFAULT_TTYDEV 	"/dev/tty13"
#define BPSU_DEFAULT_TTYDEV 	"/dev/"
#define TEMP_DEFAULT_TTYDEV 	"/dev/tty03"
#define GTU_DEFAULT_TTYDEV 	"/dev/tty04"
#define BARPRN_DEFAULT_TTYDEV 	"/dev/tty18"	/* local */
#define MAS_DEFAULT_TTYDEV 	"/dev/tty01"
#define PHYS_DEFAULT_TTYDEV 	"/dev/"
#define PREAMP_DEFAULT_TTYDEV 	"/dev/tty11"
#define MOTCTRL_DEFAULT_TTYDEV 	"/dev/"
#define LOCK_DEFAULT_TTYDEV	"/dev/tty12"
#define SWISH_DEFAULT_TTYDEV	"/dev/tty08"
#define ROBOT_DEFAULT_TTYDEV    "/dev/tty01"
#define INJECTOR_DEFAULT_TTYDEV "/dev/tty05"

/*** macros ***/

/** check if tty device is locked */
#define TTY_IS_LOCKED(tty_p)	((tty_p)->is.locked)

/** check if tty device is a RS485 */
#define TTY_IS_RS485(tty_p)	((tty_p)->is.rs485)

/*@}*/


/*** types ***/

/** Global Types */
/*@{*/

/** error codes */
typedef enum tty_errcode
{
  TTY_OKAY,
  TTY_CANCELLED,
  TTY_NULL_ARGUMENT,
  TTY_BAD_ARGUMENT,
  TTY_INSTR_NOT_CONFIGURED,
  TTY_DEV_NOT_CONFIGURED,
  TTY_BAD_CONFIG_FILE,
  TTY_DEVICE_IS_OPEN,
  TTY_DEVICE_NOT_OPEN,
  TTY_DEVICE_IS_LOCKED,
  TTY_DEVICE_NOT_LOCKED,
  TTY_DEVICE_NOT_LOCKED_BY_ME,
  TTY_DEVICE_IS_BUSY,
  TTY_TIMEOUT_ERROR,
  TTY_REMOTE_TIMEOUT,
  TTY_REMOTE_NOT_SUPPORTED,
  TTY_LOCK_TIMEOUT,
  TTY_NOT_FOR_REMOTE,
  TTY_NOT_FOR_X32,
  TTY_TIMEOUT_TOO_SHORT,
  TTY_TIMEOUT_TOO_LONG,
  TTY_MKUDIR_ERROR,
  TTY_CONSOLE_IO_ERROR,
  TTY_WRONG_ANSWER,
  TTY_EXTERNAL_ERROR,
  TTY_UNKNOWN_ERROR
} TTY_ERRCODE;

/** TTY_STATE: internal status flags */
typedef struct
{
    /** lock status: 0 = is unlocked, 1 = is locked */
    unsigned int locked:1;

    /** RS485 status: 0 = is no RS485, 1 = is RS485 */
    unsigned int rs485:1;

    /** socket status: 0 = local device, 1 = socket */
    unsigned int socket:1;

    /** if a device is busy see this as an error. */
    unsigned int handleBusyAsError:1;
} TTY_STATE;


/** represents a TTY device */
struct tty_device
{
    /*-----------------------------------------------------------*/
    /* entries which must be specified by the user at definition */
    /*-----------------------------------------------------------*/

    /** name of the configuration file

    {\tt confname} is set by {\tt tty_init()}
    */ 
    char confname[TTY_PL];

    
    /** file descriptor of tty device

        {\tt fd} is set by {\tt tty_init()}, tty_open() and {\tt tty_close()}. 

        {\tt fd} should be of type {\tt rrs_socket_t}, but casting works for 32bit RHEL and Windows.
	If {\tt select()} no longer works, we must introduce separate descriptor types
	for local and remote devices. This means also a replacement for tty_get_fd()
	and a rework of all user code which accesses 'fd' directly or indirectly. 
	Since this means a lot of work we wait until it is really necessary. 
    */
    int	 fd;

    /*-----------------------------------------------------------*/
    /* entries which may be set by the high level functions ONLY */
    /*-----------------------------------------------------------*/

    /** name of device

        {\tt device} contains the name of the TTY device (e.g. "{\em /dev/tty03}")
	and is set by {\tt tty_open()} and {\tt tty_config()}. 
    */
    char device[TTY_NL];

    /** name of host where device resides

        {\tt host} contains the name of the host (e.g. "{\em amx300}") where
	the TTY device resides, and is set by {\tt tty_open()} and {\tt tty_config()}. 
	Contains an empty string if device is on the local host.
    */ 
    char host[TTY_HL];

    /** wait time in seconds for read and write methods.

        {\tt timeout} is set by {\tt tty_init()} and modified by {\tt tty_open()}. 
    */
    unsigned int timeout;

    /** name of file used for lock mechanism of remote devices

        {\tt lockname} is set by {\tt tty_open()} and used by the locking functions. 
    */
    char lockname[TTY_PL];

#ifdef HAS_WINDOWS_FEATURES
    int		icrnl;		/* convert \r to \n */
    void*         hComm;	 	/* handle for communication port  */
    void*         lockfd; 	/* file handle for lock file */
#else
    /** file descriptor of file used for lock mechanism

        {\tt lockfd} is set by the locking functions. 
    */
    int lockfd;
#endif
    /** status flags, for internal use only */
    TTY_STATE is;
};

typedef struct tty_device TTY_DEVICE;

/** type of TTY device */
typedef enum tty_device_type
{
    /** unknown: use old default depending on device name */
    TTY_DEVICE_TYPE_UNKNOWN,
    /** device is a RS232. */
    TTY_DEVICE_TYPE_RS232,
    /** device is a RS485. */
    TTY_DEVICE_TYPE_RS485,
    /** device is a RS422. */
    TTY_DEVICE_TYPE_RS422
} TTY_DEVICE_TYPE;

/*@}*/

/*** variables ***/

/** Global Variables */
/*@{*/

/** debug file pointer */
extern DLL_INTERFACE FILE*		TtyDebFile;

/** last error code. */
extern DLL_INTERFACE TTY_ERRCODE	tty_error;

/*@}*/

/*** prototypes ***/

/** High Level Functions Working on {\tt struct tty_device}

  The first function call parameter is always the pointer to the current
  {\tt tty} object (instance). Furthermore the order of the parameters in
  the function call is such that input parameters are listed first before
  output parameters.

  The return value is always {\tt 0} (OKAY) when okay and {\tt -1}
  (FAIL) in case of error to be conform with UNIX system calls.

  The first function call parameter is always the pointer to the current 
  {\em tty object} (instance). Furthermore the order of the parameters in the 
  function call is so that input parameters are listed first before output 
  parameters.
 */
/*@{*/

    /** Basic Functions */
    /*@{*/


/** initialize tty struct components

  {\tt tty_init()} initializes the {\tt TTY_DEVICE}. This function
  {\bf must always be called first} before each of the other functions
  is called. If it is not planned to call {\tt tty_config()} (which
  is usually the case) the parameter {\tt default_device} can be
  ignored and set to {\tt NULL}. {\tt tty_init()} works as C-constructor
  for {\em TTY} objects.

  In case of an error {\tt tty_init()} returns {\tt -1} and sets
  {\tt tty_error} to one of the following error codes:
  \begin{description}
  \item {\tt TTY_NULL_ARGUMENT:} one or more of {\tt tty} or {\tt configfile} is a {\tt NULL} pointer
  \end{description}

  {\bf Interna:}

  {\tt tty_init()} initializes the internal class components of {\tt struct tty_device} as follows:
  \begin{itemize}
  \item {\tt tty->fd} and {\tt tty->lockfd} are set to {\tt -1}
  \item {\tt tty->timeout} is set to timeout
  \item {\tt tty->host} and {\tt tty->lockname} are cleared
  \item {\tt tty->device} is either set to {\tt default_device} or cleared
  \item {\tt tty->is.locked} and {\tt tty->is.rs485} are set to {\tt 0}
  \item {\tt tty->is.socket} and {\tt tty->is.handleBusyAsError} are set to {\tt 0}
  \end{itemize}

  \param tty            pointer to tty device object
  \param configfile     name of configuration file
  \param timeout        maximum wait time in seconds for open/read/write 
  \param default_device  name of default device or NULL, necessary for tty_config() only.
  \return OKAY or FAIL
*/
extern DLL_INTERFACE int tty_init (struct tty_device* tty, const char* configfile, int timeout,
				   const char* default_device);

/** open device

  {\tt tty_open()} reads the device configuration file (created
  by {\tt tty_config()}) and opens the device with the appropriate
  configuration setting.

  In case of an error {\tt tty_open()} returns {\tt -1} and sets
  {\tt tty_error} to one of the following error codes:

  \begin{description}
  \item {\tt TTY_NULL_ARGUMENT:} {\tt tty} is a {\tt NULL} pointer
  \item {\tt TTY_DEVICE_IS_OPEN:} the tty device is already open
  \item {\tt TTY_INSTR_NOT_CONFIGURED:} the spectrometer is not configured
  \item {\tt TTY_DEV_NOT_CONFIGURED:} the device is not configured
  \item {\tt TTY_BAD_CONFIG_FILE:} the configuration file contains corrupt data
  \item {\tt TTY_TIMEOUT_ERROR:} the device could not be opened
        within the specified time, see {\tt tty_init()} and {\tt tty_set_timeout()}
        how to increase the timeout
  \item {\tt TTY_EXTERNAL_ERROR:} other errors, call {\tt tty_errmsg()}
  \item other error codes: refer to {\tt tty_lock()}, {\tt tty_unlock()}
        and {\tt tty_posix_stty2termios()}
  \end{description}

  {\bf Interna:}

  {\tt tty_open()} reads the device configuration from 
  {\tt {\em \$XWINNMRHOME/conf/instr/<instrument>/<tty->confname>}}
  and stores the names of device and host in {\tt tty->device} and
  {\tt tty->host}, respectively. For remote devices (and local devices
  on Windows-NT) the full pathname of the lock file 
  {\tt {\em /usr/diskless/clients/spect/lock/<devicename>}}
  is constructed and stored in {\tt tty->lockname}. Local devices
  on Unix are locked using their own file descriptor, therefore {\tt tty->lockname}
  stays empty.

  The devicename is processed as follows:
  \begin{itemize}
  \item On Unix the directory name is stripped, e.g. {\em /dev/tty03} is reduced to {\em tty03}.
  \item On Windows-NT a '_' is inserted before the number, because it is not possible to create 
        files with the same name like a device; e.g. {\em com1} is changed to {\em com_1}.
  \end{itemize}

  Opening the device and setting its configuration vary depending on
  whether the device is local or remote:

  \begin{description}
  \item A local device is opened via the {\tt open(2)} system call
        (the timeout for the {\tt open()} system call is {\tt tty->timeout}).
	and the file descriptor is stored in {\tt tty->fd}. Then the device
	is locked to set the configuration. This is done as follows:
	\begin{enumerate}
	\item the termios flags are created according to the default configuration
	      (all flags are empty and the baudrate is set to 9600 baud) of the rrs-server
	\item the termios flags are changed according to the stty flags specified
	      in the configuration file 
	\item the termios flags are then set via the POSIX.1 compatible call {\tt tcsetattr()}.
	      The device lock is removed.
	\end{enumerate}
  \item A remote device is locked first and then opened via {\tt remote_open()}
        (the timeout for {\tt remote_open()} is the maximum of {\tt tty->timeout}
	and 20 seconds) and the file descriptor is stored in {\tt tty->fd}. 
	Then {\tt remote_status()} is called to check whether the connection to the remote device was
	already established before. Otherwise the stty flags are set via {\tt remote_stty()}.
	Finally the device lock is removed.
  \end{description}
  {\bf Note:} {\tt tty->timeout} is changed according to the local
  stty flag {\tt time} as read from the configuration file.

  \param tty   pointer to tty device object
  \return OKAY or FAIL
*/
extern DLL_INTERFACE int tty_open (struct tty_device* tty);

/** close the tty device

  {\tt tty_close()} closes the device. If the device was locked
  it will be automatically unlocked.

  In case of an error {\tt tty_close()} returns {\tt -1} and
  sets {\tt tty_error} to one of the following error codes:

  \begin{description}
  \item {\tt TTY_NULL_ARGUMENT:}   {\tt tty} is a {\tt NULL} pointer
  \item {\tt TTY_DEVICE_NOT_OPEN:} the tty device was not open
  \item {\tt TTY_EXTERNAL_ERROR:}  {\tt close(2)} failed, call {\tt tty_errmsg()} to get {\tt strerror(errno)}.
  \end{description}

  {\bf Interna:}

  {\tt tty_close()} closes the device and resets the parameter {\tt tty->d}
  to {\tt -1}. When the device was remotely locked it will close
  {\tt tty->lockfd} as well, removing a pending device lock. The
  names of device, host and lockfile are cleared, timeout is reset to
  {\tt TTY_DEFAULT_TIMEOUT}.

  \param tty     pointer to tty device object
  \return OKAY or FAIL
*/
extern DLL_INTERFACE int tty_close (struct tty_device* tty);

#ifdef	HAS_WINDOWS_FEATURES
/** set communication timeout

  {\tt tty_SetCommTimeouts} change timeout of character reads
  Use native Windows API calls to update the hareware value from the timeout parameter in 
  struct tty_device
  \begin{description}
  \item {\tt TTY_NULL_ARGUMENT:} one or more of {\tt tty} or {\tt byte} is a {\tt NULL} pointer
  \item {\tt TTY_DEVICE_NOT_OPEN:} the tty device is not open
  \item {\tt TTY_TIMEOUT_ERROR:} nothing could be read within the specified time, see {\tt tty_init()} 
        and {\tt tty_set_timeout()} how to increase the timeout
  \item {\tt TTY_EXTERNAL_ERROR:} other errors, call {\tt tty_errmsg()}
  \item {\tt TTY_UNKNOWN_ERROR:} {\tt read(2)} returns unexpected value
  \item other error codes: refer to {\tt tty_lock()}, {\tt tty_unlock()} 
  \end{description}

  \param tty   pointer to tty device object
  \return OKAY or FAIL
*/
extern DLL_INTERFACE int tty_SetCommTimeouts(struct tty_device* tty);
#endif				    


/** read one byte from tty device

  {\tt tty_readbyte} reads one byte from the device and stores it
  in {\tt *byte}.. If the device is not yet locked it will be locked
  before and unlocked afterwards.

  In case of an error {\tt tty_readbyte()} returns {\tt -1} and
  sets {\tt tty_error} to one of the following error codes:
  \begin{description}
  \item {\tt TTY_NULL_ARGUMENT:} one or more of {\tt tty} or {\tt byte} is a {\tt NULL} pointer
  \item {\tt TTY_DEVICE_NOT_OPEN:} the tty device is not open
  \item {\tt TTY_TIMEOUT_ERROR:} nothing could be read within the specified time, see {\tt tty_init()} 
        and {\tt tty_set_timeout()} how to increase the timeout
  \item {\tt TTY_EXTERNAL_ERROR:} other errors, call {\tt tty_errmsg()}
  \item {\tt TTY_UNKNOWN_ERROR:} {\tt read(2)} returns unexpected value
  \item other error codes: refer to {\tt tty_lock()}, {\tt tty_unlock()} 
  \end{description}

  \param tty   pointer to tty device object
  \param byte  where the byte is stored
  \return OKAY or FAIL
*/
extern DLL_INTERFACE int tty_readbyte (struct tty_device* tty, unsigned char *);

/** read NULL terminated string from device

  {\tt tty_gets()} reads in at most one less than {\tt size}
  characters from the specified {\tt tty} device and stores them
  into the buffer pointed to by {\tt str}. Reading stops after a
  newline. If a newline is read, it is stored into the buffer. A '\textbackslash0'
  is stored after the last character in the buffer.

  Please note:
  \begin{itemize}
  \item The buffer always contains valid data, even in case of error (except
        for {\tt TTY_NULL_ARGUMENT}), so {\bf always check {\tt str} for existing
        data}, e.g. with {\tt if (0 < strlen(str))}. 
  \item If the {\tt size} of the buffer {\tt str} was not big enough
        the last character is not a newline. 
  \end{itemize}

  In case of an error {\tt tty_gets()} returns {\tt -1} and sets
  {\tt tty_error} to one of the following error codes:

  \begin{description}
  \item {\tt TTY_NULL_ARGUMENT:} one or more of {\tt tty}
        or {\tt str} is a {\tt NULL} pointer
  \item {\tt TTY_DEVICE_NOT_OPEN:} the tty device is not open
  \item {\tt TTY_TIMEOUT_ERROR:} nothing could be read within
        the specified time, see {\tt tty_init()} and 
        {\tt tty_set_timeout()} how to increase the timeout
  \item {\tt TTY_EXTERNAL_ERROR:} other errors, call {\tt tty_errmsg()}
  \item {\tt TTY_UNKNOWN_ERROR:} {\tt read(2)} returns
        unexpected value
  \item other error codes: see tty_lock, tty_unlock 
  \end{description}

  \param tty   pointer to tty device object
  \param str   pointer to character buffer
  \param size  size of character buffer {\tt str}
  \return OKAY or FAIL
*/
extern DLL_INTERFACE int tty_gets (struct tty_device* tty, char* str, unsigned int size);

/** write one byte to tty device

  {\tt tty_writebyte} writes the {\tt byte} into the device.
  If the device is not yet locked it will be locked before and unlocked
  afterwards.

  In case of an error {\tt tty_writebyte()} returns {\tt -1}
  and sets {\tt tty_error} to one of the following error codes:

  \begin{description}
  \item {\tt TTY_NULL_ARGUMENT:} one or more of {\tt tty} or {\tt byte} is a {\tt NULL} pointer
  \item {\tt TTY_DEVICE_NOT_OPEN:} the tty device is not open
  \item {\tt TTY_TIMEOUT_ERROR:} nothing could be written within the specified time, 
        see {\tt tty_init()} and {\tt tty_set_timeout()} how to increase the timeout
  \item {\tt TTY_EXTERNAL_ERROR:} other errors, call {\tt tty_errmsg()}
  \item other error codes: refer to {\tt tty_lock()}, {\tt tty_unlock()} 
  \end{description}

  \param tty   pointer to tty device object
  \param byte  the byte to be written
  \return OKAY or FAIL
*/
extern DLL_INTERFACE int tty_writebyte (struct tty_device* tty, unsigned char byte);

/** write string of bytes to device

  {\tt tty_writestring} writes {\tt nbyte} bytes from {\tt byteaddr}
  into the device. If the device is not yet locked it will be locked
  before and unlocked afterwards.

  In case of an error {\tt tty_writestring()} returns {\tt -1}
  and sets {\tt tty_error} to one of the following error codes:
  \begin{description}
  \item {\tt TTY_NULL_ARGUMENT:} one or more of {\tt tty} or {\tt byteaddr} is a {\tt NULL} pointer
  \item {\tt TTY_DEVICE_NOT_OPEN:} the tty device is not open
  \item {\tt TTY_TIMEOUT_ERROR:} nothing could be written within the specified time, 
        see {\tt tty_init()} and {\tt tty_set_timeout()} how to increase the timeout
  \item {\tt TTY_EXTERNAL_ERROR:} other errors, call {\tt tty_errmsg()}
  \item other error codes: refer to {\tt tty_lock()}, {\tt tty_unlock()} 
  \end{description}

  \param tty   pointer to tty device object
  \param byteaddr  buffer of bytes to be written
  \param nbytes    number of bytes in buffer {\tt byteaddr}
  \return OKAY or FAIL
*/
extern DLL_INTERFACE int tty_writestring (struct tty_device* tty, const unsigned char* byteaddr,
					  int nbytes);

/** write NULL terminated string to device

  {\tt tty_puts()} writes the NULL terminated string {\tt str}
  to the specified {\tt tty} device excluding the terminating NULL.

  In case of an error {\tt tty_puts()} returns {\tt -1} and sets
  {\tt tty_error} to one of the following error codes:

  \begin{description}
  \item {\tt TTY_NULL_ARGUMENT:} one or more of {\tt tty} or {\tt str} is a {\tt NULL} pointer
  \item {\tt TTY_DEVICE_NOT_OPEN:} the tty device is not   open
  \item {\tt TTY_TIMEOUT_ERROR:} nothing could be written within the specified time, see {\tt tty_init()} 
        and {\tt tty_set_timeout()} how to increase the timeout
  \item {\tt TTY_EXTERNAL_ERROR:} other errors, call {\tt tty_errmsg()}
  \item other error codes: see {\tt tty_lock()}, {\tt tty_unlock()} 
  \end{description}

  \return OKAY or FAIL
*/
extern DLL_INTERFACE int tty_puts (struct tty_device* tty, const char* str);

    /*@} end of basic functions */

    /** Lock Handling Functions */
    /*@{*/

/** lock device

  {\tt tty_lock()} locks the device for exclusive usage. It waits
  20 seconds to establish the device lock. If the device is locked by
  another process for more than 20 seconds {\tt tty_lock()} sets
  {\tt tty_error} to {\tt TTY_LOCK_TIMEOUT} and returns {\tt -1}.

  In case of an error {\tt tty_lock()} returns {\tt -1} and sets
  {\tt tty_error} to one of the following error codes:

  \begin{description}
  \item {\tt TTY_NULL_ARGUMENT:} {\tt tty} is a {\tt NULL}  pointer
  \item {\tt TTY_DEVICE_IS_LOCKED:} the device is already locked
  \item {\tt TTY_DEVICE_NOT_OPEN:} the tty device was not open (local devices on UNIX only)
  \item {\tt TTY_LOCK_TIMEOUT:} the device could not be locked
        within 20 seconds, call {\tt tty_errmsg()} for specific information.
  \item {\tt TTY_EXTERNAL_ERROR:} other errors, call {\tt tty_errmsg()}
  \end{description}

  \param tty            pointer to tty device object
  \return OKAY or FAIL
*/
extern DLL_INTERFACE int tty_lock (struct tty_device* tty);

/** lock device without timeout

  {\tt tty_lock_nodelay()} locks the device for exclusive usage,
  but it does not wait if the device is already locked by another process.
  This means that {\tt tty_lock_nodelay()} always returns immediately.

  In case of an error {\tt tty_lock_nodelay()} returns {\tt -1}
  and sets {\tt tty_error} to one of the following error codes:

  \begin{description}
  \item {\tt TTY_NULL_ARGUMENT:} {\tt tty} is a {\tt NULL} pointer
  \item {\tt TTY_DEVICE_IS_LOCKED:} the device is already locked
  \item {\tt TTY_DEVICE_NOT_OPEN:} the tty device was not open (local devices on UNIX only)
  \item {\tt TTY_EXTERNAL_ERROR:} other errors, call {\tt tty_errmsg()}
  \end{description}

  {\bf Interna:}

  Local devices on Unix are locked using {\tt tty->fd}. Remote devices
  and local devices on Windows-NT are locked using the lock file {\tt tty->lockname}.
  If this is not already open it is opened now, its file descriptor
  is stored in {\tt tty->lockfd} and a write lock is set.

  \param tty            pointer to tty device object
  \return OKAY or FAIL
*/
extern DLL_INTERFACE int tty_lock_nodelay (struct tty_device* tty);

/** unlock tty device

  {\tt tty_unlock()} removes the lock. 

  In case of an error {\tt tty_unlock()} returns {\tt -1} and
  sets {\tt tty_error} to one of the following error codes:

  \begin{description}
  \item {\tt TTY_NULL_ARGUMENT:} {\tt tty} is a {\tt NULL} pointer
  \item {\tt TTY_DEVICE_NOT_LOCKED:} the device is not locked (by our process)
  \item {\tt TTY_DEVICE_NOT_OPEN:} the tty device was not open (local devices on UNIX only)
  \item {\tt TTY_LOCK_TIMEOUT:} the device could not be unlocked within 20 seconds, 
        call {\tt tty_errmsg()} for specific information.
  \item {\tt TTY_EXTERNAL_ERROR:} other errors, call {\tt tty_errmsg()}
  \end{description}

  {\bf Interna:}

  {\tt tty_unlock()} removes the lock. For remote devices (and local
  devices on Windows-NT) the lockfile will not be closed (it stays open
  for performance reasons). This is done in {\tt tty_close()}.

  \param tty            pointer to tty device object
  \return OKAY or FAIL
*/
extern DLL_INTERFACE int tty_unlock (struct tty_device* tty);

/** return own lock state

  {\tt tty_is_locked()} returns {\tt 1} when the device is locked
  by this process and {\tt 0} when it is not locked by this process
  or on error.

  {\bf Note:} Many programmers do not read this carefully: {\tt tty_is_locked()}
  does {\bf not} state whether the device is locked by {\bf another}
  process! This is not possible because the only atomic operation on
  file or device locks is to set a lock. 

  In case of an error ({\tt tty} is a {\tt NULL} pointer) {\tt tty_is_locked()}
  returns {\tt 0.} It does {\bf not} set {\tt tty_error.} 

  \param tty    pointer to tty device object
  \return 0 or 1
*/
extern DLL_INTERFACE int tty_is_locked (const struct tty_device* tty);

    /*@} end of lock handling functions */

    /** Configuration Functions */
    /*@{*/

/** configure tty device

  {\tt tty_config()} configures the serial device by storing all
  configuration data in a file which is read by {\tt tty_open()}. 

  \begin{itemize}
  \item The parameter {\tt unit} should contain the name of the unit connected
        to the TTY device, e.g. {\em "HPPR"}. An empty string may be given, though.
  \item The parameter {\tt device} may be set to one of the following names:
        \begin{description}
        \item /dev/tty{\em xx}: for local devices on Unix and for remote devices
              on the CCU, e.g. {\em "/dev/tty11"} or {\em "/dev/ttyd1"}
        \item tty{\em xx}: for local devices on Unix and for remote devices
              on the CCU, e.g. {\em "tty11"}or {\em "ttyd1"}
        \item com{\em x}: for local devices on Windows-NT, e.g. {\em "com1"}
        \end{description}
  \item The parameter {\tt stty} contains the stty flags for the configuration
        of the TTY device. \see tty_stty_ref()
  \end{itemize}

  In case of an error {\tt tty_config()} returns {\tt -1} and
  sets {\tt tty_error} to one of the following error codes:

  \begin{description}
  \item {\tt TTY_NULL_ARGUMENT:} one or more of {\tt tty},
        {\tt unit} or {\tt stty} is a {\tt NULL} pointer
  \item {\tt TTY_DEVICE_NOT_OPEN:} the tty device was not open
  \item {\tt TTY_MKUDIR_ERROR:} error in {\tt mkudir(util)}
  \item {\tt TTY_EXTERNAL_ERROR:} other errors from external
        functions and system calls, call {\tt tty_errmsg()}
  \end{description}

  {\bf Interna:}

  {\tt tty_config()} configures the serial device as follows: 
  \begin{itemize}
  \item it creates the file {\tt {\em $XWINNMRHOME/conf/instr/<instrument>/<tty->confname>}}
        and stores the {\tt unit} name, the host name, the {\tt device}
	name and the {\tt stty} flags into it. Additional information in
	{\tt info} is stored as well, if it is available. If the device
	is remote, then the host name is set to the instrument name, otherwise
	it is empty.
  \item In case of a local device on UNIX {\em /etc/inittab} is modified,
        the access bits of the device are set and {\em init q} is executed
	to set the appropriate device from {\em respawn} to {\em off}. 
  \end{itemize}
  The new device name is then stored in {\tt tty->device}. If {\tt remote}
  is {\tt TRUE} then it is assumed that the device resides on a remote
  host, so the name of the instrument is read with {\tt getinstr(util)}
  and stored as that host's name in {\tt tty->host}.

  \param tty     pointer to tty device object
  \param unit    name of unit connected to tty device
  \param device  name of device 
  \param stty    stty commands string 
  \param remote  0 if local, != 0 if remote
  \param info    string containing other information (NULL if no info)
  \return OKAY or FAIL
*/
extern DLL_INTERFACE int tty_config(
    struct tty_device* tty, const char* unit, const char* device, const char* stty, int remote,
    const char* info);

extern DLL_INTERFACE int tty_configure(
    struct tty_device* tty, const char *, const char *, const char *, const char *,
    enum tty_device_type, const char *);

/** perform {\tt stty} on tty device

  {\tt tty_stty()} executes the specified {\tt stty} command
  string on the device. If the {\tt stty} command reads the device
  configuration (e.g. {\tt stty="-g"}) then the
  answer is stored back into {\tt stty}. If the device is not locked
  it will be locked for the time of the command execution. 

  \begin{itemize}
  \item On remote devices all those stty flags are possible which are supported
        by the CCU operating system, see table below:
        \begin{tabular}{|l|l|}
\hline 
{\bf Control characters} & {\tt line intr quit erase kill eof eol eol2 switch} \\
                         & {\tt start stop susp dsusp rprnt flush werase} \\
                         & {\tt lnext status} \\
\hline 
{\bf Flags (set)}        & {\tt cs6 cs7 cs8 parenb parodd cstopb hupcl cread} \\
                         & {\tt clocal loblk mdmbuf rtscts ignbrk brkint} \\
                         & {\tt ignpar parmrk inpck istrip inlcr igncr icrnl }\\
                         & {\tt iuclc ixon ixany ixoff isig icanon xcase echo}\\
			 & {\tt echoe echok echonl noflsh tostop opost olcuc }\\
			 & {\tt onlcr ocrnl onocr onlret ofill ofdel} \\
			 & {\tt saved-opost saved-isig saved-icanon saved-iuclc} \\
			 & {\tt saved-olcuc saved-icrnl saved-onlcr saved-xcase} \\
			 & {\tt saved-igncr saved-inlcr saved-brkint saved-raw} \\
			 & {\tt saved-cbreak saved-crmod saved-lcase} \\
\hline 
{\bf Flags (clear)}      & {\tt cs5 -parenb -parodd -cstopb -hupcl -cread -clocal} \\
                         & {\tt -loblk -mdmbuf -rtscts -ignbrk -brkint -ignpar} \\
			 & {\tt -parmrk -inpck -istrip -inlcr -igncr -icrnl -iuclc}\\
			 & {\tt -ixon -ixany -ixoff -isig -icanon -xcase -echo} \\
			 & {\tt -echoe -echok -echonl -noflsh -tostop -opost -olcuc} \\
			 & {\tt -onlcr -ocrnl -onocr -onlret -ofill -ofdel} \\
			 & {\tt -saved-opost -saved-isig -saved-icanon -saved-iuclc}\\
			 & {\tt -saved-olcuc -saved-icrnl -saved-onlcr -saved-xcase}\\
			 & {\tt -saved-igncr -saved-inlcr -saved-brkint -saved-raw} \\
			 & {\tt -saved-cbreak -saved-crmod -saved-lcase} \\
\hline
        \end{tabular}

  \item On local devices all POSIX.1 compatible flags and control characters
        (see chapter 8 of the {\em POSIX Programmers Guide}) are supported. Several
	none POSIX.1 compatible flags and control characters (see chapter 11 of 
	{\em Stevens: Advanced Programming in the UNIX Environment})
	are supported as well if the operating system supports them (see table below):
	\begin{tabular}{|c|c|c|c|c|}
\hline 
{\bf Flag (set)} & {\bf Flag (clear)} & {\bf IRIX} & {\bf NutC} & {\bf Win32} \\ \hline\hline 
{\tt imaxbel}    & {\tt -imaxbel}     & Y          &            &             \\ \hline 
{\tt iuclc}      & {\tt -iuclc}       & Y          &            &             \\ \hline 
{\tt ixany}      & {\tt -ixany}       & Y          &            &             \\ \hline 
{\tt ocrnl}      & {\tt -ocrnl}       & Y          &            &             \\ \hline 
{\tt onlcr}      & {\tt -onlcr}       & Y          &            &             \\ \hline 
{\tt cignore}    & {\tt -cignore}     &            &            &             \\ \hline 
{\tt mdmbuf}     & {\tt -mdmbuf}      &            &            &             \\ \hline 
{\tt echoctl}    & {\tt -echoctl}     & Y          &            &             \\ \hline 
{\tt echoke}     & {\tt -echoke}      & Y          &            &             \\ \hline 
{\tt echoprt}    & {\tt -echoprt}     & Y          &            &             \\ \hline 
{\tt flusho}     & {\tt -flusho}      & Y          &            &             \\ \hline 
{\tt pendin}     & {\tt -pendin}      & Y          &            &             \\ \hline 
{\tt xcase}      & {\tt -xcase}       & Y          &            &             \\ \hline
\hline 
{\bf Control Characters} & & {\bf IRIX} & {\bf NutC} & {\bf Win32} \\ \hline\hline 
{\tt discard}            & & Y          &            &             \\ \hline 
{\tt dsusp}              & & Y          &            &             \\ \hline 
{\tt eol2}               & & Y          &            &             \\ \hline 
{\tt lnext}              & & Y          &            &             \\ \hline 
{\tt reprint}            & & Y          &            &             \\ \hline 
{\tt status}             & &            &            &             \\ \hline 
{\tt werase}             & & Y          &            &             \\ \hline 
\end{tabular}

\end{itemize}

  In case of an error {\tt tty_stty()} returns {\tt -1} and sets
  {\tt tty_error.} Refer to {\tt tty_remote_stty()} and {\tt tty_local_stty()}
  for a list of error codes.

  {\bf Interna:}

  {\tt tty_stty()} performs the unix command {\em stty} on the
  specified device. It calls {\tt tty_remote_stty()} (remote device)
  or {\tt tty_local_stty()} (local device) to do the job.

  \param tty   pointer to tty device object
  \param cmdbuf  buffer containing stty command
  \param cmdbufsize  size of command buffer {\tt cmdbuf}
  \return OKAY or FAIL
*/
extern DLL_INTERFACE int tty_stty (struct tty_device* tty, char [], int);

/** perform stty() on local device

  {\tt tty_local_stty()} does the same like {\tt tty_stty()}
  but for local devices only. Therefore it should not be used; use {\tt tty_stty()}
  instead.

  In case of an error {\tt tty_local_stty()} returns {\tt -1}
  and sets {\tt tty_error} to one of the following error codes:

  \begin{description}
  \item {\tt TTY_NULL_ARGUMENT:} one or more of {\tt tty} or {\tt cmdbuf} is a {\tt NULL} pointer
  \item {\tt TTY_DEVICE_NOT_OPEN:} the tty device was not open
  \item {\tt TTY_EXTERNAL_ERROR:} other errors, call {\tt tty_errmsg()}
  \item other error codes: refer to {\tt tty_lock()}, {\tt tty_unlock()}
        and {\tt tty_posix_stty2termios()}
  \end{description}

  {\bf Interna:}

  {\tt tty_local_stty()} performs the unix command {\em stty}
  on the specified local device. It locks the device if necessary, uses
  OS-dependent API calls to set or get the {\em stty} paramaters and
  removes the device lock if it was not locked before.

  The following OS-dependent API calls are used:
  \begin{itemize}
  \item On Unix (IRIX, Linux) and Windows-NT/NutCracker: 
        \begin{itemize}
	\item {\tt tcgetattr(POSIX.1)} is used to read the {\tt struct termios} 
	\item {\tt tcsetattr(POSIX.1)} is used to set the {\tt struct termios} 
	\item {\tt tty_posix_stty2termios(lib_tty)} is used to convert the
	      {\em stty} command string into {\tt struct termios} 
	\item {\tt tty_posix_termios2stty(lib_tty)} is used for the other direction.
	\end{itemize}
  \item On native Windows-NT: {\em not yet implemented}.
  \end{itemize}
  For the availability of {\em stty} flags and control characters
  on local devices refer to the documentation of tty_stty().

  \param tty   pointer to tty device object
  \param cmdbuf  buffer containing stty command
  \param cmdbufsize  size of command buffer {\tt cmdbuf}
  \return OKAY or FAIL
*/
extern DLL_INTERFACE int tty_local_stty (struct tty_device* tty, char cmdbuf[], int cmdbufsize);

/** perform {\tt stty} on remote device

  {\tt tty_remote_stty()} does the same like {\tt tty_stty()}
  but for remote devices only. Therefore it should no longer be used
  and is available for historic reasons only; use {\tt tty_stty()}
  instead.

  In case of an error {\tt tty_remote_stty()} returns {\tt -1}
  and sets {\tt tty_error} to one of the following error codes:

  \begin{description}
  \item {\tt TTY_NULL_ARGUMENT:} one or more of {\tt tty} or {\tt byte} is a {\tt NULL} pointer
  \item {\tt TTY_DEVICE_NOT_OPEN:} the tty device is not  open
  \item {\tt TTY_TIMEOUT_ERROR:} {\tt remote_stty(librrs)} did not return within the specified time, 
        see {\tt } {\tt tty_init()} and {\tt tty_set_timeout()} how to increase the timeout
  \item {\tt TTY_EXTERNAL_ERROR:} other errors, call {\tt tty_errmsg()}
  \item other error codes: refer to {\tt tty_lock()}, {\tt tty_unlock()} 
  \end{description}

  {\bf Interna:}

  {\tt tty_remote_stty()} performs the unix command {\em stty}
  on the specified remote device. It locks the device if necessary,
  executes {\tt remote_stty(librrs)} and removes the device lock
  if it was not locked before. 

  \param tty   pointer to tty device object
  \param cmdbuf  buffer containing stty command
  \param cmdbufsize  size of command buffer {\tt cmdbuf}
  \return OKAY or FAIL
*/
extern DLL_INTERFACE int tty_remote_stty (struct tty_device* tty, char cmdbuf[], int cmdbufsize);

/** set timeout on tty device

  {\tt tty_set_timeout()} sets the new {\tt timeout} parameter
  as timeout for (nearly) all tty operations. 

  In case of an error {\tt tty_set_timeout()} returns {\tt -1}
  and sets {\tt tty_error} to one of the following error codes:

  \begin{description}
  \item {\tt TTY_NULL_ARGUMENT:} one or more of {\tt tty} or {\tt byte} is a {\tt NULL} pointer
  \item {\tt TTY_DEVICE_NOT_OPEN:} the tty device is not open
  \item {\tt TTY_TIMEOUT_TOO_SHORT:} parameter {\tt timeout} is too short ({\tt 0} or negative)
  \item other error codes: see {\tt tty_stty} 
  \end{description}

  {\bf Interna:}

  {\tt tty_set_timeout()} sets {\tt tty->timeout} to {\tt timeout}
  and calls {\tt tty_stty()} to set the stty parameter VTIME accordingly. 

  \param tty       pointer to tty device
  \param timeout   timeout value in seconds
  \return OKAY or FAIL
*/
extern DLL_INTERFACE int tty_set_timeout (struct tty_device* tty, unsigned int timeout);

/** set minimum number of characters to be read

  {\tt tty_set_vmin()} sets the stty parameter VMIN to the new
  value. If {\tt vmin} is bigger than {\tt 255} it is set to {\tt 255}.

  In case of an error {\tt tty_set_vmin()} returns {\tt -1}
  and sets {\tt tty_error} to one of the following error codes:

  \begin{description}
  \item {\tt TTY_NULL_ARGUMENT:} one or more of {\tt tty} or {\tt byte} is a {\tt NULL} pointer
  \item {\tt TTY_DEVICE_NOT_OPEN:} the tty device is not open
  \item other error codes: see {\tt tty_stty} 
  \end{description}

  {\bf Interna:}

  {\tt tty_set_vmin()} calls {\tt tty_stty()} to set the stty
  parameter VMIN to the new value. 

  \param tty       pointer to tty device
  \param vmin      minimum number of characters to read
  \return OKAY or FAIL
*/
extern DLL_INTERFACE int tty_set_vmin (struct tty_device* tty, unsigned int);

/** delete configuration file of tty object. 
    \return 0 if success
    \return -1 in case of failure, and set tty_error
 */
extern DLL_INTERFACE int tty_delConf(const struct tty_device* tty);

    /*@} end of configuration functions */

    /** RS485 specific Functions */
    /*@{*/

/** enable RS485 transmitter chip

  {\tt tty_rs485_enable()} enables the RS485 transmitter chip and
  puts its output from tristate into active state to transmit data.
  If the device is not yet locked it will be locked before and unlocked
  afterwards.

  In case of an error {\tt tty_rs485_enable()} returns {\tt -1}
  and sets {\tt tty_error} to one of the following error codes:

  \begin{description}
  \item {\tt TTY_NULL_ARGUMENT:} one or more of {\tt tty} or {\tt byte} is a {\tt NULL} pointer
  \item {\tt TTY_DEVICE_NOT_OPEN:} the tty device is not open
  \item {\tt TTY_REMOTE_TIMEOUT:} {\tt Rrs485Enable(librrs)} did not return within the specified time, 
        see {\tt tty_init()} and {\tt tty_set_timeout()} how to increase the timeout
  \item {\tt TTY_EXTERNAL_ERROR:} other errors, call {\tt tty_errmsg()}
  \item other error codes: refer to {\tt tty_lock()}, {\tt tty_unlock()} 
  \end{description}

  {\bf Interna:}

  {\tt tty_rs485_enable()} uses {\tt Rrs485Enable(librrs)} (remote
  device) or {\tt Lrs485Enable(librrs)} (on local device) to enable
  the RS485 transmitter chip and puts its output from tri\-state into
  active state to transmit data. If the device is remote and not yet
  locked it will be locked before and unlocked afterwards. If the device
  is not a RS485 then nothing is done and {\tt tty_rs485_enable()}
  returns successfully.

  \param tty   pointer to tty device
  \return OKAY or FAIL
*/
extern DLL_INTERFACE int tty_rs485_enable (struct tty_device* tty);

/** disable RS485 transmitter chip

  {\tt tty_rs485_disable()} disables the RS485 transmitter chip
  and puts its output from active state into tristate. If the device
  is not yet locked it will be locked before and unlocked afterwards.

  In case of an error {\tt tty_rs485_disable()} returns {\tt -1}
  and sets {\tt tty_error} to one of the following error codes:

  \begin{description}
  \item {\tt TTY_NULL_ARGUMENT:} one or more of {\tt tty} or {\tt byte} is a {\tt NULL} pointer
  \item {\tt TTY_DEVICE_NOT_OPEN:} the tty device is not  open
  \item {\tt TTY_REMOTE_TIMEOUT:} {\tt Rrs485Disable(librrs)} did not return within the specified time, 
  see {\tt tty_init()} and {\tt tty_set_timeout()} how to increase the timeout
  \item {\tt TTY_EXTERNAL_ERROR:} other errors, call {\tt tty_errmsg()}
  \item other error codes: refer to {\tt tty_lock()}, {\tt tty_unlock()} 
  \end{description}

  {\bf Interna:}

  {\tt tty_rs485_disable()} uses {\tt Rrs485Disable(librrs)}
  (remote device) or {\tt Lrs485Disable(librrs)} (on local device)
  to disables the RS485 transmitter chip and puts its output from active
  state into tristate. If the device is remote and not yet locked it
  will be locked before and unlocked afterwards. If the device is not
  a RS485 then nothing is done and {\tt tty_rs485_disable()} returns
  successfully.

  \param tty   pointer to tty device
  \return OKAY or FAIL
*/
extern DLL_INTERFACE int tty_rs485_disable (struct tty_device* tty);

/** send WUP signal to RS485 device

  {\tt tty_rs485_wakeup()} sets the WUP line on the RS485 for 25ms
  to low, then back again to high. If the device is not yet locked it
  will be locked during the operation.

  In case of an error {\tt tty_rs485_wakeup()} returns {\tt -1}
  and sets {\tt tty_error} to one of the following error codes:

  \begin{description}
  \item {\tt TTY_NULL_ARGUMENT:} one or more of {\tt tty} or {\tt byte} is a {\tt NULL} pointer
  \item {\tt TTY_DEVICE_NOT_OPEN:} the tty device is not open
  \item {\tt TTY_EXTERNAL_ERROR:} other errors, call {\tt tty_errmsg()}
  \item other error codes: refer to {\tt tty_lock()}, {\tt tty_unlock()} 
  \end{description}

  \param tty   pointer to tty device
  \return OKAY or FAIL
*/
extern DLL_INTERFACE int tty_rs485_wakeup (struct tty_device* tty);

    /*@} end of RS485 specific functions */

    /** Information Functions */
    /*@{*/

/** returns local device name

  {\tt tty_local_device()} returns a pointer to a string containing
  the name of the local device, e.g. a real hardware device like {\tt "}{\tt {\em /dev/tty03}}{\tt "}
  when using a local device or a pseudo device like {\tt "{\em /dev/pty06}"}.

  If the device is no tty device the string contains {\tt {\em "<is no tty device>"}}.

  In case of an error {\tt tty_local_device()} sets {\tt tty_error}
  to one of the following error codes:

  \begin{description}
  \item {\tt TTY_NULL_ARGUMENT:} {\tt tty} is a {\tt NULL} pointer, return value is {\tt {\em "<device not defined>"}}
  \item {\tt TTY_DEVICE_NOT_OPEN:} the device is not open, return value is {\tt {\em "<device not open>"}}
  \item {\tt TTY_EXTERNAL_ERROR:} {\tt ttyname(3C)} failed, return value is {\tt {\em "<is no tty device>"}}
  \item other error codes: see tty_name()
  \end{description}

  {\bf Interna: }
  If the device is local then the device name is taken from {\tt ttyname(tty->fd)},
  see {\tt ttyname(3C)}.

  \param tty   pointer to tty device
  \return pointer to string
*/
extern DLL_INTERFACE const char *tty_local_device (const struct tty_device* tty);

/** returns file descriptor of tty device

  {\tt tty_get_fd()} returns the file descriptor from the specified
  {\tt tty} device. This function is meant for applications which
  do their own communication (not using {\tt tty_read()} or {\tt tty_write()})
  and use only the basic {\em lib_tty} functions for open/close and
  lock/unlock.

  In case of an error ({\tt tty} is a {\tt NULL} pointer or the
  device is not open) {\tt tty_get_fd()} returns {\tt -1}.

  {\bf Notes:}
  \begin{itemize}
  \item {\tt tty_get_fd()} does not set {\tt tty_error}.
  \end{itemize}

  {\bf Interna:}
  \begin{itemize}
  \item {\tt tty_get_fd()} returns {\tt tty->fd}.
  \end{itemize}

  \return file descriptor of tty device
  \return -1 in case of error
*/
extern DLL_INTERFACE int tty_get_fd (const struct tty_device* tty);

extern DLL_INTERFACE int tty_is_remote (const struct tty_device* tty);

/** returns full device name

  {\tt tty_name()} returns a pointer to a string containing the
  full name of the device, e.g. "{\em /dev/tty03}"
  when using a local device or "{\em amx300:/dev/tty11}"
  for a remote device. {\tt tty_name()} can be called both for open
  and closed {\tt tty} devices.

  {\bf Note:} {\tt tty_name()} returns a pointer to a local static
  character array. This means that any following call of {\tt tty_name()}
  overwrites the contents of the character array. 

  In case of an error {\tt tty_name()} sets {\tt tty_error}
  to one of the following error codes:

  \begin{description}
  \item {\tt TTY_NULL_ARGUMENT:} {\tt tty} is a {\tt NULL} pointer, return value is {\tt {\em "<device not defined>"}}
  \item {\tt TTY_INSTR_NOT_CONFIGURED:} the spectrometer is not configured, return value 
        is {\tt {\em "<device not configured>"}}
  \item {\tt TTY_EXTERNAL_ERROR:} other error, call {\tt tty_errmsg()}
  \end{description}

  {\bf Interna:}

  If the device is open, the returned string is composed from {\tt tty->host}
  and {\tt tty->device}. If the device is closed, the configuration
  file is read by calling {\tt tty_getConf()} to get host name and
  device name. {\tt tty_name()} returns a pointer to a local static
  string array of size {\tt 128}. 

  \param tty    pointer to tty device object
  \return pointer to string
*/
extern DLL_INTERFACE const char *tty_name (const struct tty_device* tty);

/** convert real device name suitable for BEST user

  {\tt tty_getUserDeviceName()} converts the real device name as
  returned by {\tt tty_name()} into a device name suitable for BEST
  users. Names of local devices always convert into {\tt {\em com<x>}},
  names of remote device always to {\tt {\em tty<xx>}}, and names
  of pseudo devices are not changed on platforms which support pseudo
  tty. See the following examples:

  \begin{tabular}{llll} \hline 
  Device        & {\tt devnam}           & {\tt usernam} & Comment\\ \hline\hline 
  remote CCU    & {\em spect:/dev/tty05} & {\em tty05}   & the host name ({\em spect:}) may be omitted \\ \hline 
  local Windows & {\em COM1}             & {\em com1}    & \\ \hline 
  local Linux   & {\em /dev/ttyS0}       & {\em com1}    & \\ \hline 
  local Irix    & {\em /dev/ttyd1}       & {\em com1}    & \\ \hline 
  local Irix    & {\em /dev/ttyf1}       & {\em com1}    & \\ \hline 
  pseudo Irix   & {\em /dev/ttyq2}       & {\em ttyq2}   & on IRIX only, fails on other platforms \\ \hline 
  pseudo Linux  & {\em /dev/pts/4}       & {\em pts/4}   & on LINUX only, fails on other platforms \\ \hline
  \end{tabular}

  In case of an error {\tt tty_getUserDeviceName()} returns {\tt -1}
  and sets {\tt tty_error} to one of the following error codes:

  \begin{description}
  \item {\tt TTY_NULL_ARGUMENT:} {\tt devnam} and/or {\tt usernam} is a {\tt NULL} pointer
  \item {\tt TTY_BAD_ARGUMENT:} {\tt devnam} contains an invalid device name
  \end{description}

  \param devnam   pointer to device name as returned by tty_name()
  \param usernam  pointer to store the suitable user device name
  \return OKAY or FAIL
*/
extern DLL_INTERFACE int tty_getUserDeviceName (const char* devnam, char* usernam);

/** returns port number (remote devices only)

  {\tt tty_port()} sets {\tt port} to the port number of the
  port used by the {\tt tty} device. If the device is local, {\tt port}
  is set to {\tt -1}. {\tt tty_port()} can be called both for
  open and closed {\tt tty} devices.

  In case of an error {\tt tty_port()} returns {\tt -1} and sets
  {\tt tty_error} to one of the following error codes:

  \begin{description}
  \item {\tt TTY_NULL_ARGUMENT:} {\tt tty} is a {\tt NULL}  pointer
  \item {\tt TTY_INSTR_NOT_CONFIGURED:} the spectrometer is not configured
  \item {\tt TTY_DEV_NOT_CONFIGURED:} the device is not configured
  \item {\tt TTY_BAD_CONFIG_FILE:} the configuration file contains corrupt data
  \item {\tt TTY_EXTERNAL_ERROR:} other errors, call {\tt tty_errmsg()}
  \end{description}

  {\bf Interna:}

  {\tt tty_port()} calls {\tt RRSGetPort(librrs)} and sets {\tt port}
  to the port number of the port used by the {\tt tty} device. If
  the device is local, {\tt port} is set to {\tt -1}. If the device
  is not open then {\tt getinstr(util)} and {\tt tty_getConf()}
  are called to retrieve the necessary information.

  \param tty   pointer to tty device object
  \param port  pointer to port number
  \return OKAY or FAIL
*/
extern DLL_INTERFACE int tty_port (struct tty_device* tty, int* port);

    /*@} end of information functions */


#ifdef	HAS_NUTC_FEATURES
extern DLL_INTERFACE int tty_get_handle (const struct tty_device* tty);
#endif
extern DLL_INTERFACE int tty_flush (struct tty_device* tty, int);
extern DLL_INTERFACE int tty_waitbyte (const struct tty_device* tty);

/*@}*/

/** Error Handling Functions */
/*@{*/

/** returns error message

  {\tt tty_errmsg()} returns a pointer to a string containing the
  error message specified by the global parameter {\tt tty_error}
  which is always set by all {\em tty} functions.

  {\bf Notes:}
  \begin{itemize}
  \item {\tt tty_errmsg()} does not set {\tt tty_error}.
  \end{itemize}

  \return pointer to string
*/
extern DLL_INTERFACE const char *tty_errmsg (void);

/** return last error code */
extern DLL_INTERFACE enum tty_errcode tty_errcode (void);

/** returns help text

  {\tt tty_helptxt} returns a pointer to a string containing a help
  message specified by the global parameter {\tt tty_error} which
  is always set by all {\em tty} functions. When no help message is
  available {\tt NULL} is returned.

  {\bf Notes:}
  \begin{itemize}
  \item {\tt tty_helptxt()} does not set {\tt tty_error}.
  \end{itemize}

  \return pointer to string containing help text
*/
extern DLL_INTERFACE const char *tty_helptxt (void);

/*@} end of error handling functions */

/** Debugging Functions */
/*@{*/

/** print contents of tty object into string}

  {\tt tty_debug()} returns a pointer to a character string describing
  the content of all tty struct components in readable form. This function
  is written for debugging purposes only.

  {\bf Notes:}
  \begin{itemize}
  \item {\tt tty_debug()} returns a pointer to a local static string array. 
        This means that any following call of {\tt tty_debug()} overwrites the 
        contents of the string array. 
  \item {\tt tty_debug()} does not set {\tt tty_error}.
  \end{itemize}

  \param tty   pointer to tty device
  \return pointer to string
*/
extern DLL_INTERFACE const char *tty_debug (const struct tty_device* tty);

/** print contents of tty object into string

  {\tt tty_sprintf()} prints the content of all tty struct components
  into {\tt str} and returns the number of written characters (like
  {\tt sprintf()}). This function is mainly written for debugging
  purposes.

  In case of an error {\tt tty_sprintf()} returns {\tt -1} and
  sets {\tt tty_error} to one of the following error codes:

  \begin{description}
  \item {\tt TTY_NULL_ARGUMENT:} one or more of {\tt tty} or {\tt byte} is a {\tt NULL} pointer
  \end{description}

  \param tty   pointer to tty device
  \param name  name of tty object
  \param str   string buffer for name of tty object
  \return OKAY or FAIL
*/
extern DLL_INTERFACE int tty_sprintf (const struct tty_device* tty, const char* name,
				      char * str);

/** print contents of tty object into file

  {\tt tty_fprintf()} prints the content of all {\tt tty} struct
  components into the file pointer {\tt fp}. If {\tt fp} is {\tt NULL}
  the content is printed to {\em stdout}. This function is written
  for debugging purposes only.

  {\bf Notes:}
  \begin{itemize}
  \item {\tt tty_fprintf()} does not set {\tt tty_error}.
  \end{itemize}

  {\bf Interna:}
  \begin{itemize}
  \item {\tt tty_fprintf()} calls {\tt tty_sprintf()} using a local character array of 1024 bytes. 
  \end{itemize}

  \param tty   pointer to tty device
  \param name  name of tty object
  \param fp    file pointer for printing
*/
extern DLL_INTERFACE void tty_fprintf (const struct tty_device* tty, const char* name, FILE* fp);

/*@} end of debugging functions */

/** Low Level Functions */
/*@{*/

/** conversion function

  {\tt tty_posix_stty2termios()} converts the POSIX.1 compatible
  flags (see chapter 8 of the {\em POXIS programmers guide}) in {\tt stty}
  into data in {\tt struct termios}. Several none POSIX.1 compatible
  stty flags and control characters are converted as well if they are
  supported by the operating system (see table \ref{none-posix-flags}
  on page \pageref{none-posix-flags}).

  In case of an error {\tt tty_posix_stty2termios()} returns {\tt -1}
  and sets {\tt tty_error} to one of the following error codes:

  \begin{description}
  \item {\tt TTY_NULL_ARGUMENT:} {\tt tty} is a {\tt NULL} pointer
  \end{description}

  \param str   pointer to string
  \param tio   pointer to termios data structure
  \return OKAY or FAIL
*/
extern DLL_INTERFACE int tty_posix_stty2termios (const char* str, struct termios* tio);

/** conversion function

  {\tt tty_posix_termios2stty()} converts the POSIX.1 compatible
  flags (see chapter 8 of the {\em POSIX Programmers Guide}) in {\tt struct}
  {\tt termios} into a string which can be used as command line for
  {\tt stty}. Several none POSIX.1 compatible stty flags and control
  characters are converted as well if they are supported by the operating
  system (see table \ref{none-posix-flags} on page \pageref{none-posix-flags}).

  In case of an error {\tt tty_posix_termios2stty()} returns {\tt -1}
  and sets {\tt tty_error} to one of the following error codes:

  \begin{description}
  \item {\tt TTY_NULL_ARGUMENT:} {\tt tty} is a {\tt NULL} pointer
  \end{description}

  \param tio   pointer to termios data structure
  \param mode  -1, 0, or +1
  \param str   pointer to string buffer
  \param size  size of string buffer {\tt str}
  \return OKAY or FAIL
*/
extern DLL_INTERFACE int tty_posix_termios2stty (const struct termios* tio, int mode,
						 char* str, unsigned int size);

/** clear POSIX.1 flags

  {\tt tty_posix_clear_termios()} clears all POSIX.1 compatible
  flags and control characters (see \cite[chapter 8]{book.posix-prog-guide})
  in {\tt struct termios}. 

  In case of an error {\tt tty_posix_clear_termios()} returns
  {\tt -1} and sets {\tt tty_error} to one of the following error
  codes:

  \begin{description}
  \item {\tt TTY_NULL_ARGUMENT:} {\tt terms} is a {\tt NULL} pointer
  \end{description}

  \param terms   pointer to termios data structure
  \return OKAY or FAIL
*/
extern DLL_INTERFACE int tty_posix_clear_termios (struct termios* terms);

/*@} end of low level functions */



#ifdef __cplusplus
}	/* end of extern "C" { */
#endif

#endif /* LIB_TTY_H */
/***********************************************************************/
/*			     E N D   O F   F I L E                     */
/***********************************************************************/
