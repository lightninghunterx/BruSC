/****************************************************************/
/*  IVDrBase.h                 25.08.2016                       */
/****************************************************************/
/*  Short Description :                                         */
/*  Library for IVDr Methods Parameter Setting.                 */
/****************************************************************/
/*  Keywords :                                                  */
/*  bodyfluid screening (urine, plasma, ...), acquisition       */
/****************************************************************/
/*  Description/Usage :                                         */
/*                                                              */
/*  Setting method specific parameters if IVDrMethods License   */
/*  is available at spectrometer computer.                      */
/*                                                              */
/****************************************************************/
/*  Author(s) :                                                 */
/*  Name            : Andreas Grote                             */
/*  Organisation    : Bruker BioSpin GmbH                       */
/*  Email           : andreas.grote@bruker.com                  */
/*                                                              */
/*  Name            : Hartmut Schaefer                          */
/*  Organisation    : Bruker BioSpin GmbH                       */
/*  Email           : hartmut.schaefer@bruker.com               */
/*                                                              */
/*  Name            : Tony Reinsperger                          */
/*  Organisation    : Bruker BioSpin GmbH                       */
/*  Email           : tony.reinsperger@bruker.com               */
/****************************************************************/
/*  Name        Date     Modification:                          */
/*  agro        160801   created                                */
/*  hasc        160822   modified                               */
/*  hasc        160825   modified                               */
/*  tony        190528   fixes and B.I.Methods 2.0 portfolio    */
/*  tony        190607   shape database introduced              */
/*  tony        190619   CSF modified to D1 10, PresatRF 25Hz   */
/*  tony        190731   unified B.I.Methods 1.0 and 2.0 (CSF!) */
/*  tony        191014   extension license, fixes for 2.0.1     */
/*  tony        200311   2.0.2: 5mm pars with different NS      */
/****************************************************************/


#ifdef DLL_INTERFACE
#   undef DLL_INTERFACE
#endif

#if defined(DLL_LINKAGE) && !defined(CPROTO)
#   ifdef libIVDrBase_INTERNAL
#      define DLL_INTERFACE __declspec(dllexport)
#   else
#      define DLL_INTERFACE __declspec(dllimport)
#   endif
#else
#   define DLL_INTERFACE
#endif


#define     RG16    16.0
#define     RG90    90.5


// standard sample tubes

#define     MEOD_5MM            "5mm 99.8% MeOD sample"
#define     MEOD_3MM            "3mm 99.8% MeOD sample"

#define     SUCROSE_5MM         "5mm Sucrose in 90%H2O+10%D2O"
#define     SUCROSE_3MM         "3mm Sucrose in 90%H2O+10%D2O"

#define     FILCOR_5MM          "5mm FILCOR sample"
#define     FILCOR_3MM          "3mm FILCOR sample"

#define     QUANTREF600C_5MM    "5mm QUANTREF600C sample"
#define     QUANTREF600C_3MM    "3mm QUANTREF600C sample"

#define     URINE_5MM           "5mm Urine sample"
#define     URINE_3MM           "3mm Urine sample"

#define     PLASMA_5MM          "5mm Plasma sample"
#define     PLASMA_3MM          "3mm Plasma sample"

#define     CSF_5MM             "5mm CSF sample"
#define     CSF_3MM             "3mm CSF sample"

#define     MEOH_5MM            "5mm MeOH extract sample"
#define     MEOH_3MM            "3mm MeOH extract sample"

// constant experimental parameters



// data type declarations

/** B.I.METHODS 1.0 data types **********************************/

struct par_relations
{
    char   parameterset_name[100];
    char   master_set[100];
    char   sample_info[100];
    char   solvent[100];
    char   pulprog[100];
    char   aunm[100];
    char   aunmp[100];
    float  temperature;
    float  presat_field;
    float  d1;
    double sw;
    float  rg;
    int    ns;
    int    ds;
};


/** B.I.METHODS 2.0 data types **********************************/

struct par_relations_v2
{
    char   parameterset_name[100];
    char   master_set[100];
    char   sample_info[100];
    char   solvent[100];
    char   pulprog[100];
    char   aunm[100];
    char   aunmp[100];
    float  temperature;
    float  presat_field;
    float  presat_shape;
    char   shape_name[100];
    char   gen_shape[100];
    float  d1;
    double sw;
    float  rg;
    int    ns;
    int    ds;
    int    permission;
};

struct shape_description
{
    char  shape_name[100];
    int   n;
    float p18;
    float ampl[100];
    float minf[100];
    float maxf[100];
    int linesPerBand[100];
    int setO1ToLine;
};

#ifdef  __cplusplus
extern "C" {
#endif

/** B.I.METHODS 1.0 functions ***********************************/

extern DLL_INTERFACE const struct par_relations* get_PARAREL(void);

extern DLL_INTERFACE int get_special_pars(const char*, char*, char*,
                                          float*, float*, float*, float*,
                                          double*, int*, int*);


/** B.I.METHODS 2.0 functions ***********************************/

extern DLL_INTERFACE int get_special_pars_v2 (const char*, char*, char*,
                                              float*, float*, float*,
                                              char*, float*, float*,
                                              double*, int*, int*);

extern DLL_INTERFACE int quantRefPermission (void);

extern DLL_INTERFACE const struct par_relations_v2* get_PARAREL_V2 (void);

extern DLL_INTERFACE const struct shape_description* get_SHAPEREL (void);

#ifdef  __cplusplus
}
#endif
