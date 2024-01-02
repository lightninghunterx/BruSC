/****************************************************************
 *
 * Copyright (c) 2012 - 2018
 * Bruker BioSpin MRI GmbH
 * D-76275 Ettlingen, Germany
 *
 * All Rights Reserved
 *
 * $Id$
 *
 ****************************************************************/


#ifndef OVL_TOOLBOX_STRINGBUFFER_H
#define	OVL_TOOLBOX_STRINGBUFFER_H

#ifndef CPROTO

#include <string>
#include <vector>
#include <utility>
#include <sstream>
#include <fstream>
#include "common/ovl_toolbox/DLLMacro.h"

/*==============================================================*/
/** @file
 * @defgroup PvOvlPsStringBuffer
 * Functions to access the PsStringBuffer object of a
 * parameter space.
 *
 * Functions in this group provide the means to
 * access a string buffer, which is attached to the
 * parameter space that is open at the time of calling
 * functions in this group. The main purpose is to store information,
 * during the runtime of a e.g. method or a reconstruction filter,
 * which one wants to store in a file with the dataset,
 * like for example debug information.
 *
 * To fill the string buffer, one only needs to call
 * @c PvOvlPsStringBufferAppend. By default this will create a
 * file named ".strbuf" in the procno path of the
 * parameter space. The routine that writes the
 * buffer content to disc is called in the destructor, so no
 * explicit write function needs to be called by default.
 */
/*@{*/


/** Initializes the string buffer of the currently active
  parameter space.

  Currently the function only changes the
  output file name from the default file name (".strbuf") to the
  specified file name.

  Note: The place in the code where this function is called is
  crucial to have an effect. For example, calling this function
  in the backbone() of a method will not have the desired effect
  when running a duplicated image reconstruction, since then
  backbone() is never really called.

  @PVM_PARS
  NONE

  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  NONE

  @PVM_ERROR
  NONE

  @PVM_EXAMPLE
  @code
  std::string file_name = "my_output_file";
  PvOvlPsStringBufferInit(file_name.c_str());
  @endcode

  @param[in] file_name
  Name to store content of string buffer in.

  @ingroup PVM_UT
*/
DLL_INTERFACE void PvOvlPsStringBufferInit(const char * file_name);


/** Appends the given formatted string to the buffer of the
  currently active parameter space.

  Note: The interface of this function is identical to functions like
  sprintf.

  @PVM_PARS
  NONE

  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  NONE

  @PVM_ERROR
  NONE

  @PVM_EXAMPLE
  @code
  int test_int = 5;
  double test_double = 3.141;
  PvOvlPsStringBufferAppend("test_int = %d\n", test_int);
  PvOvlPsStringBufferAppend("test_double = %.2f\n", test_double);
  @endcode

  @param[in] fmt
  A format string as e.g. in the C-standard function sprintf()

  @ingroup PVM_UT
*/
DLL_INTERFACE void PvOvlPsStringBufferAppend(const char * fmt, ...);


/** Explicitly write the content of the string buffer to the
 * given file.

  Typically, calling this function is not required, since by default the
  write routine is always called in the destructor of the PsStringBuffer
  object.

  CAUTION: If the file exists, it will be overwritten!

  @PVM_PARS
  NONE

  @PVM_TBCALL
  NONE

  @PVM_RELCALL
  NONE

  @PVM_PRECOND
  NONE

  @PVM_ERROR
  NONE

  @PVM_EXAMPLE
  @code
  char procno_path[PATH_MAX];
  PvOvlUtilGetProcnoPath(procno_path, PATH_MAX, "");
  strcat(procno_path, "my_output_file");
  PvOvlPsStringBufferWrite(procno_path);
  @endcode

  @param[in] full_file_name
  The file name with preceeding absolute path to the file that
  the buffer content shall be written to.

  @ingroup PVM_UT
*/
DLL_INTERFACE void PvOvlPsStringBufferWrite(const char * full_file_name);


#endif

#endif

