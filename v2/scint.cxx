#include "regular.hpp"
#include "stringf.hpp"
#include "scint.hpp"
#include <unordered_map>
#include <fstream>
#include <scintilla.h>
#include <SciLexer.h>
#include <ILexer.h>  
     
bool handlerset=false;
scint* csw=NULL;
void* callbackOnSave=NULL;

vector<scint*> editores;
unordered_map<string,LRESULT> files;
#define SSM(m,...) SendMessage( m_hScintilla,m,##__VA_ARGS__)
#define SSMm(...) SendMessage( m_hScintilla,##__VA_ARGS__)

string scint::text(){
	int lengthDoc = ssm(SCI_GETLENGTH)+1;
	char data[lengthDoc ];
	ssm(SCI_GETTEXT,lengthDoc, (LPARAM)data);  
	return data;
};
void scint::text(string val){ 
	ssm(SCI_SETTEXT,0, (LPARAM)val.c_str()); 
};
 
void scint::foldAll(int flag){
	int lp=ssm(SCI_LINEFROMPOSITION,10000000);
	vbool foldp(lp); 
	lop(i,0,lp){
		int a=ssm(SCI_GETFOLDEXPANDED,i);
		foldp[i]= a; 
	}
	lop(i,0,lp){
//		ssm(SCI_SETFOLDEXPANDED,i,(LPARAM)!foldp[i]);
	ssm(SCI_FOLDLINE,i,flag);
	}
//	int startl=ssm(SCI_GETSELECTIONSTART);
//	startl=ssm(SCI_LINEFROMPOSITION,startl);
//	ssm(SCI_TOGGLEFOLD,startl); 

};
void scint::commentUncommment(int flag){
//	Beep(1500,200);
	int selsz=ssm(SCI_GETSELTEXT);
	char buffsel[selsz];
	ssm(SCI_GETSELTEXT, 0, (LPARAM) buffsel);

	int startl=ssm(SCI_GETSELECTIONSTART);
	startl=ssm(SCI_LINEFROMPOSITION,startl);
	int endl=ssm(SCI_GETSELECTIONEND);
	endl=ssm(SCI_LINEFROMPOSITION,endl);

	if(flag==0)
	lop(i,startl,endl+1){
		int li=ssm(SCI_POSITIONFROMLINE,i);
		ssm(SCI_INSERTTEXT,li,(LPARAM) "//");
	}

	if(flag==1)
	lop(i,startl,endl+1){
		int li=ssm(SCI_POSITIONFROMLINE,i);
		int lisz=ssm( SCI_LINELENGTH,i);
		char buffline[lisz];
		ssm(SCI_GETLINE, i, (LPARAM) buffline);
		lop(c,0,lisz){
			if(buffline[c]=='\t' || buffline[c]==' ')continue;
			if(string(buffline+c,2)=="//")
				ssm(SCI_DELETERANGE,li+c,2);
			break;
		}

//SCI_DELETERANGE

//		ssm(SCI_INSERTTEXT,li,"//");
	}

//	SCI_GETSELECTIONEND
//	SCI_LINEFROMPOSITION(int position)
//	SCI_LINEFROMPOSITION(int position)
//	SCI_POSITIONFROMLINE(int line)
};


void scint::highLightAll(string word){
	///COULD HAVE BETTERS SPEED
//	if(word=="")return;
	ssm(SCI_SETINDICATORCURRENT,1,0);
	ssm(SCI_INDICSETSTYLE,1,INDIC_ROUNDBOX);
	int sz=ssm(SCI_GETLENGTH);
	if(word.size()<2){ssm(SCI_INDICATORCLEARRANGE,0, sz);return;}
	char buff[sz+1];
	ssm(SCI_GETTEXT, sz+1, (LPARAM) buff);
	vector<int> sp=stringPositions(buff,(char*)word.c_str(),sz+1,word.size());
	for( int i: sp)
		ssm(SCI_INDICATORFILLRANGE,i,word.size());

};

int scint::ssm(uint Msg, WPARAM wParam, LPARAM lParam) {
	return 1;//scintilla_send_message(m_hScintilla, Msg, wParam, lParam);//::SendMessage
}

void scint::save(){
	if(filenamecurrent=="")return;
	if(!ssm(SCI_GETMODIFY))return;
	FILE *fp = fopen(filenamecurrent.c_str(), "wb");
	if (fp) {
		char data[blockSize + 1];
		int lengthDoc = ssm(SCI_GETLENGTH);
		for (int i = 0; i < lengthDoc; i += blockSize) {
			 int grabSize = lengthDoc - i;
			 if (grabSize > blockSize)
				  grabSize = blockSize;
			 GetRange(i, i + grabSize, data);
			 /*if (props.GetInt("strip.trailing.spaces"))
				  grabSize = StripTrailingSpaces(
									  data, grabSize, grabSize != blockSize);*/
			 fwrite(data, grabSize, 1, fp);
		}
		fclose(fp);
		ssm(SCI_SETSAVEPOINT);
		if(callbackOnSave)voidToFunc(callbackOnSave,string)(filenamecurrent);
	}
}

void scint::load(string filename){
	save();//this will always save
	filenamecurrent=filename;
	if(files[filename]==NULL){
	//		ifstream is (filename.c_str(),ios::binary);
	FILE *fp = fopen(filename.c_str(), "rb");
	if (fp) {
		//fileModTime = GetModTime(fullPath);
		LRESULT p=ssm(SCI_GETDOCPOINTER);
		ssm(SCI_ADDREFDOCUMENT,0, p);
		p=ssm(SCI_CREATEDOCUMENT);
		ssm(SCI_SETDOCPOINTER,0, p);
		ssm(SCI_CLEARALL);
		// Disable UNDO
		ssm(SCI_SETUNDOCOLLECTION, 0);

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
		ssm(SCI_SETUNDOCOLLECTION, 1);
		ssm(SCI_SETSAVEPOINT);
		files[filename]=ssm(SCI_GETDOCPOINTER);
	}
	}else{
	LRESULT p=ssm(SCI_GETDOCPOINTER);
	ssm(SCI_ADDREFDOCUMENT,0, p);
	p=files[filename];
	ssm(SCI_SETDOCPOINTER,0, files[filename]);
	//            ssm(SCI_RELEASEDOCUMENT,0, p);

	dbgv(*((int*)p));
	//            if(*((int*)p)!=1)ssm(SCI_RELEASEDOCUMENT,0, p);
	}

	setStyle();
	}

void scint::resize(int x, int y, int w, int h){
//dbgv(99);
//	Fl_Double_Window::resize(x,y,w,h);
	if (m_hScintilla) {
		// MoveWindow(m_hScintilla,x,y,w,h,TRUE);
	}
};

void scint::GetRange(int start, int end, char *text) {
	Sci_TextRange tr;
	tr.chrg.cpMin = start;
	tr.chrg.cpMax = end;
	tr.lpstrText = text;
	ssm(SCI_GETTEXTRANGE, 0, reinterpret_cast<LPARAM>(&tr)); //SCI_GETTEXTRANGE long
}

void scint::setStyle(){
	ssm(SCI_SETLEXER, SCLEX_CPP, 0);
	ssm(SCI_STYLESETFONT, STYLE_DEFAULT, (LPARAM)"Courier new");
	ssm(SCI_STYLESETSIZE, STYLE_DEFAULT, 10);
	ssm(SCI_SETTABWIDTH, 2);
	ssm(SCI_STYLECLEARALL, 0, 0);
	ssm(SCI_SETPROPERTY,(WPARAM)"fold", (LPARAM)"1");
	ssm(SCI_SETPROPERTY, (WPARAM)"fold.comment", (LPARAM)"1");
	ssm(SCI_SETPROPERTY, (WPARAM)"fold.preprocessor", (LPARAM)"1");
	ssm(SCI_SETKEYWORDS, 0, (sptr_t)"_Char16_t _Char32_t align_union alignof asm auto bool break case catch char class const const_cast constexpr continue decltype default delete do double dynamic_cast else enum explicit export extern false final float for friend goto if import inline int long mutable namespace new nullptr operator override private protected public register reinterpret_cast return short signed sizeof static static_cast struct switch template this throw true try typedef typeid typename union unsigned using virtual void volatile wchar_t while static_assert int8_t uint8_t int16_t uint16_t int32_t uint32_t int64_t uint64_t int_least8_t uint_least8_t int_least16_t uint_least16_t int_least32_t uint_least32_t int_least64_t uint_least64_t int_fast8_t uint_fast8_t int_fast16_t uint_fast16_t int_fast32_t uint_fast32_t int_fast64_t uint_fast64_t intptr_t uintptr_t intmax_t uintmax_t wint_t wchar_t wctrans_t wctype_t size_t time_t and and_eq bitand bitor compl not not_eq or or_eq xor xor_eq");
	 ssm(SCI_STYLESETFORE, SCE_C_WORD, 0x700000);
	 ssm(SCI_STYLESETBOLD, SCE_C_WORD, 1);
	 ssm(SCI_SETKEYWORDS, 1, (sptr_t)"uchar uint ushort ulong ullong vshort vvshort vstring vvstring vbool vvbool vint vvint vfloat vvfloat to_string __gnu_cxx accumulate add_const add_cv add_lvalue_reference add_pointer add_reference add_rvalue_reference add_volatile adjacent_difference adjacent_find aligned_storage Alignment alignment_of all_of allocate_shared allocator allocator_base allocator_chunklist allocator_fixed_size allocator_newdel allocator_suballoc allocator_unbounded allocator_variable_size any_of array assign at atomic_bool atomic_char atomic_char16_t atomic_char32_t atomic_compare_exchange_strong atomic_compare_exchange_strong_explicit atomic_compare_exchange_weak atomic_compare_exchange_weak_explicit atomic_exchange atomic_exchange_explicit atomic_fetch_add atomic_fetch_and atomic_fetch_or atomic_fetch_sub atomic_fetch_xor atomic_int atomic_int_fast16_t atomic_int_fast32_t atomic_int_fast64_t atomic_int_fast8_t atomic_int_least16_t atomic_int_least32_t atomic_int_least64_t atomic_int_least8_t atomic_intmax_t atomic_intptr_t atomic_is_lock_free atomic_llong atomic_load atomic_load_explicit atomic_long atomic_ptrdiff_t atomic_schar atomic_short atomic_size_t atomic_ssize_t atomic_store atomic_store_explicit atomic_uchar atomic_uint atomic_uint_fast16_t atomic_uint_fast32_t atomic_uint_fast64_t atomic_uint_fast8_t atomic_uint_least16_t atomic_uint_least32_t atomic_uint_least64_t atomic_uint_least8_t atomic_uintmax_t atomic_uintptr_t atomic_ullong atomic_ulong atomic_ushort atomic_wchar_t auto_ptr back back_insert_iterator back_item bad_alloc bad_function_call bad_weak_ptr basic_filebuf basic_fstream basic_ifstream basic_ofstream basic_regex basic_streambuf basic_string begin bernoulli_distribution bidirectional_iterator_tag binary_function binary_negate binary_search bind bind1st bind2nd binder1st binder2nd binomial_distribution bit_and bit_or bit_xor bitset boost cache_chunklist cache_freelist cache_suballoc cauchy_distribution cbegin cend cerr char_traits checked_array_iterator checked_uninitialized_copy checked_uninitialized_fill_n chi_squared_distribution cin clear codecvt codecvt_base codecvt_byname codecvt_mode codecvt_utf16 codecvt_utf8 codecvt_utf8_utf16 collate collate_byname common_type compare_exchange_strong compare_exchange_weak complex condition_variable conditional const_iterator const_mem_fun_ref_t const_mem_fun_t const_mem_fun1_ref_t const_mem_fun1_t const_pointer_cast const_reference const_reverse_iterator copy copy_backward copy_if copy_n count count_if cout crbegin cref crend ctype ctype_base ctype_byname decay declare_no_pointers declare_reachable declval default_delete default_random_engine deque difference_type discard_block discard_block_engine discrete_distribution divides domain_error dynamic_pointer_cast empty enable_if enable_shared_from_this end endl equal equal_range equal_to EqualityComparable erase error_category error_code error_condition exception exponential_distribution extent extreme_value_distribution fetch_add fetch_and fetch_or fetch_sub fetch_xor filebuf fill fill_n find find_end find_first_of find_first_not_of find_if find_if_not find_last_of find_last_not_of fisher_f_distribution float_denorm_style float_round_style for_each forward forward_iterator_tag forward_list freelist front front_insert_iterator front_item fstream function gamma_distribution generate generate_n generic_container generic_iterator generic_reverse_iterator generic_value geometric_distribution get_deleter get_pointer_safety get_temporary_buffer greater greater_equal has_nothrow_assign has_nothrow_constructor has_nothrow_copy has_nothrow_copy_assign has_nothrow_copy_constructor has_nothrow_default_constructor has_trivial_assign has_trivial_constructor has_trivial_copy has_trivial_copy_assign has_trivial_copy_constructor has_trivial_default_constructor has_trivial_destructor has_virtual_destructor hash hash_map hash_set ifstream includes independent_bits_engine initializer_list inner_product inplace_merge input_iterator_tag insert insert_iterator integral_constant invalid_argument ios_base iostream is_abstract is_arithmetic is_array is_base_of is_bind_expression is_class is_compound is_const is_constructible is_convertible is_empty is_enum is_error_code_enum is_error_condition_enum is_explicitly_convertible is_floating_point is_function is_fundamental is_heap is_heap_until is_integral is_literal_type is_lock_free is_lvalue_reference is_member_function_pointer is_member_object_pointer is_member_pointer is_nothrow_constructible is_object is_partitioned is_placeholder is_pod is_pointer is_polymorphic is_reference is_rvalue_reference is_same is_scalar is_signed is_sorted is_sorted_until is_standard_layout is_trivial is_union is_unsigned is_void is_volatile istream istream_iterator istreambuf_iterator iter_swap iterator iterator_traits knuth_b length_error less less_equal LessThanComparable lexicographical_compare linear_congruential linear_congruential_engine list locale logic_error logical_and logical_not logical_or lognormal_distribution lower_bound make_checked_array_iterator make_heap make_shared make_signed make_unsigned map match_results max max_element max_fixed_size max_none max_unbounded max_variable_size mem_fn mem_fun mem_fun_ref mem_fun_ref_t mem_fun_t mem_fun1_ref_t mem_fun1_t merge mersenne_twister mersenne_twister_engine messages messages_base messages_byname min min_element minmax minmax_element minstd_rand minstd_rand0 minus mismatch modulus money_base money_get money_put moneypunct moneypunct_byname move move_backward move_iterator mt19937 mt19937_64 multimap multiplies multiset negate negative_binomial_distribution new_handler next_permutation none_of normal_distribution not_equal_to not1 not2 nothrow nothrow_t npos nth_element num_get num_put numeric_limits numpunct numpunct_byname ofstream ostream_iterator ostreambuf_iterator out_of_range output_iterator_tag overflow_error owner_less pair partial_sort partial_sort_copy partial_sum partition partition_copy partition_point piecewise_constant_distribution piecewise_linear_distribution plus pointer_safety pointer_to_binary_function pointer_to_unary_function poisson_distribution pop_back pop_front pop_heap prev_permutation priority_queue ptr_fun push_back push_front push_heap queue random_access_iterator_tag random_device random_shuffle range_error rank ranlux_base_01 ranlux24 ranlux24_base ranlux3 ranlux3_01 ranlux4 ranlux4_01 ranlux48 ranlux48_base ranlux64_base_01 ratio ratio_add ratio_divide ratio_multiply ratio_subtract raw_storage_iterator rbegin rdbuf ref reference reference_wrapper regex regex_constants regex_error regex_iterator regex_token_iterator regex_traits remove remove_all_extents remove_const remove_copy remove_copy_if remove_cv remove_extent remove_if remove_pointer remove_reference remove_volatile rend replace replace_copy replace_copy_if replace_if requires resize result_of return_temporary_buffer reverse reverse_copy reverse_iterator rotate rotate_copy rts_alloc runtime_error search search_n seed_seq set set_difference set_intersection set_new_handler set_symmetric_difference set_union setprecision setw shared_ptr shuffle_order_engine size size_type sort sort_heap stable_partition stable_sort stack static_pointer_cast std streambuf string stringstream student_t_distribution sub_match substr subtract_with_carry subtract_with_carry_01 subtract_with_carry_engine swap swap_ranges sync_none sync_per_container sync_per_thread sync_shared system_error time_base time_get time_get_byname time_put time_put_byname to_array tr1 transform tuple tuple_element tuple_size type_info unary_function unary_negate unchecked_uninitialized_copy unchecked_uninitialized_fill_n undeclare_no_pointers undeclare_reachable underflow_error uniform_int uniform_int_distribution uniform_real uniform_real_distribution uninitialized_copy uninitialized_copy_n uninitialized_fill uninitialized_fill_n unique unique_copy unique_ptr unordered_map unordered_multimap unordered_multiset unordered_set upper_bound valarray value_type variate_generator vector wcerr wcin wcout weak_ptr weibull_distribution wfilebuf wfstream wifstream wiostream wistream wofstream wregex xor_combine");
	 ssm(SCI_STYLESETFORE, SCE_C_WORD2, 0x007000);
	 ssm(SCI_STYLESETBOLD, SCE_C_WORD2, 1);
	 ssm(SCI_STYLESETFORE, SCE_C_COMMENT, 0x008000);
	 ssm(SCI_STYLESETFORE, SCE_C_COMMENTLINE, 0x008000);
	 ssm(SCI_STYLESETFORE, SCE_C_COMMENTDOC, 0x008010);
	 ssm(SCI_STYLESETFORE, SCE_C_PREPROCESSOR, 0x008000);
	 ssm(SCI_STYLESETFORE, SCE_C_NUMBER, 0x808040);
	 ssm(SCI_STYLESETFORE, SCE_C_STRING, 0x800080);
	 ssm(SCI_STYLESETFORE, SCE_C_DEFAULT, 0x800080);
	 ssm(SCI_STYLESETFORE, SCE_C_OPERATOR, 0x0000aa);
	 ssm(SCI_STYLESETSIZE, SCE_C_OPERATOR, 8);
	//       ssm(SCI_STYLESETFORE, SCE_C_VERBATIM, 0x800000);
	//       ssm(SCI_STYLESETFORE, SCE_C_IDENTIFIER, 0x800000);
	}



#include <FL/Fl_Menu.H>
Fl_Menu_Item rclick_menu[] = {
{ "Copy",   0, [](Fl_Widget* o, void *userdata){
	dbgv("cop");
	o->hide();
} },
{ "Paste",  0, [](Fl_Widget*, void *userdata){}},
{ 0 }};

 

// int editor=-1;
static const int MARGIN_SCRIPT_FOLD_INDEX = 1;
SCNotification *snc;
//NMHDR* nm;
static int event_handler(int e){

/*
	// if (fl_msg.message == WM_NOTIFY ) {
//		nm = (NMHDR *) fl_msg.lParam;
		snc = (SCNotification *) fl_msg.lParam;
//dbgv(nm->code);
		lop(i,0,editores.size())
			if(editores[i]->m_hScintilla==snc->nmhdr.hwndFrom)
				editor=i;
		if(editor>=0){
			csw=editores[editor];
			int code=snc->nmhdr.code;

			if(code==SCN_UPDATEUI){
				lop(i,0,csw->onUpdate.size())
					voidToFunc(csw->onUpdate[i])();
				int selsz=csw->ssm(SCI_GETSELTEXT);
				char buffsel[selsz];
				csw->ssm(SCI_GETSELTEXT, 0, (LPARAM) buffsel);
				csw->highLightAll(buffsel); ///expand to all views
			}

			if(snc->margin==MARGIN_SCRIPT_FOLD_INDEX){
				const int line_number = csw->ssm(SCI_LINEFROMPOSITION, snc->position, 0);
				csw->ssm(SCI_TOGGLEFOLD, line_number);
			}
		}

// }
*/
return 0;

//	if(fl_msg.hwnd==editores[0] || fl_msg.hwnd==editores[1]){
//
//		scint *ww = find_scintilla(fl_msg.hwnd);
//		if (!ww)return 0;
////		dbgv(fl_msg.message); 
//
//		if (fl_msg.message == WM_NOTIFY) {
//			SCNotification* snc=(SCNotification*)fl_msg.lParam;
//
//		if(snc->nmhdr.code==SCN_FOCUSIN)
//			currentScintillaWindow=ww;
////		if(snc->nmhdr.code==SCN_FOCUSOUT)
////			dbgv(snc->nmhdr.code);
//
//		if(snc->nmhdr.code==SCN_UPDATEUI){
//			lop(i,0,ww->onUpdate.size())voidToFunc(ww->onUpdate[i])();
//			int selsz=ww->ssm(SCI_GETSELTEXT);
//			char buffsel[selsz];
//			ww->ssm(SCI_GETSELTEXT, 0, (LPARAM) buffsel);
//			ww->highLightAll(buffsel); ///expand to all views
//		}
//
//			if(snc->margin==MARGIN_SCRIPT_FOLD_INDEX){
//				const int line_number = ww->ssm(SCI_LINEFROMPOSITION, snc->position, 0);
//				ww->ssm(SCI_TOGGLEFOLD, line_number);
//			}
//
//			if(snc->modificationType==SC_MOD_INSERTCHECK ){
//				if(snc->text[0]==''){
//	//				int selsz=ww->ssm(SCI_GETSELTEXT);
//	//				char buffsel[selsz];
//	//				ww->ssm(SCI_GETSELTEXT, 0, buffsel);
//	//				ww->ssm(SCI_CHANGEINSERTION, selsz, buffsel);
//				}
//			}
//		}
//	}
	return 0;
}; 

//int scint::handle(int e){
//	if(e==FL_UNFOCUS)dbgv(33);
//	return 1;
//}

#include <FL/Fl_Double_Window.H> 

struct MyWindowD: Fl_Double_Window{
	MyWindowD(int X,int Y,int W,int H,const char*L=0) ;
//	void resize(int X,int Y,int W,int H) ;
//	void draw();
//	void draw_overlay() ;
	int handle(int e);
}; 

MyWindowD::MyWindowD(int X,int Y,int W,int H,const char* L) : Fl_Double_Window(X,Y,W,H,L) {xclass("Scintilla"); } 
int MyWindowD::handle(int e){
 int rc=Fl_Double_Window::handle(e);
	return rc;
}

scint::scint (int x,int y,int w,int h,void* win){

	// if(!handlerset)
		// Scintilla_RegisterClasses( flxid(this) );
	// LoadLibrary("SciLexer.DLL");
//tune mainwindow to handle events to parent


//MyWindowD* wp=new MyWindowD(x,y,w,h);
//((Fl_Double_Window*)win)->add(wp);
//wp->show();
//dbgv(flxid(wp)==NULL);
//m_hScintilla = flxid(wp);


//char class_name[100];
//strcpy(class_name, "FLTK");
//UnregisterClass(class_name,  (HINSTANCE)GetWindowLong(fl_xid(win), -6));


//((Fl_Double_Window*)win)->add(this);
//xclass("Scintilla");	
//show();
//m_hScintilla = flxid(this);
//dbgv(flxid(this)==NULL);

	// m_hScintilla = ::CreateWindowEx(0,"Scintilla","", WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_CLIPCHILDREN,
	// x,y,w,h,flxid(win),(HMENU)(NULL), ::GetModuleHandle (NULL) ,NULL);
	
	// m_hScintilla = CreateWindowEx(  0, "Scintilla", "", 
                                        // WS_CHILD | WS_VISIBLE | WS_TABSTOP | 
                                        // WS_CLIPCHILDREN,
                                        // x,y,w,h,fl_xid((Fl_Double_Window*)this ), NULL /*(HMENU)GuiID*/, 
                                        // GetModuleHandle (0), NULL );
										// show();
										 
	// m_hScintilla = ::CreateWindowEx(0,"Scintilla","", WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_CLIPCHILDREN,
	// x,y,w,h,flxid(win),(HMENU)(501), (HINSTANCE) GetWindowLong (fl_xid(this), GWLP_HINSTANCE) ,NULL);
	 
	// m_hScintilla = CreateWindow("static", "ST_U",
                              // WS_CHILD | WS_VISIBLE | WS_TABSTOP,
                              // x, y, w, h,
                              // flxid(win), (HMENU)(501),
                              // (HINSTANCE) GetWindowLong (fl_xid(this), GWLP_HINSTANCE), NULL);
   // SetWindowText(m_hScintilla, "User:");
	
	
	
	 if (m_hScintilla==NULL)
		 //dbgv(flxid(win))
    {   dbgv( "Unable to create editor window\n" );
        // return;
    }
// ((Fl_Double_Window*)win)->add(this);

////Fl_Double_Window::default_xclass("Scintilla");
//Fl_Double_Window* nn=new Fl_Double_Window(x,y,w,h);
////nn->color(FL_GREEN);
//((Fl_Double_Window*)win)->add(nn);
//nn->xclass("Scintilla");
//nn->show();
//dbgv(flxid(nn)==NULL);
//m_hScintilla=flxid(nn);
////Scintilla_RegisterClasses( flxid(nn) );



//color(FL_BLUE);
//xclass("Scintilla");	
//show();
//	m_hScintilla=flxid(this);


	editores.push_back(this);
	//    load("Z:\\C++\\project\\_fx\\src\\fx.cpp");

//	dbgv("m_hScintilla");
	if(!handlerset)Scintilla_LinkLexers();
	// if(!handlerset){
		// Fl::add_handler(&event_handler);
		//		Fl::event_dispatch(myHandler);
		// handlerset=true;
	// }
//	g_scintilla_wnds.AddElement((HWND__)m_hScintilla);
	//		callback([&](Fl_Widget *, void* v){ dbgv(8);if ((Fl::event()==FL_SHORTCUT && Fl::event_key()==FL_Escape)||(Fl::event()==10)){if(fl_ask("Are you sure you want to exit?")) exit(0); }});

	//   ssm(SCI_USEPOPUP,0,0);
	//   ssm(SCI_CLEARALLCMDKEYS);
	//ssm(SCI_ASSIGNCMDKEY, 'S' | (SCMOD_CTRL << 16)|SCMOD_NORM ,SCI_CANUNDO);
	////ssm(SCI_ASSIGNCMDKEY, 'S' | (SCMOD_CTRL << 16),SCI_CANUNDO);
	//ssm(SCI_ASSIGNCMDKEY, ('Z' + ((SCMOD_CTRL|SCMOD_SHIFT)<<16)), SCI_REDO);
	//ssm(SCI_ASSIGNCMDKEY, ('E' + ((SCMOD_CTRL|SCMOD_SHIFT)<<16)), SCI_FINDTEXT);
	//   ssm(SCI_CLEARALLCMDKEYS ,1);
//	ssm(SCI_SETCARETPERIOD,10000);
	ssm(SCI_SETWRAPMODE,1);
//	ssm(SCI_SETMOUSEDOWNCAPTURES,0);
	//   ssm(SCI_SETTEXT, 0, (sptr_t)
	//    "int main(int argc, char **argv) {\n"
	//    "    // Start up the gnome\n"
	//    "    gnome_init(\"stest\", \"1.0\", argc, argv);\n}"
	//   );
	//   ssm(SCI_SETSELECTIONSTART, 2,0);
	//   ssm(SCI_SETSELECTIONEND, 5,0);
	//dbgv(SCI_GETINDICATORCURRENT);


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
	ssm(SCI_SETINDICATORCURRENT,1,0);
	ssm(SCI_INDICSETSTYLE,1,INDIC_ROUNDBOX);
	ssm(SCI_INDICATORFILLRANGE,2,5);
	ssm(SCI_INDICATORFILLRANGE,12,8);
	//SSM(SCI_SETSTYLING,  9,  128);

	ssm(SCI_STYLECLEARALL, 0, 0);
	//    ssm(SCI_SETPROPERTY,(WPARAM)"fold", (LPARAM)"1");
	//    ssm(SCI_SETPROPERTY, (WPARAM)"fold.comment", (LPARAM)"1");
	//    ssm(SCI_SETPROPERTY, (WPARAM)"fold.preprocessor", (LPARAM)"1");
	ssm(SCI_SETMARGINWIDTHN, MARGIN_SCRIPT_FOLD_INDEX, 0);
	ssm(SCI_SETMARGINTYPEN,  MARGIN_SCRIPT_FOLD_INDEX, SC_MARGIN_SYMBOL);
	ssm(SCI_SETMARGINMASKN, MARGIN_SCRIPT_FOLD_INDEX, SC_MASK_FOLDERS);
	ssm(SCI_SETMARGINWIDTHN, MARGIN_SCRIPT_FOLD_INDEX, 20);
	ssm(SCI_MARKERDEFINE, SC_MARKNUM_FOLDER, SC_MARK_PLUS);
	ssm(SCI_MARKERDEFINE, SC_MARKNUM_FOLDEROPEN, SC_MARK_MINUS);
	ssm(SCI_MARKERDEFINE, SC_MARKNUM_FOLDEREND, SC_MARK_EMPTY);
	ssm(SCI_MARKERDEFINE, SC_MARKNUM_FOLDERMIDTAIL, SC_MARK_EMPTY);
	ssm(SCI_MARKERDEFINE, SC_MARKNUM_FOLDEROPENMID, SC_MARK_EMPTY);
	ssm(SCI_MARKERDEFINE, SC_MARKNUM_FOLDERSUB, SC_MARK_EMPTY);
	ssm(SCI_MARKERDEFINE, SC_MARKNUM_FOLDERTAIL, SC_MARK_EMPTY);

	ssm(SCI_SETFOLDFLAGS, 16, 0); // 16  	Draw line below if not expanded
	ssm(SCI_SETMARGINSENSITIVEN, MARGIN_SCRIPT_FOLD_INDEX, 1);

//	ssm(SCI_SETFOCUS,0);
	ssm(SCI_SETTABINDENTS,1);
setStyle();
//init must come after show()
}

scint::~scint(){
	// Scintilla_ReleaseResources( );
}
 
  

//void* flw::WScintilla(int x,int y,int w,int h){
//    #define SSM(m,...) SendMessage( ww->ssm(m,##__VA_ARGS__)
//    scint* ww=new scint(x,y,w,h);
//    if(currentwidget!=NULL)((Fl_Double_Window*)currentwidget)->add(ww);
//    ww->show();
//	ww->init(currentwidget);
//
//    return ww;
//}
//void* flw::iScintillaFa(scint* widget,int comm){ 
//	if(widget==NULL)return NULL;
//	flclass(scint);
//	flclassw* ww=(flclassw*)widget;
//	ww->foldAll(comm);
//} 
//void* flw::iScintillaComment(void* widget,int comm){
//	if(widget==NULL)return NULL;
//	flclass(scint);
//	flclassw* ww=(flclassw*)widget;
//	ww->commentUncommment(comm);
//} 
////commentUncommment(0);
//void* flw::iScintillaSaveLoad(void* widget,string file){
//	flclass(scint);
//    flclassw* ww=(flclassw*)widget;
//    if(file==""){ww->save();return NULL;}
//    ww->load(file);
//}
//
//void* flw::iScintAddtext(void* widget,string text){
//	flclass(scint);
//	flclassw* ww=(flclassw*)widget;
////	mtxlock(ww->mtx);
//	ww->ssm(SCI_APPENDTEXT,text.size(),(LPARAM)text.c_str());
//	ww->setStyle();
//	ww->ssm(SCI_GOTOPOS,ww->ssm(SCI_GETTEXTLENGTH));
////	mtxunlock(ww->mtx);
//}
//