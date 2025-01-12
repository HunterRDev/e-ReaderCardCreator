// The following ifdef block is the standard way of creating macros which make exporting
// from a DLL simpler. All files within this DLL are compiled with the NEDCLIB_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see
// NEDCLIB_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef NEDCLIB_EXPORTS
#ifdef _WIN32
#define NEDCLIB_API __declspec(dllexport)
#else
#define NEDCLIB_API __attribute__ ((visibility ("default")))
#endif
#else
#ifdef _WIN32
#define NEDCLIB_API __declspec(dllimport)
#else
#define NEDCLIB_API
#endif
#endif


//#define GENERIC_DOTCODE 1

#include <stdio.h>

/*
// This class is exported from the nedclib.dll
class NEDCLIB_API Cnedclib {
public:
	Cnedclib(void);
	// TODO: add your methods here.
};*/

//Common internal functions/variables
int count_raw(FILE *f);
int read_next_raw(FILE *f, unsigned char *rawdata);

extern int bin_type;
#define BIN_TYPE_NEDC_SINGLE 0
#define BIN_TYPE_NEDC_MULTI 1
#define BIN_TYPE_DRPD_MULTI 2
//There is no DRPD single format, as there is no way to know how big each of them will be
//with certainty.

int count_bin(FILE *f);
int read_next_bin(FILE *f, unsigned char *bindata);

void backtrack_raw(FILE *f);
int close_raw(FILE *f, int return_res);

#ifdef __cplusplus
extern "C" {
#endif
//--- common external functions ---
NEDCLIB_API void nedclib_version(void);
NEDCLIB_API int is_vpk(unsigned char *bindata);
NEDCLIB_API int is_nes(unsigned char *nesdata);
NEDCLIB_API int is_bmp(char *bmpfile);

extern NEDCLIB_API int version_major;
extern NEDCLIB_API int version_minor;
extern NEDCLIB_API int MultiStrip;

//--- RAW2BMP FUNCTIONS ---
NEDCLIB_API int raw2bmp(char *rawfile, char *bmpfile);
NEDCLIB_API int bmp2raw(char *bmpfile, char *rawfile);
NEDCLIB_API int raw2bmp_f(unsigned char *rawdata, char *bmpfile);
extern NEDCLIB_API int smooth;

//--- BIN2RAW FUNCTIONS ---
NEDCLIB_API int bin2raw(char *binfile, char *rawfile);
NEDCLIB_API int raw2bin(char *rawfile, char *binfile);
NEDCLIB_API int fixraw(char *rawfile);
NEDCLIB_API int bin2raw_d(unsigned char *bindata, unsigned char *rawdata, int size);
NEDCLIB_API int bin2raw_f(unsigned char *bin, char *rawfile, int size);
extern NEDCLIB_API int signature;
extern NEDCLIB_API unsigned char signature_str[];
extern NEDCLIB_API int dpi_multiplier;

//--- NEVPK FUNCTIONS ---
NEDCLIB_API int NVPK_compress (unsigned char *buf, int size, int compression_level, int lzwindow, int lzsize, int method, FILE *f, unsigned char *bitdata=NULL);
NEDCLIB_API int vpk_decompress (unsigned char *vpk, FILE *f);
NEDCLIB_API void log_write(const char* str, ...);
extern NEDCLIB_API FILE *log_stream;
extern NEDCLIB_API int verbose;
extern NEDCLIB_API unsigned long bits_written;
extern NEDCLIB_API int best_move;
extern NEDCLIB_API int best_size;
extern NEDCLIB_API int skip_huffman;
extern NEDCLIB_API int skip_lz77;

//--- NES Functions ---
NEDCLIB_API int make_nes(unsigned char *nesdata);
NEDCLIB_API unsigned short nes_enc(unsigned short NMI_vector);
NEDCLIB_API unsigned short nes_dec(unsigned short NMI_vector);

// -- Binding functions ---
NEDCLIB_API FILE* nedc_fopen(const char* filename, const char* mode);
NEDCLIB_API int nedc_fclose(FILE *handle);

#ifdef __cplusplus
}
#endif

#define VERSION_MAJOR 1
#define VERSION_MINOR 4

#define NEDCENC_MAJOR 1
#define NEDCENC_MINOR 4
#define NEDCMAKE_MAJOR 1
#define NEDCMAKE_MINOR 4
#define NEVPK_MAJOR 1
#define NEVPK_MINOR 4
#define RAW2BMP_MAJOR 1
#define RAW2BMP_MINOR 4

#define NEDCLIB_DOWNLOAD "http://www.caitsith2.net/ereader/tools/nedclib_dll.rar"
