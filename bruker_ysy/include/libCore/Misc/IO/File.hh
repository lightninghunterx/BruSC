//==============================================================================
// $Source$
//
// Copyright (c) 1997 - 2016
//
// Bruker BioSpin AG
// MRI Division
// CH-8117 Fällanden, Switzerland
//
// All Rights Reserved
//
// $Id$
//==============================================================================

#ifndef LIB_CORE_MISC_IO_FILE_H
#define LIB_CORE_MISC_IO_FILE_H

#include <fstream>
#include <iomanip>

namespace core
{
    //--------------------------------------------------------------------------
    // Interface of class core::File
    //--------------------------------------------------------------------------
    /**
     * @headerfile "libCore/Misc/IO/File.hh"
     * Static utility class collecting file operations.
     * <code>File</code> provides methods to perform common file operations.
     *
     * @version $Id$
     * @author Markus Weiger
     * @since ParaVision 6.Alpha.0.114
     */
    class File
    {
    public:

        /**
         * Write data to ASCII file.
         *
         * For floating point data, the scientific format with a precision of 6
         * digits is used.
         *
         * @param fileName  Full file name (including path).
         * @param data      Data of arbitrary type (number of elements must be
         *                  @a nRow times @a nCol).
         * @param nRow      Number of rows to be written.
         * @param nCol      Number of columns.
         * @param mode      @c 0 = write, @c 1 = append
         * @return          @c 0 = ok, @c -1 = failure
         */
        template<class T>
        static inline int WriteDataToAscii(
            const char *fileName,
            T data,
            size_t nRow,
            size_t nCol = 1,
            unsigned int mode = 0)
        {
            std::ofstream file;
            file.open(fileName, mode == 0 ? std::ios::out : std::ios::app);

            if (file.good() == false) return -1;

            size_t k = 0;

            for (size_t i = 0; i < nRow; i++)
            {
                for (size_t j = 0; j < nCol; j++)
                {
                    file << std::right << std::setw(14) << std::showpoint
                         << std::scientific << std::setprecision(6)
                         << data[k++] << ' ';
                }
                file << '\n';
            }
            file << std::flush;

            file.close();

            if (file.good() == false) return -1;

            return 0;
        }

    private:
        /** Inaccessible and unimplemented constructor: abstract class. */
        File();
    };
}

#endif
