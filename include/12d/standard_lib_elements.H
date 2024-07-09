#ifndef standard_lib_elements_included
#define standard_lib_elements_included

Integer dynamic_element_draw(Dynamic_Element &elts)
{// returns ZERO when all draw aOK
 // returns number of failed elements if not Zero
  Integer erro = 0;

  Integer no_items;
  Get_number_of_items(elts, no_items);

  for (Integer j = 1; j <= no_items; j++) {
	  Element string;
	  Get_item (elts,j,string);  	
	  if (  Element_draw(string) != 0 ) { erro +=1; }
  }

return(erro);
}

//.................
//.................


Integer dynamic_element_draw(Dynamic_Element &elts,Integer colour)
{// returns ZERO when all draw aOK
 // returns number of failed elements if not Zero
  Integer erro = 0;

  Integer no_items;
  Get_number_of_items(elts, no_items);

  for (Integer j = 1; j <= no_items; j++) {
	  Element string;
	  Get_item (elts,j,string);  	
	  if (  Element_draw(string,colour) != 0 ) { erro +=1; }
  }

return(erro);
}

//.................
//.................
Integer dynamic_elements_delete(Dynamic_Element &elts)
{// returns ZERO when all deleted aOK
 // returns number of failed elements if not Zero
  Integer erro = 0;

  Integer no_items;
  Get_number_of_items(elts, no_items);

  for (Integer j = 1; j <= no_items; j++) {
	  Element string;
	  Get_item (elts,j,string);  	
	  if (  Element_delete(string) != 0 ) { erro +=1; }
  }

return(erro);
}

//.................
//.................

Integer dynamic_elements_duplicate(Dynamic_Element &elts,Dynamic_Element &dup)
{// returns ZERO when all duplicated aOK
 // returns number of failed elements if not Zero
  Null(dup);
  Integer erro = 0;

  Integer no_items;
  Get_number_of_items(elts, no_items);

  for (Integer j = 1; j <= no_items; j++) {
	  Element string,changed;
	  Get_item (elts,j,string);  	
	  if (  Element_duplicate(string,changed) == 0 ) { 
		  Append(changed,dup);
	  }else {
		  erro +=1; 
	  }
  }
return(erro);
}


//.................
//.................



Integer  calc_extents(Dynamic_Element &elts)
{
  Integer no_items;
  Get_number_of_items(elts, no_items);

  for (Integer j = 1; j <= no_items; j++) {
	  Element string;
	  Get_item (elts,j,string);  
	  Calc_extent (string);
  }
return(0);
}

//.................
//.................


Integer get_xy(Text &msg,Real &x,Real &y) 
{
  Dynamic_Text	name_array;
  From_text(msg,name_array); 

  Text cursx,cursy;

  Get_item(name_array,1,cursx);
  Get_item(name_array,2,cursy);

  From_text(cursx,x);
  From_text(cursy,y);

  Null(name_array);
return(0);  
}


//.................
//.................



Integer  get_extents_x(Dynamic_Element &elts,Real &min_x,Real &max_x)
{
  min_x = 0;
  max_x = 0;

  Integer no_items;
  Get_number_of_items(elts, no_items);

  for (Integer j = 1; j <= no_items; j++) {
	  Real str_x_min, str_x_max;
	  Element string;
	  Get_item (elts,j,string);  
	  Calc_extent (string);
      Get_extent_x(string,str_x_min, str_x_max); 
	  if ( str_x_min < min_x ) min_x = str_x_min;
	  if ( str_x_max > max_x ) max_x = str_x_max;
  }
return(0);
}

//.................
//.................


Integer  get_extents_y(Dynamic_Element &elts,Real &min_y,Real &max_y)
{
  min_y = 0;
  max_y = 0;

  Integer no_items;
  Get_number_of_items(elts, no_items);

  for (Integer j = 1; j <= no_items; j++) {
	  Real str_y_min, str_y_max;
	  Element string;
	  Get_item (elts,j,string);  
	  Calc_extent (string);
      Get_extent_y(string,str_y_min, str_y_max); 
	  if ( str_y_min < min_y ) min_y = str_y_min;
	  if ( str_y_max > max_y ) max_y = str_y_max;
  }
return(0);
}

//.................
//.................


Integer get_element_locks(Element &elt, Integer &read_locks,Integer &write_locks, Integer &both_locks ) 
{// get both element locks at the same time and return 0 for no lock in each case
 // note Read locks normally give 1 for no locks but write locks give 0 
 // this code brings them in line and both return 0 for no locks
Integer no_locks = 0;

Get_read_locks (elt,no_locks);
if ( no_locks == 1 ) { 
read_locks = 0;
}else {
read_locks = no_locks;
}

no_locks = 0;
Get_write_locks(elt,no_locks);
write_locks = no_locks;
both_locks = read_locks+write_locks;

return(0);
}
//.................
//.................


Integer get_model_locks(Model &a_model,Integer &no_locks) 
{// get no of locks in a model 

Integer retval = 0;
no_locks = 0;

Dynamic_Element model_elts;
Integer num_elts;
Get_elements(a_model,model_elts,num_elts);

for(Integer i=1;i<=num_elts;i++) { // step through each element
    Element element;
    Get_item(model_elts,i,element);

    Integer no_r_locks,no_w_locks, both_locks ;
    get_element_locks(element, no_r_locks,no_w_locks, both_locks );
    retval += both_locks;
}
no_locks = retval;
Null(model_elts);
return(retval); // 0 = no locks, 1++ = number of locks
} 

//.................
//.................


Integer check_locks_clean_model(Model &model,Message_Box &message) {
  //-----------------------------------------------------------
  // clean models if not locked
  // and report to message if fails
  // returns 1 if failed clean due to locked strings
      Integer  no_locks;
      get_model_locks(model,no_locks);  

      Text model_name;
      if ( no_locks >= 1 ) { 
          Get_name(model, model_name);
          //Print("\Model: " + model_name + " is LOCKED! Please unlock all elements - click [ESC] key\n");
          Set_data(message, "Model: " + model_name + " is LOCKED! error");
          return(1);
      }else {            
          clean_model(model);  
      }      
return(0);
}
//.................
//.................


Integer check_locks_clean_model(Model &model) {
  //-----------------------------------------------------------
  // clean models if not locked
  // returns 1 if failed clean due to locked strings
      Integer  no_locks;
      get_model_locks(model,no_locks);  

      if ( no_locks >= 1 ) { 
          return(1);
      }else {            
          clean_model(model);  
      }      
return(0);
}
//.................
//.................

Integer check_locks_delete_model(Model &model,Message_Box &message) {
  //-----------------------------------------------------------
  // clean models if not locked
  // and report to message if fails
  // returns 1 if failed clean due to locked strings
      Integer  no_locks;
      get_model_locks(model,no_locks);  

      Text model_name;
      if ( no_locks >= 1 ) { 
          Get_name(model, model_name);
          //Print("\Model: " + model_name + " is LOCKED! Please unlock all elements - click [ESC] key\n");
          Set_data(message, "Model: " + model_name + " is LOCKED! error");
          return(1);
      }else {            
          clean_model(model);             
          Model_delete(model);  
      }      
return(0);
}
//.................
//.................


Integer check_locks_delete_model(Model &model) {
  //-----------------------------------------------------------
  // clean models if not locked
  // returns 1 if failed clean due to locked strings
      Integer  no_locks;
      get_model_locks(model,no_locks);  

      if ( no_locks >= 1 ) { 
          return(1);
      }else {             
          clean_model(model);            
          Model_delete(model);  
      }      
return(0);
}

#endif