#ifndef mashy_lib_files_binary_H_included
#define mashy_lib_files_binary_H_included

#define INTEGER_SETS_PER_LINE_OF_TEXT 32

// see "File_Binary_to_code_dt.4dm" for usage

/* this is the newer method, used in this implementation
3 x Integer to 16 characters
bits |666662|466664|266666|
name |   a  |   b  |   c  |
chars|ccccc|c|cccc|c|ccccc|

shifts
26  20  14  08  02
28  22  16  10  04
30  24  18  12  06  

mask for 2 bits = 0x3
mask for 4 bits = 0xF
mask for 6 bits = 0x3F
*/

//////// is this the html safe characer set?, either way it could be a custom set of characters I guess...
Text i_to_a(Integer i){
	i++;
	return Get_subtext("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_",i,i);
}

Text i32_3_to_b64(Integer a, Integer b, Integer c){ //used to encode, returns 16 characters
	return i_to_a( ((a >> 26) & 0x3F) ) + i_to_a( ((a >> 20) & 0x3F) ) + i_to_a( ((a >> 14) & 0x3F) ) + i_to_a( ((a >> 8) & 0x3F) ) + i_to_a( ((a >> 2) & 0x3F) )
         + i_to_a( (((a ) << 4) | ((b >> 28) & 0xF)) & 0x3F          ) 
         + i_to_a( ((b >> 22) & 0x3F) ) + i_to_a( ((b >> 16) & 0x3F) ) + i_to_a( ((b >> 10) & 0x3F) ) + i_to_a( ((b >> 4) & 0x3F) )
         + i_to_a( (((b ) << 2) | ((c >> 30) & 0x3)) & 0x3F          )
         + i_to_a( ((c >> 24) & 0x3F) ) + i_to_a( ((c >> 18) & 0x3F) ) + i_to_a( ((c >> 12) & 0x3F) ) + i_to_a( ((c >> 6) & 0x3F) ) + i_to_a( ((c ) & 0x3F) ); 
}

Integer a_to_i(Text t){
	return Find_text("BCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_",t);  // 63 chars, 'A' ommitted to simplify i-1 in calcs. this also means '=' will be found and it is being used to pad
}                                                                                           // , however the '=' == eof should be handled elsewhere

Integer b64_to_i32_3(Text t, Integer &a, Integer &b, Integer &c){ // used to decode (t) to (3 x 32bit ints), expects t to be 16 characters, where each character is in the range of 0-@
	a = (a_to_i(Get_subtext(t,1,1)) << 26) | (a_to_i(Get_subtext(t,2,2)) << 20) | (a_to_i(Get_subtext(t,3,3)) << 14) | (a_to_i(Get_subtext(t,4,4)) << 8) | (a_to_i(Get_subtext(t,5,5)) << 2) | (a_to_i(Get_subtext(t,6,6) ) >> 4 )  ;
    b = ( a_to_i(Get_subtext(t,6,6)) << 28) | (a_to_i(Get_subtext(t,7,7)) << 22) | (a_to_i(Get_subtext(t,8,8)) << 16) | (a_to_i(Get_subtext(t,9,9)) << 10) | (a_to_i(Get_subtext(t,10,10)) << 4) | (a_to_i(Get_subtext(t,11,11) ) >> 2 )  ;
    c = (a_to_i(Get_subtext(t,11,11)) << 30) | (a_to_i(Get_subtext(t,12,12)) << 24) | (a_to_i(Get_subtext(t,13,13)) << 18) | (a_to_i(Get_subtext(t,14,14)) << 12) | (a_to_i(Get_subtext(t,15,15)) << 6) | (a_to_i(Get_subtext(t,16,16)) )  ;
    if (Get_subtext(t,7,7) == "=") return 1; // EOF at B
    if (Get_subtext(t,12,12) == "=") return 2; // EOF at C
    return 0; // no EOF
}

// >> take Dynamic_Text and write a file to the filesystem

Integer write_binary_file_from_4dm_code_dynamic_text_function(Text new_binary_filename_with_ext, Dynamic_Text &dt, Integer do_overwrite){ //this will take hex data stored in dt and write to a file
	if(!do_overwrite && File_exists(new_binary_filename_with_ext)){
		Print("write_binary_file_from_4dm_code_dynamic_text_function() failed to process on the basis of fucnction argument to NOT OVERWRITE EXISTING BINARY FILE ("+new_binary_filename_with_ext+"), FILE EXISTS!\n");
	}else{
		Integer items;
		Get_number_of_items(dt,items);
		if(items){
			Integer ok=0;
			File f;
			ok+=File_open(new_binary_filename_with_ext,"wb",f);
			for(Integer i=1;i<=items;i++){
				Text t;
				Get_item(dt,i,t);
				Integer size = Text_length(t);
				Integer c = 16;
				while (c <= size){ // this will handle whatever number of integers have been encoded to lines of text for "INTEGERS_PER_LINE_OF_TEXT"
					Integer A,B,C; // capitals so as not to conflict with little c
                    Integer eof = b64_to_i32_3(Get_subtext(t,c-15,c), A, B, C);
                    switch (eof) {
                        case (0):{
                            File_write(f,A);    File_write(f,B);    File_write(f,C);
                        }break;
                        case (1):{
                            File_write(f,A);    File_write(f,B);
                        }break;
                        case (2):{
                            File_write(f,A);
                        }break;
                    }
					c+=16;
				}
			}
			ok+=File_close(f);
			return(ok);
		}
	}
	return(1);
}

// >> take a file from the filesystem and store/encode it as Dynamic_Text

Integer read_binary_file_to_4dm_code_dynamic_text_function(Text file_name_in, Text file_name_out, Text binary_data_function_name){  // this will read a file in binary mode and convert to a custom text string... the new file has a function that retrieves the text string as Dynamic_Text
	Integer ok=0;
	File fin;
	ok+=File_open(file_name_in,"rb",fin);

	File fout;
	ok+=File_open(file_name_out,"w",fout);
	ok+=File_write_line(fout, "Dynamic_Text "+binary_data_function_name+"(){");
	ok+=File_write_line(fout, "\tDynamic_Text dt;");

	Integer count = 0;
	Integer ierr = 0;
	Text stream = "";
	while(!ierr){
		Integer a,b,c;
        ierr=0;
		if(File_read(fin,a))ierr++;
        if(File_read(fin,b))ierr++;
        if(File_read(fin,c))ierr++;
        Text data=i32_3_to_b64(a,b,c); // always get the stream
		if (ierr){
			Print("ONCE?");Print(ierr);Print();
            count = INTEGER_SETS_PER_LINE_OF_TEXT; // this means we'll force data to be written
            switch (ierr){
                case 3 :{ // all reads failed
                    data = "";
                }break;
                case 2 :{ // b and c failed
                    data = Get_subtext(data,1,6) + "==========";
                }break;
                case 1 :{ // c failed
                    data = Get_subtext(data,1,11) + "=====";
                }break;
            }
		}
		count++;
        stream+=data;
		if(count>=INTEGER_SETS_PER_LINE_OF_TEXT){
			File_write_line(fout, "\tAppend(\""+stream+"\",dt);");
			stream = "";
			count=0;
		}
	}

	Text footer =	"\treturn (dt);"
			+"\n"+	"}"
			+"\n"+	"\n// EXAMPLE USAGE TO CREATE A FILE BASED ON THE DATA ABOVE...\n"
			+"\n"+	"#include \"mashy_lib_files_base64.H\"\n"
			+"\n"+	"void main(){"
			+"\n"+	"\tDynamic_Text dt = "+binary_data_function_name+"();"
			+"\n"+	"\tInteger do_overwrite = 1;"
			+"\n"+	"\twrite_binary_file_from_4dm_code_dynamic_text_function( \"my_test_"+file_name_in+"\", dt, do_overwrite );"
			+"\n"+	"}"
			;
	ok+=File_write_line(fout,footer);
	ok+=File_close(fin);
	ok+=File_close(fout);
	return(ok);
}


#endif