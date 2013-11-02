/** 
 *  ==============================================================================
 * 
 *          \file   common_help.h
 *
 *        \author   chenghuige  
 *          
 *          \date   2010-11-15 16:33:52.681821
 *  
 *  \Description:  FIXME remove stl_help.h
 *  ==============================================================================
 */

#ifndef COMMON_HELP_H_
#define COMMON_HELP_H_
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <stdlib.h>
#include <boost/algorithm/string.hpp>
#include <boost/format.hpp>

#include "hashmap_util.h"
typedef long long int64;
typedef unsigned int uint;
typedef unsigned long long uint64;
#include <boost/lexical_cast.hpp>
#define TO_INT boost::lexical_cast<int>
#define TO_UINT boost::lexical_cast<unsigned int>
#define TO_INT64 boost::lexical_cast<long long>
#define TO_UINT64 boost::lexical_cast<unsigned long long>
#define TO_BOOL boost::lexical_cast<bool>
#define TO_FLOAT boost::lexical_cast<float>
#define TO_DOUBLE boost::lexical_cast<double>
#define TO_STRING boost::lexical_cast<std::string>
#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
	TypeName(const TypeName&); \
	void operator=(const TypeName&)
#include <boost/algorithm/string.hpp>
#include <boost/format.hpp>
using boost::format;


//TODO 如何直接推导ValueType 下面这种不行 得到的是比如const long long &
//template<typename Iter, typename Func>
//std::size_t distinct_count(Iter begin, Iter end, Func func)
//{
//    typedef typename Func::result_type ValueType;
//    std::tr1::unordered_set<ValueType> vset;
//    for(Iter it = begin; it != end; ++it)
//    {
//        vset.insert(func(*it));
//    }
//    return vset.size();
//}
using std::string;
using boost::format;
template<typename T>
std::string join(const std::vector<T>& vec, const std::string& sep)
{
	std::stringstream s;
	size_t i = 0;
	for (; i < (int)vec.size() - 1; i++)
	{
		s << vec[i] << sep;
	}
	s << vec[i];
	return s.str();
}

template<typename T>
std::string get_jason(const std::vector<T>& vec)
{
	std::vector<string> rvec;
	for (int i = 0; i < (int)vec.size(); i++)
	{
		string kv = (format("\"%1%\":\"%2%\"") %TO_STRING(i)%TO_STRING(vec[i])).str();
		rvec.push_back(kv);
	}
	return (format("{%1%}")%join(rvec, ",")).str();
}

template<typename T>
std::string get_jason(const std::vector<T>& vec, int len)
{
	std::vector<string> rvec;
		for (int i = 0; i < len; i++)
		{
			string kv = (format("\"%1%\":\"%2%\"") %TO_STRING(i)%TO_STRING(vec[i])).str();
			rvec.push_back(kv);
		}
		return (format("{%1%}")%join(rvec, ",")).str();
}

#include <boost/bind.hpp>
template<typename ValueType, typename Iter, typename Func>
std::size_t distinct_count(Iter begin, Iter end, Func func)
{
	std::tr1::unordered_set<ValueType> vset;
	for(Iter it = begin; it != end; ++it)
	{
		vset.insert(func(*it));
	}
	return vset.size();
}
template<typename Iter>
std::size_t distinct_count(Iter begin, Iter end)
{
	typedef typename Iter::value_type ValueType;
	std::tr1::unordered_set<ValueType> vset;
	for(Iter it = begin; it != end; ++it)
	{
		vset.insert(*it);
	}
	return vset.size();
}


#include <boost/date_time/posix_time/posix_time.hpp>

class MicrosecTimer
{
public:

    MicrosecTimer() : _start_time(boost::posix_time::microsec_clock::local_time())
    {
    }

    ~MicrosecTimer()
    {
    }

    void restart()
    {
        _start_time = boost::posix_time::microsec_clock::local_time();
    }

    double elapsed()
    {
        boost::posix_time::time_duration d = boost::posix_time::microsec_clock::local_time() - _start_time;
        double result = d.ticks();
        return result /= d.ticks_per_second();
    }

	double elapsed_ms()
	{
		return elapsed() * 1000.0;
	}

private:
    boost::posix_time::ptime _start_time;
};

#endif  //----end of COMMON_HELP_H_
