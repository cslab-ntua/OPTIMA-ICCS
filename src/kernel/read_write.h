// Includes
#include <ap_int.h>
#include <hls_stream.h>

static void read(float* in, hls::stream<float>& inStream, int size, int incx);
static void write(float* out, hls::stream< float>& outStream, int size,int incy);
