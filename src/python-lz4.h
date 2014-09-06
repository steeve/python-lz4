/*
 * Copyright (c) 2012-2013, Steeve Morin
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. Neither the name of Steeve Morin nor the names of its contributors may be
 *    used to endorse or promote products derived from this software without
 *    specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include "Python.h"

static PyObject *py_lz4_compress(PyObject *self, PyObject *args);
static PyObject *py_lz4_uncompress(PyObject *self, PyObject *args);
static PyObject *py_lz4_compressFileAdv(PyObject *self, PyObject *args, PyObject *keywds);
static PyObject *py_lz4_compressFileDefault(PyObject *self, PyObject *args);
static PyObject *py_lz4_decompressFileDefault(PyObject *self, PyObject *args);

PyMODINIT_FUNC initlz4(void);

#define COMPRESS_DOCSTRING      "Compress string, returning the compressed data.\nRaises an exception if any error occurs."
#define COMPRESSHC_DOCSTRING    COMPRESS_DOCSTRING "\n\nCompared to compress, this gives a better compression ratio, but is much slower."
#define UNCOMPRESS_DOCSTRING    "Decompress string, returning the uncompressed data.\nRaises an exception if any error occurs."
#define COMPRESS_FILE_DOCSTRING "Compress file, by default adds .lz4 extension to original filename."
#define COMPF_DEFAULT_DOCSTRING COMPRESS_FILE_DOCSTRING "\nAccepts two positional arguments, inputFile and compression level."
#define COMPF_ADV_DOCSTRING     COMPRESS_FILE_DOCSTRING "\nRequires the first two keyword arugments and accepts any number of the"\
                                "\nfollowing: input, compLevel, output, overwrite, blockSizeID, blockCheck, streamCheck"\
                                "\nValid values are as follows(def=default): input='string', compLevel=0(low, def)-9(High), output='string',"\
                                "\noverwrite=0/1(def), blockSizeID=4-7(def), blockCheck=0(def)/1, streamCheck=0/1(def), verbosity=0(def)-4"
#define DECOMP_FILE_DOCSTRING   "Decompresses file, removes the extension by default, preserves original."

#if defined(_WIN32) && defined(_MSC_VER)
# define inline __inline
# if _MSC_VER >= 1600
#  include <stdint.h>
# else /* _MSC_VER >= 1600 */
   typedef signed char       int8_t;
   typedef signed short      int16_t;
   typedef signed int        int32_t;
   typedef unsigned char     uint8_t;
   typedef unsigned short    uint16_t;
   typedef unsigned int      uint32_t;
# endif /* _MSC_VER >= 1600 */
#endif

#if defined(__SUNPRO_C) || defined(__hpux) || defined(_AIX)
#define inline
#endif

#ifdef __linux
#define inline __inline
#endif
