#ifndef mashy_lib_macro_arguments_h_included
#define mashy_lib_macro_arguments_h_included

Text combined_args(){
	Integer nargs = Get_number_of_command_arguments();
	Text t = "";
	for(Integer i=1;i<=nargs;i++){
		Text arg;
		Get_command_argument(i,arg);
		if(i>1)t+=" ";
		t+=arg;
	}
	return t;
}

Integer get_argument(Integer mandatory_position, Text name, Text &value){
	Integer nargs = Get_number_of_command_arguments();
	value = "";
	if(nargs >= mandatory_position){
		Text arg;
		Get_command_argument(mandatory_position,arg);
		if(Text_lower(name) == Text_lower(arg)){
			if(mandatory_position < nargs)return Get_command_argument(mandatory_position+1,value);
			return 0;	//value would = "";
		}
	}
	return 1;
}

Integer get_argument(Text name, Text &value){
	Integer nargs = Get_number_of_command_arguments();
	value = "";
	for(Integer i=1;i<=nargs;i++){
		Text arg;
		Get_command_argument(i,arg);
		if(Text_lower(name) == Text_lower(arg)){
			if(i==nargs)return 2;
			return Get_command_argument(i+1,value);
		}
	}
	return 1;
}

Integer argument_exists(Text name){
	Integer nargs = Get_number_of_command_arguments();
	for(Integer i=1;i<=nargs;i++){
		Text arg;
		Get_command_argument(i,arg);
		if(Text_lower(name) == Text_lower(arg)){
			return 1;
		}
	}
	return 0;
}

Text get_argument_as_string(){
		Text command="";
		Integer args = Get_number_of_command_arguments();
		if(args < 1 )return command;
		for(Integer i=1;i<=args;i++){
			Text t;
			Get_command_argument(i,t);
			command=command+t;
			if(i!=args)command+=" "; //add a space if there will be another argument to append to string
		}
		return command;
}

Integer process_dashed_arguments(Dynamic_Text &dashed_arguments, Text &error_msg){
		Integer args = Get_number_of_command_arguments();
		if(args < 1 )return(-1);
		Text command = get_argument_as_string();
		Null(dashed_arguments);
		Integer start=0;
		Integer count=0;
		Integer in_brackets = 0;
		Integer in_quotes = 0;
		Integer len = Text_length(command);
		for(Integer i=1;i<=len;i++){
			Text c = Get_subtext(command,i,i);
			if(c == "\""){
				if(in_quotes == 0)in_quotes =1;
				else in_quotes = 0;
				continue;
			}else if(c == "<" && !in_brackets){
				in_brackets =1;
				continue;
			}else if(c == ">" && in_brackets){
				in_brackets = 0;
				continue;
			}else if(c=="-" && !in_brackets && !in_quotes){
				if(start == 0 && i == 1){
					start = 1;
					continue;
				}
				if(count == 0){
					if(error_msg!=""){
						error_msg+=Get_subtext(command,1,i-1)+">";
						continue;
					}
				}
				Append(Get_subtext(command,start,i-1),dashed_arguments);
				count++;
				start = i;
			}
			if(i==1){			//first character
				error_msg+="Error: unknown text <";
			}else if(i==len){		//last character
				Append(Get_subtext(command,start,len),dashed_arguments);
				count++;
			}
		}
		return (count);
}

#endif