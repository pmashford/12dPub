#ifndef mashy_lib_text_functions_included
#define mashy_lib_text_functions_included


//#ifndef mashy_lib_debug_included
	#include "mashy_lib_debug.H"
//	#define mashy_lib_debug_included
//#endif


// 12d's inbuilt Find_text() returns 1 when to_find == "" (https://beta.12dmodel.com/forums/viewtopic.php?p=86583)
Integer find_text(Text t, Text to_find){
	if( Text_length(to_find) > 0 ) return(Find_text(t,to_find));
	return(0);
}

Integer is_alpha(Integer c){
	return 	(c >= 'a' && c <='z') ||
			(c >= 'A' && c <='Z') ||
			(c == '_');
}

Integer is_digit(Integer c){
	return (c >= '0' && c <= '9');
}
Integer is_numeric(Integer c){
	return (c >= '0' && c <= '9');
}

Integer is_alpha_numeric(Integer c){
	return (is_alpha(c) || is_digit(c) );
}

Integer get_unique(Text text[], Integer size, Dynamic_Text &unique){
	if(size < 1)return 1;
	for(Integer i=1;i<=size;i++){
		Integer do_append = 1;
		Integer items;
		Get_number_of_items(unique,items);
		for(Integer j=1;j<=items;j++){
			Text t;
			Get_item(unique,j,t);
			if(text[i] == t){
				do_append = 0;
				break;
			}
		}
		if(do_append){
			Append(text[i],unique);
		}
	}
	return 0;
}

Text strip_comment(Text t){
	Integer size = Text_length(t);
	for(Integer i=1;i<size;i++){
		Integer c1,c2;
		Get_char(t,i,c1);
		Get_char(t,i+1,c2);
		if(c1 == '/' && c2 == '/'){
			if(i==1)return "";
			return Get_subtext(t,1,i-1);
		}
	}
	return t;
}

Text integer_to_letters(Integer value){
//could speed up with Set_char(t,1,base+remainder) ... A == 65 therefore base = 64
		//Text t = "";
		//Set_char(t,1,64+remainder);
		//retVal = t + retVal;
	if(value < 1)return"";
	Text chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	Text retVal = "";

	do{
		Real v = value;
		Integer remainder = Mod(value,26.0);
		if(remainder == 0){
			remainder = 26;
			value -= 26.0;
		}
		retVal = Get_subtext(chars,remainder,remainder) + retVal;
		value/= 26;
		//Print(To_text(remainder) + " = Mod(" + To_text(v) + ",26.0);  retval = "+retVal+"  value = "+To_text(value)+ "\n");

	} while (value > 0);

	return retVal;
}


Text last_character(Text t){
	Integer length = Text_length(t);
	return Get_subtext(t,length,length);
}

Text confirm_last_character(Text t, Text desired_last_char){
	if(last_character(t) == desired_last_char)return t;
	return t+desired_last_char;
}

Text text_justify(Text txt){
//remove leading and trailing spaces and tabs
	Integer m1 = '\t';
	Integer m2 = ' ';
	Integer length = Text_length(txt);
	Integer start = 1;
	Integer end = length;
	if(length == 0)return"";
	while (1){
		if(start >length)return"";
		Integer c1;
		Get_char(txt,start,c1);
		if(c1 == m1 || c1 == m2){
			start++;
		}else{
			break;
		}
	}
	while (1){
		if(end == 0)return"";
		Integer c1;
		Get_char(txt,end,c1);
		if(c1 == m1 || c1 == m2){
			end--;
		}else{
			break;
		}
	}
	if( (end-start) < 0)return"";
	return Get_subtext(txt,start,end);
}

Dynamic_Text text_justify(Dynamic_Text &dt){
//overload - remove as per above (NOT TESTED!!!)
	Dynamic_Text dt2;
	Null(dt2);
	Integer items;
	Get_number_of_items(dt,items);
	for(Integer i = 1 ; i <= items ; i++ ){
		Text t;
		Get_item(dt,i,t);
		t = text_justify(t);
		Append(t,dt2);
	}
	return(dt2);
}

/* SHITTY OLD CALL
Integer get_subtext_matching_brackets(Text input,Text &output,Integer &start_pos, Integer &end_pos)
// ------------------------------------------------------------------------------
// get a string out of an input strings outer most curly brackets (delim "{" to "}")
// count returns the number of closed curly brackets, ie can querey if we should be looking for more?
// ------------------------------------------------------------------------------
{
	Integer count = 0;
	//input = Trim(input);  //dont trim because we may want to know where the start or end position of the brackets is
	Integer depth = 0;
	start_pos = 0;
	end_pos = 0;
	for(Integer i = 1; i<= Text_length(input) ; i++) {
		if(Get_subtext(input,i,i) == "{"){
			if(depth == 0 && start_pos == 0)start_pos = i; // found the first bracket
			depth = depth + 1;
		}
		if(Get_subtext(input,i,i) == "}"){
			depth = depth - 1;
			count = count + 1;
			if(depth == 0 && start_pos != 0 )end_pos = i; // found the last bracket
		}
		if(start_pos != 0 && end_pos != 0)break;
	}
	if(start_pos != 0 && end_pos != 0){
		output = Get_subtext(input,start_pos,end_pos);
	}else{
		output = "";
		count = -1; // there was an error
	}
	return(count);
}
*/
Integer get_subtext_matching_brackets(Text &input,Text &output,Integer &start_pos, Integer &end_pos)
// ------------------------------------------------------------------------------
// get a string out of an input strings outer most curly brackets (delim "{" to "}")
// count returns the number of closed curly brackets, ie can querey if we should be looking for more?
// ------------------------------------------------------------------------------
{
	Integer count = 0;
	Integer depth = 0;
	start_pos = 0;
	end_pos = 0;
	Integer length = Text_length(input);
	for(Integer i = 1; i<= length ; i++) {
		Integer c1;
		Get_char(input,i,c1);
		if(c1 == '{'){
			if(depth == 0 && start_pos == 0)start_pos = i; // found the first bracket
			depth += 1;
		}else if(c1 == '}'){
			depth -= 1;
			count += 1;
			if(depth == 0 && start_pos != 0 )end_pos = i; // found the last bracket
		}
		if(start_pos != 0 && end_pos != 0)break;
	}
	if(start_pos != 0 && end_pos != 0){
		output = Get_subtext(input,start_pos,end_pos);
	}else{
		output = "";
		count = -1; // there was an error
	}
	return(count);
}


Integer get_subtext_matching_brackets(Text input,Text &output){
	// function underload
	Integer start_pos, end_pos;
	Integer ierr = get_subtext_matching_brackets(input,output,start_pos, end_pos);
	return(ierr);
}

Integer get_next_bracket_pair(Text &input, Text &name, Text &value, Text &remainder){
	Text output;
	Integer bracket_start, bracket_end;
	if(get_subtext_matching_brackets(input,output,bracket_start, bracket_end) > 0){
		name = text_justify(Get_subtext(input,1,bracket_start-1));
		if( (bracket_end-bracket_start) > 0)	value = text_justify(Get_subtext(input,bracket_start+1,bracket_end-1));
		else value = "";
		if(bracket_end < Text_length(input)){
			remainder = text_justify(Get_subtext(input,bracket_end+1,Text_length(input)));
		}else{
			remainder = "";
		}
		return 0;
	}
	return 1;
}

Integer get_brackets(Text &input,Text &output, Integer &start_text_from, Integer &end_from)
// ------------------------------------------------------------------------------
// get a string out of an input strings outer most curly brackets (delim "{" to "}")
// count returns the number of closed curly brackets, ie can querey if we should be looking for more?
// ------------------------------------------------------------------------------
{
	Integer count = 0;
	Integer depth = 0;
	Integer start_pos = 0;
	Integer end_pos = 0;
	Integer length = Text_length(input);
	for(Integer i = start_text_from; i<= length ; i++) {
		Integer c1;
		Get_char(input,i,c1);
		if(c1 == '{'){
			if(depth == 0 && start_pos == 0)start_pos = i; // found the first bracket
			depth += 1;
		}else if(c1 == '}'){
			depth -= 1;
			count += 1;
			if(depth == 0 && start_pos != 0 )end_pos = i; // found the last bracket
		}
		if(start_pos != 0 && end_pos != 0)break;
	}
	if(start_pos != 0 && end_pos != 0){
		output = Get_subtext(input,start_text_from,end_pos);
		end_from = end_pos;
	}else{
		output = "";
		count = -1; // there was an error
	}
	return(count);
}

Integer get_all_brackets(Text &input, Dynamic_Text &dt_output)
{
	Null(dt_output);
	Text output;
	Integer start_text_from=1,end_from=0;

	while(get_brackets(input,output, start_text_from, end_from) > 0){
		start_text_from = end_from+1;
		Append(output,dt_output);
	}
	return 0;
}

#ifndef REAL_JUSTIFY_DECIMALS
	#define REAL_JUSTIFY_DECIMALS 8
#endif



Text real_justify(Real r, Integer min_dec){
#if 1
	if(min_dec < 0){
		return To_text(r,Absolute(min_dec));
	}
	Text t = To_text(r,min_dec);
	Integer dot = '.';
	Integer zero = '0';
	Integer size = Text_length(t);
	Integer dot_found = 0;
	//find dot
	for(Integer i=1;i<=size;i++){
		Integer c;
		Get_char(t,i,c);
		if(c==dot){	// is there a . ?
			for(Integer j=size;j>i;j--){
				Get_char(t,j,c);
				if(c==zero)continue;
				return Get_subtext(t,1,j);
			}
			if(j==i){
				return Get_subtext(t,1,i-1);
			}
			break;
		}
	}
	return t;

#else
// remove trailing 0's after decimal - only zeros are remove (so no rounding occours) however if we want a minimum number of decimals there are put back
// -ve values will leave a decimal
	Text t = To_text(r,REAL_JUSTIFY_DECIMALS);
	if(Find_text(t,".") > 0){ // is there a decimal
		while(1){
			if(Get_subtext(t,Text_length(t),Text_length(t)) == "0"){
				t = Get_subtext(t,1,Text_length(t)-1);
			}else{
				break;
			}
		}
	}else if(min_dec > 0){
		t = t+"."; //add decimal
	}else{
		return t;
	}
	if(min_dec > 0){
		Integer pos = Find_text(t,".");
		if(pos > 0){
			Integer count = min_dec - (Text_length(t) - pos);
			for(Integer i = 1 ; i <= count ; i++){
				t = t + "0";
			}
		}
	}else if (min_dec == 0){
		if(Get_subtext(t,Text_length(t),Text_length(t)) == "."){
			t = Get_subtext(t,1,Text_length(t)-1);
		}
	}
	return t;
#endif
}


Integer split_text_check_for_number_as_last_number_part(Text in, Text &left, Text &right, Real &value){
#if DEBUG_SPLIT
	Print(">>> SPLIT >>>");Print();
	Print(in);Print();
#endif
	Null(value);
	Integer length = Text_length(in);
	if(length <= 0){
		left = "";
		right = "";
		return (-1);
	}
	Integer count_decimals = 0; //only allow 1 decimal
	for(Integer i = length ; i >= 1 ; i--){
		Integer c; Get_char(in,i,c);
		if(c >= 48 && c <= 57){
			continue;
		}else if(c == 46 ){
			count_decimals++;
			if(count_decimals > 1){
				//Print("too many decimals\n");
				i++;
				break; // more than 1 decimal
			}
			continue;
		}else{
			if(i != length){ //if there is a + or - make sure it is not the last character
				if (c == 43 || c == 45){
					break; // found a + or -
				}
			}
			i++;
			break; //get to here the character is not a number!
		}
	}

	if (i <= length) {//number found
		if(i == 1){
			left = "";
		}else{
			left = Get_subtext(in,1,i-1);
		}
		right = Get_subtext(in,i,length);
		From_text(right,value);
#if DEBUG_SPLIT
	Print("i " + To_text(i) + " length " + To_text(length));Print();
	Print("<"+left + "><" + right+"> (number found)");Print();
	Print("<<< SPLIT <<<");Print();
#endif
		return (0);
	}else{
		left = in;
		right = "";
#if DEBUG_SPLIT
	Print("<"+left + "><" + right+"> (no number)");Print();
	Print("<<< SPLIT <<<");Print();
#endif
		return (1);

	}
	return (-2);
}

Text remove_trailing_zeros(Text t){
// will only work if there is a "."
	if(Find_text(t,".") > 0){ // is there a decimal
		while(1){
			if(Get_subtext(t,Text_length(t),Text_length(t)) == "0"){
				t = Get_subtext(t,1,Text_length(t)-1);
			}else{
				break;
			}
		}
		if(Get_subtext(t,Text_length(t),Text_length(t)) == "."){
			t = Get_subtext(t,1,Text_length(t)-1);
		}

	}
	return t;
}

Text to_text(Real r,Integer decimals){
	return(remove_trailing_zeros(To_text(r,decimals)));
}


Integer text_split(Text &t,Integer c1, Text &left, Text &right){
	Integer length = Text_length(t);
	for(Integer i=1;i<=length;i++){
		Integer c2;
		Get_char(t,i,c2);
		if(c1 == c2){
			if(i == 1)	left = "";
			else		left = Get_subtext(t,1,i-1);

			if(i == length)	right = "";
			else		right = Get_subtext(t,i+1,length);

			return 0;
		}
	}
	return 1; //failed
}

Integer text_split(Text t,Text to_find,Text &left, Text &right){
	Integer pos = Find_text(t,to_find);
	if(pos == 0){
		left = t; right = "";return -1; //no to_find found
	}else{
		Integer len = Text_length(to_find);
		left = Get_subtext(t,1,pos-1);
		right = Get_subtext(t,pos+len,Text_length(t));
		return 0;
	}
	return -2; // somethings wrong if we got here!

}

Integer remove_items_between_values(Dynamic_Text &dt, Text start, Text finish){
//removes "start" to "found" - the whole line where found is 'found' will be ignored
// returns the nw number of items in dt
	Dynamic_Text no_match; //this is what we want to keep!, throw the rest away!
	Integer count = 0;
	Integer items;
	Get_number_of_items(dt,items);
	Integer found = 0;
	for(Integer i = 1 ; i <= items ; i++ ){
		Text t; //the original string
		Get_item(dt,i,t);
		Text pre;
		Text remainder;
		Integer pos_start = Find_text(t,start);
		Integer pos_end = Find_text(t,finish);
		//p(t+ "<<<start" + To_text(pos_start) + " end"+ To_text(pos_end));
		if(found == 0){
			if(pos_start > 1) pre = Get_subtext(t,1,pos_start-1);
			if(pos_end != 0 && pos_start != 0 ){
				if(pos_start < pos_end){
					if(pos_start > 1){
						Append(pre,no_match);
						count++;
						continue;
					}
					continue;
				}else {
					Append(pre,no_match); //if we get here
					count++;
					p("Possible error? : remove_items_between_values : finish found before start where i = " + To_text(i));
				}
			}
			if(pos_start != 0){
				found = 1;
				if(pos_start >1 ) {
					Append(pre,no_match);
					count++;
				}
				continue;
			}
		}


		if (found == 1 ){
			if(pos_end != 0){
				found = 0;
				continue; //next item if there is no match
			}
			continue;
		}
		Append(t,no_match);
	}
	Null(dt);
	Append(no_match,dt);
	Null(no_match);
	return(count);
}

Integer remove_items_starting_with(Dynamic_Text &dt, Text tofind){
// for getting rid of commented lines, or
	Dynamic_Text no_match; //this is what we want to keep!, throw the rest away!
	Integer count = 0;
	Integer items;
	Get_number_of_items(dt,items);
	for(Integer i = 1 ; i <= items ; i++ ){
		Text t; //the original string (for reading and appending)
		Text t2; // we will strip tabs and spaces for testing
		Get_item(dt,i,t);
		t2 = text_justify(t);
		if( Find_text(t2,tofind) != 1 ){
			Append(t,no_match);
			count++;
		}
	}
	Null(dt);
	Append(no_match,dt);
	Null(no_match);
	return(count);
}

Integer remove_items_starting_with_no_strip(Dynamic_Text &dt, Text tofind){
// for getting rid of commented lines, or
	Dynamic_Text no_match; //this is what we want to keep!, throw the rest away!
	Integer count = 0;
	Integer items;
	Get_number_of_items(dt,items);
	for(Integer i = 1 ; i <= items ; i++ ){
		Text t; //the original string (for reading and appending)
		Text t2; // we will strip tabs and spaces for testing
		Get_item(dt,i,t);
		if( Find_text(t,tofind) != 1 ){
			Append(t,no_match);
			count++;
		}
	}
	Null(dt);
	Append(no_match,dt);
	Null(no_match);
	return(count);
}

Integer remove_items_after(Dynamic_Text &dt, Text tofind){
// for getting rid of text after certain text
	Dynamic_Text no_match; //this is what we want to keep!, throw the rest away!
	Integer count = 0;
	Integer items;
	Get_number_of_items(dt,items);
	for(Integer i = 1 ; i <= items ; i++ ){
		Text t; //the original string (for reading and appending)
		Get_item(dt,i,t);
		if( Find_text(t,tofind) > 0 ){
			Integer pos = Find_text(t,tofind);
			if (pos > 1 ){
				Append(Get_subtext(t,1,pos-1),no_match);
				count++;
			}
		} else {
			Append(t,no_match);
			count++;
		}
	}
	Null(dt);
	Append(no_match,dt);
	Null(no_match);
	return(count);
}

Integer remove_items_blank(Dynamic_Text &dt){
//Remove any items which == ""
	Dynamic_Text no_match;
	Integer count;
	Integer items;
	Get_number_of_items(dt,items);
	for(Integer i = 1 ; i <= items ; i++){
		Text t;
		Get_item(dt,i,t);
		if( text_justify(t) == "")continue;
		Append(t,no_match);
		count++;
	}
	Null(dt);
	Append(no_match,dt);
	Null(no_match);
	return(count);
}

Integer append_no_duplicate(Text t,Dynamic_Text &dt){
//append text to the list as long as there is no existing match
	Integer items;
	Get_number_of_items(dt,items);
	//what if items == 0
	if(items == 0){
		Append(t,dt);
		return 1;
	}
	for(Integer i=1;i<=items;i++){
		Text txt;
		Get_item(dt,i,txt);
		if(t == txt)return 0; // if there is a match get out of here
	}
	Append(t,dt);
	return 1;
}

Integer remove_duplicates(Dynamic_Text &dt){
	// remove duplicate items from dt, return number of items removed
	Integer items;
	Dynamic_Text new_dt;
	Get_number_of_items(dt,items);
	for (Integer i = 1 ; i <= items ; i++){
		Integer match_found=0;
		Text ti;
		Get_item(dt,i,ti);
		Integer itemsj;
		Get_number_of_items(new_dt,itemsj);
		for(Integer j = 1 ; j <= itemsj ; j++){
			Text tj;
			Get_item(new_dt,j,tj);
			if(ti == tj){
				match_found = 1;
				break;
			}
		}
		if(match_found == 0)Append(ti,new_dt);
	}
	Null(dt);
	Append(new_dt,dt);
	i=0;
	Get_number_of_items(dt,i);
	Null(new_dt);
	return(items-i);
}

Integer match_name(Dynamic_Text &dt, Text pat, Dynamic_Text &dtr){
	// return any matches from dt to dtr (append to existins array)
	// return the number of matches
	Integer items;
	Integer count = 0;
	Get_number_of_items(dt,items);
	for (Integer i =1 ; i <= items ; i++){
		Text t;
		Get_item(dt,i,t);
		if(Match_name(t,pat)!=0){
			Append(t,dtr);
			count++;
		}
	}

	return count;
}

Integer get_max_number_of_chars(Dynamic_Text &dt){
	Integer max_chars = 0;
	Integer items;	Get_number_of_items(dt,items);
	for(Integer i = 1 ; i <= items ; i++){
		Text t;
		Get_item(dt,i,t);
		Integer test = Text_length(t);
		if(test>max_chars){
			max_chars = test;
		}
	}
	return max_chars;
}

Integer get_max_number_of_chars(Text t[], Integer size){
	Integer max_chars = 0;
	//Integer items;	Get_number_of_items(dt,items);
	for(Integer i = 1 ; i <= size ; i++){
		Integer test = Text_length(t[i]);
		if(test>max_chars){
			max_chars = test;
		}
	}
	return max_chars;
}


Text find_replace_repeat_fork_bomb(Text str, Text pat, Text replace){
	// find and replace all occourances and return a new string
	// obvious bug/issue here if 'replace' contains 'pat' you'll get an endless loop
	// the returned str has the characters replaced
	Integer lpat = Text_length(pat);
	while( Find_text(str,pat)){
		Integer i = Find_text(str,pat);
		Integer lstr = Numchr(str);
		str = Get_subtext(str,1,i-1) + replace + Get_subtext(str,i+lpat,lstr);
	}
	return str;
}

// handle Find_text(text,tofind) bug where tofind == "";
// search haystack for needle, replacing needle => returns a new Text
Text find_replace_repeat(Text haystack, Text needle, Text replace){
    Integer needle_length = Text_length(needle);
    if(needle_length<1) return(haystack);
    Integer i=0;
    Text prefix="";
	while( i = Find_text(haystack,needle) ){
        prefix += Get_subtext(haystack,1,i-1) + replace;
        haystack = Get_subtext(haystack,i+needle_length,Text_length(haystack));
	}
	return (prefix + haystack);
}


Text find_replace_faster(Text t_in, Text t_find, Text t_replace){
	if(!Text_length(t_in) || !Text_length(t_find)){
		return t_in;
	}
	Text t_start = "";
	Integer len = Text_length(t_find);
	Integer doit = 1;
	while(doit){
		Integer i = Find_text(t_in,t_find);
		Integer t_in_len = Text_length(t_in);
		if(t_in_len == 0)i = 0; // handle bug in builtin
		switch(i) {
			case (0) : {
				doit = 0;
			}break;
			case (1) : {
				t_start = t_start + t_replace;
				t_in = Get_subtext(t_in, 1 + len ,t_in_len);
			}break;
			default :{
				t_start = t_start + Get_subtext(t_in, 1, i - 1) + t_replace;
				t_in = Get_subtext(t_in, i + len ,t_in_len);
			}
		}
		//Integer textLength = Text_length(text);
	}
	return (t_start + t_in);
}

Text find_replace(Text in,Text find, Text replace){
//find and replace funciton
	Text retval;
	Integer len = Text_length(find);
	Integer c[len];
	for(Integer j = 1;j<=len;j++)Get_char(find,j,c[j]);
	Integer len_i = Text_length(in);
	Integer start =1;
	for(Integer i=1;i<=len_i;i++){
		for(j=1;j<=len;j++){
			Integer c1;	Get_char(in,i+j-1,c1);
			if(c1!=c[j])break;
			else if(j==len){
				retval += Get_subtext(in,start,i-1)+replace;
				start = i+len;
				i+=len-1; //(-1) becuse i gets iterated at end of for
			}
		}
	}
	if(start<=len_i)retval += Get_subtext(in,start,len_i);
	return(retval);
}

// tested for speed, it is quicker to use the Find_text() builtin to find and replace
Text find_replace_char(Text string_in, Integer char_find, Integer char_replace){
	Text t = string_in;
	Integer n_chars = Text_length(t);
	for(Integer i=1;i<=n_chars;i++){
		Integer this_char;
		Get_char(t,i,this_char);
		if(this_char==char_find){
			Set_char(t,i,char_replace);
		}
	}
	return(t);
}




// this is the fastest way to find and replace spaces
Text find_replace_spaces(Text text){
	if(!Text_length(text)){
		return text;
	}
	while(1){
		Integer i = Find_text(text," ");
		if(!i)break;
		//Integer textLength = Text_length(text);
		//text = Get_subtext(text, 1, i - 1) + "_" + Get_subtext(text, i + 1, textLength);
		Set_subtext(text,i,"_");
	}
	return text;
}



Text quotes_strip(Text txt){
// remove and leading and trailing "quotes" or 'quotes' - return the resulting string
	Integer len;
	Text txt_char;
	len = Text_length(txt);
	txt_char = Get_subtext(txt,len,len);
	if ( txt_char == "\"" || txt_char == "'"){ //is there a trailing quote, if so strip it
		txt = Get_subtext(txt,1,len-1);
	}
	len = Text_length(txt);
	txt_char = Get_subtext(txt,1,1);
	if ( txt_char == "\"" || txt_char == "'"){ // is there a leading quote, if so strip it
		txt = Get_subtext(txt,2,len);
	}
	return txt;
}
Text quotes_add(Text txt){
// add quotes
	txt = "\"" + txt + "\"";
	return txt;
}

Text get_item_local(Dynamic_Text &dt,Integer i){
	Text t;
	Get_item(dt,i,t);
	return t;
}

Integer sort_lists_to_matching_lists(Dynamic_Text &l1, Dynamic_Text &l2, Dynamic_Text &l3, Dynamic_Text &l4){
	//take l1 & l2 and spit out l3 & l4
	//	l1	l2	l3	l4
	//	XX	XX	XX	XX
	//	YY	Y		Y
	//	ZZ	Z	YY
	//				Z
	//			ZZ
	//
	//Return code of 1 = success, -1 = l1 & l2 were 0 in size
	Integer i1,i2,i3,i4,i,j;
	Get_number_of_items(l1,i1);
	Get_number_of_items(l2,i2);
	if(i1 == 0 && i2 != 0){
		for(i=1;i<=i2;i++){
			Append("",l3);
			Append(get_item_local(l2,i),l4);
	}
	}else if(i2 == 0 && i1 != 0){
		for(i=1;i<=i1;i++){
			Append("",l4);
			Append(get_item_local(l1,i),l3);
		}
	}else if(i1 == 0 && i2 == 0){
		l3 = l1;
		l4 = l2;
		return -1;
	}else{
		//need to work out how many items there will be by checking number of matches in each array




	}

	return 1;
}
Integer debug_dynamic_text(Dynamic_Text &dt){
	p("Dumping dynamic text->");
	Integer items;
	Get_number_of_items(dt,items);
	for(Integer i=1;i<=items;i++){
		p(get_item_local(dt,i));
	}
	p("<-End dump");
	return 0;
}

Integer dump_dynamic_text(Dynamic_Text &dt){
	Integer items;
	Get_number_of_items(dt,items);
	for(Integer i=1;i<=items;i++){
		Print(get_item_local(dt,i)+"\n");
	}
	return 0;
}

Dynamic_Text get_umcommented_lines(Dynamic_Text &lines){
	//lines is the lines of a file, returns Dynamic_Text with the comments removed
	Dynamic_Text dt;
	Null(dt);
	Integer items;
	Get_number_of_items(lines,items);
	for(Integer i = 1;i<=items;i++){
		Text line = Text_justify(get_item_local(lines,i));	//trim white space
		if(line == "")continue; //blank line or was just spaces
		if(Match_name(line,"//*"))continue;		//treat the whole line as a comment
		Integer comment_start = Find_text(line,"/*");
		Integer comment_end = Find_text(line,"*/");
		Integer len = Text_length(line);
		if(comment_start == 1 && comment_end != 0)continue; //treat the whole line as a comment
		if(comment_start != 0 || comment_start == 1){
			if(comment_start > 1)Append(Get_subtext(line,1,comment_start-1),dt);	//get the preceeding text if '/*' is not at pos = 1
			if(comment_end!=0)continue;	//treat the remainder of the line as a comment
			for(Integer j=i+1; j <= items; j++){		//start searching for the end of the multiline comment
				Text l = Text_justify(get_item_local(lines,j));
				comment_end = Find_text(l,"*/");
				if(comment_end != 0){
					i = j;
					j = j+items; //continue;treat the whole line as a comment
				}
			}
			continue;
		}
		comment_start = Find_text(line,"//");
		if(comment_start != 0){
			Append(Get_subtext(line,1,comment_start-1),dt);
			continue;
		}
		Append(line,dt);
	}

	return dt;
}

Integer find_item(Dynamic_Text &dt, Dynamic_Text &to_find){
	//do any items in to_find match any itmes in dt... retval = first item in dt which matches OR 0 = no match
	Integer itemsi;
	Get_number_of_items(dt,itemsi);
	for(Integer i = 1 ; i <= itemsi ; i++ ){
		Text ti;
		Get_item(dt,i,ti);
		Integer itemsj;
		Get_number_of_items(to_find,itemsj);
		for(Integer j = 1 ; j <= itemsj ; j++){
			Text tj;
			Get_item(to_find,j,tj);
			//Print(ti + " == " + tj + "?\n");
			if(ti == tj ){
			//	Print("Match\n");
				return(i);
			}
		}
	}
	return(0);
}

Integer find_item(Dynamic_Text &dt, Text to_find){
	Dynamic_Text dt_to_find;
	Append(to_find,dt_to_find);
	Integer retval = find_item(dt,dt_to_find);
	Null(dt_to_find);
	return(retval);
}

Integer find_item(Text t, Dynamic_Text &to_find){
	Dynamic_Text dt;
	Append(t,dt);
	Integer retval = find_item(dt,to_find);
	Null(dt);
	return(retval);
}

Integer set_clipboard_text(Dynamic_Text &dt){
	Text send;
	Integer items;
	Get_number_of_items(dt,items);
	if(items < 1 )return -1; //no items to send to clipboard
	for(Integer i = 1 ; i <= items ; i++ ){
		Text t;	Get_item(dt,i,t);
		if(i == 1){
			send = t;
		}else{
			send += "\n" + t;
		}
	}
	Integer ierr = Set_clipboard_text(send);
	return ierr;
}

Integer replace_env_path(Text source, Text &output){
//expects a $prefix env name and will attempt to resolve via Getenv(), will do a search with _4D suffixed to find 12d userlibs ect
//so check before hand that you have "$*"
// 0 = success
// 1 = input "source" string did not match $ as first character
// -2 = no env found
// -1 = huh, shouldn't have got here???
	source = find_replace_repeat(source,"/","\\");	//pre-process string: replace any '/' with '\'
	if(Find_text(source,"$") == 1){			//make sure $ is the first character
		Text env;
		Integer i;
		if(Find_text(source,"\\") != 0 ){	//
			i = Find_text(source,"\\");
			env = Get_subtext(source,2,i-1);
		}else {
			i = Text_length(source) + 1;
			env = Get_subtext(source,2,Text_length(source));
		}
		Text path;
		path = Getenv(Text_upper(env)); // make the users variable uppercase
		if(path == ""){
			path = Getenv(Text_upper(env)+"_4D");//some environment variables are suffixed *_4D, do a search for them if previous falied
		}
		if(path == ""){	//if no source path here then the is no env with the specified $name: error!
			output = source;
			return(-2);
		}
		output = path + Get_subtext(source,i,Text_length(source));
		return(0);
	} else {
		output = source;
		return(1);
	}
	return(-1);
}

//--------------------------------------------------------------------------------
//                        PROCESS TEXT STRINGS WITH NEW LINES AND DELIMIERS
//--------------------------------------------------------------------------------

#ifndef TAB_CHARACTER
#define TAB_CHARACTER       9
#endif
#ifndef NEW_LINE_CHARACTER
#define NEW_LINE_CHARACTER 10
#endif

Integer count_char(Text &t, Integer c1){
//returns the number of characters which match c1
	Integer count =0;
	Integer length = Text_length(t);
	for(Integer i=1;i<=length;i++){
		Integer c2;
		Get_char(t,i,c2);
		if(c1 == c2)count++;
	}
	return count;
}

Integer count_tabs(Text &t){
	return count_char(t,TAB_CHARACTER);
}

Integer count_new_lines(Text &t){
	return count_char(t,NEW_LINE_CHARACTER);
}

Integer split_by_char(Text &t, Integer char, Text split_t[], Integer max_t){
	Integer size = Text_length(t);
	Integer count=0;
	Integer start_pos = 1;
	for(Integer i=1;i<=size;i++){
		Integer c;
		Get_char(t,i,c);
		if(c == char){
			count++;
			if(count > max_t)return 1;
			split_t[count] = "";
			if(start_pos < i){
				split_t[count] = Get_subtext(t,start_pos,i-1);
			}else{
				split_t[count] = "";
			}
			start_pos = i+1;
		}
	}
	if(start_pos <= size){
		count++;
		if(count > max_t)return 1;
		split_t[count] = Get_subtext(t,start_pos,size);
	}
	if(count == 0) return 2;	//no text in t

	return 0;

}

Integer split_by_tab(Text t,Dynamic_Text &parts){
	Integer ntabs = count_tabs(t);
	if(ntabs<=1){
		Append(t,parts);
	}
	ntabs++;
	Text split_t[ntabs];
	split_by_char(t, TAB_CHARACTER, split_t, ntabs);
	for(Integer i=1;i<=ntabs;i++){
		Append(split_t[i],parts);
	}
	return 0;
}

#if 0
	//clipboard split to lines
	Text clipbaord;	Get_clipboard_text(clipboard);
	Integer rows = count_new_lines(clipboard) + 1;
	Text clipbaord_row[rows];
	split_by_char(clipboard, NEW_LINE_CHARACTER, clipbaord_row, rows);

#endif
/*
Integer wildmatch(Text str, Text pat){
// DONT NEED THIS FUNCTION ANYMORE AS THERE IS AN INTERNAL CALL... Match_name
//Integer Match_name(Text name,Text reg_exp)
//Description
//Checks to see if the Text name matches a regular expression given by Text reg_exp.
//The regular expression uses
//* 	for a wild cards
//?	for a wild character
//A non-zero function return value indicates that there is a match.
//A zero function return value indicates there is no match.
//Warning  this is the opposite of most 4DML function return values

	//TEST A STRING AGAINST A WILDCARD PATTERN
	// RETURN VALUE OF 0 = NO MATCH
	//                 1 = MATCH
	pat = find_replace_repeat(pat,"**","*"); //should check and remove consecutive ** in pat
	Integer ipat,istr;
	Integer ret = 0;
	Integer lpat = Text_length(pat);
	Integer lstr = Text_length(str);
	if(pat == "*"||pat == str){ret = 1;}
	//if(pat == str){ret = 1;}
	Integer j=1;
	for (Integer i=1;i<=lstr;i++){
		if ( Get_subtext(str,i,i) == Get_subtext(pat,j,j)){
			//next for
			if (i == lstr && j ==lpat){ret = 1;} // match, keep going
			j++;
		}else if( Get_subtext(pat,j,j+1) == "*?"){
			if (lpat == j+1){ret = 1;i = i+lstr;} // match, exit for
			if (i == lstr ){ret = 1;i = i+lstr;} // match, exit for
			//find j+2
			Integer i1 = Find_text(Get_subtext(str,i,lstr),Get_subtext(pat,j+2,j+2)); //does pat character after *? match anywhere after str(i)
			if (i1 > 0){
				//there is a match
				i=i+i1-2;
				j=j+2;
			}else {
				ret = 0; //No match
				i = i+lstr;// exit for
			}
		}else if ( Get_subtext(pat,j,j) == "*" && j==lpat){
			ret=1;i = i+lstr;//* is the last character in pat so words match
		}else if ( Get_subtext(pat,j,j) == "*"){
			Integer i1 = Find_text(Get_subtext(str,i,lstr),Get_subtext(pat,j+1,j+1)); //does pat character after * match anywhere after str(i)
			if (i1 > 0){
				//there is a match
				i=i+i1-2;
				j=j+1;
			}else {
				ret = 0; //No match
				i = i+lstr;// exit for
			}
		}else if( Get_subtext(pat,j,j) == "?"){
			if (i == lstr && j ==lpat){ret = 1;} // if we are here then words match
			j++;
		}else {
			ret = 0; //No match
			i = i+lstr;//exit for
		}
		if(i==lstr ){ //not sure how I got this to work, matches the case of WORD vs WORD* (j dosn't make sense, I would have thought j+1)
			if(Get_subtext(pat,j,j) == "*"){
				if (j==lpat){
					ret = 1;i = i+lstr;// Match and exit for
				}
			}
		}
	}
	return ret;
}
*/


#endif
