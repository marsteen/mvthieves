//
//                              JPEG I/O Routines 11/1997 Martin Steen
//

#include <string>
#include <iostream>
#include <fstream>

using namespace std;

#include <stdio.h>
#include <setjmp.h>
#include <CFileIO.h>
#include <CGraphicsJpeg.h>


extern "C"
{
#include <jpeglib/jpeglib.h>
};


struct my_error_mgr
{
    struct jpeg_error_mgr	pub;            /* "public" fields */
    jmp_buf					setjmp_buffer;  /* for return to caller */
};

typedef struct my_error_mgr* my_error_ptr;


struct SJpegParams
{
    SJpegParams()
    {
        memset(this, 0, sizeof(SJpegParams));
    }


    struct jpeg_decompress_struct	cinfo;
    struct jpeg_compress_struct		cinfo2;
    struct my_error_mgr				jerr;

    FILE*							infile;     /* source file */
    FILE*							outfile;
    JSAMPARRAY						buffer;     /* Output row buffer */
    JSAMPLE*						image_buffer;
    int								row_stride; /* physical row width in output buffer */
    int								soll_linesize;

    int								xsize;
    int								ysize;
    int								image_height;   /* Number of rows in image */
    int								image_width;    /* Number of columns in image */


    unsigned char*					GLbuffer;
    unsigned char*					MainBuffer;
};


CGraphicsJpeg::CGraphicsJpeg()
{
    mFileOffset = 0;
    mAppendMode = false;
    mJparams = (char*)(new SJpegParams);
}


CGraphicsJpeg::~CGraphicsJpeg()
{
    delete[] mJparams;
}


#if 0
static JSAMPLE* image_buffer;   /* Points to large array of R,G,B-order data */
static int image_height;        /* Number of rows in image */
static int image_width;         /* Number of columns in image */
static int soll_linesize;
static unsigned char* GLbuffer;
#endif


//extern DxDebug Debug;


/*
 * Here's the routine that will replace the standard error_exit method:
 */


METHODDEF(void)
my_error_exit(j_common_ptr cinfo)
{
#if 1
    /* cinfo->err really points to a my_error_mgr struct, so coerce pointer */
    my_error_ptr myerr = (my_error_ptr)cinfo->err;

    /* Always display the message. */
    /* We could postpone this until after returning, if we chose. */
    (*cinfo->err->output_message)(cinfo);

    /* Return control to the setjmp point */
    longjmp(myerr->setjmp_buffer, 1);
#endif
}

//---------------------------------------------------------------------------
//
// Klasse:    CGraphicsJpeg
// Methode:   OpenJpegFile
//
//
//---------------------------------------------------------------------------

void* CGraphicsJpeg::OpenJpegFile(const char* filename)
{
    FILE* fp = fopen(filename, "rb");

    if (mFileOffset > 0)
    {
        fseek(fp, mFileOffset, 0);
    }
    return fp;
}


//---------------------------------------------------------------------------
//
// Klasse:    CGraphicsJpeg
// Methode:   OpenJpegRead
//
//
//---------------------------------------------------------------------------

bool CGraphicsJpeg::OpenJpegRead(const char* filename, int* Width, int* Height, int* ByteProPixel)
{
    //Debug2 << "OpenJpegRead start " << filename << DBLF;

    memset(mJparams, 0, sizeof(SJpegParams));
    SJpegParams* jp = (SJpegParams*)mJparams;

//  Debug.Print("CGraphicsJpeg::OpenJpegRead File=", filename);

    jp->infile = (FILE*)OpenJpegFile(filename);

    if (jp->infile == NULL)
    {
        //Debug2 << "***** CGraphicsJpeg::OpenJpegRead Fehler:" << filename << DBLF;
        //fprintf(stderr, "can't open %s\n", filename);
        *Width = 0;
        *Height = 0;
        return false;
    }
    return InitJpegRead(Width, Height, ByteProPixel);
}


#if 1
//---------------------------------------------------------------------------
//
// Klasse:    CGraphicsJpeg
// Methode:   InitJpegRead
//
//
//---------------------------------------------------------------------------

bool CGraphicsJpeg::InitJpegRead(int* Width, int* Height, int* ByteProPixel)
{
    SJpegParams* jp = (SJpegParams*)mJparams;

    jp->cinfo.err = jpeg_std_error(&jp->jerr.pub);
    //jp->jerr.pub.error_exit = my_error_exit;

    if (setjmp(jp->jerr.setjmp_buffer))
    {
        jpeg_destroy_decompress(&(jp->cinfo));
        fclose(jp->infile);
        return false;
    }

    jpeg_create_decompress(&jp->cinfo);
    jpeg_stdio_src(&(jp->cinfo), jp->infile);
    jpeg_read_header(&(jp->cinfo), TRUE);

    jpeg_start_decompress(&(jp->cinfo));
    jp->row_stride = jp->cinfo.output_width * jp->cinfo.output_components;
    jp->buffer = (*(jp->cinfo.mem->alloc_sarray))
            ((j_common_ptr) &(jp->cinfo), JPOOL_IMAGE, jp->row_stride, 1);

    *Width = jp->cinfo.image_width;
    *Height = jp->cinfo.image_height;
    *ByteProPixel = jp->cinfo.output_components;
    mByteProPixel = jp->cinfo.output_components;

    //Debug2 << "CGraphicsJpeg::InitJpegRead output_components   =" << jp->cinfo.output_components << DBLF;
    //Debug2 << "CGraphicsJpeg::InitJpegRead out_color_components=" << jp->cinfo.out_color_components << DBLF;

    return true;
}


#endif

//---------------------------------------------------------------------------
//
// Klasse:    CGraphicsJpeg
// Methode:
//
//
//---------------------------------------------------------------------------

bool CGraphicsJpeg::ReadJpegLine()
{
    SJpegParams* jp = (SJpegParams*)mJparams;

    if (jp->cinfo.output_scanline < jp->cinfo.output_height)
    {
        jpeg_read_scanlines(&(jp->cinfo), jp->buffer, 1);
        put_scanline(jp->buffer[0], jp->row_stride);
        return true;
    }
    return false;
}


//---------------------------------------------------------------------------
//
// Klasse:    CGraphicsJpeg
// Methode:
//
//
//---------------------------------------------------------------------------

void CGraphicsJpeg::CloseJpeg()
{
    SJpegParams* jp = (SJpegParams*)mJparams;

    jpeg_finish_decompress(&(jp->cinfo));
    jpeg_destroy_decompress(&(jp->cinfo));
    fclose(jp->infile);
}


//---------------------------------------------------------------------------
//
// Klasse:    CGraphicsJpeg
// Methode:
//
//
//---------------------------------------------------------------------------

bool CGraphicsJpeg::GetJpegSize(const char* fname, short* xsize, short* ysize, int* channels)
{
    //Debug2 << "CGraphicsJpeg::GetJpegSize START fname=" << fname << DBLF;

    SJpegParams jp;

    jp.infile = (FILE*)OpenJpegFile(fname);

    jp.cinfo.err = jpeg_std_error(&jp.jerr.pub);
    jp.jerr.pub.error_exit = my_error_exit;

    jpeg_create_decompress(&jp.cinfo);

    jpeg_stdio_src(&(jp.cinfo), jp.infile);

    jpeg_read_header(&(jp.cinfo), TRUE);

    *xsize = jp.cinfo.image_width;
    *ysize = jp.cinfo.image_height;


    if (channels != NULL)
    {
        *channels = jp.cinfo.num_components;
    }

    jpeg_destroy_decompress(&(jp.cinfo));

    fclose(jp.infile);

    return true;
}


//---------------------------------------------------------------------------
//
// Klasse:    CGraphicsJpeg
// Methode:		ReadJpegLine
//
//
//---------------------------------------------------------------------------

bool CGraphicsJpeg::ReadJpegLine(void* PixelBuffer)
{
    SJpegParams* jp = (SJpegParams*)mJparams;

    if (jp->cinfo.output_scanline < jp->cinfo.output_height)
    {
        jpeg_read_scanlines(&(jp->cinfo), jp->buffer, 1);
        memcpy(PixelBuffer, jp->buffer[0], jp->row_stride);
        return true;
    }
    return false;
}


//---------------------------------------------------------------------------
//
// Klasse:    CGraphicsJpeg
// Methode:
//
//
//---------------------------------------------------------------------------

void CGraphicsJpeg::CloseJpegLines()
{
    SJpegParams* jp = (SJpegParams*)mJparams;

    delete jp->MainBuffer;
    jp->MainBuffer = NULL;
    CloseJpeg();
}


//---------------------------------------------------------------------------
//
// Klasse:    CGraphicsJpeg
// Methode:		ReadJpeg
//
//
//---------------------------------------------------------------------------

void* CGraphicsJpeg::ReadJpeg(const char* fname, int* Width, int* Height, int* ByteProPixel)
{
    // ,,
    if (OpenJpegRead(fname, Width, Height, ByteProPixel))
    {
        SJpegParams* jp = (SJpegParams*)mJparams;

        unsigned char* PixelBuffer = new unsigned char[(*Width) * (*Height) * (*ByteProPixel)];
        jp->GLbuffer = PixelBuffer;

        jp->image_buffer = (JSAMPLE*)jp->GLbuffer;

        jp->soll_linesize = (*Width) * (*ByteProPixel);

        while (ReadJpegLine())
        {
        }

        CloseJpeg();

        return PixelBuffer;
    }
    return NULL;
}


//---------------------------------------------------------------------------
//
// Klasse:    CGraphicsJpeg
// Methode:   ReadJpeg
//
//
//---------------------------------------------------------------------------

void* CGraphicsJpeg::ReadJpeg(const char* Filename, short* Width, short* Height, int TargetBitPerPixel)
{
    int w32, h32;

    void* PixelBuffer = ReadJpeg(Filename, &w32, &h32, &mByteProPixel);

    *Width = w32;
    *Height = h32;

    return PixelBuffer;
}


//---------------------------------------------------------------------------
//
// Klasse:    CGraphicsJpeg
// Methode:
//
//
//---------------------------------------------------------------------------

int CGraphicsJpeg::WriteJpeg(const char* fname, short xw, short yw, unsigned char* buffer, int jq)
{
    if (OpenJpegWrite(fname, xw, yw, jq))
    {
        for (int i = 0; i < yw; i++)
        {
            //cout << "Write line " << i << endl;

            WriteJpegLine(buffer);
            buffer += xw * 3;
        }
        CloseJpegWrite();
        return 1;
    }
    return 0;
}


//---------------------------------------------------------------------------
//
// Klasse:    CGraphicsJpeg
// Methode:		WriteJpeg
//
// Return:    Länge der geschriebenen Daten
//
//---------------------------------------------------------------------------

int CGraphicsJpeg::WriteJpeg(FILE* fp, short xw, short yw, unsigned char* buffer, int jq)
{
    SJpegParams* jp = (SJpegParams*)mJparams;

    jp->image_buffer = (JSAMPLE*)buffer;
    jp->image_height = yw;
    jp->image_width = xw;

    int FilePos = ftell(fp);

    write_JPEG_file(fp, jq);

    return ftell(fp) - FilePos;
}


//---------------------------------------------------------------------------
//
// Klasse:    CGraphicsJpeg
// Methode:
//
//
//---------------------------------------------------------------------------

int CGraphicsJpeg::read_JPEG_file(const char* filename)
{
    bool r;
    int Width, Height;


    if (OpenJpegRead(filename, &Width, &Height, &mByteProPixel))
    {
        while (ReadJpegLine())
        {
        }

        CloseJpeg();
        r = true;
    }
    else
    {
        r = false;
    }
    return r;
}


//---------------------------------------------------------------------------
//
// Klasse:    CGraphicsJpeg
// Methode:
//
//
//---------------------------------------------------------------------------

int CGraphicsJpeg::write_JPEG_file(const char* filename, int quality)
{
    SJpegParams* jp = (SJpegParams*)mJparams;

    struct jpeg_compress_struct cinfo;
    struct jpeg_error_mgr jerr;
    FILE* outfile;              /* target file */
    JSAMPROW row_pointer[1];    /* pointer to JSAMPLE row[s] */
    int row_stride;             /* physical row width in image buffer */

    jp->cinfo.err = jpeg_std_error(&jerr);
    jpeg_create_compress(&cinfo);

    if ((outfile = fopen(filename, "wb")) == NULL)
    {
        fprintf(stderr, "can't open %s\n", filename);
        exit(1);
    }
    jpeg_stdio_dest(&cinfo, outfile);

    cinfo.image_width = jp->image_width;    /* image width and height, in pixels */
    cinfo.image_height = jp->image_height;
    cinfo.input_components = 3;             /* # of color components per pixel */
    cinfo.in_color_space = JCS_RGB;         /* colorspace of input image */
    jpeg_set_defaults(&cinfo);
    jpeg_set_quality(&cinfo, quality, TRUE /* limit to baseline-JPEG values */);

    jpeg_start_compress(&cinfo, TRUE);

    row_stride = jp->image_width * 3; /* JSAMPLEs per row in image_buffer */

    while (cinfo.next_scanline < cinfo.image_height)
    {
        row_pointer[0] = &jp->image_buffer[cinfo.next_scanline * row_stride];
        jpeg_write_scanlines(&cinfo, row_pointer, 1);
    }

    jpeg_finish_compress(&cinfo);
    fclose(outfile);

    jpeg_destroy_compress(&cinfo);

    return 1;
}


//---------------------------------------------------------------------------
//
// Klasse:    CGraphicsJpeg
// Methode:		OpenJpegWrite
//
//
//---------------------------------------------------------------------------

bool CGraphicsJpeg::OpenJpegWrite(const char* filename, int Width, int Height, int JpegQuality)
{
    SJpegParams* jp = (SJpegParams*)mJparams;

    struct jpeg_error_mgr jerr;

    jp->image_width = Width;
    jp->image_height = Height;

    jp->cinfo2.err = jpeg_std_error(&jerr);
    jpeg_create_compress(&jp->cinfo2);

    if (mAppendMode)
    {
        //cout <<

        if ((jp->outfile = fopen(filename, "ab+")) == NULL)
        {
            return false;
        }
    }
    else
    if ((jp->outfile = fopen(filename, "wb")) == NULL)
    {
        return false;
    }
    jpeg_stdio_dest(&jp->cinfo2, jp->outfile);

    jp->cinfo2.image_width = jp->image_width;           /* image width and height, in pixels */
    jp->cinfo2.image_height = jp->image_height;
    jp->cinfo2.input_components = 3;                    /* # of color components per pixel */
    jp->cinfo2.in_color_space = JCS_RGB;                /* colorspace of input image */
    jpeg_set_defaults(&jp->cinfo2);
    jpeg_set_quality(&jp->cinfo2, JpegQuality, TRUE);   /* limit to baseline-JPEG values */

    jpeg_start_compress(&jp->cinfo2, TRUE);

    return true;
}


//---------------------------------------------------------------------------
//
// Klasse:    CGraphicsJpeg
// Methode:		CloseJpegWrite
//
//
//---------------------------------------------------------------------------


void CGraphicsJpeg::CloseJpegWrite()
{
    //cout << "CloseJpegWrite START" << endl;

    SJpegParams* jp = (SJpegParams*)mJparams;

    jpeg_finish_compress(&jp->cinfo2);

    //cout << "1" << endl;

    fclose(jp->outfile);

    //cout << "2" << endl;

    jpeg_destroy_compress(&jp->cinfo2);

    //cout << "CloseJpegWrite OK" << endl;
}


//---------------------------------------------------------------------------
//
// Klasse:    CGraphicsJpeg
// Methode:		WriteJpegLine
//
// Schreiben einer Zeile in ein JPEG-File
//
//---------------------------------------------------------------------------

void CGraphicsJpeg::WriteJpegLine(void* LineBuffer)
{
    SJpegParams* jp = (SJpegParams*)mJparams;

    JSAMPROW row_pointer[1];

    row_pointer[0] = (JSAMPROW)LineBuffer;
    jpeg_write_scanlines(&jp->cinfo2, row_pointer, 1);
}


//---------------------------------------------------------------------------
//
// Klasse:    CGraphicsJpeg
// Methode:		write_JPEG_file
//
// Schreiben in ein GEÖFFNETES Outfile
//
//---------------------------------------------------------------------------

int CGraphicsJpeg::write_JPEG_file(FILE* outfile, int quality)
{
    SJpegParams* jp = (SJpegParams*)mJparams;

    struct jpeg_compress_struct cinfo;
    struct jpeg_error_mgr jerr;
    JSAMPROW row_pointer[1];    /* pointer to JSAMPLE row[s] */

    cinfo.err = jpeg_std_error(&jerr);
    jpeg_create_compress(&cinfo);

    jpeg_stdio_dest(&cinfo, outfile);

    cinfo.image_width = jp->image_width;    /* image width and height, in pixels */
    cinfo.image_height = jp->image_height;
    cinfo.input_components = 3;             /* # of color components per pixel */
    cinfo.in_color_space = JCS_RGB;         /* colorspace of input image */
    jpeg_set_defaults(&cinfo);
    jpeg_set_quality(&cinfo, quality, TRUE /* limit to baseline-JPEG values */);

    jpeg_start_compress(&cinfo, TRUE);

    jp->row_stride = jp->image_width * 3; /* JSAMPLEs per row in image_buffer */

    while (cinfo.next_scanline < cinfo.image_height)
    {
        row_pointer[0] = &jp->image_buffer[cinfo.next_scanline * jp->row_stride];
        jpeg_write_scanlines(&cinfo, row_pointer, 1);
    }

    jpeg_finish_compress(&cinfo);

    jpeg_destroy_compress(&cinfo);

    return 1;
}


//---------------------------------------------------------------------------
//
// Klasse:    CGraphicsJpeg
// Methode:
//
//
//---------------------------------------------------------------------------

void CGraphicsJpeg::put_scanline(unsigned char* line_buffer, int line_size)
{
    SJpegParams* jp = (SJpegParams*)mJparams;

    //static int linecount = 0;

    if (line_size == jp->soll_linesize)
    {
        //GLbuffer = jp->MainBuffer;
        memcpy(jp->GLbuffer, line_buffer, line_size);
        jp->GLbuffer += line_size;
    }
    else
    if (line_size == (jp->soll_linesize / 3))
    {
        //asmSplitLine(line_buffer, jp->GLbuffer, line_size);
        jp->GLbuffer += jp->soll_linesize;
    }
}
