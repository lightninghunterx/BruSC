/*
 *******************************************************************
 *
 * $Source: /bscl/CvsTree/bscl/gen/h/Path/Platform.h,v $
 *
 * Copyright (c) 1997
 * BRUKER BIOSPIN GMBH
 * D-76287 Rheinstetten, Germany
 *
 * All Rights Reserved
 *
 * $Id: Platform.h,v 1.26 2020/02/18 14:28:15 christoph.kessler Exp $
 *
 *******************************************************************
 */

#ifndef PATH_PLATFORM_H
#define PATH_PLATFORM_H


/*:=INFO=:**************************************************************
 *
 * Library: libPath.a
 * Header : Platform.h
 *
 * Description:	interface to class PathPlatform.
 *		It contains all platform specific functionalities
 *		needed by libPath.
 *
 * Sources:		bscl/src/lib/Path/Platform.c
 * Naming Conventions:	1.0
 * Used libraries:	none
 *
 *::=info=:*************************************************************/

#ifndef LOCAL_PATHPLATFORM_USAGE_ALLOWED
#error It is not allowed to use class PathPlatform outside of libPath. \
Ask the maintainer to add the missing functionality into classes \
PathSystem or PathXWinNmr.
#endif

/** Installation/Program type definition.
 Installation/Program type definition for initialization of
 path variables.  This is necessary because NMR and FTMS use
 slightly different paths for file locations and we do not
 want #ifdef's everywhere!

 This definition is used in Platform, XWinNMR, and System.
*/
enum PathInstType { PathInstTypeNmr , PathInstTypeFtms };


#ifdef __cplusplus

#include <string>

/** handles all platform specific functionality.

 The class {\tt PathPlatform} contains all platform specific
 functions that are needed within {\em libPath.a}. It is used in the classes
 {\tt PathXWinNMR} and {\tt PathSystem}, and therefore we do also put
 functions into {\tt PathPlatform} which are used by both these classes
 to prevent multiple dereferencing between them.

 Also extremely time consuming system calls are only called once and
 the results are cached (within its embedded classes).

 {\bf THIS MEANS OF COURSE} that the class {\tt PathPlatform} should
 not be used outside of {\em libPath.a}.
 @author \URL[Andreas H�nnebeck]{mailto:ah@bruker.de}
 @see PathAmix, PathSystem, PathXWinNMR
 */

class PathPlatform
{
    /** Embedded class User determines both user name and user homepath when created. */
    class User
    {
	static char* name;	/* user name */
	static char* home;	/* user home */
	static char* profile;	/* user profile */

      public:
	/** constructor. */
	User();

	/* access functions : */
	const char* getName() const { return (name ? name : ""); }
	const char* getHome() const { return (home ? home : ""); }
	const char* getProfile() const { return (profile ? profile : ""); }
    };

    /** Embedded class Host determines ip address of host when created (Linux only). */
    class Host
    {
	static char* fullName;	/* name of host */
	static char* name;	/* name of host */
	static char* ipAdr;	/* IP address of host */
	void determineIpAdrOnLinux();
    public:
	/** constructor. */
	Host();

	/* access functions : */
	const char* getFullName() const { return (fullName ? fullName : ""); }
	const char* getName() const { return (name ? name : ""); }
	const char* getIpAdr() const { return (ipAdr ? ipAdr : ""); }
    };

    /** Embedded class PdfViewer determines path to a PDF viewer when created. */
    class PdfViewer
    {
	static char* path;	/** path to PDF viewer. */

      public:
	/** constructor. */
	explicit PdfViewer(const char* xwinprog);

	/** get path to PDF viewer. */
	const char* getPath() const { return (path); }
    };

    /** Embedded class Xwin determines path to TopSpin subdirectories when created. */
    class Xwin
    {
	static char* home;	/* path to XWINNMRHOME. */
	static char* prog;	/* path to XWINNMRPROG. */

      public:
	/** constructor with type. */
	explicit Xwin(PathInstType type = PathInstTypeNmr);

	/** get path to TopSpin installation directory. */
	const char* getHome() const { return (home ? home : ""); }

	/** get path to TopSpin program directory. */
	const char* getProg() const { return (prog ? prog : ""); }
    };

    /** Embedded class Diskless determines path to diskless subdirectories when created. */
    class Diskless
    {
	static char* inst;		/** path to /usr/diskless. */
	static char* root;		/** path to /usr/diskless/clients/spect/root. */
	static char* lock;		/** path to /usr/diskless/clients/spect/lock. */

      public:
	/** constructor. */
	Diskless();

	/** get path to diskless installation directory. */
	const char* getInst() const { return (inst ? inst : ""); }

	/** get path to diskless root directory. */
	const char* getRoot() const { return (root ? root : ""); }

	/** get path to diskless lock directory. */
	const char* getLock() const { return (lock ? lock : ""); }
    };

    /** Embedded class Analysis determines path to Analysis directories when created. */
    class Analysis
    {
	static const char* registry;		/* path to Analysis registry */

      public:
	/** constructor. */
	Analysis();

	/** get path to Analysis registry. */
	const char* getRegistry() const
	    { return (registry ? registry : ""); }
    };

    /** Embedded class Amix determines path to AMIX directories when created. */
    class Amix
    {
	static const char* registry;	/* path to registry */
	static char* prog;		/* path to AMIX program. */
	static char* exe;
	static char* win;
/*	static char* user; */

	void readRegistryInformation (const char *program, char *progpath,
				      char *exename, char *winname,
				      char *username, int max_len);
      public:
	/** constructor. */
	explicit Amix(const char* reg);

	/* access functions : */
	const char* getProg() const { return (prog ? prog : ""); }
	const char* getExe() const { return (exe ? exe : ""); }
	const char* getWin() const { return (win ? win : ""); }
/*	const char* getUser() const { return (user ? user : ""); } */
    };

    /** Embedded class Perch determines path to PERCH directories when created. */
    class Perch
    {
	static char* path;		/* path to PERCH installation directory */

      public:
	/** constructor. */
	Perch();

	/** get path to PERCH installation directory. */
	const char* getPath() const { return (path); }
    };

    /** Embedded class Auremol determines path to AUREMOL directories when created. */
    class Auremol
    {
	static char* path;		/* path to AUREMOL installation directory */

      public:
	/** constructor. */
	Auremol();

	/** get path to AUREMOL installation directory. */
	const char* getPath() const { return (path); }
    };

    /* begin of master class (end of embedded classes) */

    static int		objnum;	/* number of instantiated objects */

    /* we need only one instance of each embedded class, and as */
    /* we desire to prevent unnecessary system calls we create */
    /* them only when needed: */
    static User*	user;
    static Host*	host;
    static PdfViewer*	pdfviewer;
    static Xwin*	xwin;
    static Analysis*	analysis;
    static Amix*	amix;
    static Diskless*	diskless;
    static Perch*	perch;
    static Auremol*	auremol;

    PathInstType	instType;	/* installation/program type: controls path formats initialized in platform() constructor */

    static std::string centralAuditLogFilePath;

    static std::string spectStatusProxyLogFilePath;
    static std::string spectStatusProxyLogFileDir;




  public:
    /** constructor with installation/program type. */
    explicit PathPlatform(PathInstType type = PathInstTypeNmr);

    /** destructor. */
    ~PathPlatform();

#if   defined(HAS_WINDOWS_FEATURES)
    std::string getWindowsFolderPath() const;
#endif

    /*--------------------------------------------------*/
    /* access functions (wrappers to embedded classes): */
    /*--------------------------------------------------*/

    /** returns path of central audit log file.

      {\tt getUserName()} returns the name of the current user, e.g.

      System dependencies:
      \begin{itemize}
      \item on {\bf Unix}: in /var/opt
      \item on {\bf Windows-NT}: in C:\ProgramData
      \end{itemize}

    */
    const char* getCentralAuditLogFilePath() const;

    const char* getSpectStatusProxyLogFileDir() const;
    const char* getSpectStatusProxyLogFilePath() const;


    /** returns name of user.

      {\tt getUserName()} returns the name of the current user, e.g.
      {\em "guest"}. If the name cannot be determined, an empty string
      is returned ({\em ""}).

      System dependencies:
      \begin{itemize}
      \item on {\bf Unix}: the name of the user is read
            from the password file (using {\tt getpwuid(3C)}.
	    If the user cannot be found {\tt getUserName()} returns
	    an empty string ({\em ""}).
      \item on {\bf Windows-NT}: the name of the user is read from
            the content of the environment variable {\tt USERNAME}
	    If {\tt USERNAME} is not set {\tt getUserName()} returns
	    an empty string ({\em ""}).
      \end{itemize}

    */
    const char* getUserName();


    /** returns home directory of user.

      {\tt getUserHome()} returns the complete path to the home directory
      of the current user, e.g. {\em "/usr/people/guest"}. If the home
      directory cannot be determined, an empty string is returned ({\em ""}).

      System dependencies:
      \begin{itemize}
      \item on {\bf Unix}: the path to the home directory is read
            from the password file (using {\tt getpwuid(3C)}.
	    If the user cannot be found {\tt getUserHome()} returns
	    an empty string ({\em ""}).
      \item on {\bf Windows-NT}: the path to the home directory is build from
            the contents of the environment variables {\tt HOMEDRIVE}
	    and {\tt HOMEPATH}. If one or both of these variables are not
	    set or if {\tt HOMEPATH} is the root directory (this can happen
	    if the administrator did not define the home directory) then the
	    home directory is built from the contents of the environment
	    variable {\tt USERPROFILE}. If this is also not defined then
	    {\tt getUserHome()} returns an empty string ({\em ""}).
      \end{itemize}

    */
    const char* getUserHome();


    /** returns profile directory of user.

      {\tt getUserProfile()} returns the complete path to the profile directory
      of the current user, e.g. {\em "C:/Documents and Settings/guest"}. On non
      Windows platforms this is the same path as getUserHome(). If the profile
      directory cannot be determined, an empty string is returned ({\em ""}).

      System dependencies:
      \begin{itemize}
      \item on {\bf Unix}: the path to the profile directory is read
            from the password file (using {\tt getpwuid(3C)}.
	    If the user cannot be found {\tt getUserProfile()} returns
	    an empty string ({\em ""}).
      \item on {\bf Windows-NT}: the path to the profile directory is build
	    by evaluating GetUserProfileDirectory() in Userenv.lib.
	    If this fails {\tt getUserProfile()} returns an empty string ({\em ""}).
      \end{itemize}

    */
    const char* getUserProfile();

    /** return complete name (name + domain) of this host.

      \return name and domain (e.g. "mypc.bruker.com"), but not yet on Windows
      \return pointer to empty string in case of error, and on Windows
      */
    const char* getFullHostName();

    /** return name (without domain) of this host

      \return name without domain (e.g. "mypc")
      \return pointer to empty string in case of error
      */
    const char* getHostName();

    /** return IP address of this host.

      \return IP address (e.g. "192.168.1.20"), but not yet on Windows
      \return pointer to empty string in case of error, and on Windows
      */
    const char* getHostIpAdr();

    /** returns path to PDF viewer.

      {\tt getPdfviewerPath()} returns the complete path to a PDF viewer,
      e.g. {\em "/usr/local/bin/acroread"}. If no PDF viewer is installed,
      a {\tt NULL} pointer is returned.

      System dependencies:
      \begin{itemize}
      \item on {\bf Unix}:
            \begin{enumerate}
            \item all directories specified in {\tt PATH} are scanned
	          for the existence of the Acrobat reader {\em acroread}.
            \item the XWIN-NMR standalone directory as specified in
	          {\tt XWINNMRPROG}/{\em bin} is checked for the existence
		  of the Acrobat reader {\em acroread}.
            \item all directories specified in {\tt PATH} are scanned
	          for the existence of the PDF viewer {\em xpdf}.
            \item the XWIN-NMR standalone directory as specified in
	          {\tt XWINNMRPROG}/{\em bin} is checked for the existence
		  of the  PDF viewer {\em xpdf}.
	    \end{enumerate}
      \item on {\bf Windows-NT}:
            \begin{enumerate}
	    \item the Windows registry is checked for
                  an entry of the Acrobat reader {\em AcroRd32.exe}.
            \item all directories specified in {\tt PATH} are scanned
	          for the existence of the Acrobat reader {\em AcroRd32.exe}.
            \item the XWIN-NMR standalone directory as specified in
	          {\tt XWINNMRPROG}/{\em bin} is checked for the existence
		  of the Acrobat reader {\em AcroRd32.exe}.
            \item all directories specified in {\tt PATH} are scanned
	          for the existence of the PDF viewer {\em xpdf.exe}.
            \item the XWIN-NMR standalone directory as specified in
	          {\tt XWINNMRPROG}/{\em bin} is checked for the existence
		  of the  PDF viewer {\em xpdf.exe}.
	    \end{enumerate}
      \end{itemize}
    */
    const char* getPdfViewerPath();


    /** returns install directory of XWIN-NMR ({\tt XWINNMRHOME}).

      {\tt getXwinHome()} returns the path to the XWIN-NMR installation
      directory, as it is set with the environment variable
      {\tt XWINNMRHOME}, e.g. {\em "/u"}. If {\tt XWINNMRHOME} is not
      set then {\em "/undefined"} is returned.
     */
    const char* getXwinHome();


    /** returns module directory of XWIN-NMR ({\tt XWINNMRPROG}).

      {\tt getXwinProg()} returns the path to the XWIN-NMR program
      directory (by default depending on the environment variable
      {\tt XWINNMRHOME}), e.g. {\em "/u/prog"}.

      This default setting can be changed by setting the environment
      variable {\tt XWINNMRPROG}. If neither {\tt XWINNMRHOME} nor
      {\tt XWINNMRPROG} are set then {\em "/undefined/prog"} is returned.
     */
    const char* getXwinProg();


    /** returns root directory of diskless tree.

      {\tt getDisklessRoot()} returns the complete path to the diskless
      root directory for the CCU unix operating system, meaning
      \begin{itemize}
      \item on Unix system: {\em "/usr/diskless/clients/spect/root"}
      \item on Windows-NT: the path as specified by Windows-NT registry entry.
            If there is no entry in the registry then {\tt getDisklessRoot()}
	    returns {\em "C:/bruker/usr/diskless/clients/spect/root"}.
      \end{itemize}
    */
    const char* getDisklessRoot();


    /** returns installation directory of diskless tree.

      {\tt getDisklessInst()} returns the complete path to the diskless
      distribution, meaning
      \begin{itemize}
      \item on Unix system: {\em "/usr/diskless"}
      \item on Windows-NT: the path as specified by Windows-NT registry entry.
            If there is no entry in the registry then {\tt getDisklessRoot()}
	    returns {\em "C:/bruker/diskless"}.
      \end{itemize}
    */
    const char* getDisklessInst();


    /** returns lock directory of diskless tree.

      {\tt getDisklessLock()} returns the complete path to the remote lock
      directory (located within the diskless distribution for the CCU unix
      operating system), meaning
      \begin{itemize}
      \item on Unix system: {\em "/usr/diskless/clients/spect/lock"}
      \item on Windows-NT: the path as specified by Windows-NT registry entry.
            If there is no entry in the registry then {\tt getDisklessLock()}
	    returns {\em "C:/bruker/usr/diskless/clients/spect/lock"}.
      \end{itemize}
    */
    const char* getDisklessLock();


    /** returns registry of programs of the Analysis group.

      {\tt getAnalysisRegistry()} returns the registry of the programs of
      the Analysis group.
      \begin{itemize}
      \item Windows: it returns the path into the Windows registry.
      \item Unix: it returns the path to the ini file. If the shell variable
            {\tt BRUKER_REGISTRY_FILE} is set it will be used as the path
	    to the ini file.
      \end{itemize}
     */
    const char* getAnalysisRegistry();


    /** returns program directory of AMIX.

      {\tt getAmixProg()} returns the path to the AMIX program
      directory.

      If AMIX is not installed then an empty string (not a NULL pointer)
      is returned.
     */
    const char* getAmixProg();

    /** returns program name of AMIX.

      {\tt getAmixExe()} returns the name of the AMIX program.

      If AMIX is not installed then an empty string (not a NULL pointer)
      is returned.
     */
    const char* getAmixExe();

    /** returns windows program name of AMIX.

      {\tt getAmixWin()} returns the windows name of the AMIX program.

      If AMIX is not installed then an empty string (not a NULL pointer)
      is returned.
     */
    const char* getAmixWin();

    /** returns installation/program type

      {\tt getInstType()} returns the installation setting for the current
      instance of platform.  The installation/program type is initialized
      in XWinNMR.c, based on compiler flags to either:
      \begin{itemize}
      \item PathInstTypeNmr: for NMR using Xwin-NMR or TopSpin
      \item PathInstTypeFtms: for FTMS using ApexControl
      \end{itemize}
     */
    PathInstType getInstType() const { return instType; }

    /** returns path to PERCH installation directory.

      {\tt getPERCHPath()} returns the complete path to the installation directory
      of the PERCH prediction software. The path does not end with a directory
      delimiter (e.g. "C:\Bruker\TS20\PERCH"). If PERCH is not installed (which is always
      the case on all platforms except Windows), a NULL pointer is returned.

      System dependencies:
      \begin{itemize}
      \item on {\bf Unix}: PERCH is not supported, so NULL is always returned.
      \item on {\bf Windows}: the Windows registry is checked for an entry of the
		PERCH NMR Software.
      \end{itemize}
    */
    const char* getPERCHPath();

    /** returns path to AUREMOL executable.

      {\tt getAuremolExecPath()} returns the complete path to the AUREMOL executable,
      (e.g. "/opt/auremol/auremol"). If AUREMOL is not installed (which is always
      the case on Mac OS-X), a NULL pointer is returned.

      System dependencies:
      \begin{itemize}
      \item on {\bf Linux}: the system search path is checked for the 'auremol' executable
      \item on {\bf Windows}: the shell variable AUREMOL_INST_DIR is checked
      \item on {\bf Mac}: Null is returned
      \end{itemize}
    */
    const char* getAuremolExecPath();
};

#endif /* __cplusplus */
#endif /* PATH_PLATFORM_H */
