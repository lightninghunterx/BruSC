/****************************************************************
 *
 * Copyright (c) 2012 - 2019
 * Bruker BioSpin MRI GmbH
 * D-76275 Ettlingen, Germany
 *
 * All Rights Reserved
 *
 ****************************************************************/

/* prevent double includes */
#ifndef DSETPVM_H
#define DSETPVM_H

#ifndef CPROTO
#include <vector>
#include "subject/subjtyp.h"
#include "Visu/Common/VisuTypes.h"
#include "common/ovl_toolbox/DLLMacro.h"

namespace core
{
    class MatrixDescription;
}

namespace Visu
{
    class FrameGroupDescription;
}

/*==============================================================*/
/** @file
 * @defgroup PvOvlTools ParaVision overlay toolbox
 *  <p>This utility library contains general functions for:</p>
 *  - handling of completed datasets within relations
 *  - handling boolean vectors used as pixel mask
 */
/*@{*/

namespace DsetPvm
{



    /** Delivers number of true entries in boolean vectors.
        @param mask boolean array
        @return number of true entries
     */
    DLL_INTERFACE uint64_t trueEntriesInMask(std::vector<bool> &mask);

    /** Initializes boolean array.
        Sets all elements of vector to boolean value delivered
        by (optional) argument val 
        @param mask boolean array
        @param val (optional) boolean state that should be initialized
     */
    DLL_INTERFACE void initMask(std::vector<bool> &mask, bool val = false);

    /** Builds intersection of two boolean arrays.
        Array elements of two boolean vectors @c a and @c b of 
        same size are linked by a logical && operation and result 
        is written to vector @c c.
        c[i] = a[i] && b[i].

        Preconditions:
        vectors a,b and c must have the same size

        @param a (input) boolean vector
        @param b (input) boolean vector
        @param c (output) boolean vector 
        @return number of true entries in boolean vector c
     */
    DLL_INTERFACE uint64_t intersectMasks(std::vector<bool> &a,
        std::vector<bool> &b,
        std::vector<bool> &c);
    /** Builds union of two boolean arrays.
        Array elements of two boolean vectors @c a and @c b of 
        same size are linked by a logical || operation and result 
        is written to vector @c c.
        c[i] = a[i] || b[i].

        Preconditions:
        vectors a,b and c must have the same size

        @param a (input) boolean vector
        @param b (input) boolean vector
        @param c (output) boolean vector 
        @return number of true entries in boolean vector c
     */
    DLL_INTERFACE uint64_t unionOfMasks(std::vector<bool> &a,
        std::vector<bool> &b,
        std::vector<bool> &c);

    /** Sets values of excluded pixels.
        Array elements of @c pixels that are excluded (corrsponding
        pixel mask array @c mask is @c false ) are set to value

     */
    DLL_INTERFACE uint64_t
    setMaskedPixels(double *pixels,
        uint64_t size,
        std::vector<bool> &mask,
        double val = 0.0);

    /** Class for adjusted parameter maps.
        contains basic functionality to read in data and retrieve
        mandatory information from visu pargroup necessary to
        convert pixel values as stored in raw data file (2dseq)
        into physical units (represented as double) and to interpret
        the frames and their geometry.
        Information about dataset path is retrieved by the
        procnoResult (AdjProcnoResultType) 
     */
    class DLL_INTERFACE Map
    {
    private:
        /** forbidden constructors */
        Map(void);
        /** forbidden constructors */
        Map(const Map &);
        /** forbidden copy constructor */
        void operator=(const Map &);

        // private data members
        struct ImplType;
        /** information to interpret data stored in 2dseq file */
        struct ImplType *pimpl_;
        


    public:
        /** destructor */
        ~Map();
        /** constructor using AdjProcnoResult */
        Map(const AdjProcnoResultType & res);

        /** compares dataset description of two maps.
            This compare routine may be used to check whether 

         */
        bool matchedDsetDescr(Map &map) const;

        /** dataset description routines: values retrieved from visu */

        /** returns number of frames in visu description of dset*/
        int getVisuNframes() const;

        /** returns dimension of frames in visu description of dset */
        int getVisuFrameDim() const;

        /** returns sizes of frames in visu description of dset,
            dimension is getVisuFrameDim() 
         */
        const int *getVisuFrameSize() const;

        /** returns dimension description of frames in visu description of dset,
            dimension is getVisuFrameDim();
         */

        const VISU_DIM_TYPE *getVisuFrameDimDescr() const;

        /** returns number of spatial dimensions of frames in visu description 
            of dset
         */
        int getVisuFrameNSpatDim() const;


        /** returns extents of frames in visu description of dset,
            dimension is getVisuFrameNSpatDim()
         */
        const double *getVisuFrameExtent() const;

        /** returns dimension and thickness of visu frames in visu
            description of dset 
         */

        const double *getVisuFrameThick(int &dim) const;

        /** returns dimension dependent units of frames in visu description
            of dset, argument dim must be in [0, getVisuFrameDim()].
            Returns 0 pointer if dim is out of range or no frame dimension
            is available
         */

        const char *getVisuFrameDimUnits(int dim) const;

        /** returns transposition of frames in visu description of dset,
            if 0 returned, frames are not transposed, otherwise dimension
            is getVisuFrameDim()
         */
        const int *getVisuFrameTransposition() const;


        /** returns disk slice order of frame.
         */
        VisuDiskSliceOrderType getVisuDiskSliceOrder(int frame) const;

        /** returns patient position of study as stored in visu description
            of dset.
         */

        SubjectInstrPosType getVisuFramePatPos() const;

        /** returns dimension of orientations and positions in visu description
            of dset. If dimension is 1 , all frames have the same orientation
            and position. If dimension is 0, no spatial dimension is defined,
            otherwise the dimension is defined by framegroups or it is the
            total number of frames

         */
        int getVisuFrameOriDim() const;


        /** returns orientation of frames and dimension of frame orientation
            in visu description of dset, argument dim must lie within
            [0, getVisuFrameOriDim()]. 
            Each element is a pointer to a double vector of length 9.
         */

        const double *getVisuFrameOri(int dim) const;

        
        /** returns dimension of positions in visu description
            of dset. If dimension is 1 , all frames have the same position. 
            If dimension is 0, no spatial dimension is defined,
            otherwise the dimension is defined by framegroups or it is the
            total number of frames

         */
        int getVisuFramePosDim() const;
        /** returns position of frames 
            in visu description of dset, argument dim must lie within
            [0, getVisuFramePosDim()]. 
            Each element is a pointer to a double vector of length 3.
         */
        const double *getVisuFramePos(int dim) const;

        
        /** returns dimension of modality offsets in visu description
            of dset. If dimension is 1 , all frames have the same position. 
            If dimension is 0, no spatial dimension is defined,
            otherwise the dimension is defined by framegroups or it is the
            total number of frames
         */
        int getVisuFrameModalityOffsetDim() const;
        
        /** returns modality offset of frames 
            in visu description of dset, argument dim must lie within
            [0, getVisuFrameModalityOffsetDim()]. 
            Each element is a pointer to a double vector of length 3.
         */
        const double *getVisuFrameModalityOffset(int dim) const;
        
        
        /** Delivers the modality offset of frame.
            Selects the correct VisuCoreModalityOffset entry for the
            frame given as argument.

            Preconditions:
            Pixels must have been read in.

            Error conditions:
            frameno must lie within legal ranges according to read
            data subset description (see checkFrameIdx).
     

            @param modalityOffset : 3 element double array
            @param frameno : index of the frame according to subset
            @return false is returned for illegal indices or internal error 
            state
         */
        bool getModalityOffset( double *modalityOffset, int frameno) const;        


        /** returns maximum value of frames (to be transformed with slope
            and offset), dimension is getVisuNFrames().
         */
        const double *getVisuFrameMaxVal() const;

        /** returns minimum value of frames (to be transformed with slope
            and offset), dimension is getVisuNFrames().
         */
        const double *getVisuFrameMinVal() const;


        /** returns offset of frames in visu description of dset. 
            Dimension is getVisuNFrames(). See slope
         */
        const double *getVisuFrameOffs() const;

        /** returns slope of frames in visu description of dset. 
            Dimension is getVisuNFrames(). Pixel values as stored
            in 2dseq file may be transformed to values in (possibly 
            specified units) according to
            val = slope * pix + offs;
            with pix: pixel value
         */
        const double *getVisuFrameSlope() const;

        /** returns the dimension of frame data units in visu 
            description of dset.
            Possible return values: 
            0 (no data units)
            1 (same data units for all frames)
            other: data units defined as frame dependent parameter.
         */

        int getVisuFrameDataUnitsDim() const;


        /** returns the frame data units for frame specified by argument
            fidx. Fidx must lie within [0, getVisuFrameDataUnitsDim()]
         */
        const char* getVisuFrameDataUnits(int fidx) const;



        /** returns the number of framegroups in visu description of dset.
         */

        int getVisuNFrameGroups() const;

        /** 
         * Returns the frame group description in visu description of dset.
         */

        const Visu::FrameGroupDescription *getVisuFrameGroups() const;


        /** Checks core dataset description. 
            Checks arguments (values >0) against visu dataset 
            description. If arguments have values <= 0 they are
            set to corresponding values of visu dataset description
            parameters.

            @param reqDim check against or set to value of VisuCoreDim
            @param reqFrames check against or set to value of VisuCoreFrameCount
            @param reqFrameGroups check against or set to value of 
            VisuFGOrderDescDim
            @throws Exception in case of error.
         */
        void checkDsetDescr(int & reqDim,
            int & reqFrames,
            int & reqFrameGroups);


        /** 
         * Reads pixel values.
         * Reads the specified pixel data into an internal buffer.
         * @param subsetBound Optional subset bounds to get only a part 
         * of the dataset.
         * @throws PvException in case of an error.
         */
        void readPixelValues(const core::MatrixDescription * subsetBounds);

        /** 
         * Copy pixel values into a given buffer
         * Preconditions: Data must have been read in (see readPixelValues)
 
            @param frameno Index of the frame according to subset 
            MatrixDescription
            @param pixels array of size \c npixels
         * @param npixels the number of pixels to be copied.
         * @throws PvException Error conditions that throw exceptions:

            - successfully read of pixel values (see readPixelValues)
            - valid range of argument frameno
            - valid size of npixels (number of pixels of one frame)

         * @see readPixelValues

         */
        void getPixelValues(int frameno, double *pixels, uint64_t npixels);


        /** returns number of frames that are read in
            error conditions that throw exceptions:

            successfully read of pixel values (see readPixelValues)

         */
        int framesRead();

        /** returns number of pixels per frame
            error conditions that throw exceptions:

            successfully read of pixel values (see readPixelValues)
        

         */
        uint64_t pixelsPerFrame(void);
        /** returns number of pixels
            error conditions that throw exceptions:

            successfully read of pixel values (see readPixelValues)
        
         */
        uint64_t npixels(void);

        /** returns volume of pixel.
            The unit is dependent on the frame dim units of the dataset
            (ul in case of mm)

            error conditions that throw exceptions:

            successfully read of pixel values (see readPixelValues)
         */
        double pixelVolume(void);

        /** Returns the path to the procno directory of the dataset 
         */
        const char *getDsetPath();


        /** Check for identical ATS position.
            Checks whether the ATS position (represented as 3-element array)
            is the same (within a given accuracy) as position at measurement
            time of the dataset)

            @param ats_position position to be checked in master coordinate 
                   system (for geometry objects the mod_offset entry) represented
                   as 3-element vector with entries in mm
            @param frameno : index of the frame according to subset
            @param accuracy (value >= 0.0 defining the accuracy of the 
                             position in mm) For values < 0 false is returned,
                             for values == 0 always true is returned                         
            @return false if one component is significantly different to 
                    the ats position of the dataset.

         */


        bool checkATSPosition( const double ats_position[3], 
                                const double accuracy,
                                int frameno) const; 

        /** Delivers the modality type of dataset.

            Preconditions:
            Successful read of visu parameter.

            @return modality type converted to PvModalityType 
         */
        PvModalityType getModalityType() const;



        /** Creates coordinates for the given frame.

            Preconditions: Data must have been read in (see readPixelValues)
 
            Argument siz is checked to be 3 times the number of pixels per 
            frame as defined by subset MatrixDescription

            @param frameno Index of the frame according to subset 
            MatrixDescription
            @param coord pixel coordinates in magnet system for given frame

         */
        void setCoordinates(int frameno, double *coord, uint64_t siz);

        inline bool validPixel(double val)
        {
            return val != 0.0;
        };

        uint64_t validatePixels(int frameno, std::vector<bool> &mask);


        /** Sets pointer to internal allocated memory to process/write out 
            pixel values.
   
            Prepares internal state of class to write out pixel values.

            Error conditions that throw exceptions:
            - illegal value of @c frameno 
            - illegal value of size (must be number of pixels/frame)
     
            @param frameno number of frame to be written
            @param size number of double values to be processed/written
            @return pointer to date for required frame.
         */
        double * getOutPixels(int frameno, uint64_t size);




        /**
         */

        void writeDataset(int &procno, bool calc_offset, char *dsetpath);

    };
} // end namespace DsetPvm

#endif /* CPROTO */

/*@}*/

#endif
