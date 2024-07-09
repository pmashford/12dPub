#ifndef text_library_included
#define text_library_included

Text trim(Text input)
//----------------------------------------------------------------------------------
//
//----------------------------------------------------------------------------------
{

// trim blank spaces

  input = Text_justify(input);
    
// remove quotation marks

  Integer last = Text_length(input);
  
  if (Get_subtext(input,last,last) == "\"") input = Get_subtext(input,1,last-1);
  if (Get_subtext(input,1,1)       == "\"") input = Get_subtext(input,2,last);

  return input;
}
Integer from_text(Text input,Dynamic_Text &words,Text word_delimiter)
// ------------------------------------------------------------------------------
// get all the words out of a string (delim normally a , here)
// ------------------------------------------------------------------------------
{
  Integer count = 0;
  Null(words);
  
  while(1) {

    Integer pos = Find_text(input,word_delimiter);
    if(pos == 0) {

// last word

      Append(trim(input),words);
      count++;

      break;
    }
    if(pos > 1) {

// get word up to the 'delim' char

      Append(trim(Get_subtext(input,1,pos-1)),words);
      count++;
    }
    input = Get_subtext(input,pos+1,999999);
  }
  return(count);
}
Integer remove_extension(Text name,Text &new_name,Text &ext)
//--------------------------------------------------------
// Programmer    Alan Gray
// Date          27/09/96
//
// Description : Remove_Extenstion
//
// Remove trailing extension from a bit of text
//
// return value is 0 for success
//
//--------------------------------------------------------
{
  Dynamic_Text words;
  Integer      no_words = from_text(name,words,".");

  ext      = "";
  new_name = name;

// if no extension - it must be a model ?

  if(no_words == 1) return(0);
  if(no_words <  2) return(-1);

  Text temp;

  for(Integer w=1;w<no_words;w++) {

    if(w > 1) temp += ".";

    Text word;
    Get_item(words,w,word);

    temp += word;
  }
  new_name = temp;

  Get_item(words,no_words,ext);

  return(0);
}
Text radians_to_dms_text(Real angle)
// ---------------------------------------------------------------------
// routine to convert a radians angle into a dd^mm'ss.ss'
// ---------------------------------------------------------------------
{
 	Radians_to_degrees(angle,angle);

 	Integer d,m;
 	Real    s;

 	Degrees_to_dms(angle,d,m,s);

 	Text t = To_text(d,"%4d") + "d" + To_text(m,"%2d") + "'" + To_text(s,"%2.0lf") + "\"";
 	return(t);
}

#endif

