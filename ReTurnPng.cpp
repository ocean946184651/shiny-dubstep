
#include "math.h"  
#include <windows.h>
#include <tchar.h>
#include<string.h>
#include<vector>
#include<iterator>
#include<iostream>
#define represent "*.*"
using namespace std;


class FindFilePath
{
public:
	void FindFiles(LPCTSTR szPath, std::vector<wstring>& vecPath);
	vector<wstring> vecPath_without_name;
	bool check_vecPath_without_name(wstring &path);
	BOOL Check_has_0(wstring path);
};

bool FindFilePath::check_vecPath_without_name(wstring &path)//check path in check_vecPath_without_name or not 
{
  	for(vector<wstring>::iterator it_path=vecPath_without_name.begin();it_path!=vecPath_without_name.end();it_path++)
    {
     	if(wcscmp(it_path->c_str(),path.c_str())==0)
		{return false;}    
	}
	return true;
}

VOID FindFilePath::FindFiles(LPCTSTR szPath, std::vector<wstring>& vecPath)
{
	HANDLE hFind(NULL);
	WIN32_FIND_DATAW FindFileData;
	BOOL	bNext = TRUE;
	


	TCHAR szFind[MAX_PATH];
	TCHAR FileName[MAX_PATH];
	int i=0;
	lstrcpy(szFind, szPath);
	lstrcat(szFind,_T ("\\*.*")); // 找所有文件 
	

	
	hFind = FindFirstFileW(szFind, &FindFileData);
	if (hFind != INVALID_HANDLE_VALUE)
	{
		//cout<<"read success"<<endl;

		while (bNext)
		{
			if (wcscmp(FindFileData.cFileName, L".") == 0 || wcscmp(FindFileData.cFileName, L"..") == 0)//文件夹直接跳过
			{
				bNext = FindNextFileW(hFind, &FindFileData);
				continue;
			}
			if (FindFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)//是目录的话
			{
				WCHAR szFolder[MAX_PATH];
				if (szPath[wcslen(szPath) - 1] == '\\')//路径设置到下一级
					swprintf(szFolder, MAX_PATH, L"%s%s", szPath, FindFileData.cFileName);
				else
					swprintf(szFolder,MAX_PATH, L"%s\\%s", szPath, FindFileData.cFileName);
				FindFiles(szFolder, vecPath);//递归找下级文件

				bNext = FindNextFileW(hFind, &FindFileData);
				continue;
			}
			else
			{
			    wstring path=szPath;
				if(check_vecPath_without_name(path))
				{
				  vecPath_without_name.push_back(path);//绝对路径不重复
				}
			
				//swprintf(FileName,MAX_PATH,L"%s\\%s",szPath,FindFileData.cFileName);
				wstring filename=FindFileData.cFileName;
				vecPath.push_back(filename);//绝对路径下的文件名
			}
			bNext = FindNextFileW(hFind, &FindFileData);
		}
		FindClose(hFind);
	}
}


char* WcharToChar(const wchar_t* wp)  
{  
    char *m_char;
    int len= WideCharToMultiByte(CP_ACP,0,wp,wcslen(wp),NULL,0,NULL,NULL);  
    m_char=new char[len+1];  
    WideCharToMultiByte(CP_ACP,0,wp,wcslen(wp),m_char,len,NULL,NULL);  
    m_char[len]='\0';  
    return m_char;  
}  



 BOOL WStringToString(const std::wstring &wstr,std::string &str)
  {    
      int nLen = (int)wstr.length();    
     str.resize(nLen,' ');

    int nResult = WideCharToMultiByte(CP_ACP,0,(LPCWSTR)wstr.c_str(),nLen,(LPSTR)str.c_str(),nLen,NULL,NULL);
  
    if (nResult == 0)
     {
         return FALSE;
     }
  
     return TRUE;
  }


BOOL FindFilePath::Check_has_0(wstring path)
{
  vector<wstring> filename;
  FindFiles(path.c_str(), filename);
  vector <wstring>::iterator it_pngsname;
		for (it_pngsname=filename.begin();it_pngsname!=filename.end();it_pngsname++)
		{
	       string oldname;
		   char newname[MAX_PATH];
		   WStringToString(*it_pngsname,oldname);

		   string middle=oldname.substr(oldname.rfind("-")+1);
		   string num=middle.substr(0,middle.rfind("."));
		   int k= atoi(num.c_str());
		   if(k==0) return true;
		}
		return false;
}
//we test the git
//......
//test..
////test____then
//test_branch
int main()
{
	FindFilePath src;
	WCHAR szPath_src[MAX_PATH] = L"C:\\Data\\Golf_Videos_with_Impact\\extracted_frames\\front-view";
	std::vector<wstring> vecpngname_src;
	src.FindFiles(szPath_src, vecpngname_src);
	
		vector <wstring>::iterator it_path;
		for (it_path=src.vecPath_without_name.begin();it_path!=src.vecPath_without_name.end();it_path++)
		{
			string path_src;
			 WStringToString(*it_path,path_src);

	       if(src.Check_has_0(*it_path))
		   {
		      FindFilePath dst;
		      vector<wstring> filename;
              dst.FindFiles(it_path->c_str(), filename);
              vector <wstring>::iterator it_pngsname;
	        for (it_pngsname=filename.begin();it_pngsname!=filename.end();it_pngsname++)
		     {
	          string oldname;
		      char newname[MAX_PATH],oldname1[MAX_PATH];
		      WStringToString(*it_pngsname,oldname);
  
		     string middle=oldname.substr(oldname.rfind("-")+1);
		     string num=middle.substr(0,middle.rfind("."));
		     int k= atoi(num.c_str());
		     string exter=middle.substr(middle.rfind("."));
		     string path1=oldname.substr(0,oldname.rfind("-")+1);
	         sprintf(newname,"%s\\%s%04d%s",path_src.c_str(),path1.c_str(),k,exter.c_str());
		     sprintf(oldname1,"%s\\%s",path_src.c_str(),oldname.c_str());
		     rename(oldname1,newname);  
		   }
		   }
		   else
		   {
		   FindFilePath dst;
		   vector<wstring> filename;
           dst.FindFiles(it_path->c_str(), filename);
           vector <wstring>::iterator it_pngsname;
	      for (it_pngsname=filename.begin();it_pngsname!=filename.end();it_pngsname++)
		   {
	          string oldname;
		      char newname[MAX_PATH],oldname1[MAX_PATH];
		      WStringToString(*it_pngsname,oldname);
  
		     string middle=oldname.substr(oldname.rfind("-")+1);
		     string num=middle.substr(0,middle.rfind("."));
		     int k= atoi(num.c_str());
		     string exter=middle.substr(middle.rfind("."));
		     string path1=oldname.substr(0,oldname.rfind("-")+1);
	         sprintf(newname,"%s\\%s%04d%s",path_src.c_str(),path1.c_str(),k-1,exter.c_str());
		     sprintf(oldname1,"%s\\%s",path_src.c_str(),oldname.c_str());
		     rename(oldname1,newname);  
		  }
		 }
		}
}

