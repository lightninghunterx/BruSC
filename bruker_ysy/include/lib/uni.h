/*
 *******************************************************************
 *
 * $Source: /pr/CvsTree/pr/gen/h/lib/uni.h,v $
 *
 * Copyright (c) 1995
 * BRUKER BIOSPIN GMBH
 * D-76287 Rheinstetten, Germany
 *
 * All Rights Reserved
 *
 * $Id: uni.h,v 1.73 2020/01/24 04:21:49 wem Exp $
 *
 *******************************************************************
*/


#ifndef LIB_UNI_H		/* prevent multiple includes	     */
#define LIB_UNI_H

#ifdef	HAS_UNIX_FEATURES
#include <sys/types.h>
#endif

#ifdef	HAS_WINDOWS_FEATURES
#include <unistd.h>
#endif
		 /* The error codes for CPR and all UNIMAR processes */
#define RETRY_COMMAND	-5
#define STOP_CHANGER	-4
#define LAST_SAMPLE	-3
#define SKIP_SAMPLE	-2
#define SKIP_EXP	-1
#define COMMAND_DONE	 0
#define	NORM_TERM	COMMAND_DONE	       /* normal termination */
#define ERR_TERM	SKIP_EXP		/* error termination */

		   /* common internal errors - transfered via exit() */
#define SYNTAX_ERR	 1	    /* cpr command line syntax error */
#define MSGQ_ERR	 2	    /* message queue operation error */
/* exit value 3 indicates abort() call on Windows		     */
#define BUSY_ERR	 4	    /* session with same id detected */
					      /* CPR internal errors */
#define	CMDTAB_ERR	 5		    /* command table missing */
#define SYST_PR_ERR	 6     /* system component cannot be started */
#define CURDAT_ERR	 7	/* current data set processing error */
#define	HIST_ERR	 8	       /* error in history mechanism */
#define MEM_ERR		10	   /* no memory for malloc available */
#define GRAPH_ERR	12		  /* graphic system panicked */
#define LICENSE_ERR	14		  /* license not got or lost */

	       /* Command administration "errors" - set in CPR_errno */
#define CPR_ERROR	20			  /* undefined error */
#define	CPR_REFUSED	21	   /* cmdtable full or NQUEUE is set */
#define CPR_SYNTAX	22	   /* syntax error in command string */
#define CPR_UNKNOWN	23		/* unknown command or module */
#define CPR_START_ERR	24	  /* error on start of exec. process */
#define CPR_KILLED	25    /* exec. process is dead and/or killed */
		   /*  Command execution "errors" - set in CPR_errno */
#define SIGTERM_ERR	27			/* terminated by cpr */
#define	QUIT_ERR	28		      /* illegal termination */

		      /* test, if there was an UNIMAR internal error */
#define UNIMAR_ERR(stat)	(stat == QUIT_ERR)

				 /* internal administrative "errors" */
#define	ADM_DONE	30    /* administration done after a command */
#define	EXIT_REQ	31		    /* external exit request */

					  /* Special commands follow */
#define	TERM_COMMAND	"$"		/* terminate process command */

		   /* The possible program status definitions follow */
#define	RUN_DEAD	-1		/* process cannot be started */
#define	RUN_UNDEF	 0			   /* Status unknown */
#define	RUN_WAITCOM	 1	     /* Waiting for command from CPR */
#define	RUN_WAITRES	 2	      /* Waiting for resume from CPR */
#define	RUN_EXEC	 3		   /* Executing user command */
#define	RUN_INTRPT	 4		     /* processing interrupt */
#define	RUN_WAITERM	 5     /* Waiting for term. of an issued cmd */

	 /* the wait parameter values of the CPR_exec routine follow */
#define	CONT_EX		 0		      /* execution continues */
#define	WAIT_TERM	 1		     /* wait for termination */
#define	WAIT_OBJ	 2    /* wait for term. and curdat file copy */
#define	WAIT_NQUEUE	 3	 /* wait for termination, but do not */
				 /* queue it, if object is occupied  */
#define	CONT_STAT	 4	 /* execution continues and the exit */
				 /* status is reported to issuer     */
#define	WAIT_START	 5	    /* wait for start of the command */
#define CONT_NOACK	 6 /* continue without waiting for acknowl.  */

		 /* test, if resume is required after action is done */
#define RESUME_REQ(flag) ((flag == WAIT_TERM)   ||  \
			  (flag == WAIT_OBJ)    ||  \
			  (flag == WAIT_NQUEUE) ||  \
			  (flag == WAIT_START)    )

#define RESUME_ADM(flag) ((flag == CONT_EX) ||  \
			  (flag == CONT_STAT) )

#define ACCESS_TOKEN_SEPARATOR ','

/*********************************************************************/
/*								     */
/* changes in Show_meta() arguments must be mirrored in java sources:*/
/*								     */
/* pr/gen/java/src/de/bruker/nmr/prsc/cpr/CprConstants.java	     */
/*								     */
/*********************************************************************/
					/* Show_meta() arguments     */
#define SM_RAW		   1		/* update raw data	     */
#define SM_RAWP		   2		/* update acquisition params */
#define SM_PROC		   4		/* update processed data     */
#define SM_PROCP	   8		/* update processing params  */
#define SM_ALL		 0xf		/* update data and params    */
#define SM_SHOWR	0x10		/* switch to raw data	     */
#define SM_SHOWP	0x20		/* switch to processed data  */
#define SM_DEL		0x40		/* removed data		     */
#define SM_PEAK	       0x100		/* update peaks		     */
#define SM_INT	       0x200		/* update integrals	     */
#define SM_ASED	       0x400		/* update ased		     */


		    /* the type of the user defined wrapup procedure */
typedef void (*WRUPROC)(void);


#ifdef __cplusplus
extern "C" {
#endif

#ifdef DLL_INTERFACE
#   undef DLL_INTERFACE
#endif

#if defined(DLL_LINKAGE) && !defined(CPROTO)
#   ifdef uni_INTERNAL
#      define DLL_INTERFACE __declspec(dllexport)
#   else
#      define DLL_INTERFACE __declspec(dllimport)
#   endif
#else
#   define DLL_INTERFACE
#endif

struct msgq_buffer;
struct MsgBufferStruct;
/*** function prototypes ***/

/* unimar.c	*/
extern DLL_INTERFACE void Start_unimar (int, char **, void (*)(void));
extern DLL_INTERFACE void Exit_unimar (int);
extern DLL_INTERFACE void Leave_unimar (int);
extern DLL_INTERFACE int CPR_exec (const char *, int);
extern DLL_INTERFACE const char * GetCPR_object (void);
extern DLL_INTERFACE void Get_command (int *, char ***);
extern DLL_INTERFACE void Show_meta (int);
extern DLL_INTERFACE void Change_object (void);
#if	defined(HAS_UNIX_FEATURES) || defined(HAS_WINDOWS_FEATURES)
extern DLL_INTERFACE int WakeupUextrChannel(pid_t);
#endif
extern DLL_INTERFACE int UniSetExitAtFatal(int);
extern DLL_INTERFACE unsigned int WaitForUextrMessage(unsigned int);
extern DLL_INTERFACE unsigned int ClearUextrMessages(void);
#ifdef	HAS_UNIX_FEATURES
extern DLL_INTERFACE unsigned int ReadUextrMessage(pid_t);
extern DLL_INTERFACE void PrintMsgQueueId(char*, size_t);
#endif
extern DLL_INTERFACE void (*Setint (void (*)(int, char **))) (int, char **);
extern DLL_INTERFACE void (*SetProcErrHandler (void (*)(const char *)))
						       (const char *);
extern DLL_INTERFACE void (*
#ifndef	BRUKER_SCA
#ifdef	__GNUC__
				__attribute__ ((format (printf, 1, 2)))
#endif	/* __GNUC__ */
#endif	/* ! BRUKER_SCA	*/
			    SetErrorPrintf (void (*)(const char *, ...)
#ifdef	__GNUC__
				__attribute__ ((format (printf, 1, 2)))
#endif	/* __GNUC__ */
									))
						    (const char *, ...);
extern DLL_INTERFACE const char * Get_accesstoken(int);
extern DLL_INTERFACE const char * Gets (const char *, const char *);
extern DLL_INTERFACE void Set_stat (int);
extern DLL_INTERFACE void Perror (int, const char *);
extern DLL_INTERFACE void Show_status (const char *);
extern DLL_INTERFACE void Show_status_all (const char *);
extern DLL_INTERFACE void (*UniWrupProcSet (void (*)(void)))(void);
extern DLL_INTERFACE short Proc_err (int, const char *, ...)
#ifdef	__GNUC__
			   __attribute__ ((format (printf, 2, 3)))
#endif
								  ;
/* message.c	*/
extern DLL_INTERFACE int setup_communication(int);
extern DLL_INTERFACE int send_message(int, const struct MsgBufferStruct *, int);
extern DLL_INTERFACE int receive_message(struct msgq_buffer *, int, int);
extern DLL_INTERFACE int check_MsgQueue_length(unsigned int);
extern DLL_INTERFACE int check_MsgQueueChannel_length(int);
#ifdef	HAS_WINDOWS_FEATURES
extern DLL_INTERFACE int MessageReleaseSemaphore(pid_t);
extern DLL_INTERFACE int send_message_handle(int, const struct MsgBufferStruct*,
					     void*, void*, void*);
extern DLL_INTERFACE void MsgClientName(char*, int);
extern DLL_INTERFACE void MutexClientName(char*, int);
#endif
extern DLL_INTERFACE void cleanup_communication(void);
extern DLL_INTERFACE void cleanup_communication_children(void);

#ifdef __cplusplus
}
#endif
#endif /* LIB_UNI_H */
/***********************************************************************/
/*			     E N D   O F   F I L E		       */
/***********************************************************************/
