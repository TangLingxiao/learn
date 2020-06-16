#include "StringHelper.h"
#include <cstdio>
#include <cstring>
#include <cstdarg>
#include <stdexcept>

string UtilString::trim(const string &s)
{
	string::size_type i = 0, j = s.size();
	while (i < j && isspace(s[i])) ++i;
	while (i < j && isspace(s[j - 1])) --j;
	return s.substr(i, j - i);
}

string UtilString::trimLeft(const string &s)
{
	string::size_type i = 0, j = s.size();
	while (i < j && isspace(s[i])) ++i;
	return s.substr(i, j - i);
}

string UtilString::trimRight(const string &s)
{
	string::size_type i = 0, j = s.size();
	while (i < j && isspace(s[j - 1])) --j;
	return s.substr(i, j - i);
}

bool UtilString::isTrimEmpty(const string &s)
{
	string::size_type i = 0, j = s.size();
	while (i < j && isspace(s[i])) ++i;
	while (i < j && isspace(s[j - 1])) --j;
	return i >= j;
}

string UtilString::toupper(const string &s)
{
	string r = s;
	toupperInplace(r);
	return r;
}

string UtilString::tolower(const string &s)
{
	string r = s;
	tolowerInplace(r);
	return r;
}

void UtilString::toupperInplace(string &s)
{
	for (string::size_type i = 0; i < s.size(); ++i)
	{
		s[i] = ::toupper(s[i]);
	}
}

void UtilString::tolowerInplace(string &s)
{
	for (string::size_type i = 0; i < s.size(); ++i)
	{
		s[i] = ::tolower(s[i]);
	}
}

bool UtilString::isContains(const string &s, const string &match)
{
	return s.find(match) != string::npos;
}

bool UtilString::isBeginsWith(const string &s, const string &match)
{
	if (match.size() > s.size())
	{
		return false;
	}
	return memcmp(s.data(), match.data(), match.size()) == 0 ? true : false;
}

bool UtilString::isEndsWith(const string &s, const string &match)
{
	if (match.size() > s.size())
	{
		return false;
	}
	return memcmp(s.data() + (s.size() - match.size()), match.data(), match.size()) == 0 ? true : false;
}

bool UtilString::isdigit(const string &s)
{
	for (string::size_type i = 0; i < s.size(); ++i)
	{
		if (i == 0 && s[i] == '-' && s.size() > 1) //支持负数，跳过前面的负号
        {
            continue;
        }
		if (!::isdigit(s[i]))
		{
			return false;
		}
	}
	return true;
}

bool UtilString::isxdigit(const string &s)
{
	for (string::size_type i = 0; i < s.size(); ++i)
	{
		if (!::isxdigit(s[i]))
		{
			return false;
		}
	}
	return true;
}

bool UtilString::isalnum(const string &s)
{
	for (string::size_type i = 0; i < s.size(); ++i)
	{
		if (!::isalnum(s[i]))
		{
			return false;
		}
	}
	return true;
}

bool UtilString::isalpha(const string &s)
{
	for (string::size_type i = 0; i < s.size(); ++i)
	{
		if (!::isalpha(s[i]))
		{
			return false;
		}
	}
	return true;
}

bool UtilString::islower(const string &s)
{
	for (string::size_type i = 0; i < s.size(); ++i)
	{
		if (!::islower(s[i]))
		{
			return false;
		}
	}
	return true;
}

bool UtilString::isupper(const string &s)
{
	for (string::size_type i = 0; i < s.size(); ++i)
	{
		if (!::isupper(s[i]))
		{
			return false;
		}
	}
	return true;
}

string UtilString::replace(const string &s, const string &from, const string &to)
{
	if (from.empty())
	{
		return s;
	}

	string r;
	const char *begin = s.data();
	string::size_type p = 0;
	string::size_type q = s.find(from);
	while (q != string::npos)
	{
		r.append(begin + p, begin + q);
		r.append(to);
		p = q + from.size();
		q = s.find(from, p);
	}
	r.append(begin + p, begin + s.size());
	return r;
}

string UtilString::format(const char *fmt, ...)
{
	string res;
	char buf[16];
	va_list args;
	va_start(args, fmt);
	int n = vsnprintf(buf, sizeof(buf), fmt, args);
	if (n >= 0 && n < static_cast<int>(sizeof(buf)))
	{
		res = string(buf, buf + n);
	}
	else if (n < 0)
	{
		res = "";
	}
	else
	{
		char *data = new char[n + 1];
		n = vsnprintf(data, n + 1, fmt, args);
		res = string(data, data + n);
		delete []data;
	}
	va_end(args);
	return res;
}

void UtilString::splitString(const string &str, const string &sep, bool bStrip, vector<string> &vResult)
{
	string::size_type p = 0, q = 0;
	while (true)
	{
		q = str.find_first_of(sep, p);
		if (q == string::npos)
		{
			if (!bStrip || p < str.size())
			{
				vResult.push_back(str.substr(p));
			}
			break;
		}
		if (q == p)
		{
			if (!bStrip)
			{
				vResult.push_back("");
			}
		}
		else
		{
			vResult.push_back(str.substr(p, q - p));
			p = q;
		}
		++p;
	}
}

vector<string> UtilString::splitString(const string &str, const string &sep, bool bStrip)
{
	vector<string> vResult;
	splitString(str, sep, bStrip, vResult);
	return vResult;
}

string UtilString::joinString(const vector<string> &v, const string &sep)
{
	string sResult;
	for (unsigned i = 0; i < v.size(); ++i)
	{
		if (!sResult.empty())
		{
			sResult.append(sep);
		}
		sResult.append(v[i]);
	}
	return sResult;
}


void UtilString::splitString2(const string &str, const string &sep1, const string &sep2, vector<pair<string, string>> &vResult)
{
	vector<string> vData;
	UtilString::splitString(str, sep1, true, vData);
	for (unsigned i = 0; i < vData.size(); ++i)
	{
		const string &sData = vData[i];
		string::size_type pos = sData.find_first_of(sep2);
		if (pos == string::npos)
		{
			vResult.push_back(make_pair(sData, ""));
		}
		else
		{
			vResult.push_back(make_pair(sData.substr(0, pos), sData.substr(pos + 1)));
		}
	}
}

void UtilString::splitString2(const string &str, const string &sep1, const string &sep2, map<string, string> &mResult)
{
	vector<pair<string, string>> vResult;
	splitString2(str, sep1, sep2, vResult);
	for (unsigned i = 0; i < vResult.size(); ++i)
	{
		const pair<string, string> &kv = vResult[i];
		mResult[kv.first] = kv.second;
	}
}

map<string, string> UtilString::splitString2(const string &str, const string &sep1, const string &sep2)
{
	map<string, string> mResult;
	splitString2(str, sep1, sep2, mResult);
	return mResult;
}

string UtilString::joinString2(const vector<pair<string, string>> &v, const string &sep1, const string &sep2)
{
	string sResult;
	for (unsigned i = 0; i < v.size(); ++i)
	{
		if (!sResult.empty())
		{
			sResult.append(sep1);
		}

		const pair<string, string> &kv = v[i];
		sResult.append(kv.first);
		sResult.append(sep2);
		sResult.append(kv.second);
	}
	return sResult;
}

string UtilString::repeat(const string &s, uint32_t n)
{
	string r;
	for (uint32_t i = 0; i < n; ++i)
	{
		r += s;
	}
	return r;
}

string UtilString::repeat(const string &s, const string &sep, uint32_t n)
{
	string r;
	for (uint32_t i = 0; i < n; ++i)
	{
		if (i != 0)
		{
			r += sep;
		}
		r += s;
	}
	return r;
}

static const string g_sEmptyString;
const string &UtilString::getEmptyString()
{
	return g_sEmptyString;
}

uint64_t UtilString::parseHumanReadableSize(const string &s)
{
	if (s.empty())
	{
		return 0;
	}

	uint64_t iSize = UtilString::strto<uint64_t>(s);
	char c = s[s.size() - 1];
	if (c == 'k' || c == 'K')
	{
		iSize *= 1024;
	}
	else if (c == 'm' || c == 'M')
	{
		iSize *= 1024 * 1024;
	}
	else if (c == 'g' || c == 'G')
	{
		iSize *= 1024 * 1024 * 1024;
	}
	return iSize;
}


string UtilString::replaceVariable(const string &s, const map<string, string> &mVariable, REPACE_MODE mode)
{
	string sResult;
	const char *p = s.c_str(), *q = p + s.size();
	while (p < q)
	{
		const char *t1 = strstr(p, "${");
		if (t1 == NULL)
		{
			sResult.append(p, q);
			break;
		}
		sResult.append(p, t1);

		const char *t2 = strchr(t1, '}');
		if (t2 == NULL)
		{
			sResult.append(t1, q);
			break;
		}

		string sKey = string(t1 + 2, t2);
		map<string, string>::const_iterator it = mVariable.find(sKey);
		if (it != mVariable.end())
		{
			sResult.append(it->second);
		}
		else if (mode == REPLACE_MODE_ALL_MATCH)
		{
			throw std::runtime_error("replaceVariable: " + sKey);
		}
		else if (mode == REPLACE_MODE_KEEP_VARIABLE_ON_MISS)
		{
			sResult.append(t1, t2 + 1);
		}
		else if (mode == REPLACE_MODE_EMPTY_VARIABLE_ON_MISS)
		{
		}
		else if (mode == REPLACE_MODE_EMPTY_RESULT_ON_MISS)
		{
			return "";
		}

		p = t2 + 1;
	}
	return sResult;
}
