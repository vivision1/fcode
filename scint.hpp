#ifndef flcint_HPP_INCLUDED
#define flcint_HPP_INCLUDED
#include "regular.hpp" 
#define LRESULT __int64
#define WPARAM unsigned int
#define LPARAM int

// #include <windef.h>
struct scint{
	void* m_hScintilla=NULL;

	string filenamecurrent;
	const int blockSize = 1310720;
	int ssm(uint Msg, WPARAM wParam=0, LPARAM lParam=0);
	void save();
	void load(string filename);
	scint(int x,int y,int w, int h,void* win );
	void init(void* mainwindow=NULL);
	virtual ~scint();
	void resize(int x, int y, int w, int h);
	void GetRange(int start, int end, char *text);
	string text();
	void text(string val);
	void setStyle();
	void commentUncommment(int flag);
	void foldAll(int flag);
	void highLightAll(string word);
	void highLightbrace();
	void* mtx;
//	bool hasfocus;
	vector<void*> onUpdate;
//	int handle(int e);
};
 


#endif
