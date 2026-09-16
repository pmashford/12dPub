// tracing routines, where is the macro at
// for debugging

#ifndef mashy_lib_debug_trace_h_included
#define mashy_lib_debug_trace_h_included

{
	Integer trace_mode = 0 ;
}


void trace_in(Text msg){
	msg = ">>>>> " + msg + "\n";
	Print(msg);
}

void trace_out(Text msg){
	msg = "<<<<< " + msg + "\n";
	Print(msg);
}

// should use this in pairs
void trace(Text msg){
	if (trace_mode == 0 ){
		trace_mode = 1;
		trace_in(msg);
	} else if (trace_mode == 1){
		trace_mode = 0;
		trace_out(msg);
	}
}

#endif