#ifndef Assert_included
#define Assert_included

// Asserts are a great way of debugging code to check on return values
// and at what line they accur on.
//
// to use an assert on the bit of code Set_2d_data(elt,level)
//
// just ASSERT(Set_2d_data(elt,level))
//

{
  Integer assert_error_value;
}

#include "X:\12d\MACROS\_INCLUDES\debug_panel.H"

void assert_function(Text filename,Integer lineno,Text function_call)
// -----------------------------------------------------------
// -----------------------------------------------------------
{
  Text msg = filename + ":" + To_text(lineno) + " " + function_call + ") failed with " + To_text(assert_error_value);

  debug_print("ASSERT",msg);
}
void assert_function(Text filename,Integer lineno,Text function_call,Text function_test)
// -----------------------------------------------------------
// -----------------------------------------------------------
{
  Text msg = filename + ":" + To_text(lineno) + " " + function_call + " " + function_test + ") failed with " + To_text(assert_error_value);

  debug_print("ASSERT",msg);
}

#define ASSERT(expression) if((assert_error_value=expression)) assert_function(__FILE__,__LINE__,#expression)

#define ASSERT_TEST(expression,test) if((assert_error_value=expression) test) assert_function(__FILE__,__LINE__,#expression,#test)

void trace_function(Text filename,Integer lineno,Text function_call)
// -----------------------------------------------------------
// -----------------------------------------------------------
{
  Text msg = filename + ":" + To_text(lineno) + " " + function_call + ") failed with " + To_text(assert_error_value);

  Print(msg);  Print();
}
void trace_function(Text filename,Integer lineno,Text function_call,Text function_test)
// -----------------------------------------------------------
// -----------------------------------------------------------
{
  Text msg = filename + ":" + To_text(lineno) + " " + function_call + " " + function_test + ") failed with " + To_text(assert_error_value);

  Print(msg);  Print();
}

#define TRACE(expression) if((assert_error_value=expression)) trace_function(__FILE__,__LINE__,#expression)

#define TRACE_TEST(expression,test) if((assert_error_value=expression) test) trace_function(__FILE__,__LINE__,#expression,#test)

#endif

