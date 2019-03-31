#ifndef _CPPUTIL_BASIC_MATH_MAT_3X3_H_
#define _CPPUTIL_BASIC_MATH_MAT_3X3_H_

#include <CppUtil/Basic/Error.h>
#include <CppUtil/Basic/Vector.h>
#include <CppUtil/Basic/Quat.h>
#include <CppUtil/Basic/EulerYXZ.h>

#include <iostream>
#include <algorithm>

namespace CppUtil {
	namespace Basic {
		template <typename T>
		class Mat4x4;

		// �ڲ��洢Ϊ������
		template<typename T>
		class Mat3x3 {
		public:
			using type = Mat3x3;

		public:
			Mat3x3(T d0, T d1, T d2)
				: m{ {d0, static_cast<T>(0),static_cast<T>(0)},
			{static_cast<T>(0),d1,static_cast<T>(0)},
			{static_cast<T>(0),static_cast<T>(0),d2} } { }

			explicit Mat3x3(T d = static_cast<T>(1))
				: Mat3x3(d, d, d) { }

			// mat Ϊ������
			explicit Mat3x3(const T mat[3][3]) { memcpy(m, mat, 9 * sizeof(T)); }

			Mat3x3(
				T t00, T t01, T t02,
				T t10, T t11, T t12,
				T t20, T t21, T t22
			) : m{
			{t00, t10, t20},
			{t01, t11, t21},
			{t02, t12, t22} } { }

			Mat3x3(const Mat4x4<T> & mat) :
				m{
			{mat(0,0),mat(1,0),mat(2,0)},
			{mat(0,1),mat(1,1),mat(2,1)},
			{mat(0,2),mat(1,2),mat(2,2)}
			} { }
			
		public:
			const Vector<T> & GetCol(int i) const { return m[i]; }
			Vector<T> & GetCol(int i) { return m[i]; }

		public:
			bool IsIdentity() const {
				static constexpr T one = static_cast<T>(1);
				static constexpr T zero = static_cast<T>(0);

				return (m[0][0] == one && m[0][1] == zero && m[0][2] == zero
					&& m[1][0] == zero && m[1][1] == one && m[1][2] == zero
					&& m[2][0] == zero && m[2][1] == zero && m[2][2] == one);
			}

			T Tr() const { return m[0][0] + m[1][1] + m[2][2]; }

			Mat3x3 Transpose() const {
				return Mat3x3(m[0][0], m[1][0], m[2][0],
					m[0][1], m[1][1], m[2][1],
					m[0][2], m[1][2], m[2][2]);
			}

			Mat3x3 Inverse() const {
				static const auto & ERROR = ErrorRetVal(&Mat3x3::Inverse);
				
				T OneOverDeterminant = static_cast<T>(1) / (
					+m[0][0] * (m[1][1] * m[2][2] - m[2][1] * m[1][2])
					- m[1][0] * (m[0][1] * m[2][2] - m[2][1] * m[0][2])
					+ m[2][0] * (m[0][1] * m[1][2] - m[1][1] * m[0][2]));

				Mat3x3 Inverse;
				Inverse[0][0] = +(m[1][1] * m[2][2] - m[2][1] * m[1][2]) * OneOverDeterminant;
				Inverse[1][0] = -(m[1][0] * m[2][2] - m[2][0] * m[1][2]) * OneOverDeterminant;
				Inverse[2][0] = +(m[1][0] * m[2][1] - m[2][0] * m[1][1]) * OneOverDeterminant;
				Inverse[0][1] = -(m[0][1] * m[2][2] - m[2][1] * m[0][2]) * OneOverDeterminant;
				Inverse[1][1] = +(m[0][0] * m[2][2] - m[2][0] * m[0][2]) * OneOverDeterminant;
				Inverse[2][1] = -(m[0][0] * m[2][1] - m[2][0] * m[0][1]) * OneOverDeterminant;
				Inverse[0][2] = +(m[0][1] * m[1][2] - m[1][1] * m[0][2]) * OneOverDeterminant;
				Inverse[1][2] = -(m[0][0] * m[1][2] - m[1][0] * m[0][2]) * OneOverDeterminant;
				Inverse[2][2] = +(m[0][0] * m[1][1] - m[1][0] * m[0][1]) * OneOverDeterminant;

				return Inverse;
			}

			// ������
			T * Data() { return &(m[0][0]); }

			// ������
			const T * Data() const { return const_cast<type*>(this)->Data(); }

		public:
			bool operator ==(const Mat3x3 & rhs) const {
				return
					m[0][0] == rhs.m[0][0]
					&& m[0][1] == rhs.m[0][1]
					&& m[0][2] == rhs.m[0][2]
					&& m[1][0] == rhs.m[1][0]
					&& m[1][1] == rhs.m[1][1]
					&& m[1][2] == rhs.m[1][2]
					&& m[2][0] == rhs.m[2][0]
					&& m[2][1] == rhs.m[2][1]
					&& m[2][2] == rhs.m[2][2];
			}

			bool operator!=(const Mat3x3 & rhs) const {
				return
					m[0][0] != rhs.m[0][0]
					|| m[0][1] != rhs.m[0][1]
					|| m[0][2] != rhs.m[0][2]
					|| m[1][0] != rhs.m[1][0]
					|| m[1][1] != rhs.m[1][1]
					|| m[1][2] != rhs.m[1][2]
					|| m[2][0] != rhs.m[2][0]
					|| m[2][1] != rhs.m[2][1]
					|| m[2][2] != rhs.m[2][2];
			}

			const Mat3x3 operator*(const Mat3x3 & rhs) const {
				const auto & lhs = *this;
				T t00 = lhs(0, 0) * rhs(0, 0) + lhs(0, 1) * rhs(1, 0) + lhs(0, 2) * rhs(2, 0);
				T t01 = lhs(0, 0) * rhs(0, 1) + lhs(0, 1) * rhs(1, 1) + lhs(0, 2) * rhs(2, 1);
				T t02 = lhs(0, 0) * rhs(0, 2) + lhs(0, 1) * rhs(1, 2) + lhs(0, 2) * rhs(2, 2);
				T t10 = lhs(1, 0) * rhs(0, 0) + lhs(1, 1) * rhs(1, 0) + lhs(1, 2) * rhs(2, 0);
				T t11 = lhs(1, 0) * rhs(0, 1) + lhs(1, 1) * rhs(1, 1) + lhs(1, 2) * rhs(2, 1);
				T t12 = lhs(1, 0) * rhs(0, 2) + lhs(1, 1) * rhs(1, 2) + lhs(1, 2) * rhs(2, 2);
				T t20 = lhs(2, 0) * rhs(0, 0) + lhs(2, 1) * rhs(1, 0) + lhs(2, 2) * rhs(2, 0);
				T t21 = lhs(2, 0) * rhs(0, 1) + lhs(2, 1) * rhs(1, 1) + lhs(2, 2) * rhs(2, 1);
				T t22 = lhs(2, 0) * rhs(0, 2) + lhs(2, 1) * rhs(1, 2) + lhs(2, 2) * rhs(2, 2);
				return Mat3x3(
					t00, t01, t02,
					t10, t11, t12,
					t20, t21, t22
				);
			}

			T operator()(int row, int col) const {
				return const_cast<type*>(this)->operator()(row, col);
			}

			T & operator()(int row, int col) {
				return m[col][row];
			}

			friend std::ostream & operator<<(std::ostream & os, const Mat3x3 & mat) {
				os << "[" << mat(0, 0) << ", " << mat(0, 1) << ", " << mat(0, 2) << endl;
				os << mat(1, 0) << ", " << mat(1, 1) << ", " << mat(1, 2) << endl;
				os << mat(2, 0) << ", " << mat(2, 1) << ", " << mat(2, 2) << "]" << endl;
				return os;
			}

		private:
			// ������
			// m[i] Ϊ�� i ��
			// m[i][j] Ϊ�� j �е� j ��
			Vector<T> m[3];
		};

		using Mat3f = Mat3x3<float>;
		// using Mat3d = Mat3x3<double>;
	}

}

#endif // !_CPPUTIL_BASIC_MATH_MAT_3X3_H_