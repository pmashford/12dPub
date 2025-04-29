#ifndef MASHY_LIB_TIMER_START_H
#define MASHY_LIB_TIMER_START_H

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