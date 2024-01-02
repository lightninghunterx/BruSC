//===============================================================
//
// $Source$ 
//    ParaVision general exception, replacement of c-exceptions
//
// Copyright (c) 2006 - 2016
//
// Bruker BioSpin MRI GmbH
// D-76275 Ettlingen, Germany
//
// All Rights Reserved
//
// $Id$
//
//===============================================================

#ifndef PVUTIL_PVEXCEPT_HH
#define PVUTIL_PVEXCEPT_HH

//---------------------------------------------------------------
// includes
//---------------------------------------------------------------

#ifndef CPROTO
#include <exception>
#include <utility>
#include <string>
#include <cstdarg>
#ifdef HAS_UNIX_FEATURES
#include <csignal>
#endif
#include <deque>
#include <iosfwd>

#include "BacktraceElement.hh"
#include "PvUtil/DLLMacro.h"



/** 
 * @file
 * @addtogroup PvUtil
 * @{
 */


/// Maximum length of an exception message (including a 0 character).
#define PVEXCEPT_MAX_LEN 4096


//---------------------------------------------------------------
// interface of class PvException
//---------------------------------------------------------------

/** @class PvException PvExcept.hh "PvUtil/PvExcept.hh"
    Base class for ParaVision exception. 

    @since ParaVision 6.Alpha.0.88
 */

class DLL_INTERFACE PvException : public std::exception
{
public:

    /// Destructor.
    virtual ~PvException() throw();

    /// Returns exception code
    virtual const char * code() const throw() = 0;

    /** Test if a given string is equal to the exception code.
     *  @param codeStr code to test with exception code.
     *  @return true it tested codes are identical.
     */
    virtual bool compareCode(const char * codeStr) const throw() = 0;


    /** Get the backtrace as a list of stack elements.
        @return The backtrace as list.
    */  
    const std::deque<BacktraceElement> & getStacktrace() const throw();

    /** Print backtrace in human readable format to a stream.
        @param[in] out The stream to which the backtrace is printed.
    */
    void printStacktrace(std::ostream & out) const throw();

    /// Indicates if this exception contains a stacktrace.
    bool hasStacktrace() const throw();

    /** Set the stacktrace in the exception.
        @param[in] stacktrace stacktrace to set.
    */ 
    void setStacktrace(const std::deque<BacktraceElement> & stacktrace) throw();

protected:

    /**
     * Empty constructor.
     * @param incbt Indicates if a stacktrace should be added to the exception.
     */
    explicit PvException(bool incbt);

    /**
     * Copy constructor.
     * @param e Source exception.
     */
    explicit PvException(const PvException & e);
    
private:
    /// Deleted private constructor.
    PvException() = delete;

    /// Deleted assignment operator.
    PvException & operator=(const PvException & e) = delete;

    /// List of stack traces.
    std::deque<BacktraceElement> * stacktrace_;
};

/**
 * @class PvBaseException PvExcept.hh "PvUtil/PvExcept.hh"
 *   A general ParaVision exception base from which other exceptions can be 
 *   derived.
 *  This exception stores an exception indicator (<code>code</code>) and
 *  an exception message (<code>message</code>).
 *
 *  @since ParaVision 7.Alpha.0.2
 */
class DLL_INTERFACE PvBaseException : public PvException
{
public:
    /// Returns exception message
    virtual const char * what() const throw() { return  message_; } 

    /// Returns exception code
    virtual const char * code() const throw() { return code_; }

    /** Test if a given string is equal to the exception code.
     *  @param codeStr code to test with exception code.
     *  @return true it tested codes are identical.
     */
    virtual bool compareCode(const char * codeStr) const throw();
    
protected:
    /**
     * Constructor.
     * @param includeStacktrace Indicates if program stack trace is included.
     */
    explicit PvBaseException(bool includeStacktrace);

    /**
     * Constructor.
     * @param includeStacktrace Indicates if program stack trace is included.
     * @param args Pair of error code and va_list containing the message.
     */
    PvBaseException(bool includeStacktrace,
        std::pair<const char *, va_list* > & args);

    /**
     * Constructor.
     * @param includeStacktrace Indicates if program stack trace is included.
     * @param code The exception code.
     * @param msg The exception message.
     */
    PvBaseException(bool includeStacktrace, const char * code, const char * msg);
    
    /// Copy Constructor.
    PvBaseException(const PvBaseException & ex);
    
    /// Destructor
    virtual ~PvBaseException();
    
    
    /** Set code and message. To be used from constructors of
     *  inheriting classes.
     *  @param code code a printf format string.
     *  @param ap   variable argument list
     */
    void vSetMessage(const char * code, va_list ap) throw();
    
    /**
     * Set code and message of exception.
     * @param code The code of the exection,  a \c printf format string.
     * @param ... The arguments of the \c printf format string.
     */
    void setMessage(const char * code, ...) throw()
#if defined(__GNUC__) && __GNUC__ >= 3
    __attribute__ ((format (printf, 2, 3)))
#endif
;

    
private:
    
    /// Deleted assignment operator.
    PvBaseException & operator=(const PvBaseException &) = delete;
    
    /// Exception message for the user.
    char message_[PVEXCEPT_MAX_LEN];

    /// Exception code.
    const char * code_;
};

/** 
 * @class PvStdException PvExcept.hh "PvUtil/PvExcept.hh"
 * A general ParaVision exception class.
 *
 * This exception stores an exception indicator (<code>code</code>) and
 * an exception message (<code>message</code>).
 * In addition to the exception information also stores the stacktrace
 * (if available).
 *
 *  @since ParaVision 5.Alpha.0.63
 */
class DLL_INTERFACE PvStdException : public PvBaseException
{
public:
    /** 
     * Constructor of ParaVision exception.
     * @param[in] exception string.
    */
    PvStdException(const std::string& code);

    /** 
     * Constructor of ParaVision exception.
     * @param[in] code exception code a printf format string.
     * @param[in] ... arguments of printf format string.
    */
    PvStdException(const char * code, ...) 
#ifdef	__GNUC__
#if	__GNUC__ >= 3
    __attribute__ ((format (printf, 2, 3)))
#endif
#endif
    ;

    /** 
     * Constructor of ParaVision exception.
     * @param args Pair of printf format and arguments. This is a pair
     * since the standard method using a const char * and a va_list
     * argument leads to an inconsistent overloading with the ... 
     * constructor.
     */
    explicit PvStdException(std::pair<const char *,va_list* > & args);

    /// Copy Constructor
    PvStdException(const PvStdException & e);

    /// Destructor
    virtual ~PvStdException() throw();

private:

    /// Deleted default constructor.
    PvStdException() = delete;
    
    /// Deleted assignment operator.
    PvStdException & operator=(const PvStdException &) = delete;  

};

/** 
 * @class PvStdNoStacktraceException PvExcept.hh "PvUtil/PvExcept.hh"
 * A general ParaVision exception meant as replacement for C-Exception.
 *
 * This exception stores an exception indicator (<code>code</code>) and
 * an exception message (<code>message</code>).
 *
 *  @since ParaVision 7.Alpha.0.2
 */
class DLL_INTERFACE PvStdNoStacktraceException : public PvBaseException
{
public:

    /** 
     * Constructor of ParaVision exception.
     * @param[in] code exception code a printf format string.
     * @param[in] ... arguments of printf format string.
    */
    PvStdNoStacktraceException(const char * code, ...) 
#ifdef	__GNUC__
#if	__GNUC__ >= 3
    __attribute__ ((format (printf, 2, 3)))
#endif
#endif
    ;

    /** 
     * Constructor of ParaVision exception.
     * @param args Pair of printf format and arguments. This is a pair
     * since the standard method using a const char * and a va_list
     * argument leads to an inconsistent overloading with the ... 
     * constructor.
     */
    explicit PvStdNoStacktraceException(
        std::pair<const char *, va_list* > & args);

    /// Copy Constructor
    explicit PvStdNoStacktraceException(const PvStdException & e);

    /// Destructor
    virtual ~PvStdNoStacktraceException() throw();

private:

    /// Deleted default constructor.
    PvStdNoStacktraceException() = delete;

    /// Deleted assignment operator.
    PvStdNoStacktraceException & operator=(const PvStdNoStacktraceException &) = delete;  

};


#ifdef HAS_UNIX_FEATURES

/** Throw an PvException in a signal handler.
    The exception contains the stacktrace from the signal creating
    code.
    @param[in] sig The signal number.
    @param[in] info The signal information struct.
    @param[in] secret Information about the context where the signal was created.
 */
DLL_INTERFACE void PvExceptThrowInSignalHandler(int sig, 
                                                siginfo_t *info, void *secret); 

#endif


//---------------------------------------------------------------
// interface of non-member functions / methods
//---------------------------------------------------------------

#endif

/** @} */



#endif

