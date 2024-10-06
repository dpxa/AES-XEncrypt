#include <iostream>
#include <cmath>
#include <immintrin.h>

/*
measure time on windows terminal - Measure-Command {./a arg1 arg2}

Inverse square roots are used in 3d graphics to normalize vectors

From ChatGPT:

invSqrtClassic(float):
This method simply uses the standard sqrt() function provided by the <cmath>
header. It calculates the square root of a number using the traditional
method.

Q_rsqrt(float):
This method implements the fast inverse square root algorithm famously used
in the Quake III Arena game. It's a bit cryptic due to some bit-level
manipulation and magic constants. The algorithm achieves a rough approximation
of the inverse square root, which is often sufficient for certain
applications, especially in 3D graphics.

SSE_rsqrt(float):
This method uses inline assembly to directly call the rsqrtss instruction,
which computes the approximate reciprocal square root of a single-precision
floating-point value (float) using SSE (Streaming SIMD Extensions)
instructions. It's a low-level approach and might provide better performance
compared to standard methods.

SSE_rsqrt2(float):
This method uses Intel's SSE intrinsics, which are functions provided by
Intel compilers to directly use SSE instructions without writing inline
assembly. It loads the float value into an SSE register, computes the
reciprocal square root using SSE instructions, and then extracts the result
back to a float.
*/

float invSqrtClassic(float);
float Q_rsqrt(float);
float SSE_rsqrt(float);
float SSE_rsqrt2(float);

int main(int argc, char *argv[]) {
    // remember program name is argv[0]
    float number = std::stof(argv[1]);
    int method = std::stoi(argv[2]);

    switch(method) {
        case 1:
            std::cout << invSqrtClassic(number) << std::endl;
            break;
        case 2:
            std::cout << Q_rsqrt(number) << std::endl;
            break;
        case 3:
            std::cout << SSE_rsqrt(number) << std::endl;
            break;
        case 4:
            std::cout << SSE_rsqrt2(number) << std::endl;
            break;
    }

    return 0;
}

// using my very own handcrafted genius algorithm
float invSqrtClassic(float number) {
    return 1 / sqrt(number);
}

// using Fast inverse square root (Quake 3) algorithm
float Q_rsqrt(float number)
{
    long i;
    float x2, y;
    const float threehalfs = 1.5F;

    x2 = number * 0.5F;
    y  = number;
    i  = * ( long * ) &y;                       // evil floating point bit level hacking
    i  = 0x5f3759df - ( i >> 1 );               // what the f***?
    y  = * ( float * ) &i;
    y  = y * ( threehalfs - ( x2 * y * y ) );   // 1st iteration
    // y  = y * ( threehalfs - ( x2 * y * y ) );   // 2nd iteration, this can be removed

    return y;
}

// using inline assembly
float SSE_rsqrt(float x) {
    float result;
    __asm__ ("rsqrtss %1, %0" : "=x" (result) : "x" (x));
    return result;
}

// using intel intrinsics
float SSE_rsqrt2(float x) {
    __m128 xmm = _mm_set_ss(x); // Load x into SSE register
    xmm = _mm_rsqrt_ss(xmm);    // Compute reciprocal square root
    return _mm_cvtss_f32(xmm);  // Extract result from SSE register
}
