#include "FileHelper.h"
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <stdexcept>
#include <unistd.h>
#ifndef BUILD_ON_MINGW
#include <pwd.h>
#else
#include <shlwapi.h>
#endif
#include <sys/stat.h>
#include <dirent.h>
#ifndef BUILD_ON_MINGW
#include <sys/wait.h>
#endif

string UtilFile::getHomeDir()
{
#ifndef BUILD_ON_MINGW
	struct passwd *pw = getpwuid(getuid());
	if (pw != NULL)
	{
		return pw->pw_dir;
	}
	return "";
#else
	return "~";
#endif
}

string UtilFile::getCurrentDir()
{
	char buf[PATH_MAX];
	if (getcwd(buf, sizeof(buf)))
	{
		return buf;
	}
	return "";
}

static void splitString(const string &str, const string &sep, bool bStrip, vector<string> &vResult)
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

string UtilFile::getSimplifiedPath(const string &sPath)
{
	if (sPath.empty())
	{
		return sPath;
	}

#ifndef BUILD_ON_MINGW
	vector<string> v;
	splitString(sPath, "/", true, v);
	vector<string*> vp;
	for (unsigned i = 0; i < v.size(); ++i)
	{
		if (v[i] == "..")
		{
			if (!vp.empty())
			{
				vp.resize(vp.size() - 1);
			}
		}
		else if (v[i] != ".")
		{
			vp.push_back(&v[i]);
		}
	}
	string r;
	if (sPath[0] == '/')
	{
		r.push_back('/');
	}
	for (unsigned i = 0; i < vp.size(); ++i)
	{
		if (i != 0)
		{
			r.push_back('/');
		}
		r.append(*vp[i]);
	}
	return r;
#else
	char buf[MAX_PATH];
	if (PathCanonicalize(buf, sPath.c_str()))
	{
		return buf;
	}
	return sPath;
#endif
}

string UtilFile::getAbsolutePath(const string &sPath, const string &sBasePath)
{
#ifndef BUILD_ON_MINGW
	// absolute
	if (!sPath.empty() && sPath[0] == '/')
	{
		return getSimplifiedPath(sPath);
	}
	if (!sPath.empty() && sPath[0] == '~' && (sPath.size() == 1 || sPath[1] == '/'))
	{
		return getSimplifiedPath(getHomeDir() + "/" + sPath.substr(1));
	}

	// relative
	if (sBasePath.empty())
	{
		return getSimplifiedPath(getCurrentDir() + "/" + sPath);
	}
	if (sBasePath[0] == '/')
	{
		return getSimplifiedPath(sBasePath + "/" + sPath);
	}
	if (sBasePath[0] == '~' && (sBasePath.size() == 1 || sBasePath[1] == '/'))
	{
		return getSimplifiedPath(getHomeDir() + "/" + sBasePath.substr(1) + "/" + sPath);
	}
	return getSimplifiedPath(getCurrentDir() + "/" + sBasePath + "/" + sPath);
#else
	string sPathWin = UtilString::replace(sPath, "/", "\\");
	string sBasePathWin = UtilString::replace(sBasePath, "/", "\\");

	// absolute
	if (!PathIsRelative(sPathWin.c_str()))
	{
		return getSimplifiedPath(sPathWin);
	}

	// relative
	if (sBasePathWin.empty())
	{
		return getSimplifiedPath(getCurrentDir() + "\\" + sPathWin);
	}
	if (!PathIsRelative(sBasePathWin.c_str()))
	{
		return getSimplifiedPath(sBasePathWin + "\\" + sPathWin);
	}
	return getSimplifiedPath(getCurrentDir() + "\\" + sBasePathWin + "\\" + sPathWin);
#endif
}

string UtilFile::getCanonicalPath(const string &sPath)
{
#ifndef BUILD_ON_MINGW
	char buf[PATH_MAX];
	if (realpath(sPath.c_str(), buf))
	{
		return buf;
	}
	return "";
#else
	return getAbsolutePath(sPath);
#endif
}

string UtilFile::getFileBasename(const string &sPath)
{
#ifndef BUILD_ON_MINGW
	if (sPath.empty())
	{
		return "";
	}
	string::size_type pos = sPath.size();
	while (pos > 0 && sPath[pos - 1] == '/') --pos;
	if (pos == 0)
	{
		return "/";
	}

	string::size_type end = pos;
	while (pos > 0 && sPath[pos - 1] != '/') --pos;
	return sPath.substr(pos, end - pos);
#else
	char buf[MAX_PATH];
	int n = sizeof(buf) > sPath.size() ? sPath.size() : sizeof(buf) - 1;
	strncpy(buf, sPath.c_str(), n);
	buf[n] = 0;
	for (char *p = buf; *p; ++p)
	{
		if (*p == '/')
			*p = '\\';
	}
	PathStripPath(buf);
	return buf;
#endif
}

string UtilFile::getFileDirname(const string &sPath)
{
#ifndef BUILD_ON_MINGW
	if (sPath.empty())
	{
		return ".";
	}
	string::size_type pos = sPath.size();
	while (pos > 0 && sPath[pos - 1] == '/') --pos;
	if (pos == 0)
	{
		return "/";
	}
	while (pos > 0 && sPath[pos - 1] != '/') --pos;
	if (pos == 0)
	{
		return ".";
	}
	while (pos > 0 && sPath[pos - 1] == '/') --pos;
	if (pos == 0)
	{
		return "/";
	}
	return sPath.substr(0, pos);
#else
	char buf[MAX_PATH];
	int n = sizeof(buf) > sPath.size() ? sPath.size() : sizeof(buf) - 1;
	strncpy(buf, sPath.c_str(), n);
	buf[n] = 0;
	for (char *p = buf; *p; ++p)
	{
		if (*p == '/')
			*p = '\\';
	}
	PathRemoveFileSpec(buf);
	return buf;
#endif
}

string UtilFile::getFileExt(const string &sPath, bool bWithDot)
{
	string::size_type pos = sPath.rfind('.');
	if (pos == string::npos)
	{
		return "";
	}
#ifndef BUILD_ON_MINGW
	string::size_type pos1 = sPath.rfind('/');
#else
	string::size_type pos1 = sPath.rfind('\\');
#endif
	if (pos < pos1)
	{
		return "";
	}
	return sPath.substr(bWithDot ? pos : pos + 1);
}

string UtilFile::replaceFileExt(const string &sPath, const string &sReplace, bool bWithDot)
{
	string::size_type pos = sPath.rfind('.');
	if (pos == string::npos)
	{
		return sPath;
	}
#ifndef BUILD_ON_MINGW
	string::size_type pos1 = sPath.rfind('/');
#else
	string::size_type pos1 = sPath.rfind('\\');
#endif
	if (pos1 != string::npos && pos < pos1)
	{
		return sPath;
	}
	return sPath.substr(0, bWithDot ? pos : pos + 1) + sReplace;
}

string UtilFile::loadFromFile(const string &sFileName)
{
    ifstream ifs(sFileName.c_str(), ios_base::in | ios_base::binary);
    return string(istreambuf_iterator<char>(ifs), istreambuf_iterator<char>());
}

void UtilFile::saveToFile(const string &sFileName, const string &sContent)
{
	ofstream ofs(sFileName.c_str(), ios_base::out | ios_base::binary);
	ofs.write(sContent.c_str(), sContent.size());
}

bool UtilFile::removeFile(const string &sPath)
{
	return remove(sPath.c_str()) == 0 ? true : false;
}

bool UtilFile::moveFile(const string &sPathFrom, const string &sPathTo)
{
	return rename(sPathFrom.c_str(), sPathTo.c_str()) == 0 ? true : false;
}

bool UtilFile::makeDirectory(const string &sPath)
{
#ifndef BUILD_ON_MINGW
	int ret = mkdir(sPath.c_str(), S_IRUSR | S_IWUSR | S_IXUSR | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH);
#else
	if (sPath.empty() || sPath[sPath.size() - 1] == ':' || isDirectoryExists(sPath))
	{
		return true;
	}
	int ret = mkdir(sPath.c_str());
#endif
	if (ret == 0)
	{
		return true;
	}
	if (errno == EEXIST && isDirectoryExists(sPath))
	{
		return true;
	}
	return false;
}

bool UtilFile::makeDirectoryRecursive(const string &sPath)
{
	string sAbsPath = getAbsolutePath(sPath);
	string::size_type pos = 0;
	while (true)
	{
#ifndef BUILD_ON_MINGW
		pos = sAbsPath.find('/', pos);
#else
		pos = sAbsPath.find('\\', pos);
#endif
		if (pos == string::npos)
		{
			return makeDirectory(sAbsPath);
		}
		else if (pos != 0)
		{
			string sSub = sAbsPath.substr(0, pos);
			if (!makeDirectory(sSub))
			{
				return false;
			}
		}
		++pos;
	}
	return false;
}

bool UtilFile::isFileExists(const string &sPath)
{
    struct stat buf;
    if (stat(sPath.c_str(), &buf) == 0 && S_ISREG(buf.st_mode))
    {
        return true;
    }
    return false;
}

bool UtilFile::isDirectoryExists(const string &sPath)
{
    struct stat buf;
    if (stat(sPath.c_str(), &buf) == 0 && S_ISDIR(buf.st_mode))
    {
        return true;
    }
    return false;
}

static void scanOneDirectory(const string &sPath, vector<string> &vFiles, bool bWithDirectory)
{
#ifndef BUILD_ON_MINGW
	struct dirent **namelist = NULL;
	int n = scandir(sPath.c_str(), &namelist, NULL, alphasort);
	if (n < 0)
	{
		throw std::runtime_error("list directory: scandir failed");
	}
	for (int i = 0; i < n; ++i)
	{
		if (strcmp(namelist[i]->d_name, ".") != 0 && strcmp(namelist[i]->d_name, "..") != 0)
		{
			if (bWithDirectory || namelist[i]->d_type != DT_DIR)
			{
				vFiles.push_back(namelist[i]->d_name);
			}
		}
		free(namelist[i]);
	}
	free(namelist);
#else
	DIR *dirp = opendir(sPath.c_str());
	if (dirp == NULL)
	{
		return;
	}
	struct dirent *dir = NULL;
	while ((dir = readdir(dirp)) != NULL)
	{
		if (strcmp(dir->d_name, ".") != 0 && strcmp(dir->d_name, "..") != 0)
		{
			if (bWithDirectory || UtilFile::isDirectoryExists(sPath + "\\" + dir->d_name))
			{
				vFiles.push_back(dir->d_name);
			}
		}
	}
	closedir(dirp);
#endif
}

static void scanRecDirectory(const string &sPath, const string &sPrefix, vector<string> &vFiles, bool bWithDirectory)
{
	vector<string> v;
	scanOneDirectory(sPath, v, true);
	for (unsigned i = 0; i < v.size(); ++i)
	{
		const string &sFile = v[i];
		const string &sAbsFile = sPath + "/" + sFile;
		bool bIsDir = UtilFile::isDirectoryExists(sAbsFile);
		if (!bIsDir || bWithDirectory)
		{
			if (sPrefix.empty())
			{
				vFiles.push_back(sFile);
			}
			else
			{
				vFiles.push_back(sPrefix + sFile);
			}
		}

		if (bIsDir)
		{
			scanRecDirectory(sAbsFile, sPrefix + sFile + "/", vFiles, bWithDirectory);
		}
	}
}

void UtilFile::listDirectory(const string &sPath, vector<string> &vFiles, bool bRecursive, bool bWithDirectory)
{
	if (!isDirectoryExists(sPath))
	{
		throw std::runtime_error("list directory: no such directory");
	}

	if (!bRecursive)
	{
		scanOneDirectory(sPath, vFiles, bWithDirectory);
	}
	else
	{
		scanRecDirectory(sPath, "", vFiles, bWithDirectory);
	}
}

int32_t UtilFile::doSystem(const char* pCommand)
{
#ifndef BUILD_ON_MINGW
	pid_t pid; 
	int32_t status; 

	if (pCommand == NULL) 
	{ 
		return (1); 
	} 

	if((pid = fork()) < 0)
	{ 
		status = -1; 
	} 
	else if(pid == 0) 
	{ 
		/* close all descriptors in child sysconf(_SC_OPEN_MAX) */ 
		for (int i = 3; i < ::sysconf(_SC_OPEN_MAX); i++) 
		{
			close(i); 
		}

		execl("/bin/sh", "sh", "-c", pCommand, (char *)0); 
		_exit(127); 
	} 
	else 
	{ 
		/*当进程中SIGCHLD做SIG_IGN操作时，获取不到status返回值，XOS_System无法正常工作*/
		while(::waitpid(pid, &status, 0) < 0) 
		{ 
			if(errno != EINTR) 
			{ 
				status = -1; 
				break; 
			} 
		} 
	} 

	return status; 
#else
	return ::system(pCommand);
#endif
}

int32_t UtilFile::doFile(const string &sFileName)
{
	if (sFileName.empty())
	{
		return -3;
	}
	int32_t iRet = doSystem(sFileName.c_str());
	if (iRet != 0)
	{
		throw std::runtime_error("system: " + string(strerror(errno)));
	}

	return 0;
}


