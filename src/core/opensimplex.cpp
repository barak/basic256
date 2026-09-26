/** OpenSimplex2 noise for BASIC-256.
 **
 ** Ported from the reference Java implementation OpenSimplex2.java by Kurt
 ** Spencer (KdotJPG), https://github.com/KdotJPG/OpenSimplex2, which is
 ** released under CC0 1.0 Universal.  CC0 is a public domain dedication, so
 ** the code may be carried into this GPL program unchanged in substance.
 **
 ** The two dimensional part and the three dimensional noise3_ImproveXY are
 ** carried over - that is what the BASIC-256 NOISE function offers.  The
 ** constants, the gradient tables, the lattice traversals and the hashes are
 ** the reference's; what changed is the language (Java to C++), the seed type
 ** (a long long rather than Java's long, which is the same 64 bits), and the
 ** multiplications and the three dimensional additions, which are done through
 ** unsigned arithmetic here because signed overflow is undefined behaviour in
 ** C++ while Java defines it to wrap.
 **/

#include "opensimplex.h"

namespace OpenSimplex2 {

	static const int64_t PRIME_X = (int64_t)0x5205402B9270C86FLL;
	static const int64_t PRIME_Y = (int64_t)0x598CD327003817B5LL;
	static const int64_t HASH_MULTIPLIER = (int64_t)0x53A3F72DEEC546F5LL;

	static const double ROOT2OVER2 = 0.7071067811865476;
	static const double SKEW_2D = 0.366025403784439;
	static const double UNSKEW_2D = -0.21132486540518713;

	static const float RSQUARED_2D = 0.5f;
	static const double NORMALIZER_2D = 0.01001634121365712;

	static const int N_GRADS_2D_EXPONENT = 7;
	static const int N_GRADS_2D = 1 << N_GRADS_2D_EXPONENT;		// 128

	// The 24 gradient directions of the reference table, 15 degrees apart.  The
	// reference divides each by NORMALIZER_2D and then cycles them into an
	// array of N_GRADS_2D pairs; 24 does not divide 128, and the wrap is on the
	// source index, so the table is not simply repeated a whole number of times.
	static const float GRAD2[] = {
		 0.38268343236509f,   0.923879532511287f,
		 0.923879532511287f,  0.38268343236509f,
		 0.923879532511287f, -0.38268343236509f,
		 0.38268343236509f,  -0.923879532511287f,
		-0.38268343236509f,  -0.923879532511287f,
		-0.923879532511287f, -0.38268343236509f,
		-0.923879532511287f,  0.38268343236509f,
		-0.38268343236509f,   0.923879532511287f,
		 0.130526192220052f,  0.99144486137381f,
		 0.608761429008721f,  0.793353340291235f,
		 0.793353340291235f,  0.608761429008721f,
		 0.99144486137381f,   0.130526192220051f,
		 0.99144486137381f,  -0.130526192220051f,
		 0.793353340291235f, -0.60876142900872f,
		 0.608761429008721f, -0.793353340291235f,
		 0.130526192220052f, -0.99144486137381f,
		-0.130526192220052f, -0.99144486137381f,
		-0.608761429008721f, -0.793353340291235f,
		-0.793353340291235f, -0.608761429008721f,
		-0.99144486137381f,  -0.130526192220052f,
		-0.99144486137381f,   0.130526192220051f,
		-0.793353340291235f,  0.608761429008721f,
		-0.608761429008721f,  0.793353340291235f,
		-0.130526192220052f,  0.99144486137381f,
	};

	static const int GRAD2_LEN = (int)(sizeof(GRAD2) / sizeof(GRAD2[0]));

	// Built once, on first use.  A function local static is initialized exactly
	// once and the C++11 rules make that thread safe, which matters only
	// because the interpreter runs on its own thread.
	struct GradientTable {
		float g[N_GRADS_2D * 2];
		GradientTable() {
			for (int i = 0, j = 0; i < N_GRADS_2D * 2; i++, j++) {
				if (j == GRAD2_LEN) j = 0;
				g[i] = (float)(GRAD2[j] / NORMALIZER_2D);
			}
		}
	};

	static const GradientTable& gradients() {
		static const GradientTable table;
		return table;
	}

	// Java's long arithmetic wraps; C++ signed overflow does not, so go through
	// unsigned for anything that can overflow.
	static inline int64_t mul(int64_t a, int64_t b) {
		return (int64_t)((uint64_t)a * (uint64_t)b);
	}

	static inline int fastFloor(double x) {
		int xi = (int)x;
		return x < xi ? xi - 1 : xi;
	}

	static float grad(int64_t seed, int64_t xsvp, int64_t ysvp, float dx, float dy) {
		int64_t hash = seed ^ xsvp ^ ysvp;
		hash = mul(hash, HASH_MULTIPLIER);
		hash ^= hash >> (64 - N_GRADS_2D_EXPONENT + 1);
		int gi = (int)hash & ((N_GRADS_2D - 1) << 1);
		const float *g = gradients().g;
		return g[gi] * dx + g[gi | 1] * dy;
	}

	// The lattice traversal, on coordinates that have already been skewed.
	static float noise2_UnskewedBase(int64_t seed, double xs, double ys) {
		int xsb = fastFloor(xs), ysb = fastFloor(ys);
		float xi = (float)(xs - xsb), yi = (float)(ys - ysb);

		int64_t xsbp = mul((int64_t)xsb, PRIME_X), ysbp = mul((int64_t)ysb, PRIME_Y);

		float t = (xi + yi) * (float)UNSKEW_2D;
		float dx0 = xi + t, dy0 = yi + t;

		float value = 0;
		float a0 = RSQUARED_2D - dx0 * dx0 - dy0 * dy0;
		if (a0 > 0) {
			value = (a0 * a0) * (a0 * a0) * grad(seed, xsbp, ysbp, dx0, dy0);
		}

		float a1 = (float)(2 * (1 + 2 * UNSKEW_2D) * (1 / UNSKEW_2D + 2)) * t +
			((float)(-2 * (1 + 2 * UNSKEW_2D) * (1 + 2 * UNSKEW_2D)) + a0);
		if (a1 > 0) {
			float dx1 = dx0 - (float)(1 + 2 * UNSKEW_2D);
			float dy1 = dy0 - (float)(1 + 2 * UNSKEW_2D);
			value += (a1 * a1) * (a1 * a1) *
				grad(seed, xsbp + PRIME_X, ysbp + PRIME_Y, dx1, dy1);
		}

		if (dy0 > dx0) {
			float dx2 = dx0 - (float)UNSKEW_2D;
			float dy2 = dy0 - (float)(UNSKEW_2D + 1);
			float a2 = RSQUARED_2D - dx2 * dx2 - dy2 * dy2;
			if (a2 > 0) {
				value += (a2 * a2) * (a2 * a2) *
					grad(seed, xsbp, ysbp + PRIME_Y, dx2, dy2);
			}
		} else {
			float dx2 = dx0 - (float)(UNSKEW_2D + 1);
			float dy2 = dy0 - (float)UNSKEW_2D;
			float a2 = RSQUARED_2D - dx2 * dx2 - dy2 * dy2;
			if (a2 > 0) {
				value += (a2 * a2) * (a2 * a2) *
					grad(seed, xsbp + PRIME_X, ysbp, dx2, dy2);
			}
		}

		return value;
	}

	double noise2(int64_t seed, double x, double y) {
		double s = SKEW_2D * (x + y);
		return (double)noise2_UnskewedBase(seed, x + s, y + s);
	}

	double noise1(int64_t seed, double x) {
		// The reference's noise2_ImproveX domain rotation with the second
		// coordinate held at zero, so the line walked through the field is not
		// parallel to any row of the lattice.
		double xx = x * ROOT2OVER2;
		double yy = 0.0;
		return (double)noise2_UnskewedBase(seed, yy + xx, yy - xx);
	}

	// ---------------------------------------------------------------------
	// Three dimensions
	// ---------------------------------------------------------------------

	static const int64_t PRIME_Z = (int64_t)0x5BCC226E9FA0BACBLL;
	static const int64_t SEED_FLIP_3D = -(int64_t)0x52D547B2E96ED629LL;

	static const double ROOT3OVER3 = 0.577350269189626;
	static const double ROTATE_3D_ORTHOGONALIZER = UNSKEW_2D;

	static const float RSQUARED_3D = 0.6f;
	static const double NORMALIZER_3D = 0.07969837668935331;

	static const int N_GRADS_3D_EXPONENT = 8;
	static const int N_GRADS_3D = 1 << N_GRADS_3D_EXPONENT;		// 256

	// The 48 gradient directions of the reference table, four floats each (the
	// fourth is padding, so an index can be built with a shift).  Built into
	// an array of N_GRADS_3D the same way as the two dimensional one.
	static const float GRAD3[] = {
		 2.22474487139f,       2.22474487139f,      -1.0f,                 0.0f,
		 2.22474487139f,       2.22474487139f,       1.0f,                 0.0f,
		 3.0862664687972017f,  1.1721513422464978f,  0.0f,                 0.0f,
		 1.1721513422464978f,  3.0862664687972017f,  0.0f,                 0.0f,
		-2.22474487139f,       2.22474487139f,      -1.0f,                 0.0f,
		-2.22474487139f,       2.22474487139f,       1.0f,                 0.0f,
		-1.1721513422464978f,  3.0862664687972017f,  0.0f,                 0.0f,
		-3.0862664687972017f,  1.1721513422464978f,  0.0f,                 0.0f,
		-1.0f,                -2.22474487139f,      -2.22474487139f,       0.0f,
		 1.0f,                -2.22474487139f,      -2.22474487139f,       0.0f,
		 0.0f,                -3.0862664687972017f, -1.1721513422464978f,  0.0f,
		 0.0f,                -1.1721513422464978f, -3.0862664687972017f,  0.0f,
		-1.0f,                -2.22474487139f,       2.22474487139f,       0.0f,
		 1.0f,                -2.22474487139f,       2.22474487139f,       0.0f,
		 0.0f,                -1.1721513422464978f,  3.0862664687972017f,  0.0f,
		 0.0f,                -3.0862664687972017f,  1.1721513422464978f,  0.0f,

		-2.22474487139f,      -2.22474487139f,      -1.0f,                 0.0f,
		-2.22474487139f,      -2.22474487139f,       1.0f,                 0.0f,
		-3.0862664687972017f, -1.1721513422464978f,  0.0f,                 0.0f,
		-1.1721513422464978f, -3.0862664687972017f,  0.0f,                 0.0f,
		-2.22474487139f,      -1.0f,                -2.22474487139f,       0.0f,
		-2.22474487139f,       1.0f,                -2.22474487139f,       0.0f,
		-1.1721513422464978f,  0.0f,                -3.0862664687972017f,  0.0f,
		-3.0862664687972017f,  0.0f,                -1.1721513422464978f,  0.0f,
		-2.22474487139f,      -1.0f,                 2.22474487139f,       0.0f,
		-2.22474487139f,       1.0f,                 2.22474487139f,       0.0f,
		-3.0862664687972017f,  0.0f,                 1.1721513422464978f,  0.0f,
		-1.1721513422464978f,  0.0f,                 3.0862664687972017f,  0.0f,
		-1.0f,                 2.22474487139f,      -2.22474487139f,       0.0f,
		 1.0f,                 2.22474487139f,      -2.22474487139f,       0.0f,
		 0.0f,                 1.1721513422464978f, -3.0862664687972017f,  0.0f,
		 0.0f,                 3.0862664687972017f, -1.1721513422464978f,  0.0f,
		-1.0f,                 2.22474487139f,       2.22474487139f,       0.0f,
		 1.0f,                 2.22474487139f,       2.22474487139f,       0.0f,
		 0.0f,                 3.0862664687972017f,  1.1721513422464978f,  0.0f,
		 0.0f,                 1.1721513422464978f,  3.0862664687972017f,  0.0f,
		 2.22474487139f,      -2.22474487139f,      -1.0f,                 0.0f,
		 2.22474487139f,      -2.22474487139f,       1.0f,                 0.0f,
		 1.1721513422464978f, -3.0862664687972017f,  0.0f,                 0.0f,
		 3.0862664687972017f, -1.1721513422464978f,  0.0f,                 0.0f,
		 2.22474487139f,      -1.0f,                -2.22474487139f,       0.0f,
		 2.22474487139f,       1.0f,                -2.22474487139f,       0.0f,
		 3.0862664687972017f,  0.0f,                -1.1721513422464978f,  0.0f,
		 1.1721513422464978f,  0.0f,                -3.0862664687972017f,  0.0f,
		 2.22474487139f,      -1.0f,                 2.22474487139f,       0.0f,
		 2.22474487139f,       1.0f,                 2.22474487139f,       0.0f,
		 1.1721513422464978f,  0.0f,                 3.0862664687972017f,  0.0f,
		 3.0862664687972017f,  0.0f,                 1.1721513422464978f,  0.0f,
	};

	static const int GRAD3_LEN = (int)(sizeof(GRAD3) / sizeof(GRAD3[0]));

	struct GradientTable3 {
		float g[N_GRADS_3D * 4];
		GradientTable3() {
			for (int i = 0, j = 0; i < N_GRADS_3D * 4; i++, j++) {
				if (j == GRAD3_LEN) j = 0;
				g[i] = (float)(GRAD3[j] / NORMALIZER_3D);
			}
		}
	};

	static const GradientTable3& gradients3() {
		static const GradientTable3 table;
		return table;
	}

	// Wrapping addition and subtraction, for the same reason as mul().
	static inline int64_t add(int64_t a, int64_t b) {
		return (int64_t)((uint64_t)a + (uint64_t)b);
	}

	static inline int64_t sub(int64_t a, int64_t b) {
		return (int64_t)((uint64_t)a - (uint64_t)b);
	}

	static inline int fastRound(double x) {
		return x < 0 ? (int)(x - 0.5) : (int)(x + 0.5);
	}

	static float grad3(int64_t seed, int64_t xrvp, int64_t yrvp, int64_t zrvp, float dx, float dy, float dz) {
		int64_t hash = (seed ^ xrvp) ^ (yrvp ^ zrvp);
		hash = mul(hash, HASH_MULTIPLIER);
		hash ^= hash >> (64 - N_GRADS_3D_EXPONENT + 2);
		int gi = (int)hash & ((N_GRADS_3D - 1) << 2);
		const float *g = gradients3().g;
		return g[gi | 0] * dx + g[gi | 1] * dy + g[gi | 2] * dz;
	}

	// Two overlapping cubic lattices, together a body centred cubic one, on
	// coordinates that have already been rotated.
	static float noise3_UnrotatedBase(int64_t seed, double xr, double yr, double zr) {
		int xrb = fastRound(xr), yrb = fastRound(yr), zrb = fastRound(zr);
		float xri = (float)(xr - xrb), yri = (float)(yr - yrb), zri = (float)(zr - zrb);

		// -1 if the offset is positive, 1 if it is negative.
		int xNSign = (int)(-1.0f - xri) | 1, yNSign = (int)(-1.0f - yri) | 1, zNSign = (int)(-1.0f - zri) | 1;

		float ax0 = xNSign * -xri, ay0 = yNSign * -yri, az0 = zNSign * -zri;

		int64_t xrbp = mul((int64_t)xrb, PRIME_X), yrbp = mul((int64_t)yrb, PRIME_Y), zrbp = mul((int64_t)zrb, PRIME_Z);

		float value = 0;
		float a = (RSQUARED_3D - xri * xri) - (yri * yri + zri * zri);
		for (int l = 0; ; l++) {

			// The closest point on this lattice.
			if (a > 0) {
				value += (a * a) * (a * a) * grad3(seed, xrbp, yrbp, zrbp, xri, yri, zri);
			}

			// The second closest.
			if (ax0 >= ay0 && ax0 >= az0) {
				float b = a + ax0 + ax0;
				if (b > 1) {
					b -= 1;
					value += (b * b) * (b * b) *
						grad3(seed, sub(xrbp, mul((int64_t)xNSign, PRIME_X)), yrbp, zrbp, xri + xNSign, yri, zri);
				}
			} else if (ay0 > ax0 && ay0 >= az0) {
				float b = a + ay0 + ay0;
				if (b > 1) {
					b -= 1;
					value += (b * b) * (b * b) *
						grad3(seed, xrbp, sub(yrbp, mul((int64_t)yNSign, PRIME_Y)), zrbp, xri, yri + yNSign, zri);
				}
			} else {
				float b = a + az0 + az0;
				if (b > 1) {
					b -= 1;
					value += (b * b) * (b * b) *
						grad3(seed, xrbp, yrbp, sub(zrbp, mul((int64_t)zNSign, PRIME_Z)), xri, yri, zri + zNSign);
				}
			}

			if (l == 1) break;

			// Move over to the other lattice copy.
			ax0 = 0.5f - ax0;
			ay0 = 0.5f - ay0;
			az0 = 0.5f - az0;

			xri = xNSign * ax0;
			yri = yNSign * ay0;
			zri = zNSign * az0;

			a += (0.75f - ax0) - (ay0 + az0);

			// The reference's (sign >> 1) & PRIME: the prime where the sign is
			// negative, nothing where it is positive.
			if (xNSign < 0) xrbp = add(xrbp, PRIME_X);
			if (yNSign < 0) yrbp = add(yrbp, PRIME_Y);
			if (zNSign < 0) zrbp = add(zrbp, PRIME_Z);

			xNSign = -xNSign;
			yNSign = -yNSign;
			zNSign = -zNSign;

			seed ^= SEED_FLIP_3D;
		}

		return value;
	}

	double noise3(int64_t seed, double x, double y, double z) {
		// The reference's noise3_ImproveXY: an orthonormal rotation that points
		// Z up the main diagonal of the lattice, so XY planes are far out of
		// line with the cube faces.  Not a skew.
		double xy = x + y;
		double s2 = xy * ROTATE_3D_ORTHOGONALIZER;
		double zz = z * ROOT3OVER3;
		double xr = x + s2 + zz;
		double yr = y + s2 + zz;
		double zr = xy * -ROOT3OVER3 + zz;
		return (double)noise3_UnrotatedBase(seed, xr, yr, zr);
	}

}
