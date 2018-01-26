#ifndef CVECTOR_H
#define CVECTOR_H

#include <iostream>
//#include <iterator>
#include <ostream>
#include <cmath>
#include <typeinfo>
#include <sstream>
#include <cstring>


template<size_t N, typename type>
class CVector
{
    public:
        CVector()
        {
            memset(m_data, 0, sizeof(type) * N);
        }

//        template<typename InputIterator>
//        CVector(InputIterator& First, InputIterator & Second)
//        {
//            InputIterator Begin = First;
//            size_t i = 0;
//            while((Begin != Second) && (i < N))
//            {
//                m_data[i++] = *Begin++;
//            }
//        }
//
//
//        template<typename InputIterator, typename Distance>
//        CVector(InputIterator &Begin,  Distance n)
//        {
//            InputIterator tmp = Begin;
//            size_t i = 0;
//            while((i < n) &&(i < N))
//            {
//                m_data[i++] = *tmp++;
//            }
//
//        }

        template<typename Num>
        CVector(Num *First, Num *Second)
        {
            size_t i = 0;
            while((First < Second) && (i < N))
            {
                m_data[i++] = *First++;
            }

        }

        template<typename Num>
        CVector(Num *Begin, size_t n)
        {
            size_t i = 0;
            while((i < n) && (i < N))
            {
                m_data[i++] = *Begin++;
            }
        }
        template<typename Num>
        CVector(Num a, size_t n)
        {
            for(size_t i = 0; (i < n) && (i < N); ++i)
            {
                m_data[i] = a;
            }
        }

		template<typename Num, typename T>
		CVector(const CVector<N - 1, Num> &a, T k = T(0))
		{
			if (N == 1)
			{
				throw "Invalid construct method";
				return;
			}
			for (size_t i = 0; i < N - 1; ++i)
			{
				m_data[i] = a[i];
			}
			m_data[N - 1] = k;
		}

		template<size_t M, typename T>
		CVector(const CVector<M, T> &a)
		{
			size_t tmpMin = std::min(N, M);
			for (size_t i = 0; i < tmpMin; ++i)
			{
				m_data[i] = a[i];
			}
		}

		template<size_t M,typename T>
		CVector<N, type> operator=(const CVector<M, T> &a)
		{
			size_t tmpMin = N > M ? M : N;//std::min(N, M);
			for (size_t i = 0; i < tmpMin; ++i)
			{
				m_data[i] = a[i];
			}
			return *this;
		}

        
        ~CVector(){};

        size_t GetDimension()
        {
            return N;
        }

        bool IsZero()
        {
            for(size_t i = 0; i < N; ++i)
            {
                if(m_data[i] != type(0))
                {
                    return false;
                }
            }
            return true;
        }

        decltype(type(0) * type(0)) dotMultiply()
        {
            using reType = decltype(m_data[0] * m_data[0]);
            reType result = 0;
            for(size_t i = 0; i < N; ++i)
            {
                result += m_data[0] * m_data[0];
            }
            return result;
        }

        template<typename Num>
        decltype(type(0) * Num(0)) dotMultiply(const CVector<N, Num> &a)//->//a.m_data[0] * this->m_data[0])
        {
            using reType = decltype(a.m_data[0] * m_data[0]);
            reType result = 0;
            for(size_t i = 0; i < N; ++i)
            {
                result += a.m_data[i] * m_data[i];
            }
            return result;
        }
		CVector<N, type> Negative()
		{
			CVector<N, type> result;
			for (size_t i = 0; i < N; ++i)
			{
				result[i] = -m_data[i];
			}
			return result;
		}

		long double Length()
		{
			return sqrtl(Magnitude());
		}

		CVector<N, double> Normalize()
		{
			long double quad = Magnitude();
			long double Length = sqrtl(quad);
			CVector<N, double> result;
			for (size_t i = 0; i < N; ++i)
			{
				result[i] = m_data[i] / Length;
			}
			return result;
		}

		long double Magnitude()
		{
			long double result = 0;
			for (size_t i = 0; i < N; ++i)
			{
				result += m_data[i] * m_data[i];
			}
			return result;
		}

		template<typename T, typename Num>
		static auto CrossProduct(const CVector<3, T> &a, const CVector<3, Num> &b)->
			CVector<3, decltype(T(0) * Num(0))>
		{
			using reType = decltype(T(0) * Num(0));
			CVector<3, reType> result;
			result[0] = a[1] * b[2] - a[2] * b[1];
			result[1] = a[2] * b[0] - a[0] * b[2];
			result[2] = a[0] * b[1] - a[1] * b[0];
			return result;
		}

		template<typename T>
		auto CrossProduct(const CVector<3, T> &b)->
			CVector<3, decltype(T(0) * type(0))>
		{
			using reType = decltype(T(0) * type(0));
			CVector<3, reType> result;
			result[0] = m_data[1] * b[2] - m_data[2] * b[1];
			result[1] = m_data[2] * b[0] - m_data[0] * b[2];
			result[2] = m_data[0] * b[1] - m_data[1] * b[0];
			return result;
		}


		void NormalizeW()
		{
			if (m_data[N - 1] == type(0))
			{
				throw "error Last is zero";
			}
			for (size_t i = 0; i < N; ++i)
			{
				m_data[i] /= m_data[N - 1];
			}
		}
		void Abs()
		{
			for (size_t i = 0; i < N; ++i)
			{
				if (m_data[i] < type(0))
				{
					m_data[i] = -m_data[i];
				}
			}
		}

		std::string FormatToString()
		{
			std::stringbuf OutStr;
			std::iostream OutStream(&OutStr);
			for (size_t i = 0; i < N; ++i)
			{
				OutStream << m_data[i] << "\t";
			}
			OutStream.flush();
			return OutStr.str();
		}

		template<size_t M, typename T>
        friend std::ostream& operator<< (std::ostream &out, const CVector<M, T> & a);

        //////////////////////////////////
        
        type& operator[](size_t index)
        {
            if(index >= N)
            {
                throw "Index out of range ";
            }
            return m_data[index];
        }


        const type& operator[](size_t index) const
        {
            if(index >= N)
            {
                throw "Index out of range";
            }
            return m_data[index];
        }
		
		CVector<N, type> operator-()
		{
			CVector<N, type> result;
			for (size_t i = 0; i < N; ++i)
			{
				result[i] = -m_data[i];
			}
			return result;
		}

		template<typename Num>
        CVector<N, type>& operator+=(const CVector<N, Num> &a)
        {
            for(size_t i = 0; i < N; ++i)
            {
                m_data[i] += a[i];
            }
            return *this;
        }

        template<typename Num>
        CVector<N, type>& operator-=(const CVector<N, Num> &a)
        {
            for(size_t i = 0; i < N; ++i)
            {
                m_data[i] -= a.m_data[i];
            }
            return *this;
        }

        template<typename Num>
        CVector<N, type>& operator*=(const CVector<N, Num> &a)
        {
            for(size_t i = 0; i < N; ++i)
            {
                m_data[i] *= a.m_data[i];
            }
            return *this;
        }

        template<typename Num>
        CVector<N, type>& operator/=(const CVector<N, Num> &a)
        {
            for(size_t i = 0; i < N; ++i)
            {
                m_data[i] /= a.m_data[i];
            }
            return *this;
        }

        /////////////////////////////////
        template<typename Num>
        CVector<N, type>& operator+=(Num a)
        {
            for(size_t i = 0; i < N; ++i)
            {
                m_data[i] += a;
            }
            return *this;
        }

        template<typename Num>
        CVector<N, type>& operator-=(Num a)
        {
            for(size_t i = 0; i < N; ++i)
            {
                m_data[i] -= a;
            }
            return *this;
        }

        template<typename Num>
        CVector<N, type>& operator*=(Num a)
        {
            for(size_t i = 0; i < N; ++i)
            {
                m_data[i] *= a;
            }
            return *this;
        }

        template<typename Num>
        CVector<N, type>& operator/=(Num a)
        {
            for(size_t i = 0; i < N; ++i)
            {
                m_data[i] /= a;
            }
            return *this;
        }

        //////////////////////////////////////////////
//        template<size_t K, typename Number>
//        friend bool operator==(const CVector<N, type> &a, const CVector<K, Number> &b);
//
//        template<size_t K, typename Number>
//        friend bool operator!=(const CVector<N, type> &a, const CVector<K, Number> &b);

        template<size_t K, typename Num>
        bool operator==(const CVector<K, Num> &a) const
        {
            if(K != N)
            {
                return false;
            }
            if(typeid(m_data[0]) != typeid(a.m_data[0]))
            {
                return false;
            }
            for(size_t i = 0; i < N; ++i)
            {
                if(m_data[i] != a.m_data[i])
                {
                    return false;
                }
            }
            return true;
        }

        template<size_t K, typename Num>
        bool operator!=(const CVector<K, Num> &a) const
        {
            return !this->operator==(a);
        }

        ///////////////////////////////////////////

		template<size_t M, typename T, typename Num>
		friend CVector<M, decltype(T(0) + Num(0))>
			operator+(const CVector<M, T> &a, const CVector<M, Num> &b);

		template<size_t M, typename T, typename Num>
		friend CVector<M, decltype(T(0) - Num(0))>
			operator-(const CVector<M, T> &a, const CVector<M, Num> &b);

		template<size_t M, typename T, typename Num>
		friend CVector<M, decltype(T(0) * Num(0))>
			operator*(const CVector<M, T> &a, const CVector<M, Num> &b);

		template<size_t M, typename T, typename Num>
		friend CVector<M, decltype(T(0) / Num(0))>
			operator/(const CVector<M, T> &a, const CVector<M, Num> &b);

        ////////////////////////////////////
        template<size_t M, typename K, typename T>
        friend CVector<M, decltype(K(0) * T(0))> \
        operator*(const CVector<M, K> &a, T k);

		template<size_t M, typename K, typename T>
		friend CVector<M, decltype(K(0) / T(0))> \
			operator/(const CVector<M, K> &a, T k);

		template<size_t M, typename K, typename T>
		friend CVector<M, decltype(K(0) + T(0))> \
			operator+(const CVector<M, K> &a, T k);

		template<size_t M, typename K, typename T>
		friend CVector<M, decltype(K(0) - T(0))> \
			operator-(const CVector<M, K> &a, T k);

        ///////////////////////////////////////////

        template<size_t M, typename T, typename Num>
        friend CVector<M, decltype(Num(0) * T(0))> \
        operator*(Num k, const CVector<M, T> &a);

		template<size_t M, typename T, typename Num>
		friend CVector<M, decltype(Num(0) + T(0))> \
			operator+(Num k, const CVector<M, T> &a);


		template<size_t M, typename T, typename Num>
		friend CVector<M, decltype(Num(0) - T(0))> \
			operator-(Num k, const CVector<M, T> &a);


		template<size_t M, typename T, typename Num>
		friend CVector<M, decltype(Num(0) / T(0))> \
			operator/(Num k, const CVector<M, T> &a);
    private:
        type m_data[N];
};


template<size_t M, typename T>
std::ostream& operator<< (std::ostream &out, const CVector<M, T> & a)
{
    size_t i = 0;
    while(i < M)
    {
        out << a[i++] << " ";
    }
    out << "\n";
    return out;
}

//////template<size_t N, typename type, size_t K, typename Number>
//////bool operator==(const CVector<N, type> &a, const CVector<K, Number> &b)
//////{
//////    if(N != K)
//////    {
//////        return false;
//////    }
//////    if(typeid(a.m_data[0]) != typeid(b.m_data[0]))
//////    {
//////        return false;
//////    }
//////    for(size_t i = 0; i < N; ++i)
//////    {
//////        if(a.m_data[i] != b.m_data[i])
//////        {
//////            return false;
//////        }
//////    }
//////    return true;
//////}
//////
//////template<size_t N, typename type, size_t K, typename Number>
//////bool operator!=(const CVector<N, type> &a, const CVector<K, Number> &b)
//////{
//////    return !operator==(a, b);
//////}





template<size_t M, typename T, typename Num>
CVector<M, decltype(T(0) + Num(0))> 
operator+(const CVector<M, T> &a, const CVector<M, Num> &b)
{
    using reType = decltype(T(0) + Num(0));
    CVector<M, reType> result;
    for(size_t i = 0; i < M; ++i)
    {
        result.m_data[i] = a.m_data[i] + b.m_data[i];
    }
    return result;
}


template<size_t M, typename T, typename Num>
CVector<M, decltype(T(0) - Num(0))>
operator-(const CVector<M, T> &a, const CVector<M, Num> &b)
{
	using reType = decltype(T(0) - Num(0));
	CVector<M, reType> result;
	for (size_t i = 0; i < M; ++i)
	{
		result.m_data[i] = a.m_data[i] - b.m_data[i];
	}
	return result;
}

template<size_t M, typename T, typename Num>
CVector<M, decltype(T(0) * Num(0))>
operator*(const CVector<M, T> &a, const CVector<M, Num> &b)
{
	using reType = decltype(T(0) * Num(0));
	CVector<M, reType> result;
	for (size_t i = 0; i < M; ++i)
	{
		result.m_data[i] = a.m_data[i] * b.m_data[i];
	}
	return result;
}

template<size_t M, typename T, typename Num>
CVector<M, decltype(T(0) / Num(0))>
operator/(const CVector<M, T> &a, const CVector<M, Num> &b)
{
	using reType = decltype(T(0) / Num(0));
	CVector<M, reType> result;
	for (size_t i = 0; i < M; ++i)
	{
		result.m_data[i] = a.m_data[i] / b.m_data[i];
	}
	return result;
}


template<size_t M, typename K, typename T>
CVector<M, decltype(K(0)*T(0))> operator*(const CVector<M, K>& a, T k)
{
	using reType = decltype(K(0) * k);
	CVector<M, reType> result(k, M);
	return operator*(result, a);
}

template<size_t M, typename K, typename T>
CVector<M, decltype(K(0) / T(0))> operator/(const CVector<M, K>& a, T k)
{

	using reType = decltype(K(0) / k);
	CVector<M, reType> result(k, M);
	return operator/(a, result);
}

template<size_t M, typename K, typename T>
CVector<M, decltype(K(0) + T(0))> operator+(const CVector<M, K>& a, T k)
{
	using reType = decltype(K(0) + k);
	CVector<M, reType> result(k, M);
	return operator+(a, result);
}

template<size_t M, typename K, typename T>
CVector<M, decltype(K(0) - T(0))> operator-(const CVector<M, K>& a, T k)
{
	using reType = decltype(K(0) - k);
	CVector<M, reType> result(k, M);
	return operator-(a, result);
}


template<size_t M, typename T, typename Num>
CVector<M, decltype(Num(0) * T(0))> operator*(Num k, const CVector<M, T> &a)
{
	using reType = decltype(k * a.m_data[0]);
	CVector<M, reType> result(k, M);
	return operator*(result, a);
}

template<size_t M, typename T, typename Num>
CVector<M, decltype(Num(0) + T(0))> operator+(Num k, const CVector<M, T> &a)
{
	using reType = decltype(k + a.m_data[0]);
	CVector<M, reType> result(k, M);
	return operator+(result, a);
}


template<size_t M, typename T, typename Num>
CVector<M, decltype(Num(0) - T(0))> operator-(Num k, const CVector<M, T> &a)
{
	using reType = decltype(k - a.m_data[0]);
	CVector<M, reType> result(k, M);
	return operator-(result, a);
}


template<size_t M, typename T, typename Num>
CVector<M, decltype(Num(0) / T(0))> operator/(Num k, const CVector<M, T> &a)
{
	using reType = decltype(Num(0) / T(0));
	CVector<M, reType> result(k, M);
	return operator/(result, a);
}



#endif // CVECTOR_H
