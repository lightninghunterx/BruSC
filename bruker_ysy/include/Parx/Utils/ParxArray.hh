//===============================================================
//
// $Source$ 
//    String array class for usage with PARX
//
// Copyright (c) 2007 - 2016
//
// Bruker BioSpin MRI GmbH
// D-76275 Ettlingen, Germany
//
// All Rights Reserved
//
// $Id$
//
//===============================================================

#ifndef PARX_UTILS_PARXARRAY_HH
#define PARX_UTILS_PARXARRAY_HH

//---------------------------------------------------------------
// includes
//---------------------------------------------------------------

#ifndef CPROTO
#include <vector>
#include <algorithm>
#include <string>
#include <cstring>
#include "Parx/Utils/DLLMacro.h"


//---------------------------------------------------------------
// interface of class example
//---------------------------------------------------------------

namespace Parx
{
    /**
       @addtogroup ParxUtils
       @{
    */
    namespace Utils
    {

        /** 
         * Base array definition for 2 dimensional PARX arrays. 
         *  This class cannot be instantiated. 
         *  @see Parx2dArray
         *  @see Parx2dArrayRef
         *  @see ParxStringArray
         */
        template<typename T> class Parx2dArrayBase
        {
        public:
            
            /// Return 1 dimensional array representation for usage
            // with PARX functions.
            T * get() const { return array_[0]; }
    
            /// Return string array.
            T ** get2dArray() const { return array_; }
    
            /** Return pointer to 1d element at given index.
             *  @param i index into 2d array.
             *  @return pointer to array at slow index i. No range checking.
             */
            T * operator[](int i) { return array_[i]; }

            /** Get second dimension.
                @return Second dimension length.
            */
            int getDim2() const { return dim2_; }

            /** Get first dimension.
                @return First dimension length.
            */
            int getDim1() const { return dim1_; }

        protected:

            /** Protected constructor.
                @param array Pointer to array that is used as PARX parameter.
                @param dim2 2nd dimension.
                @param dim1 1st dimension.
            */
            Parx2dArrayBase(T ** array, int dim2, int dim1)
                : array_(array), dim2_(dim2), dim1_(dim1)
            {
            }

            /// Destructor.
            virtual ~Parx2dArrayBase() { delete[] array_; }
      
            /// The string array;
            T ** array_;

            /** Second dimension of array. */
            int dim2_;

            /** First dimension of array. */
            int dim1_;

        private:
            /// Unaccessible default constructor.
            Parx2dArrayBase();

            /// Unaccessible copy constructor.
            Parx2dArrayBase(const Parx2dArrayBase & rhs);

            /// Unaccessible assignment operator.
            void operator=(const Parx2dArrayBase & rhs);
        };


        /** Generic class for PARX 2-dimensional arrays. */
        template<typename T> class Parx2dArray : public Parx2dArrayBase<T>
        {
        public:

            /** 
             * Create a one dimensional PARX array from a 2-dimensional array.
             * @param array The 2 dimensional array.
             * @param dim2 slow dimension length.
             * @param dim1 fast dimension length.
             */
            Parx2dArray(const T * const * array, int dim2, int dim1)
                : Parx2dArrayBase<T>(Create(array, dim2, dim1), dim2, dim1)
            {
            }
                
           /**
            * Create a 2-dimensional PARX array from an vector of 
            * 1-dimensional arrays.
            * @param array The vector of 1-dimensional arrays.
            * @param dim1  The length of the 1-dimensional arrays.
            */
            Parx2dArray(const std::vector<T *> & array, int dim1)
                : Parx2dArrayBase<T>(Create(array, dim1), (int)array.size(), dim1)
            {
            }
                

            /** Creating a 2 dimensional PARX array.
             *  @param dim2 number of elements in second dimension.
             *  @param dim1 number of elements in first dimension.
             */
            Parx2dArray(int dim2, int dim1) 
                : Parx2dArrayBase<T>(Create(0, dim2, dim1), dim2, dim1)
            { 
            }

            /// Copy constructor.
            Parx2dArray(const Parx2dArray & array ) 
                : Parx2dArrayBase<T>(Create(array.array_, array.dim2_, 
                                            array.dim1_), 
                                     array.dim2_, array.dim1_)
            {
            }

            /// Assignment operator,
            Parx2dArray & operator=(const Parx2dArray & rhs)
            {
                T** array = Create(rhs.array_, rhs.dim2_, rhs.dim1_);
                std::swap(this->array_, array);
                Delete(array);
                this->dim2_ = rhs.dim2_;
                this->dim1_ = rhs.dim1_;
                return *this;
            }

            /// Destructor.
            virtual ~Parx2dArray()
            {
                if (this->array_)
                    delete[] this->array_[0];
            } 

            /** Release internal array and return it.
             *  @return the internal array. The ownership is transferred
             *    to the caller and the array must be deleted using the static
             *    \c Delete method.
             * @see Delete
            */
            T ** release() { T ** a = this->array_; this->array_ = 0; return a; }

            /** Delete the PARX 2d array.
                @param array Array to be deleted, 0 after this call.
            */
            static void Delete(T ** & array)
            {
                if (array)
                    delete[] array[0];
                delete[] array;
                array = 0;
            }

        private:

            /** Creator function of PARX 2d array.
                @param src Source two dimensional array.
                @param dim2 Second dimension length of the array.
                @param dim1 First dimension length of the array.
                @return a 2 dimensional PARX array or null on error.
            */
            static T** Create(const T * const * src, int dim2, int dim1)
            {
                T ** array = 0;
                if (0 < dim2 && 0 < dim1)
                {
                    array = new T*[dim2];
                    array[0] = 0;
                    array[0] = new T[dim1*dim2];

                    if (array && array[0])
                    {
                        for (int i = 1; i < dim2; ++i)
                            array[i] = &(array[0][dim1*i]);
          
                        if (src)
                        {
                            for (int i = 0; i < dim2; ++i)
                                memcpy(array[i], src[i], dim1 * sizeof(T));
                        }
                    }
                    else
                        Delete(array);
                }

                return array;
            }
            
            /** Creator function of PARX 2d array.
                @param src Vector a one dimensional arrays.
                @param dim1 Number of elements in the one dimensional arrays.
                @return a 2 dimensional PARX array or null on error.
            */
            static T** Create(const std::vector<T *> & src, int dim1)
            {
                T ** array = 0;
                const int dim2 = (int)src.size();
                if (0 < dim2 && 0 < dim1)
                {
                    array = new T*[dim2];
                    array[0] = new T[dim1*dim2];

                    for (int i = 1; i < dim2; ++i)
                        array[i] = &(array[0][dim1*i]);
                    
                    for (int i = 0; i < dim2; ++i)
                        memcpy(array[i], src[i], dim1 * sizeof(T));
                }
                return array;
            }

            /// Unaccessible default constructor.
            Parx2dArray();
        };


        /** Class for 2-dimensional array views. */
        template <typename T> class Parx2dArrayRef : public Parx2dArrayBase<T>
        {
        public:
            /** 
             * Constructor to view a 1-dimension array in two dimensions.
             * 
             * @param ref Array that should be viewed two dimensional.
             * @param dim2 Second dimension length of the array.
             * @param dim1 First dimension length of the array.
             */
            Parx2dArrayRef(T * ref, int dim2, int dim1)
                : Parx2dArrayBase<T>(Create(ref, dim2, dim1), dim2, dim1)
            {
            } 

            /// Copy constructor.
            Parx2dArrayRef(const Parx2dArrayRef & array ) 
                : Parx2dArrayBase<T>(Create(array.array_[0], array.dim2_, 
                array.dim1_), array.dim2_, array.dim1_)
            {
            }

            /// Assignment operator.
            Parx2dArrayRef & operator=(const Parx2dArrayRef & rhs)
            {
                T** array = Create(rhs.array_, rhs.dim2_, rhs.dim1_);
                std::swap(this->array_, array);
                delete [] array;
                this->dim2_ = rhs.dim2_;
                this->dim1_ = rhs.dim1_;
                return *this;
            }

            /// Destructor.
            virtual ~Parx2dArrayRef()
            {
            } 

        private:
            /** Creator function of PARX 2d reference array.
                @param ref Array that should be viewed as 2 dimensional array.
                @param dim2 Second dimension length of the array.
                @param dim1 First dimension length of the array.
                @return a 2 dimensional PARX array or null on error.
            */
            static T** Create(T * ref, int dim2, int dim1)
            {
                T ** array = 0;
                if (0 < dim2 && 0 < dim1)
                {
                    array = new T*[dim2];
                    array[0] = ref;

                    if (array[0])
                    {
                        if (1 < dim2)
                        {
                            for (int i = 1; i < dim2; ++i)
                                array[i] = &(array[0][dim1*i]);
                        }
                    }
                    else
                    {
                        delete[] array;
                        array = 0;
                    }
                }
                return array;
            }

            /// Unaccessible default constructor.
            Parx2dArrayRef();
        };


        /** Class for a PARX string array.
            It is only defined to use forward reference.
        */
        class DLL_INTERFACE ParxStringArray : public Parx2dArray<char>
        {
        public:
            /** Create a PARX string array.
                @param array pointer to 2 d array.
                @param dim2 number of strings.
                @param dim1 number of characters in a string.
            */
            ParxStringArray(const char * const * array, int dim2, int dim1);
            
            /** 
             * Create a PARX string array from a vector.
             * @param array A vector of C++-Strings.
             * @param strlen The maximum string length.
             */
            ParxStringArray(const std::vector<std::string> & array, int strlen);

            /** Creating a PARX string array.
                @param dim2 number of strings.
                @param dim1 number of characters in a string.
            */
            ParxStringArray(int dim2, int dim1);
  
            /// Copy Constructor.
            ParxStringArray(const ParxStringArray & array);
  
            /// Destructor.
            virtual ~ParxStringArray();
        };
        
        /** Base class for 2-dimensional string arrays. */
        class DLL_INTERFACE Parx2dStringArrayBase
        {
        public:
            
            /// Destructor.
            virtual ~Parx2dStringArrayBase();

            /// Get 1-dimensional representation of the array (for PARX).
            char * get() const { return array_ ? array_[0][0] : 0; }
            
            /// Get the 2-dimensional representation of the string array.
            char *** getArray() const { return array_; }
            
            /** Get a sub-array of strings at given index.
             * @param ndx Valid index.
             * @return sub-array of given index.
             */
            char ** getStringArray(int ndx) const { return array_[ndx]; }
            
            /**
             * Get a string value from the 2-dimensional string array.
             * @param dim2 Index into array of string arrays.
             * @param dim1 Index into with \a dim2 chosen string array.
             * @return The string at given indices.
             */
            char * getString(int dim2, int dim1) const
            { return array_[dim2][dim1]; }
            
            /// Return number of string arrays.
            int getNumOfStringArrays() const { return dim2_; }
            
            /// Return number of string in the string arrays.
            int getNumOfStringsInArrays() const { return dim1_; }
            
            /// Return maximum string length.
            int getStringLength() const { return stringLength_; }
            
            
        protected:
            /**
             * Constructor.
             * @param array 2-dimensional string array.
             * @param dim2 Number of 1-dimensional string arrays.
             * @param dim1 Number strings in the string arrays.
             * @param stringLength Maximum length of each string.
             */
            Parx2dStringArrayBase(char *** array, int dim2, int dim1, 
                int stringLength);
            
            
            Parx2dStringArrayBase(const Parx2dStringArrayBase &) = delete;
            Parx2dStringArrayBase & operator=(const Parx2dStringArrayBase &) = delete;
            
             /** The 2-dimensional string array. */
            char *** array_;
            
            /** The number of string arrays. */
            int dim2_;
            /** The number of strings in the string arrays.*/
            int dim1_;
            /** The maximum string length.*/
            int stringLength_;
       };
        
       /** 
        * Class for viewing a 1-dimension char array as 
        * 2-dimensional string array. 
        */
        class DLL_INTERFACE Parx2dStringArrayRef : public Parx2dStringArrayBase
        {
        public:
            /**
             * Constructor.
             * @param array 1-dimensional array that should be viewed as 
             * 2-dimensional string array.
             * @param dim2 Number of 1-dimensional string arrays
             * @param dim1 Number strings in the string arrays.
             * @param stringLength Maximum length of each string.
             */
            Parx2dStringArrayRef(char * array,int dim2, int dim1, 
                int stringLength);
            
            /// Destructor.
            virtual ~Parx2dStringArrayRef() = default;
            
        private:
            
            /// Unavailable  Copy Constructor.
            Parx2dStringArrayRef(const Parx2dStringArrayRef &) = delete;
            
            /// Deleted assignment operator.
            Parx2dStringArrayRef & operator=(const Parx2dStringArrayRef &) = delete;
            
        };

        /** Class for a PARX 2-dimensional string array.       
        */
        class DLL_INTERFACE Parx2dStringArray : public Parx2dStringArrayBase
        {
        public:

            /** Create a PARX string array.
             *  @param array pointer to 2 d array.
             *  @param dim2 number of string arrays.
             *  @param dim1 number of strings.
             *  @param stringLength number of string length.
             */
            Parx2dStringArray(const char * const * const * array, 
                int dim2, int dim1, int stringLength);
            

            /** Creating a PARX string array.
             *  @param dim2 number of string arrays.
             *  @param dim1 number of strings.
             *  @param stringLength number of string length.
             */
            Parx2dStringArray(int dim2, int dim1, int stringLength);
  
  
            /// Destructor.
            virtual ~Parx2dStringArray();
            
        private:
 
            /// Unavailable  Copy Constructor.
            Parx2dStringArray(const Parx2dStringArray &) = delete;
            
            /// Deleted assignment operator.
            Parx2dStringArray & operator=(const Parx2dStringArray &) = delete;
        };
    }

    //---------------------------------------------------------------
    // interface of non-member functions / methods
    //---------------------------------------------------------------

    /** @}*/
}

#endif

#endif

