#ifndef MASHY_LIB_TIMER_START_H
#define MASHY_LIB_TIMER_START_H


#if 0
// USAGE

    Real t_time = 0.0;
    Text t_msg="\n";
    Show_console(1);
    Clear_console();

    t_msg+= timer_msg(t_time,"function_to_time()") + "\n";						// start the timer
    function_to_time(args);														// do something or run a function(s)
    t_msg+= timer_msg(t_time,"function_to_time()") + "\n\n";    t_time = 0.0; 	// collect the timer in a message and reset the timer
#endif	

Text timer_msg(Real &t, Text msg){ // if the passed in t is null or 0, then it will return the current time, otherwise it will print the elapsed time since t
    msg = "TIMER: " + msg;
    if(Is_null(t) || (t == 0.0)){
		Get_time(t);
		return(msg + " Started....");
	}
    Real x;
    Get_time(x);
    msg += " "+ To_text(x-t,6) + " secs elapsed, ( stop["+To_text(x,6)+"] - start ["+To_text(t,6)+"] )";
	Get_time(t);
	return(msg);
}

Real timer(Real &t){
	Real x;
	Get_time(x);
	if(Is_null(t) || Xeqy(t,0.0)){
		Get_time(t);
		return(t);
	}else{
		Print("Elapsed="+To_text(x-t,6)+" , ( stop["+To_text(x,6)+"] - start ["+To_text(t,6)+"] )\n");
		Null(x);
		Null(t);
	}
	return(x);
}

Real timer(Real &t, Message_Box &box){
	Real x;
	Get_time(x);
	if(Is_null(t) || Xeqy(t,0.0)){
		Get_time(t);
		return(t);
	}else{
		Text my_text = "Elapsed="+To_text(x-t,6)+" , (stop["+To_text(x,6)+"] - start["+To_text(t,6)+"])";
		Print(my_text+"\n");
		Set_data(box,my_text);
		Null(x);
		Null(t);
	}
	return(x);
}

Real timer_start(){
	Real t;
	Get_time(t);
	return(t);
}

Real timer(){
	return(timer_start());
}

Real timer_stop(Real &t){
	Real x;
	Get_time(x);
	t = x-t;
	//Null(t);
	return(t);
}

void timer_restart(Real &t){
	timer(t);
	Null(t);
}

#endif