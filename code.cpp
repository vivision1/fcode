#define appname "FastCoder.v2"

// #include "filesystem.hpp"
#include "regular.hpp"
#include <iostream>
#include <unordered_map>

#include <gtk/gtk.h>

#include <Scintilla.h>
#include <SciLexer.h>  
#include <ILexer.h> 
#include <ScintillaWidget.h>

#define UNUSE_ATTR	__attribute__((unused)) 
#define ssm(m, w, l) scintilla_send_message(sci, (m), (w), (l))
#define MARGIN_SCRIPT_FOLD_INDEX 1

#define WPARAM long long unsigned int
#define LPARAM long long int
#define LRESULT long long int

unordered_map<string,LRESULT> files;

enum {
	COLUMN = 0,
	PTF,
	NUM_COLS
};
struct ptfs{
	GtkTreeIter gti;
	string path;
	string fpath;
	uptr_t scrollpos=-1;
	bool saved=1;
	
	GtkWidget* scint=0;
	GtkWidget* scint2=0; //for splitview
};

GtkWidget * window;
GtkWidget * gtf;
ptfs* ptfcurr=0;
ScintillaObject *scicurr=0;
vector<ptfs*> editorptf(2);
vector<GtkWidget*> weditor(2);
vector<GtkWidget*> editor(2);   
GtkWidget *wtree;
GtkWidget *tree;
GtkTreeStore *treestore; 
vector<GtkTreeIter> ichild;

#if 1
#include <algorithm>
#include <boost/filesystem.hpp> 

template <typename  T> void dbg(T current)   try {   cout<<current; fflush(stdout);  }catch (...) {}
template void dbg<int>(int current);
template void dbg<const char*>(const char* current);
template void dbg<const unsigned char*>(const unsigned char* current);
template void dbg<const wchar_t*>(const wchar_t* current);
template void dbg<wchar_t*>(wchar_t* current);
template void dbg<string>(string current);
template void dbg<char*>(char* current);
template void dbg<char>(char current);
template void dbg<short>(short current);
template void dbg<unsigned short>(unsigned short current);
template void dbg<unsigned char>(unsigned char current);
template void dbg<void*>(void* current);
template void dbg<bool>(bool current);
//template void dbg<_Bit_reference>(_Bit_reference current);
template void dbg<float>(float current);
template void dbg<double>(double current);
template void dbg<unsigned long long>(unsigned long long current);
template void dbg< long >(  long current);
template void dbg< long long>( long long current);
template void dbg<long unsigned int>(long unsigned int current);
template void dbg<unsigned int>(unsigned int current);

#include <boost/thread/thread.hpp>
#include <boost/thread/mutex.hpp> 
void* threadDetach(std::function<void()> tf){
    boost::thread th(tf);
//    SetPriorityClass(th.native_handle(), REALTIME_PRIORITY_CLASS);
    th.detach();
//    boost::thread(tf).detach();
//	return (void*)&th;
	return (void*)th.native_handle();
}

void sleep(int milisecs){
	boost::this_thread::sleep_for( boost::chrono::milliseconds(milisecs) );
}
bool fileExist(string fname){
	return boost::filesystem::exists(fname);
}

vstring split(const string& s, const string delim, const bool keep_empty = true);
vector<string> split(const string& s, const string delim, const bool keep_empty) {
	vector<string> result;
	if (delim.empty()) {result.push_back(s);
	return result;}
	string::const_iterator substart = s.begin(), subend;while (true) {
		subend = search(substart, s.end(), delim.begin(), delim.end());	
		string temp(substart, subend);	if (keep_empty || !temp.empty()) {	
			result.push_back(temp);	}if (subend == s.end()) {break;	}
			substart = subend + delim.size();	}	
	return result;
}
// The main function that checks if two given strings 
// match. The first string may contain wildcard characters 
bool stringMatch(const char *first,const char * second) { 
	// If we reach at the end of both strings, we are done 
	if (*first == '\0' && *second == '\0') 
		return true; 

	// Make sure that the characters after '*' are present 
	// in second string. This function assumes that the first 
	// string will not contain two consecutive '*' 
	if (*first == '*' && *(first+1) != '\0' && *second == '\0') 
		return false; 

	// If the first string contains '?', or current characters 
	// of both strings match 
	if (*first == '?' || *first == *second) 
		return stringMatch(first+1, second+1); 

	// If there is *, then there are two possibilities 
	// a) We consider current character of second string 
	// b) We ignore current character of second string. 
	if (*first == '*') 
		return stringMatch(first+1, second) || stringMatch(first, second+1); 
	return false; 
} 
string toLower(string data){std::transform(data.begin(), data.end(), data.begin(), ::tolower);return data;}
string ExtractFilename( const std::string& pathf ){
	boost::filesystem::path path(pathf);
	return path.filename().string();
};
string ExtractDirectory( string pathf) {
	boost::filesystem::path path(pathf);
	return path.parent_path().string();
};	

vector<string> dirWildcard(string path,string wildcard,bool onlyfilename=1);
vstring dirWildcard(string path,string wildcard,bool onlyfilename){
	vstring res;
	if(!fileExist(path))return res;
	wildcard=toLower(wildcard);
	vstring w=split(wildcard,";");
	string fname;
	string fnamewpath;
	bool add;
	for(auto & p : boost::filesystem::directory_iterator( path )){ 
		const boost::filesystem::path cp = (p);
		fnamewpath=cp.string();
		fname=ExtractFilename(fnamewpath);
		add=0;
		lop(i,0,w.size()){
			if(stringMatch(w[i].c_str(),toLower(fname).c_str())){ add=1; break;}
		}
		if(!add)continue;
		if(onlyfilename)
			res.push_back(fname);
		else
			res.push_back(fnamewpath);
	}
	return res;
}
#define NeedleThreshold2vs4swampLITE 9+10 // Should be bigger than 9. BMH2 works up to this value (inclusive), if bigger then BMH4 takes over.
char * Railgun_Swampshine_BailOut (char * pbTarget, char * pbPattern, uint32_t cbTarget, uint32_t cbPattern){
	char * pbTargetMax = pbTarget + cbTarget;
	register uint32_t ulHashPattern;
	signed long count;

	unsigned char bm_Horspool_Order2[256*256]; // Bitwise soon...
	uint32_t i, Gulliver;

	uint32_t PRIMALposition, PRIMALpositionCANDIDATE;
	uint32_t PRIMALlength, PRIMALlengthCANDIDATE;
	uint32_t j, FoundAtPosition;

	if (cbPattern > cbTarget) return(NULL);

	if ( cbPattern<4 ) {
		// SSE2 i.e. 128bit Assembly rules here:
		// ...
        	pbTarget = pbTarget+cbPattern;
		ulHashPattern = ( (*(char *)(pbPattern))<<8 ) + *(pbPattern+(cbPattern-1));
		if ( cbPattern==3 ) {
			for ( ;; ) {
				if ( ulHashPattern == ( (*(char *)(pbTarget-3))<<8 ) + *(pbTarget-1) ) {
					if ( *(char *)(pbPattern+1) == *(char *)(pbTarget-2) ) return((pbTarget-3));
				}
				if ( (char)(ulHashPattern>>8) != *(pbTarget-2) ) {
					pbTarget++;
					if ( (char)(ulHashPattern>>8) != *(pbTarget-2) ) pbTarget++;
				}
				pbTarget++;
				if (pbTarget > pbTargetMax) return(NULL);
			}
		} else {
		}
		for ( ;; ) {
			if ( ulHashPattern == ( (*(char *)(pbTarget-2))<<8 ) + *(pbTarget-1) ) return((pbTarget-2));
			if ( (char)(ulHashPattern>>8) != *(pbTarget-1) ) pbTarget++;
			pbTarget++;
			if (pbTarget > pbTargetMax) return(NULL);
		}
	} else { //if ( cbPattern<4 )
		if ( cbPattern<=NeedleThreshold2vs4swampLITE ) {
			// BMH order 2, needle should be >=4:
			ulHashPattern = *(uint32_t *)(pbPattern); // First four bytes
			for (i=0; i < 256*256; i++) {bm_Horspool_Order2[i]=0;}
			for (i=0; i < cbPattern-1; i++) bm_Horspool_Order2[*(unsigned short *)(pbPattern+i)]=1;
			i=0;
			while (i <= cbTarget-cbPattern) {
				Gulliver = 1; // 'Gulliver' is the skip
				if ( bm_Horspool_Order2[*(unsigned short *)&pbTarget[i+cbPattern-1-1]] != 0 ) {
					if ( bm_Horspool_Order2[*(unsigned short *)&pbTarget[i+cbPattern-1-1-2]] == 0 ) Gulliver = cbPattern-(2-1)-2; else {
						if ( *(uint32_t *)&pbTarget[i] == ulHashPattern) { // This fast check ensures not missing a match (for remainder) when going under 0 in loop below:
							count = cbPattern-4+1;
							while ( count > 0 && *(uint32_t *)(pbPattern+count-1) == *(uint32_t *)(&pbTarget[i]+(count-1)) )
								count = count-4;
							if ( count <= 0 ) return(pbTarget+i);
						}
					}
				} else Gulliver = cbPattern-(2-1);
				i = i + Gulliver;
				//GlobalI++; // Comment it, it is only for stats.
			}
			return(NULL);
		} else { // if ( cbPattern<=NeedleThreshold2vs4swampLITE )

// Swampwalker_BAILOUT heuristic order 4 (Needle should be bigger than 4) [
// Needle: 1234567890qwertyuiopasdfghjklzxcv            PRIMALposition=01 PRIMALlength=33  '1234567890qwertyuiopasdfghjklzxcv'
// Needle: vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv            PRIMALposition=29 PRIMALlength=04  'vvvv'
// Needle: vvvvvvvvvvBOOMSHAKALAKAvvvvvvvvvv            PRIMALposition=08 PRIMALlength=20  'vvvBOOMSHAKALAKAvvvv'
// Needle: Trollland                                    PRIMALposition=01 PRIMALlength=09  'Trollland'
// Needle: Swampwalker                                  PRIMALposition=01 PRIMALlength=11  'Swampwalker'
// Needle: licenselessness                              PRIMALposition=01 PRIMALlength=15  'licenselessness'
// Needle: alfalfa                                      PRIMALposition=02 PRIMALlength=06  'lfalfa'
// Needle: Sandokan                                     PRIMALposition=01 PRIMALlength=08  'Sandokan'
// Needle: shazamish                                    PRIMALposition=01 PRIMALlength=09  'shazamish'
// Needle: Simplicius Simplicissimus                    PRIMALposition=06 PRIMALlength=20  'icius Simplicissimus'
// Needle: domilliaquadringenquattuorquinquagintillion  PRIMALposition=01 PRIMALlength=32  'domilliaquadringenquattuorquinqu'
// Needle: boom-boom                                    PRIMALposition=02 PRIMALlength=08  'oom-boom'
// Needle: vvvvv                                        PRIMALposition=01 PRIMALlength=04  'vvvv'
// Needle: 12345                                        PRIMALposition=01 PRIMALlength=05  '12345'
// Needle: likey-likey                                  PRIMALposition=03 PRIMALlength=09  'key-likey'
// Needle: BOOOOOM                                      PRIMALposition=03 PRIMALlength=05  'OOOOM'
// Needle: aaaaaBOOOOOM                                 PRIMALposition=02 PRIMALlength=09  'aaaaBOOOO'
// Needle: BOOOOOMaaaaa                                 PRIMALposition=03 PRIMALlength=09  'OOOOMaaaa'
PRIMALlength=0;
for (i=0+(1); i < cbPattern-((4)-1)+(1)-(1); i++) { // -(1) because the last BB order 4 has no counterpart(s)
	FoundAtPosition = cbPattern - ((4)-1) + 1;
	PRIMALpositionCANDIDATE=i;
	while ( PRIMALpositionCANDIDATE <= (FoundAtPosition-1) ) {
		j = PRIMALpositionCANDIDATE + 1;
		while ( j <= (FoundAtPosition-1) ) {
			if ( *(uint32_t *)(pbPattern+PRIMALpositionCANDIDATE-(1)) == *(uint32_t *)(pbPattern+j-(1)) ) FoundAtPosition = j;
			j++;
		}
		PRIMALpositionCANDIDATE++;
	}
	PRIMALlengthCANDIDATE = (FoundAtPosition-1)-i+1 +((4)-1);
	if (PRIMALlengthCANDIDATE >= PRIMALlength) {PRIMALposition=i; PRIMALlength = PRIMALlengthCANDIDATE;}
	if (cbPattern-i+1 <= PRIMALlength) break;
	if (PRIMALlength > 128) break; // Bail Out for 129[+]
}
// Swampwalker_BAILOUT heuristic order 4 (Needle should be bigger than 4) ]

// Here we have 4 or bigger NewNeedle, apply order 2 for pbPattern[i+(PRIMALposition-1)] with length 'PRIMALlength' and compare the pbPattern[i] with length 'cbPattern':
PRIMALlengthCANDIDATE = cbPattern;
cbPattern = PRIMALlength;
pbPattern = pbPattern + (PRIMALposition-1);

		if ( cbPattern<=NeedleThreshold2vs4swampLITE ) {

			// BMH order 2, needle should be >=4:
			ulHashPattern = *(uint32_t *)(pbPattern); // First four bytes
			for (i=0; i < 256*256; i++) {bm_Horspool_Order2[i]=0;}
			for (i=0; i < cbPattern-1; i++) bm_Horspool_Order2[*(unsigned short *)(pbPattern+i)]=1;
			i=0;
			while (i <= cbTarget-cbPattern) {
				Gulliver = 1; // 'Gulliver' is the skip
				if ( bm_Horspool_Order2[*(unsigned short *)&pbTarget[i+cbPattern-1-1]] != 0 ) {
					if ( bm_Horspool_Order2[*(unsigned short *)&pbTarget[i+cbPattern-1-1-2]] == 0 ) Gulliver = cbPattern-(2-1)-2; else {
						if ( *(uint32_t *)&pbTarget[i] == ulHashPattern) { // This fast check ensures not missing a match (for remainder) when going under 0 in loop below:
							count = cbPattern-4+1;
							while ( count > 0 && *(uint32_t *)(pbPattern+count-1) == *(uint32_t *)(&pbTarget[i]+(count-1)) )
								count = count-4;
// If we miss to hit then no need to compare the original: Needle
if ( count <= 0 ) { 
	if ( ((signed int)(i-(PRIMALposition-1)+(count-1)) >= 0) && (&pbTarget[i-(PRIMALposition-1)] <= pbTargetMax - 4) ) {

		if ( *(uint32_t *)&pbTarget[i-(PRIMALposition-1)] == *(uint32_t *)(pbPattern-(PRIMALposition-1))) { // This fast check ensures not missing a match (for remainder) when going under 0 in loop below:
			count = PRIMALlengthCANDIDATE-4+1;
			while ( count > 0 && *(uint32_t *)(pbPattern-(PRIMALposition-1)+count-1) == *(uint32_t *)(&pbTarget[i-(PRIMALposition-1)]+(count-1)) )
				count = count-4;
			if ( count <= 0 ) return(pbTarget+i-(PRIMALposition-1));
		}
	}
}
						}
					}
				} else Gulliver = cbPattern-(2-1);
				i = i + Gulliver;
				//GlobalI++; // Comment it, it is only for stats.
			}
			return(NULL);

		} else { // if ( cbPattern<=NeedleThreshold2vs4swampLITE )

			// BMH pseudo-order 4, needle should be >=8+2:
			ulHashPattern = *(uint32_t *)(pbPattern); // First four bytes
			for (i=0; i < 256*256; i++) {bm_Horspool_Order2[i]=0;}
			// In line below we "hash" 4bytes to 2bytes i.e. 16bit table, how to compute TOTAL number of BBs, 'cbPattern - Order + 1' is the number of BBs for text 'cbPattern' bytes long, for example, for cbPattern=11 'fastest fox' and Order=4 we have BBs = 11-4+1=8:
			//"fast"
			//"aste"
			//"stes"
			//"test"
			//"est "
			//"st f"
			//"t fo"
			//" fox"
			//for (i=0; i < cbPattern-4+1; i++) bm_Horspool_Order2[( *(unsigned short *)(pbPattern+i+0) + *(unsigned short *)(pbPattern+i+2) ) & ( (1<<16)-1 )]=1;
			//for (i=0; i < cbPattern-4+1; i++) bm_Horspool_Order2[( (*(uint32_t *)(pbPattern+i+0)>>16)+(*(uint32_t *)(pbPattern+i+0)&0xFFFF) ) & ( (1<<16)-1 )]=1;
			// Above line is replaced by next one with better hashing:
			for (i=0; i < cbPattern-4+1; i++) bm_Horspool_Order2[( (*(uint32_t *)(pbPattern+i+0)>>(16-1))+(*(uint32_t *)(pbPattern+i+0)&0xFFFF) ) & ( (1<<16)-1 )]=1;
			i=0;
			while (i <= cbTarget-cbPattern) {
				Gulliver = 1;
				//if ( bm_Horspool_Order2[( (*(uint32_t *)&pbTarget[i+cbPattern-1-1-2]>>16)+(*(uint32_t *)&pbTarget[i+cbPattern-1-1-2]&0xFFFF) ) & ( (1<<16)-1 )] != 0 ) { // DWORD #1
				// Above line is replaced by next one with better hashing:
				if ( bm_Horspool_Order2[( (*(uint32_t *)&pbTarget[i+cbPattern-1-1-2]>>(16-1))+(*(uint32_t *)&pbTarget[i+cbPattern-1-1-2]&0xFFFF) ) & ( (1<<16)-1 )] != 0 ) { // DWORD #1
					//if ( bm_Horspool_Order2[( (*(uint32_t *)&pbTarget[i+cbPattern-1-1-2-4]>>16)+(*(uint32_t *)&pbTarget[i+cbPattern-1-1-2-4]&0xFFFF) ) & ( (1<<16)-1 )] == 0 ) Gulliver = cbPattern-(2-1)-2-4; else {
					// Above line is replaced in order to strengthen the skip by checking the middle DWORD,if the two DWORDs are 'ab' and 'cd' i.e. [2x][2a][2b][2c][2d] then the middle DWORD is 'bc'.
					// The respective offsets (backwards) are: -10/-8/-6/-4 for 'xa'/'ab'/'bc'/'cd'.
					//if ( ( bm_Horspool_Order2[( (*(uint32_t *)&pbTarget[i+cbPattern-1-1-2-6]>>16)+(*(uint32_t *)&pbTarget[i+cbPattern-1-1-2-6]&0xFFFF) ) & ( (1<<16)-1 )] ) + ( bm_Horspool_Order2[( (*(uint32_t *)&pbTarget[i+cbPattern-1-1-2-4]>>16)+(*(uint32_t *)&pbTarget[i+cbPattern-1-1-2-4]&0xFFFF) ) & ( (1<<16)-1 )] ) + ( bm_Horspool_Order2[( (*(uint32_t *)&pbTarget[i+cbPattern-1-1-2-2]>>16)+(*(uint32_t *)&pbTarget[i+cbPattern-1-1-2-2]&0xFFFF) ) & ( (1<<16)-1 )] ) < 3 ) Gulliver = cbPattern-(2-1)-2-4-2; else {
					// Above line is replaced by next one with better hashing:
					// When using (16-1) right shifting instead of 16 we will have two different pairs (if they are equal), the highest bit being lost do the job especialy for ASCII texts with no symbols in range 128-255.
					// Example for genomesque pair TT+TT being shifted by (16-1):
					// T            = 01010100
					// TT           = 01010100 01010100
					// TTTT         = 01010100 01010100 01010100 01010100
					// TTTT>>16     = 00000000 00000000 01010100 01010100
					// TTTT>>(16-1) = 00000000 00000000 10101000 10101000 <--- Due to the left shift by 1, the 8th bits of 1st and 2nd bytes are populated - usually they are 0 for English texts & 'ACGT' data.
					//if ( ( bm_Horspool_Order2[( (*(uint32_t *)&pbTarget[i+cbPattern-1-1-2-6]>>(16-1))+(*(uint32_t *)&pbTarget[i+cbPattern-1-1-2-6]&0xFFFF) ) & ( (1<<16)-1 )] ) + ( bm_Horspool_Order2[( (*(uint32_t *)&pbTarget[i+cbPattern-1-1-2-4]>>(16-1))+(*(uint32_t *)&pbTarget[i+cbPattern-1-1-2-4]&0xFFFF) ) & ( (1<<16)-1 )] ) + ( bm_Horspool_Order2[( (*(uint32_t *)&pbTarget[i+cbPattern-1-1-2-2]>>(16-1))+(*(uint32_t *)&pbTarget[i+cbPattern-1-1-2-2]&0xFFFF) ) & ( (1<<16)-1 )] ) < 3 ) Gulliver = cbPattern-(2-1)-2-4-2; else {
					// 'Maximus' uses branched 'if', again.
					if ( \
					( bm_Horspool_Order2[( (*(uint32_t *)&pbTarget[i+cbPattern-1-1-2-6 +1]>>(16-1))+(*(uint32_t *)&pbTarget[i+cbPattern-1-1-2-6 +1]&0xFFFF) ) & ( (1<<16)-1 )] ) == 0 \
					|| ( bm_Horspool_Order2[( (*(uint32_t *)&pbTarget[i+cbPattern-1-1-2-4 +1]>>(16-1))+(*(uint32_t *)&pbTarget[i+cbPattern-1-1-2-4 +1]&0xFFFF) ) & ( (1<<16)-1 )] ) == 0 \
					) Gulliver = cbPattern-(2-1)-2-4-2 +1; else {
					// Above line is not optimized (several a SHR are used), we have 5 non-overlapping WORDs, or 3 overlapping WORDs, within 4 overlapping DWORDs so:
// [2x][2a][2b][2c][2d]
// DWORD #4
// [2a] (*(uint32_t *)&pbTarget[i+cbPattern-1-1-2-6]>>16) =     !SHR to be avoided! <--
// [2x] (*(uint32_t *)&pbTarget[i+cbPattern-1-1-2-6]&0xFFFF) =                        |
//     DWORD #3                                                                       |
// [2b] (*(uint32_t *)&pbTarget[i+cbPattern-1-1-2-4]>>16) =     !SHR to be avoided!   |<--
// [2a] (*(uint32_t *)&pbTarget[i+cbPattern-1-1-2-4]&0xFFFF) = ------------------------  |
//         DWORD #2                                                                      |
// [2c] (*(uint32_t *)&pbTarget[i+cbPattern-1-1-2-2]>>16) =     !SHR to be avoided!      |<--
// [2b] (*(uint32_t *)&pbTarget[i+cbPattern-1-1-2-2]&0xFFFF) = ---------------------------  |
//             DWORD #1                                                                     |
// [2d] (*(uint32_t *)&pbTarget[i+cbPattern-1-1-2-0]>>16) =                                 |
// [2c] (*(uint32_t *)&pbTarget[i+cbPattern-1-1-2-0]&0xFFFF) = ------------------------------
//
// So in order to remove 3 SHR instructions the equal extractions are:
// DWORD #4
// [2a] (*(uint32_t *)&pbTarget[i+cbPattern-1-1-2-4]&0xFFFF) =  !SHR to be avoided! <--
// [2x] (*(uint32_t *)&pbTarget[i+cbPattern-1-1-2-6]&0xFFFF) =                        |
//     DWORD #3                                                                       |
// [2b] (*(uint32_t *)&pbTarget[i+cbPattern-1-1-2-2]&0xFFFF) =  !SHR to be avoided!   |<--
// [2a] (*(uint32_t *)&pbTarget[i+cbPattern-1-1-2-4]&0xFFFF) = ------------------------  |
//         DWORD #2                                                                      |
// [2c] (*(uint32_t *)&pbTarget[i+cbPattern-1-1-2-0]&0xFFFF) =  !SHR to be avoided!      |<--
// [2b] (*(uint32_t *)&pbTarget[i+cbPattern-1-1-2-2]&0xFFFF) = ---------------------------  |
//             DWORD #1                                                                     |
// [2d] (*(uint32_t *)&pbTarget[i+cbPattern-1-1-2-0]>>16) =                                 |
// [2c] (*(uint32_t *)&pbTarget[i+cbPattern-1-1-2-0]&0xFFFF) = ------------------------------
					//if ( ( bm_Horspool_Order2[( (*(uint32_t *)&pbTarget[i+cbPattern-1-1-2-4]&0xFFFF)+(*(uint32_t *)&pbTarget[i+cbPattern-1-1-2-6]&0xFFFF) ) & ( (1<<16)-1 )] ) + ( bm_Horspool_Order2[( (*(uint32_t *)&pbTarget[i+cbPattern-1-1-2-2]&0xFFFF)+(*(uint32_t *)&pbTarget[i+cbPattern-1-1-2-4]&0xFFFF) ) & ( (1<<16)-1 )] ) + ( bm_Horspool_Order2[( (*(uint32_t *)&pbTarget[i+cbPattern-1-1-2-0]&0xFFFF)+(*(uint32_t *)&pbTarget[i+cbPattern-1-1-2-2]&0xFFFF) ) & ( (1<<16)-1 )] ) < 3 ) Gulliver = cbPattern-(2-1)-2-6; else {
// Since the above Decumanus mumbo-jumbo (3 overlapping lookups vs 2 non-overlapping lookups) is not fast enough we go DuoDecumanus or 3x4:
// [2y][2x][2a][2b][2c][2d]
// DWORD #3
//         DWORD #2
//                 DWORD #1
					//if ( ( bm_Horspool_Order2[( (*(uint32_t *)&pbTarget[i+cbPattern-1-1-2-4]>>16)+(*(uint32_t *)&pbTarget[i+cbPattern-1-1-2-4]&0xFFFF) ) & ( (1<<16)-1 )] ) + ( bm_Horspool_Order2[( (*(uint32_t *)&pbTarget[i+cbPattern-1-1-2-8]>>16)+(*(uint32_t *)&pbTarget[i+cbPattern-1-1-2-8]&0xFFFF) ) & ( (1<<16)-1 )] ) < 2 ) Gulliver = cbPattern-(2-1)-2-8; else {
						if ( *(uint32_t *)&pbTarget[i] == ulHashPattern) {
							// Order 4 [
						// Let's try something "outrageous" like comparing with[out] overlap BBs 4bytes long instead of 1 byte back-to-back:
						// Inhere we are using order 4, 'cbPattern - Order + 1' is the number of BBs for text 'cbPattern' bytes long, for example, for cbPattern=11 'fastest fox' and Order=4 we have BBs = 11-4+1=8:
						//0:"fast" if the comparison failed here, 'count' is 1; 'Gulliver' is cbPattern-(4-1)-7
						//1:"aste" if the comparison failed here, 'count' is 2; 'Gulliver' is cbPattern-(4-1)-6
						//2:"stes" if the comparison failed here, 'count' is 3; 'Gulliver' is cbPattern-(4-1)-5
						//3:"test" if the comparison failed here, 'count' is 4; 'Gulliver' is cbPattern-(4-1)-4
						//4:"est " if the comparison failed here, 'count' is 5; 'Gulliver' is cbPattern-(4-1)-3
						//5:"st f" if the comparison failed here, 'count' is 6; 'Gulliver' is cbPattern-(4-1)-2
						//6:"t fo" if the comparison failed here, 'count' is 7; 'Gulliver' is cbPattern-(4-1)-1
						//7:" fox" if the comparison failed here, 'count' is 8; 'Gulliver' is cbPattern-(4-1)
							count = cbPattern-4+1;
							// Below comparison is UNIdirectional:
							while ( count > 0 && *(uint32_t *)(pbPattern+count-1) == *(uint32_t *)(&pbTarget[i]+(count-1)) )
								count = count-4;
// count = cbPattern-4+1 = 23-4+1 = 20
// boomshakalakaZZZZZZ[ZZZZ] 20
// boomshakalakaZZ[ZZZZ]ZZZZ 20-4
// boomshakala[kaZZ]ZZZZZZZZ 20-8 = 12
// boomsha[kala]kaZZZZZZZZZZ 20-12 = 8
// boo[msha]kalakaZZZZZZZZZZ 20-16 = 4

// If we miss to hit then no need to compare the original: Needle
if ( count <= 0 ) {
// I have to add out-of-range checks...
// i-(PRIMALposition-1) >= 0
// &pbTarget[i-(PRIMALposition-1)] <= pbTargetMax - 4
// i-(PRIMALposition-1)+(count-1) >= 0
// &pbTarget[i-(PRIMALposition-1)+(count-1)] <= pbTargetMax - 4

// FIX from 2014-Apr-27:
// Because (count-1) is negative, above fours are reduced to next twos:
// i-(PRIMALposition-1)+(count-1) >= 0
// &pbTarget[i-(PRIMALposition-1)] <= pbTargetMax - 4
	// The line below is BUGGY:
	//if ( (i-(PRIMALposition-1) >= 0) && (&pbTarget[i-(PRIMALposition-1)] <= pbTargetMax - 4) && (&pbTarget[i-(PRIMALposition-1)+(count-1)] <= pbTargetMax - 4) ) {
	// The line below is OKAY:
	if ( ((signed int)(i-(PRIMALposition-1)+(count-1)) >= 0) && (&pbTarget[i-(PRIMALposition-1)] <= pbTargetMax - 4) ) {

		if ( *(uint32_t *)&pbTarget[i-(PRIMALposition-1)] == *(uint32_t *)(pbPattern-(PRIMALposition-1))) { // This fast check ensures not missing a match (for remainder) when going under 0 in loop below:
			count = PRIMALlengthCANDIDATE-4+1;
			while ( count > 0 && *(uint32_t *)(pbPattern-(PRIMALposition-1)+count-1) == *(uint32_t *)(&pbTarget[i-(PRIMALposition-1)]+(count-1)) )
				count = count-4;
			if ( count <= 0 ) return(pbTarget+i-(PRIMALposition-1));
		}
	}
}
 						}
					}
				} else Gulliver = cbPattern-(2-1)-2; // -2 because we check the 4 rightmost bytes not 2.
				i = i + Gulliver;
				//GlobalI++; // Comment it, it is only for stats.
			}
			return(NULL);

		} // if ( cbPattern<=NeedleThreshold2vs4swampLITE )
		} // if ( cbPattern<=NeedleThreshold2vs4swampLITE )
	} //if ( cbPattern<4 )
};
vector<int> stringPositions(char * pbTarget, char * pbPattern, uint32_t cbTarget, uint32_t cbPattern){
	vector<int> res;
	char* stoken=pbTarget;
	uint32_t nlen=cbTarget;
	uint32_t dist;
	while(stoken!=NULL){
		char* stokenp=stoken;
		stoken=Railgun_Swampshine_BailOut (stokenp, pbPattern, nlen, cbPattern);
		if(stoken!=NULL){
			int dist=distance(pbTarget,stoken);
			nlen=cbTarget-dist-cbPattern;
			stoken+=cbPattern;
			res.push_back(dist);
		}
	}
	return res;
};
#endif

#if 1 //sci
static void sci_setup(ScintillaObject *sci);
void setttbar( ){
		stringstream ttbar;
		ttbar<<appname<<" - ";
		if(editorptf[0]){
			ttbar<<(editorptf[0]?(editorptf[0]->fpath):"");
			ttbar<<(editorptf[0]->saved?"*":"");
		}
		if(editorptf[1]){
			ttbar<< " | "<<(editorptf[1]?editorptf[1]->fpath:"");
			ttbar<<(editorptf[1]->saved?"*":"");
		}
		gtk_window_set_title(GTK_WINDOW (window),ttbar.str().c_str());
}

//toggle=0 commment=1 uncommment=-1
void sciCommment(int flag){
	char* commstr="//";
	ScintillaObject *sci=scicurr;  
	int selsz=ssm(SCI_GETSELTEXT,0,0);
	char buffsel[selsz];
	ssm(SCI_GETSELTEXT, 0, (LPARAM) buffsel);

	int startp=ssm(SCI_GETSELECTIONSTART,0,0);
	int startl=ssm(SCI_LINEFROMPOSITION,startp,0);
	int endp=ssm(SCI_GETSELECTIONEND,0,0);
	int endl=ssm(SCI_LINEFROMPOSITION,endp,0);

	if(flag==0)
	lop(i,startl,endl+1){
		int li=ssm(SCI_POSITIONFROMLINE,i,0);
		int lisz=ssm( SCI_LINELENGTH,i,0);
		char buffline[lisz];
		ssm(SCI_GETLINE, i, (LPARAM) buffline);
		lop(c,0,lisz){
			if(buffline[c]=='\t' || buffline[c]==' ')continue;
			if(string(buffline+c,2)==commstr)
				ssm(SCI_DELETERANGE,li+c,2);
			else
				ssm(SCI_INSERTTEXT,li,(LPARAM) commstr);		
			break;
		}
	}
 
	if(flag==1)
	lop(i,startl,endl+1){
		int li=ssm(SCI_POSITIONFROMLINE,i,0);
		ssm(SCI_INSERTTEXT,li,(LPARAM) commstr);
	}

	if(flag==-1)
	lop(i,startl,endl+1){
		int li=ssm(SCI_POSITIONFROMLINE,i,0);
		int lisz=ssm( SCI_LINELENGTH,i,0);
		char buffline[lisz];
		ssm(SCI_GETLINE, i, (LPARAM) buffline);
		lop(c,0,lisz){
			if(buffline[c]=='\t' || buffline[c]==' ')continue;
			if(string(buffline+c,2)==commstr)
				ssm(SCI_DELETERANGE,li+c,2);
			break;
		}
	}

	ssm(SCI_SETSELECTIONSTART,startp,0);
	ssm(SCI_SETSELECTIONEND,endp,0);
}


void scintHighLightAll(string word){
	//COULD HAVE BETTER SPEED
//	if(word=="")return;
	ScintillaObject *sci=scicurr; 
	ssm(SCI_SETINDICATORCURRENT,1,0);
	ssm(SCI_INDICSETSTYLE,1,INDIC_ROUNDBOX);
	int sz=ssm(SCI_GETLENGTH,0,0);
	if(word.size()<2){ssm(SCI_INDICATORCLEARRANGE,0, sz);return;}
	char buff[sz+1];
	ssm(SCI_GETTEXT, sz+1, (LPARAM) buff);
	vector<int> sp=stringPositions(buff,(char*)word.c_str(),sz+1,word.size());
	for( int i: sp)
		ssm(SCI_INDICATORFILLRANGE,i,word.size());
}

static void onScintillaNotifcation(ScintillaObject *sci,gint noClueWhatThisIs,struct SCNotification *notification, gpointer ieditor){
	int code=notification->nmhdr.code;
	if(code==SCN_UPDATEUI){
		cout<<"SCNotification "<<code; 
		//useful to show sugestion list
		//SCI_POINTXFROMPOSITION  SCI_POINTYFROMPOSITION 
		
		//savededitor[(sptr_t)ieditor]=ssm(SCI_CANUNDO , 0,0);
		//setttbar();

		scicurr=sci;
		ptfcurr=editorptf[(sptr_t)ieditor]; 
		if(ptfcurr){ 
			ptfcurr->scrollpos=ssm(SCI_GETFIRSTVISIBLELINE,0,0);
			ptfcurr->scrollpos=ssm(SCI_DOCLINEFROMVISIBLE,(uptr_t) ptfcurr->scrollpos,0);
			ptfcurr->saved=ssm(SCI_GETMODIFY  , 0,0);
			setttbar();  
		} 

		//highlightAll word
		int selsz=ssm(SCI_GETSELTEXT,0,0);
		char buffsel[selsz];
		ssm(SCI_GETSELTEXT, 0, (LPARAM) buffsel);
		scintHighLightAll(buffsel); //expand to all views
	
		//highlight brackets matching
		int iPosA=ssm(SCI_GETCURRENTPOS, 0, 0);
		LPARAM iPosB = ssm(SCI_BRACEMATCH , (WPARAM)iPosA, 0);
	
	
	}
	 // if (notification->nmhdr.idFrom == IDM_SRCWIN) {
		// if (notification->nmhdr.code == SCN_FOCUSIN)
			// Activate(true);
		// else if (notification->nmhdr.code == SCN_FOCUSOUT)
			// Activate(false);
	// }
	if(notification->margin==MARGIN_SCRIPT_FOLD_INDEX){
		const int line_number = ssm(SCI_LINEFROMPOSITION, notification->position, 0);
		// dbgv(line_number)
		ssm(SCI_TOGGLEFOLD, line_number,0);
	}
	
	if (notification->nmhdr.code == SCN_FOCUSOUT){
		ptfcurr=editorptf[(sptr_t)ieditor];
		if(ptfcurr){
		dbgv(0)
		// gets first line displayed on screen
		// uptr_t line=0;
		// uptr_t linecount=ssm(SCI_GETLINECOUNT,0,0);
		// for(line=0;line<linecount;line++){ 
			// if(ssm(SCI_GETLINEVISIBLE, (uptr_t) line, 0))break;
		// }
	// ssm(SCI_SETWRAPMODE, SC_WRAP_NONE , 0);
		ptfcurr->scrollpos=ssm(SCI_GETFIRSTVISIBLELINE,0,0);
		ptfcurr->scrollpos=ssm(SCI_DOCLINEFROMVISIBLE,(uptr_t) ptfcurr->scrollpos,0);
		// uptr_t linecount=ssm(SCI_GETLINECOUNT,0,0);
		// for(line=0;line<linecount;line++){ 
		
		
		
	// ssm(SCI_SETWRAPMODE, SC_WRAP_WORD, 0);
		// ptfcurr->scrollpos=ssm(SCI_GETCURRENTPOS, 0, 0);
		// dbgv(ptfcurr->scrollpos);
		}
	}
}
void sci_setStyleCommon(ScintillaObject *sci){ 
	
	// ssm(SCI_SETWRAPMODE,1,0);
		ssm(SCI_SETINDICATORCURRENT,1,0);
	ssm(SCI_INDICSETSTYLE,1,INDIC_ROUNDBOX);
	// ssm(SCI_INDICATORFILLRANGE,2,5);
	// ssm(SCI_INDICATORFILLRANGE,12,8);
	//SSM(SCI_SETSTYLING,  9,  128);
// return;
	// ssm(SCI_STYLECLEARALL, 0, 0);
	// return;
	
	//a ver nada
	ssm( SCI_SETHIGHLIGHTGUIDE, 0, 0);
	ssm(SCI_BRACEBADLIGHT, (uptr_t)-1, 0);
	
	
	
	
	//    ssm(SCI_SETPROPERTY,(WPARAM)"fold", (LPARAM)"1");
	//    ssm(SCI_SETPROPERTY, (WPARAM)"fold.comment", (LPARAM)"1");
	//    ssm(SCI_SETPROPERTY, (WPARAM)"fold.preprocessor", (LPARAM)"1");
	ssm(SCI_SETMARGINWIDTHN, MARGIN_SCRIPT_FOLD_INDEX, 0);
	ssm(SCI_SETMARGINTYPEN,  MARGIN_SCRIPT_FOLD_INDEX, SC_MARGIN_SYMBOL);
	ssm(SCI_SETMARGINMASKN, MARGIN_SCRIPT_FOLD_INDEX, SC_MASK_FOLDERS);
	ssm(SCI_SETMARGINWIDTHN, MARGIN_SCRIPT_FOLD_INDEX, 20);
	ssm(SCI_MARKERDEFINE, SC_MARKNUM_FOLDER, SC_MARK_PLUS);
	ssm(SCI_MARKERDEFINE, SC_MARKNUM_FOLDEROPEN, SC_MARK_MINUS);
	ssm(SCI_MARKERDEFINE, SC_MARKNUM_FOLDEREND, SC_MARK_VLINE);
	ssm(SCI_MARKERDEFINE, SC_MARKNUM_FOLDERMIDTAIL, SC_MARK_VLINE);
	ssm(SCI_MARKERDEFINE, SC_MARKNUM_FOLDEROPENMID, SC_MARK_VLINE);
	ssm(SCI_MARKERDEFINE, SC_MARKNUM_FOLDERSUB, SC_MARK_VLINE);
	ssm(SCI_MARKERDEFINE, SC_MARKNUM_FOLDERTAIL, SC_MARK_VLINE);
	
  ssm(SCI_SETEDGEMODE, EDGE_NONE, 0);
  ssm(SCI_SETINDENT,0,0);
	
	// ssm(SCI_MARKERDEFINE, SC_MARKNUM_FOLDEREND, SC_MARK_BOXPLUSCONNECTED);
	// ssm(SCI_MARKERDEFINE, SC_MARKNUM_FOLDERMIDTAIL, SC_MARK_TCORNER);
	// ssm(SCI_MARKERDEFINE, SC_MARKNUM_FOLDEROPENMID, SC_MARK_BOXMINUSCONNECTED);
	// ssm(SCI_MARKERDEFINE, SC_MARKNUM_FOLDERSUB, SC_MARK_VLINE);
	// ssm(SCI_MARKERDEFINE, SC_MARKNUM_FOLDERTAIL, SC_MARK_LCORNER);
	// ssm(SCI_MARKERDEFINE, SC_MARKNUM_FOLDER, SC_MARK_BOXPLUS);
	// ssm(SCI_MARKERDEFINE, SC_MARKNUM_FOLDEROPEN, SC_MARK_BOXMINUS);

	ssm(SCI_SETFOLDFLAGS, 16, 0); // 16  	Draw line below if not expanded
	ssm(SCI_SETMARGINSENSITIVEN, MARGIN_SCRIPT_FOLD_INDEX, 1);

ssm(SCI_SETEDGEMODE,EDGE_MULTILINE,0);

//	ssm(SCI_SETFOCUS,0);
	ssm(SCI_SETTABINDENTS,1,0);
}
void sci_setStyleC(ScintillaObject *sci){	//return;
	
	//vertical line attempt
	// ssm( SCI_MULTIEDGEADDLINE , 0, 0);
	
	// ssm(SCI_SETLEXER, SCLEX_CPP, 0);
	// ssm(SCI_STYLECLEARALL, 0, 0);
	// ssm(SCI_STYLESETFONT, STYLE_DEFAULT, (LPARAM)"Courier new");
	// ssm(SCI_STYLESETSIZE, STYLE_DEFAULT, 10);
	
	ssm(SCI_SETTABWIDTH, 3,0);
	ssm(SCI_SETPROPERTY,(long long unsigned int)"fold", (long long int) "1");
	ssm(SCI_SETPROPERTY, (uptr_t)"fold.comment", (sptr_t)"1");
	ssm(SCI_SETPROPERTY, (uptr_t)"fold.preprocessor", (sptr_t)"1");

	ssm(SCI_SETKEYWORDS, 0, (sptr_t)"_Char16_t _Char32_t align_union alignof asm auto bool break case catch char class const const_cast constexpr continue decltype default delete do double dynamic_cast else enum explicit export extern false final float for friend goto if import inline int long mutable namespace new nullptr operator override private protected public register reinterpret_cast return short signed sizeof static static_cast struct switch template this throw true try typedef typeid typename union unsigned using virtual void volatile wchar_t while static_assert int8_t uint8_t int16_t uint16_t int32_t uint32_t int64_t uint64_t int_least8_t uint_least8_t int_least16_t uint_least16_t int_least32_t uint_least32_t int_least64_t uint_least64_t int_fast8_t uint_fast8_t int_fast16_t uint_fast16_t int_fast32_t uint_fast32_t int_fast64_t uint_fast64_t intptr_t uintptr_t intmax_t uintmax_t wint_t wchar_t wctrans_t wctype_t size_t time_t and and_eq bitand bitor compl not not_eq or or_eq xor xor_eq");
	 ssm(SCI_STYLESETFORE, SCE_C_WORD, 0x700000);
	 ssm(SCI_STYLESETBOLD, SCE_C_WORD, 1);
	 ssm(SCI_SETKEYWORDS, 1, (sptr_t)"uchar uint ushort ulong ullong vshort vvshort vstring vvstring vbool vvbool vint vvint vfloat vvfloat to_string __gnu_cxx accumulate add_const add_cv add_lvalue_reference add_pointer add_reference add_rvalue_reference add_volatile adjacent_difference adjacent_find aligned_storage Alignment alignment_of all_of allocate_shared allocator allocator_base allocator_chunklist allocator_fixed_size allocator_newdel allocator_suballoc allocator_unbounded allocator_variable_size any_of array assign at atomic_bool atomic_char atomic_char16_t atomic_char32_t atomic_compare_exchange_strong atomic_compare_exchange_strong_explicit atomic_compare_exchange_weak atomic_compare_exchange_weak_explicit atomic_exchange atomic_exchange_explicit atomic_fetch_add atomic_fetch_and atomic_fetch_or atomic_fetch_sub atomic_fetch_xor atomic_int atomic_int_fast16_t atomic_int_fast32_t atomic_int_fast64_t atomic_int_fast8_t atomic_int_least16_t atomic_int_least32_t atomic_int_least64_t atomic_int_least8_t atomic_intmax_t atomic_intptr_t atomic_is_lock_free atomic_llong atomic_load atomic_load_explicit atomic_long atomic_ptrdiff_t atomic_schar atomic_short atomic_size_t atomic_ssize_t atomic_store atomic_store_explicit atomic_uchar atomic_uint atomic_uint_fast16_t atomic_uint_fast32_t atomic_uint_fast64_t atomic_uint_fast8_t atomic_uint_least16_t atomic_uint_least32_t atomic_uint_least64_t atomic_uint_least8_t atomic_uintmax_t atomic_uintptr_t atomic_ullong atomic_ulong atomic_ushort atomic_wchar_t auto_ptr back back_insert_iterator back_item bad_alloc bad_function_call bad_weak_ptr basic_filebuf basic_fstream basic_ifstream basic_ofstream basic_regex basic_streambuf basic_string begin bernoulli_distribution bidirectional_iterator_tag binary_function binary_negate binary_search bind bind1st bind2nd binder1st binder2nd binomial_distribution bit_and bit_or bit_xor bitset boost cache_chunklist cache_freelist cache_suballoc cauchy_distribution cbegin cend cerr char_traits checked_array_iterator checked_uninitialized_copy checked_uninitialized_fill_n chi_squared_distribution cin clear codecvt codecvt_base codecvt_byname codecvt_mode codecvt_utf16 codecvt_utf8 codecvt_utf8_utf16 collate collate_byname common_type compare_exchange_strong compare_exchange_weak complex condition_variable conditional const_iterator const_mem_fun_ref_t const_mem_fun_t const_mem_fun1_ref_t const_mem_fun1_t const_pointer_cast const_reference const_reverse_iterator copy copy_backward copy_if copy_n count count_if cout crbegin cref crend ctype ctype_base ctype_byname decay declare_no_pointers declare_reachable declval default_delete default_random_engine deque difference_type discard_block discard_block_engine discrete_distribution divides domain_error dynamic_pointer_cast empty enable_if enable_shared_from_this end endl equal equal_range equal_to EqualityComparable erase error_category error_code error_condition exception exponential_distribution extent extreme_value_distribution fetch_add fetch_and fetch_or fetch_sub fetch_xor filebuf fill fill_n find find_end find_first_of find_first_not_of find_if find_if_not find_last_of find_last_not_of fisher_f_distribution float_denorm_style float_round_style for_each forward forward_iterator_tag forward_list freelist front front_insert_iterator front_item fstream function gamma_distribution generate generate_n generic_container generic_iterator generic_reverse_iterator generic_value geometric_distribution get_deleter get_pointer_safety get_temporary_buffer greater greater_equal has_nothrow_assign has_nothrow_constructor has_nothrow_copy has_nothrow_copy_assign has_nothrow_copy_constructor has_nothrow_default_constructor has_trivial_assign has_trivial_constructor has_trivial_copy has_trivial_copy_assign has_trivial_copy_constructor has_trivial_default_constructor has_trivial_destructor has_virtual_destructor hash hash_map hash_set ifstream includes independent_bits_engine initializer_list inner_product inplace_merge input_iterator_tag insert insert_iterator integral_constant invalid_argument ios_base iostream is_abstract is_arithmetic is_array is_base_of is_bind_expression is_class is_compound is_const is_constructible is_convertible is_empty is_enum is_error_code_enum is_error_condition_enum is_explicitly_convertible is_floating_point is_function is_fundamental is_heap is_heap_until is_integral is_literal_type is_lock_free is_lvalue_reference is_member_function_pointer is_member_object_pointer is_member_pointer is_nothrow_constructible is_object is_partitioned is_placeholder is_pod is_pointer is_polymorphic is_reference is_rvalue_reference is_same is_scalar is_signed is_sorted is_sorted_until is_standard_layout is_trivial is_union is_unsigned is_void is_volatile istream istream_iterator istreambuf_iterator iter_swap iterator iterator_traits knuth_b length_error less less_equal LessThanComparable lexicographical_compare linear_congruential linear_congruential_engine list locale logic_error logical_and logical_not logical_or lognormal_distribution lower_bound make_checked_array_iterator make_heap make_shared make_signed make_unsigned map match_results max max_element max_fixed_size max_none max_unbounded max_variable_size mem_fn mem_fun mem_fun_ref mem_fun_ref_t mem_fun_t mem_fun1_ref_t mem_fun1_t merge mersenne_twister mersenne_twister_engine messages messages_base messages_byname min min_element minmax minmax_element minstd_rand minstd_rand0 minus mismatch modulus money_base money_get money_put moneypunct moneypunct_byname move move_backward move_iterator mt19937 mt19937_64 multimap multiplies multiset negate negative_binomial_distribution new_handler next_permutation none_of normal_distribution not_equal_to not1 not2 nothrow nothrow_t npos nth_element num_get num_put numeric_limits numpunct numpunct_byname ofstream ostream_iterator ostreambuf_iterator out_of_range output_iterator_tag overflow_error owner_less pair partial_sort partial_sort_copy partial_sum partition partition_copy partition_point piecewise_constant_distribution piecewise_linear_distribution plus pointer_safety pointer_to_binary_function pointer_to_unary_function poisson_distribution pop_back pop_front pop_heap prev_permutation priority_queue ptr_fun push_back push_front push_heap queue random_access_iterator_tag random_device random_shuffle range_error rank ranlux_base_01 ranlux24 ranlux24_base ranlux3 ranlux3_01 ranlux4 ranlux4_01 ranlux48 ranlux48_base ranlux64_base_01 ratio ratio_add ratio_divide ratio_multiply ratio_subtract raw_storage_iterator rbegin rdbuf ref reference reference_wrapper regex regex_constants regex_error regex_iterator regex_token_iterator regex_traits remove remove_all_extents remove_const remove_copy remove_copy_if remove_cv remove_extent remove_if remove_pointer remove_reference remove_volatile rend replace replace_copy replace_copy_if replace_if requires resize result_of return_temporary_buffer reverse reverse_copy reverse_iterator rotate rotate_copy rts_alloc runtime_error search search_n seed_seq set set_difference set_intersection set_new_handler set_symmetric_difference set_union setprecision setw shared_ptr shuffle_order_engine size size_type sort sort_heap stable_partition stable_sort stack static_pointer_cast std streambuf string stringstream student_t_distribution sub_match substr subtract_with_carry subtract_with_carry_01 subtract_with_carry_engine swap swap_ranges sync_none sync_per_container sync_per_thread sync_shared system_error time_base time_get time_get_byname time_put time_put_byname to_array tr1 transform tuple tuple_element tuple_size type_info unary_function unary_negate unchecked_uninitialized_copy unchecked_uninitialized_fill_n undeclare_no_pointers undeclare_reachable underflow_error uniform_int uniform_int_distribution uniform_real uniform_real_distribution uninitialized_copy uninitialized_copy_n uninitialized_fill uninitialized_fill_n unique unique_copy unique_ptr unordered_map unordered_multimap unordered_multiset unordered_set upper_bound valarray value_type variate_generator vector wcerr wcin wcout weak_ptr weibull_distribution wfilebuf wfstream wifstream wiostream wistream wofstream wregex xor_combine");
	 ssm(SCI_STYLESETFORE, SCE_C_WORD2, 0x007000);
	 ssm(SCI_STYLESETBOLD, SCE_C_WORD2, 1);
	 ssm(SCI_STYLESETFORE, SCE_C_COMMENT, 0x008000);
	 ssm(SCI_STYLESETFORE, SCE_C_COMMENTLINE, 0x008000);
	 ssm(SCI_STYLESETFORE, SCE_C_COMMENTDOC, 0x008010);
	 // ssm(SCI_STYLESETFORE, SCE_C_PREPROCESSOR, 0x008000);
	 // ssm(SCI_STYLESETFORE, SCE_C_NUMBER, 0x808040);
	 // ssm(SCI_STYLESETFORE, SCE_C_STRING, 0x800080);
	 // return;
	 ssm(SCI_STYLESETFORE, SCE_C_DEFAULT, 0x800080);
	 ssm(SCI_STYLESETFORE, SCE_C_OPERATOR, 0x0000aa);
	 // ssm(SCI_STYLESETSIZE, SCE_C_OPERATOR, 12);//brackets size
	//       ssm(SCI_STYLESETFORE, SCE_C_VERBATIM, 0x800000);
	//       ssm(SCI_STYLESETFORE, SCE_C_IDENTIFIER, 0x800000);
	}


void setscint(int ieditor,ptfs* ptf){
	string filename=ptf->fpath;
	editorptf[ieditor]=ptf;
	ScintillaObject *sci=SCINTILLA(editor[ieditor]);
	 
	setttbar();	
	
	if(files[filename]==NULL){
		//		ifstream is (filename.c_str(),ios::binary);
		FILE *fp = fopen(filename.c_str(), "rb");
		if (fp) {
			//fileModTime = GetModTime(fullPath);
			LRESULT p=ssm(SCI_GETDOCPOINTER,0,0);
			ssm(SCI_ADDREFDOCUMENT,0, p);
			p=ssm(SCI_CREATEDOCUMENT,0,0);
			ssm(SCI_SETDOCPOINTER,0, p);
			// ssm(SCI_CLEARALL,0,0);
			// Disable UNDO
			ssm(SCI_SETUNDOCOLLECTION, 0,0);

			char data[128*1024];
			int lenFile = fread(data, 1, sizeof(data), fp);
			while (lenFile > 0) {
				ssm(SCI_ADDTEXT, lenFile,
				reinterpret_cast<LPARAM>(static_cast<char *>(data)));
				lenFile = fread(data, 1, sizeof(data), fp);
			}
			fclose(fp);

			ssm(SCI_SETSEL, 0, 0);
			// Re-Enable UNDO
			ssm(SCI_SETUNDOCOLLECTION, 1,0);
			ssm(SCI_SETSAVEPOINT,0,0);
			files[filename]=ssm(SCI_GETDOCPOINTER,0,0);
		}
	}else{
		// LRESULT p=ssm(SCI_GETDOCPOINTER,0,0);
		// ssm(SCI_ADDREFDOCUMENT,0, p);
		// p=files[filename];
		ssm(SCI_SETDOCPOINTER,0, files[filename]);
		
		
	// ssm(SCI_SETWRAPMODE, SC_WRAP_NONE , 0);
	
	
		// ssm(SCI_SETFIRSTVISIBLELINE, ssm(SCI_DOCLINEFROMVISIBLE,(uptr_t) ptf->scrollpos,0), 0);
		// ptf->scrollpos=411;
		ssm(SCI_SETFIRSTVISIBLELINE, (uptr_t) ptf->scrollpos, 0);
		
	// ssm(SCI_SETWRAPMODE, SC_WRAP_WORD, 0);
		// SSM(sci, SCI_GETCURRENTPOS, 0, 0); 
		// ptf->scrollpos=10000;
		// uptr_t line=80;
		// uptr_t linecount=ssm(SCI_GETLINECOUNT,0,0);
		// ssm( SCI_GOTOLINE, (uptr_t) linecount, 0);
		// ssm( SCI_GOTOLINE, (uptr_t) ptf->scrollpos, 0);
		// ptf->scrollpos=ssm(SCI_POSITIONFROMLINE, (uptr_t) line, 0);
		// ssm(SCI_GOTOPOS, (uptr_t) ptf->scrollpos, 0);
	}
		sci_setup(SCINTILLA(editor[ieditor]));
		sci_setStyleCommon(SCINTILLA(editor[ieditor]));
		sci_setStyleC(SCINTILLA(editor[ieditor]));
}

#endif

int treeMouseButtonClicked=0;
static void treeClick(GtkWidget *widget,GdkEvent *event, gpointer data) { 
	// dbgv(0)
	
	//THIS fflush MUST BE HERE TO CODE WORK...DONT KNOW WHY
	fflush(stdout);
	// dbgv( event->button.button);
	treeMouseButtonClicked=0;
	if(event->button.button==3)treeMouseButtonClicked=1;
	
	GtkTreeSelection* selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(tree));
	if(gtk_tree_selection_count_selected_rows(selection) <= 1) {
	   GtkTreePath *path;
	   /* Get tree path for row that was clicked and select*/
	   if (gtk_tree_view_get_path_at_pos(GTK_TREE_VIEW(tree),(gint) ((GdkEventButton*)event)->x,(gint) ((GdkEventButton*)event)->y,&path, NULL, NULL, NULL)){
		 gtk_tree_selection_unselect_all(selection);
		 gtk_tree_selection_select_path(selection, path);
		 gtk_tree_path_free(path);
	   }
	}
		
	GtkTreeIter iter;
	GtkTreeModel *model;
	gchar *value; 
	ptfs * ptf;
	if (gtk_tree_selection_get_selected(GTK_TREE_SELECTION(selection), &model, &iter)) {
		gtk_tree_model_get(model, &iter, COLUMN, &value,PTF,(ptfs *)( &ptf),  -1);
		// setbuf(stdout, NULL);
		// cout<<"gm "<<( (ptf))->path<<endl;
		// fflush(stdin);fflush(stdout);
		
		// ptfcurr=ptf;
		
		if(ptf->path!="teste")
		setscint(treeMouseButtonClicked,ptf);

		g_free(value); fflush(stdout);

	}
}

struct tfiles{
	string dir; 
	vector<ptfs*> child;
	GtkTreeIter toplevel;
	tfiles(string path){
		dir=path;
		add_dir();
		add_files_to_dir();
	}
	void add_dir(){
		gtk_tree_store_append(treestore, &toplevel, NULL);
		ptfs* ptf=new ptfs;
		ptf->path="teste";
		gtk_tree_store_set(treestore, &toplevel,COLUMN, dir.c_str(), PTF,ptf,-1); 		
	}
	void add_files_to_dir(){
		// vector<string> 	dd=dirFilesIncludingSubdirs(dir,"hpp;cpp;c;php;html;txt;");
		vector<string> 	dd=dirWildcard(dir,"*.hpp;*.cpp;*.c;*.php;*.html;*.txt;makefile;"); 
		child=vector<ptfs*>(dd.size());
		dbgvecall(dd);
		lop(i,0,dd.size()){		
			ptfs* ptf=new ptfs;
			child[i]=ptf;
			ptf->path=dd[i];
			ptf->fpath=dir+"/"+dd[i];
			cout<<"gm "<<ptf->path<<endl;
		
			gtk_tree_store_append(treestore, &child[i]->gti, &toplevel);
			gtk_tree_store_set(treestore, &child[i]->gti,
			COLUMN, dd[i].c_str(),
			PTF,  ptf,
			-1);
			ptf->gti=child[i]->gti;
		}  
		
	}
};
vector<tfiles> vtfiles;

GtkTreeModel *create_and_fill_model(void) {
	treestore = gtk_tree_store_new(NUM_COLS, G_TYPE_STRING,G_TYPE_POINTER);//G_TYPE_POINTER

	vtfiles.push_back(tfiles("/home/super/desk/code"));
	vtfiles.push_back(tfiles("/home/super/desk/tests"));
	vtfiles.push_back(tfiles("c:/desk/Mpm/fxeu/")); 
	vtfiles.push_back(tfiles("c:/desk/Mpm/fxrobotcr/")); 
	vtfiles.push_back(tfiles("c:/desk/Mpm/fxresist/"));
	vtfiles.push_back(tfiles("c:/desk/Mpm/cad"));
	vtfiles.push_back(tfiles("c:/desk/Mpm/lib"));
	vtfiles.push_back(tfiles("c:/desk/Mpm/idecpp"));
	vtfiles.push_back(tfiles("c:/desk/Mpm/idecppw"));
	vtfiles.push_back(tfiles("c:/desk/tests/"));
	vtfiles.push_back(tfiles("c:/desk/hibook"));
	vtfiles.push_back(tfiles("c:/desk/code"));
	vtfiles.push_back(tfiles("C:/desk/resoapp"));
	vtfiles.push_back(tfiles("C:/desk/Proj/site"));
// vtfiles.push_back(tfiles("E:\\zzd\\Bkp\\backup C++\\mpm\\src"));

	return GTK_TREE_MODEL(treestore);
}

GtkWidget *create_view_and_model(void) {    
  GtkTreeViewColumn *col;
  GtkCellRenderer *renderer;
  GtkWidget *view;
  GtkTreeModel *model;

  view = gtk_tree_view_new();

  col = gtk_tree_view_column_new();
  gtk_tree_view_column_set_title(col, "Explorer");
  gtk_tree_view_append_column(GTK_TREE_VIEW(view), col);

  renderer = gtk_cell_renderer_text_new();
  gtk_tree_view_column_pack_start(col, renderer, TRUE);
  gtk_tree_view_column_add_attribute(col, renderer, "text", COLUMN);
  
  // gtk_tree_view_column_add_attribute(col, renderer, "foreground", COLOR1);
   
   //g_object_set (G_OBJECT (renderer), "foreground", "blue", NULL);

  model = create_and_fill_model();
  gtk_tree_view_set_model(GTK_TREE_VIEW(view), model);
  g_object_unref(model); 

  return view;
}

GtkWidget *treeSymbCreate(void) {    
  GtkTreeViewColumn *col;
  GtkCellRenderer *renderer;
  GtkWidget *view;
  GtkTreeModel *model;

  view = gtk_tree_view_new();

  col = gtk_tree_view_column_new();
  gtk_tree_view_column_set_title(col, "Symbols");
  gtk_tree_view_append_column(GTK_TREE_VIEW(view), col);

  renderer = gtk_cell_renderer_text_new();
  gtk_tree_view_column_pack_start(col, renderer, TRUE);
  gtk_tree_view_column_add_attribute(col, renderer, "text", COLUMN);
  
  // gtk_tree_view_column_add_attribute(col, renderer, "foreground", COLOR1);
   
   //g_object_set (G_OBJECT (renderer), "foreground", "blue", NULL);

  model = create_and_fill_model();
  gtk_tree_view_set_model(GTK_TREE_VIEW(view), model);
  g_object_unref(model); 

  return view;
}
   


static void sci_setup(ScintillaObject *sci) {
	scintilla_set_id(sci, 0);
// #define ssm(m, w, l) scintilla_send_message(sci, (m), (w), (l))

//backcolor
// ssm(SCI_STYLESETBACK, STYLE_DEFAULT, 0x008000);

	ssm(SCI_SETCODEPAGE, SC_CP_UTF8, 0);
	ssm(SCI_SETIMEINTERACTION, SC_IME_WINDOWED, 0);
	//ssm(SCI_SETIMEINTERACTION, SC_IME_INLINE, 0);

	ssm(SCI_STYLESETCHARACTERSET, STYLE_DEFAULT, SC_CHARSET_DEFAULT);
	// ssm(SCI_STYLESETFONT, STYLE_DEFAULT, (sptr_t)"Monospace");
	ssm(SCI_STYLESETFONT, STYLE_DEFAULT, (sptr_t)"Courier new");
	ssm(SCI_STYLESETSIZEFRACTIONAL, STYLE_DEFAULT, 10*SC_FONT_SIZE_MULTIPLIER);
	ssm(SCI_STYLECLEARALL, 0, 0);

	ssm(SCI_SETTABWIDTH, 4, 0);
	// show line number margin
	ssm(SCI_SETMARGINWIDTHN, 0, (int)ssm(SCI_TEXTWIDTH, STYLE_LINENUMBER, (sptr_t)" 199"));
	// hide bookmark and selection margin
	ssm(SCI_SETMARGINWIDTHN, 1, 0);
	// word wrap
	ssm(SCI_SETWRAPMODE, SC_WRAP_WORD, 0);
	ssm(SCI_SETWRAPVISUALFLAGS, SC_WRAPVISUALFLAG_END, 0);
	ssm(SCI_SETWRAPINDENTMODE, SC_WRAPINDENT_INDENT, 0);

	ssm(SCI_SETLEXER, SCLEX_CPP, 0);
	// ssm(SCI_SETLEXER, SCLEX_HTML, 0);
	// ssm(SCI_SETKEYWORDS, 0, (sptr_t)"int char return");
	ssm(SCI_STYLESETFORE, SCE_C_PREPROCESSOR, 0x0080ff);
	ssm(SCI_STYLESETFORE, SCE_C_COMMENT, 0x008000);
	ssm(SCI_STYLESETFORE, SCE_C_COMMENTLINE, 0x008000);
	ssm(SCI_STYLESETFORE, SCE_C_NUMBER, 0x808000);
	ssm(SCI_STYLESETFORE, SCE_C_WORD, 0xff0000);
	ssm(SCI_STYLESETFORE, SCE_C_STRING, 0x800080);
	ssm(SCI_STYLESETBOLD, SCE_C_OPERATOR, TRUE);
	ssm(SCI_SETPROPERTY, (uptr_t)"styling.within.preprocessor", (sptr_t)"1");


}

int swidth,sheight=0;
int getHper(float per){
	return swidth*per;
}
int getVper(float per){
	return sheight*per;
}
//maximize resize window
void resizewidgets(int new_width,int new_height){
	
	int wscint=getHper(0.425);
	gtk_fixed_move (GTK_FIXED (gtf),weditor[0],getHper(0.15),0);
	gtk_widget_set_size_request(weditor[0],wscint,getVper(1));
	
	gtk_fixed_move (GTK_FIXED (gtf),weditor[1],getHper(0.15)+wscint,0);
	gtk_widget_set_size_request(weditor[1],wscint,getVper(1));
	
	gtk_fixed_move (GTK_FIXED (gtf),wtree,0,0);
	gtk_widget_set_size_request(wtree,getHper(0.15),getVper(0.7));	
	
	
	
	
	
	return;
	//gtk_window_set_resizable (GTK_WINDOW(window), 0);
	
	if(gtk_window_is_maximized (GTK_WINDOW (window) )){
		gtk_widget_set_size_request(weditor[0],700,new_height);	
		// gtk_widget_set_size_request(weditor[1],700,new_height);	
	}else{
		// gtk_window_move (GTK_WINDOW (window), 0, 400);
		gtk_widget_set_size_request(weditor[0],400,new_height);	
		// gtk_widget_set_size_request(weditor[1],400,new_height);	
	}
	
}
void sciGetRange(int start, int end, char *text) {
	ScintillaObject *sci=scicurr;
	Sci_TextRange  tr;
	tr.chrg.cpMin = start;
	tr.chrg.cpMax = end;
	tr.lpstrText = text;
	ssm(SCI_GETTEXTRANGE, 0, reinterpret_cast<LPARAM>(&tr)); //SCI_GETTEXTRANGE long
}

void sciSave(){
	ScintillaObject *sci=scicurr;
	if(ptfcurr==0)return;
	if(!ssm(SCI_GETMODIFY,0,0))return;
	FILE *fp = fopen(ptfcurr->fpath.c_str(), "wb");
	if (fp) {
		const int blockSize = 1310720;
		char data[blockSize + 1];
		int lengthDoc = ssm(SCI_GETLENGTH,0,0);
		for (int i = 0; i < lengthDoc; i += blockSize) {
			 int grabSize = lengthDoc - i;
			 if (grabSize > blockSize)
				  grabSize = blockSize;
			 sciGetRange(i, i + grabSize, data);
			 /*if (props.GetInt("strip.trailing.spaces"))
				  grabSize = StripTrailingSpaces(
									  data, grabSize, grabSize != blockSize);*/
			 fwrite(data, grabSize, 1, fp);
		}
		fclose(fp);
		ssm(SCI_SETSAVEPOINT,0,0);
		ptfcurr->saved=ssm(SCI_GETMODIFY  , 0,0);
		// if(callbackOnSave)voidToFunc(callbackOnSave,string)(filenamecurrent);
	}
}
static void on_size_allocate (GtkWidget *widget, GtkAllocation *allocation){
	cout<<endl<<"alloc "<<allocation<<endl;
	gtk_window_get_size (GTK_WINDOW (widget), &swidth, &sheight);
	printf ("W: %u x H:%u\n", swidth, sheight);
	resizewidgets( swidth, sheight);
}


gboolean on_key_press (GtkWidget *widget, GdkEventKey *event, gpointer user_data){
      printf("key pressed: %d\n", event->keyval);
  switch (event->keyval) {
		case GDK_KEY_G:
		case GDK_KEY_g:
		if (event->state & GDK_CONTROL_MASK) {
			printf("key pressed: %s\n", "ctrl + g");
			ScintillaObject *sci=scicurr;
			ssm(SCI_FOLDALL,SC_FOLDACTION_TOGGLE,0);	
			
		}
		break;
		case GDK_KEY_S:
		case GDK_KEY_s:
		if (event->state & GDK_CONTROL_MASK) {
			printf("key pressed: %s\n", "ctrl + s");
			ScintillaObject *sci=scicurr;
			sciSave();  	
			setttbar();
		}
		break;
		case GDK_KEY_Q:
		case GDK_KEY_q:
		if (event->state & GDK_CONTROL_MASK) { 
			ScintillaObject *sci=scicurr; 
			sciCommment(0);
		}
		break;
    case GDK_KEY_p:
      printf("key pressed: %s\n", "p");
      break;
    // case GDK_KEY_S:
    // case GDK_KEY_s:
      // if (event->state & GDK_SHIFT_MASK) {
        // printf("key pressed: %s\n", "shift + s");
      // }
      // else if (event->state & GDK_CONTROL_MASK) {
        // printf("key pressed: %s\n", "ctrl + s");
      // }
      // else{
        // printf("key pressed: %s\n", "s");
      // }
      // break;
    case GDK_KEY_E:
    case GDK_KEY_e:
      if (event->state & GDK_SHIFT_MASK) {
        printf("key pressed: %s\n", "shift + m");
      }
      else if (event->state & GDK_CONTROL_MASK){
        printf("key pressed: %s\n", "ctrl + m");
	  // gtk_tree_store_set(treestore, &ichild[4],
                     // COLUMN, "Python44",
                     // -1);
					 
					 
		
		vtfiles.push_back(tfiles("c:/desk/coder"));
      }
      break;

    default:
      return FALSE; 
  }

  return FALSE; 
}

// https://developer.gnome.org/gtk3/stable/gtk-getting-started.html
static void app_activate(GtkApplication *app, UNUSE_ATTR gpointer user_data) {
	window = gtk_application_window_new(app);
	g_signal_connect (G_OBJECT (window), "key_press_event", G_CALLBACK (on_key_press), NULL);
	gtk_window_set_title (GTK_WINDOW (window), appname);
	// gtk_window_set_default_size (GTK_WINDOW (window), 800, 600);
	gtk_window_maximize(GTK_WINDOW (window)); 
	
	// janela sem title bar
	// gtk_window_set_decorated(GTK_WINDOW (window),0);
	gtk_widget_show_all (window);
	 
	 g_signal_connect(G_OBJECT(window), "configure-event", G_CALLBACK(on_size_allocate), NULL);
	
	GdkRectangle workarea = {0};
	gdk_monitor_get_workarea(   gdk_display_get_primary_monitor(gdk_display_get_default()),  &workarea);
	printf ("W: %u x H:%u\n", workarea.width, workarea.height);

	int new_width, new_height;
	gtk_window_get_size (GTK_WINDOW (window), &new_width, &new_height);
	printf ("W: %u x H:%u\n", new_width, new_height);

	gtf=gtk_fixed_new ( );
	gtk_container_add (GTK_CONTAINER (window), gtf);
	
	wtree = gtk_scrolled_window_new (NULL, NULL);
	//gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(wtree), GTK_POLICY_NEVER, GTK_POLICY_ALWAYS); //never horizontal, always vertical
	g_object_set(wtree, "overlay-scrolling", FALSE, NULL);
	gtk_fixed_put (GTK_FIXED (gtf),wtree,0,0);
	gtk_widget_set_size_request(wtree,400,900);
	
	
	
	    GtkWidget *box;
	GtkWidget *menubar;
    GtkWidget *filemenu;
    GtkWidget *file;
    GtkWidget *open;
    GtkWidget *quit;
	menubar = gtk_menu_bar_new();
    filemenu = gtk_menu_new();
    file = gtk_menu_item_new_with_label("File");
    open = gtk_menu_item_new_with_label("Open");
    quit = gtk_menu_item_new_with_label("Quit");
    gtk_menu_item_set_submenu(GTK_MENU_ITEM(file), filemenu);
    gtk_menu_shell_append(GTK_MENU_SHELL(filemenu), open);
    gtk_menu_shell_append(GTK_MENU_SHELL(filemenu), quit);
    gtk_menu_shell_append(GTK_MENU_SHELL(menubar), file);	
    box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 5);
    gtk_box_pack_start(GTK_BOX(box), menubar, FALSE, FALSE, 3);
	
	
	dbgv(0);
	 
	
	
	tree = create_view_and_model();
        // gtk_box_pack_start (GTK_BOX (box), tree, FALSE, 0, 0);
    // gtk_container_add(GTK_CONTAINER(wtree), box);
	// gtk_widget_show (tree);
	
	
	GtkTreeSelection* selection = gtk_tree_view_get_selection(GTK_TREE_VIEW(tree));
	// g_signal_connect(selection, "changed", G_CALLBACK(on_changed), NULL);	
	// g_signal_connect(tree, "row-activated", G_CALLBACK(treeClick), NULL); //funciona duploclick
	g_signal_connect(tree, "button-press-event", G_CALLBACK(treeClick), selection);
	// gtk_fixed_put (GTK_FIXED (gtf),tree,0,0);
	// gtk_widget_set_size_request(tree,400,200);
	// setup_tree();
	gtk_container_add (GTK_CONTAINER (wtree), tree);
        // gtk_box_pack_start (GTK_BOX (box), tree, FALSE, TRUE, 5);
	// gtk_widget_show (tree);
	
	weditor[0]= gtk_scrolled_window_new(NULL,0);
	editor[0] = scintilla_new();	
	
	gtk_fixed_put (GTK_FIXED (gtf),weditor[0],400,0);
	gtk_widget_set_size_request(weditor[0],700,new_height);	 
	sci_setup(SCINTILLA(editor[0]));
	
		sci_setStyleCommon(SCINTILLA(editor[0]));
		sci_setStyleC(SCINTILLA(editor[0]));
		
	
	ScintillaObject *sci=SCINTILLA(editor[0]);
	scicurr=sci;
	ssm(SCI_INSERTTEXT, 0, (sptr_t)
	 "#include <gtk/gtk.h>\n"
	 "\n"
	 "int main(int argc, char **argv) {\n"
	 "	// show GTK version\n"
	 "	printf(\"GTK %d.%d.%d\\n\", GTK_MAJOR_VERSION, GTK_MINOR_VERSION, GTK_MICRO_VERSION);\n"
	 "	return 0;\n"
	 "}\n"
	 "\n"
	);
	char buf[128];
	int length = snprintf(buf, sizeof(buf) - 1, "// GTK %d.%d.%d\n", GTK_MAJOR_VERSION, GTK_MINOR_VERSION, GTK_MICRO_VERSION);
	ssm(SCI_APPENDTEXT, length, (sptr_t)buf);
		
		
		
		
		
		
	gtk_container_add (GTK_CONTAINER (weditor[0]), editor[0]);
	
	// g_object_ref(editor[0]);
	// gtk_container_remove (GTK_CONTAINER(gtf),editor[0]);
	// editor[0] = scintilla_new();
	// gtk_fixed_put (GTK_FIXED (gtf),editor[0],400,0);
	// gtk_widget_set_size_request(editor[0],700,new_height);
	
	// editor[1] = editor[0];
	weditor[1]= gtk_scrolled_window_new(NULL,0);
	editor[1] = scintilla_new();
	gtk_fixed_put (GTK_FIXED (gtf),weditor[1],400+700,0);
	// gtk_window_move (GTK_WINDOW(editor[1]),400+750,0);//dont work
	gtk_fixed_move (GTK_FIXED (gtf),weditor[1],400+750,0);//
	gtk_widget_set_size_request(weditor[1],700,new_height);	 
	sci_setup(SCINTILLA(editor[1]));
	gtk_container_add (GTK_CONTAINER (weditor[1]), editor[1]);
	
	gtk_widget_show (gtf);
	
	gtk_widget_show_all (window);
	// gtk_widget_grab_focus(editor[0]);
	// scicurr=sci;
	
	// g_signal_connect(SCINTILLA(editor[0]),
                      // "sci-notify",
                      // G_CALLBACK(onScintillaNotifcation),
                      // NULL);
	
	g_signal_connect( (editor[0]), "sci-notify", G_CALLBACK(onScintillaNotifcation), (gpointer) 0);
	g_signal_connect( (editor[1]), "sci-notify", G_CALLBACK(onScintillaNotifcation), (gpointer) 1);
	
	// GtkAllocation* alloc = g_new(GtkAllocation, 1);
    // gtk_widget_get_allocation(window, alloc);
    // printf("widget size is currently %dx%d\n",alloc->width, alloc->height);
    // g_free(alloc);
	
	
	// gtk_widget_set_name(widget,"nameid"); 
	GtkCssProvider *cssProvider = gtk_css_provider_new();
	// GtkCssProvider *cssProvider = gtk_css_provider_get_default ();
	// gtk_css_provider_load_from_path(cssProvider, "theme.css", NULL);
	gtk_css_provider_load_from_data (cssProvider,  ".scrollbar.vertical slider,scrollbar.vertical slider {min-height: 50px;min-width: 15px;} scrollbar.hover,scrollbar { margin-left: 0px; -GtkScrollbar-has-backward-stepper: 1; -GtkScrollbar-has-forward-stepper: 1;}", -1, NULL);
	gtk_style_context_add_provider_for_screen(gdk_screen_get_default(), GTK_STYLE_PROVIDER(cssProvider), GTK_STYLE_PROVIDER_PRIORITY_USER);
	
}
int singleinstancearg(int argc, char **argv);
int main(int argc, char **argv) { 
	// AppSetWorkingDir("c:/desk");
	// ShellExecute(0,"open","cmd","/C cmd ",0,SW_SHOW);
	
	if(singleinstancearg(argc, argv)==0)return 0;
	gtk_init(&argc, &argv); 
	g_object_set (gtk_settings_get_default (), "gtk-primary-button-warps-slider", FALSE, NULL);
	g_object_set (gtk_settings_get_default (), "gtk-theme-name", "Adwaita", NULL);
	g_object_set (gtk_settings_get_default (), "gtk-application-prefer-dark-theme", TRUE, NULL);
	// make sure the type is realized
	// g_type_class_unref (g_type_class_ref (GTK_TYPE_IMAGE_MENU_ITEM));
	// GdkScreen *screen=gdk_screen_get_default();
	// GtkSettings *settings=gtk_settings_get_for_screen  (screen);
	// cout<<endl<<settings;
	// GtkSettingsValue* gsv=new GtkSettingsValue();
	// gsv->value=(GValue* )0;
	// gtk_settings_set_property_value (settings,"gtk-primary-button-warps-slider",gsv);
	// gtk_settings_set_string_property (settings,"gtk-primary-button-warps-slider","0",0);
	
	GtkApplication *app = gtk_application_new(appname, G_APPLICATION_FLAGS_NONE);
	g_signal_connect(app, "activate", G_CALLBACK(app_activate), NULL);
	int status = g_application_run(G_APPLICATION(app), argc, argv);	

	g_object_unref(app);

	return status;
}

#include <boost/interprocess/managed_shared_memory.hpp>
#include <boost/interprocess/allocators/allocator.hpp>
#include <boost/interprocess/containers/string.hpp>
#include <cstring>
#include <cstdlib>
#include <string>
#include <iostream>
#include <thread>
//#include "threads.hpp"
#include <signal.h>

const char* MySharedMemory=appname;

void handleSIGINT(int param=0) {
      boost::interprocess::shared_memory_object::remove(MySharedMemory); 
}
void onexit(void) {
      boost::interprocess::shared_memory_object::remove(MySharedMemory); 
} 
 
int singleinstancearg(int argc, char **argv){ 
	// return 1;
//	  boost::interprocess::shared_memory_object::remove(MySharedMemory); 
	using namespace boost::interprocess;
	typedef boost::interprocess::allocator<char, boost::interprocess::managed_shared_memory::segment_manager> CharAllocator;
	typedef boost::interprocess::basic_string<char, std::char_traits<char>, CharAllocator> string;
	try{
		//second instance
		managed_shared_memory shm (open_only, MySharedMemory); //nao existe da erro
		std::pair<string *,std::size_t> ret = shm.find<string>("instance");
		std::cout <<"instance " << *(ret.first)   << std::endl; 
		if(argc<2)return 0;
		char* arg=argv[1];
		shm.destroy<string>("newdoc");
		shm.find_or_construct<string>("newdoc")(arg, shm.get_segment_manager()); 
		return 0;
	}catch(...){	
		//first instance
		//Create a shared memory object.
		managed_shared_memory shm (create_only, MySharedMemory, 1024);
		// cout<<endl<<"id "<<getpid()<<endl;
		// string var=std::this_thread::get_id()
		// shm.find_or_construct<string>("newdoc")("teste", shm.get_segment_manager()); 
		string *s = shm.find_or_construct<string>("instance")("Hello!", shm.get_segment_manager());
		
		// shm.destroy<string>("newdoc");
		// shm.find_or_construct<string>("newdoc")("teste1", shm.get_segment_manager());
		std::cout << *s << std::endl; 
		atexit(onexit);
		signal(SIGINT, handleSIGINT);
		
		//listener
		threadDetach([](){
			managed_shared_memory shm (open_only, MySharedMemory);
			for(;;){
				sleep(100); 
				std::pair<string *,std::size_t> ret = shm.find<string>("newdoc");
				if(ret.first){
					stringstream res;
					res<<*(ret.first) ;
					std::cout <<"newdoc " << *(ret.first)   << std::endl;
					shm.destroy<string>("newdoc");
					vtfiles.push_back(tfiles(ExtractDirectory(res.str())));
				} 
			}
		});
		return 1;
	}
	return 0;
}
