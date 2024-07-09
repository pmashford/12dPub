#ifndef mashy_lib_widgets_progress_bar_included_H
#define mashy_lib_widgets_progress_bar_included_H

// progress bar global variables

{
	Panel progress_bar_panel;
	Message_Box progress_bar_message;
	Integer progress_bar_size;
	Text progress_bar_text;

	Integer progress_bar_panel_created;

	Integer progress_bar_console_messages;

	Real progress_bar_time_for_last_tick;
	Real progress_bar_time_interval;
	Real progress_bar_timer;
	Real progress_bar_start_time;

	Text progress_bar_spinner;
	Integer progress_bar_spinner_char;
	Integer progress_bar_spinner_length;
}

#ifndef MASHY_LIB_PROGRESS_PANEL_UPDATE_TIME_STEP
#define MASHY_LIB_PROGRESS_PANEL_UPDATE_TIME_STEP 0.35
#endif

#ifndef MASHY_LIB_PROGRESS_PANEL_SPINNER_TEXT
#define MASHY_LIB_PROGRESS_PANEL_SPINNER_TEXT "\\|/-"
//#define MASHY_LIB_PROGRESS_PANEL_SPINNER_TEXT "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
#endif

Integer progress_bar_test_time_interval(){
	Real this_time;
	Time(this_time);
	if( (this_time - progress_bar_time_for_last_tick)>progress_bar_time_interval ){
		progress_bar_time_for_last_tick = this_time;
		progress_bar_timer = this_time - progress_bar_start_time;
		return 0;
	}
	return 1;
}

void show_progress_bar(Integer max_count, Message_Box &message_box){ // this can be called if a panel already has a message box, it initialised message_box
	progress_bar_size = max_count;
	progress_bar_time_for_last_tick = 0.0;
	//progress_bar_time_interval = 0.25;
	progress_bar_time_interval = MASHY_LIB_PROGRESS_PANEL_UPDATE_TIME_STEP;
	progress_bar_message = message_box;
	Set_data(progress_bar_message,"0.0% complete");
	progress_bar_timer = 0.0;
	Time(progress_bar_start_time);
	//progress_bar_spinner = "\\|/-";
	progress_bar_spinner = MASHY_LIB_PROGRESS_PANEL_SPINNER_TEXT ;
	progress_bar_spinner_char = 1;
	progress_bar_spinner_length = Text_length(progress_bar_spinner);
	progress_bar_panel_created = 0;  // using existing panels message box
}

void show_progress_bar(Integer max_count){ // this should be called if a panel does not exist, or an additional panel should be created to show the progress
	progress_bar_message = Create_message_box("0.0% complete");
	Set_width_in_chars(progress_bar_message,30);

	show_progress_bar(max_count, progress_bar_message);

	progress_bar_panel = Create_panel("Progress");
	Append(progress_bar_message,progress_bar_panel);
	Show_widget(progress_bar_panel);
	progress_bar_panel_created = 1;  // new panel created
}


Integer is_odd(Integer i){ // compare the last bit
	return i & 1;
}


void update_progress_bar(Integer count){
	if(progress_bar_test_time_interval())return; // skip if it has been less than 0.25 seconds

	Real percent = count * 100.0 / progress_bar_size;
	Integer pos = 21 - Ceil(percent / 5.0);

	//progress_bar_spinner = Get_subtext(progress_bar_spinner,2,Text_length(progress_bar_spinner) )+ Get_subtext(progress_bar_spinner,1,1);  // shuffle the spinner

	Get_subtext(progress_bar_spinner,progress_bar_spinner_char,progress_bar_spinner_char);

	//Text t = Get_subtext("||||||||||||||||||||....................",pos,pos+19)+ " " + To_text(percent,1)+" % complete  " + Get_subtext(progress_bar_spinner,1,1) + " ( " + To_text(progress_bar_timer,1) + " secs )";
	Text t = Get_subtext("||||||||||||||||||||....................",pos,pos+19)+ " " + To_text(percent,1)+" % complete  " + Get_subtext(progress_bar_spinner,progress_bar_spinner_char,progress_bar_spinner_char) + " ( " + To_text(progress_bar_timer,1) + " secs )";
	progress_bar_spinner_char++;
	if(!(progress_bar_spinner_char%(progress_bar_spinner_length+1)))progress_bar_spinner_char =1; //restart char if at the end

	Set_data(progress_bar_message,t);

	if(progress_bar_console_messages==1){
		Print(t+"\n");
	}

}


void close_progress_bar(){
	if(progress_bar_panel_created == 1){
		Hide_widget(progress_bar_panel);
		progress_bar_panel_created = 0;
	}else{
		Set_data(progress_bar_message,"finished");
	}
	//progress_bar_created = -1;
}

void sleep_timer_with_progress_bar(Real seconds_to_run, Integer time_steps){
	Integer millisecs_to_run = seconds_to_run * 1000;
	Integer time_step_to_sleep_millisecs = millisecs_to_run / time_steps;
	show_progress_bar(time_steps);
	for(Integer i=1;i<=time_steps;i++){
			Sleep(time_step_to_sleep_millisecs);
			update_progress_bar(i);
	}
	close_progress_bar();
}

void sleep_timer_with_progress_bar(Real seconds_to_run){
	sleep_timer_with_progress_bar(seconds_to_run, 15);
}


	#if 0
	// example usage

	void main(){
		Integer items = 7540082;
		show_progress_bar(items); // or void show_progress_bar(Integer max_count, Message_Box &message_box){
		for (Integer i=1;i<=items;i++){
			update_progress_bar(i);
		}
		close_progress_bar();

		Real seconds_to_run = 5.5;
		sleep_timer_with_progress_bar(seconds_to_run);

	}

	#endif

#endif