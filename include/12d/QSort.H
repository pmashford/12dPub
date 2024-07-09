#ifndef qsort_included
#define qsort_included

// (TODO:mashy) remove these 2 lines, was a hack for something??, not neaded with new Qsort() anyway
#include "mashy_lib_get_item.H"
#define USE_MASHY_LIB_GET_ITEM_CALLS 1
//--------------------------------------------------------
// Programmer    David Dudley
// Date          13/06/96
//
// Description :
//
//    Function to sort a list of real numbers and return an index
//    to the sorted values. A quicksort is used.
//    The list is sorted in ascending order. ie. index[1] points to
//    the smallest real in the list.
//
// Arguments :
//   list   ==> array containing the values to be sorted.
//   index  <== index to the sort items in list.
//   number ==> number of items in the list.
//
// Function return values : None
//
//--------------------------------------------------------

void partit (Real list[], Integer &index[], Integer &i, Integer &j, Integer left, Integer right )
//--------------------------------------------------------
// auxiliary routine used by sort routine
//--------------------------------------------------------
{
   Integer temp;
   Integer ii;
   Integer jj;
   Integer pivot;

   ii = left;
   jj = right;
   pivot = index [(ii+jj)/2];

   while ( ii <= jj )
   {
      while ( list[index[ii]] < list[pivot]     ) ++ii;
      while ( list[pivot]     < list[index[jj]] ) --jj;
      if ( ii <= jj )
      {
         temp      = index[ii];
         index[ii] = index[jj];
         index[jj] = temp;
         ++ii;
         --jj;
      }
   }

   i = ii;
   j = jj;

   return;
}
void qisort1 (Real &list[], Integer &index[], Integer m, Integer n)
//--------------------------------------------------------
// auxiliary routine used by sort routine
//--------------------------------------------------------
{
   Integer i;
   Integer j;

   if ( m < n )
   {
      partit  (list,index,i,j,m,n);
      qisort1 (list,index,m,j);
      qisort1 (list,index,i,n);
   }
   return;
}
void Qsort(Real list[],Integer &index[], Integer number)
//--------------------------------------------------------
// sort routine
//--------------------------------------------------------
{
   Integer j;

   // initialise the index
   for ( j=1 ; j<=number ; ++j ) index[j]=j;

   qisort1 (list,index,1,number);

   return;
}





void partit (Text list[], Integer &index[], Integer &i, Integer &j, Integer left, Integer right )
//--------------------------------------------------------
// auxiliary routine used by sort routine
//--------------------------------------------------------
{
   Integer temp;
   Integer ii;
   Integer jj;
   Integer pivot;

   ii = left;
   jj = right;
   pivot = index [(ii+jj)/2];

   while ( ii <= jj )
   {
      while ( list[index[ii]] < list[pivot]     ) ++ii;
      while ( list[pivot]     < list[index[jj]] ) --jj;
      if ( ii <= jj )
      {
         temp      = index[ii];
         index[ii] = index[jj];
         index[jj] = temp;
         ++ii;
         --jj;
      }
   }

   i = ii;
   j = jj;

   return;
}
void qisort1 (Text &list[], Integer &index[], Integer m, Integer n)
//--------------------------------------------------------
// auxiliary routine used by sort routine
//--------------------------------------------------------
{
   Integer i;
   Integer j;

   if ( m < n )
   {
      partit  (list,index,i,j,m,n);
      qisort1 (list,index,m,j);
      qisort1 (list,index,i,n);
   }
   return;
}
void Qsort(Text list[],Integer &index[],Integer number)
//--------------------------------------------------------
// sort routine
//--------------------------------------------------------
{
   Integer j;

   // initialise the index
   for ( j=1 ; j<=number ; ++j ) index[j]=j;

   qisort1 (list,index,1,number);

   return;
}


#ifndef USE_MASHY_LIB_GET_ITEM_CALLS
Integer get_item(Dynamic_Integer &list,Integer pos)
//--------------------------------------------------------
//--------------------------------------------------------
{
  Integer i;
  if(Get_item(list,pos,i) == 0) return(i);

// eek programming error :-(

  return(-99999999);
}
#endif
void partit (Dynamic_Integer &list, Integer &index[], Integer &i, Integer &j, Integer left, Integer right )
//--------------------------------------------------------
// auxiliary routine used by sort routine
//--------------------------------------------------------
{
   Integer temp;
   Integer ii;
   Integer jj;
   Integer pivot;

   ii = left;
   jj = right;
   pivot = index [(ii+jj)/2];

   while ( ii <= jj )
   {
      while ( get_item(list,index[ii]) < get_item(list,pivot)     ) ++ii;
      while ( get_item(list,pivot)     < get_item(list,index[jj]) ) --jj;
      if ( ii <= jj )
      {
         temp      = index[ii];
         index[ii] = index[jj];
         index[jj] = temp;
         ++ii;
         --jj;
      }
   }

   i = ii;
   j = jj;

   return;
}
void qisort1 (Dynamic_Integer &list, Integer &index[], Integer m, Integer n)
//--------------------------------------------------------
// auxiliary routine used by sort routine
//--------------------------------------------------------
{
   Integer i;
   Integer j;

   if ( m < n )
   {
      partit  (list,index,i,j,m,n);
      qisort1 (list,index,m,j);
      qisort1 (list,index,i,n);
   }
   return;
}
void Qsort(Dynamic_Integer &list,Integer &index[],Integer number)
//--------------------------------------------------------
// sort routine
//--------------------------------------------------------
{
   Integer j;

   // initialise the index
   for ( j=1 ; j<=number ; ++j ) index[j]=j;

   qisort1 (list,index,1,number);

   return;
}



#ifndef USE_MASHY_LIB_GET_ITEM_CALLS
Real get_item(Dynamic_Real &list,Integer pos)
//--------------------------------------------------------
//--------------------------------------------------------
{
  Real r;
  if(Get_item(list,pos,r) == 0) return(r);

// eek programming error :-(

  return(-9.9e29);
}
#endif

void partit (Dynamic_Real &list, Integer &index[], Integer &i, Integer &j, Integer left, Integer right )
//--------------------------------------------------------
// auxiliary routine used by sort routine
//--------------------------------------------------------
{
   Integer temp;
   Integer ii;
   Integer jj;
   Integer pivot;

   ii = left;
   jj = right;
   pivot = index [(ii+jj)/2];

   while ( ii <= jj )
   {
      while ( get_item(list,index[ii]) < get_item(list,pivot)     ) ++ii;
      while ( get_item(list,pivot)     < get_item(list,index[jj]) ) --jj;
      if ( ii <= jj )
      {
         temp      = index[ii];
         index[ii] = index[jj];
         index[jj] = temp;
         ++ii;
         --jj;
      }
   }

   i = ii;
   j = jj;

   return;
}
void qisort1 (Dynamic_Real &list, Integer &index[], Integer m, Integer n)
//--------------------------------------------------------
// auxiliary routine used by sort routine
//--------------------------------------------------------
{
   Integer i;
   Integer j;

   if ( m < n )
   {
      partit  (list,index,i,j,m,n);
      qisort1 (list,index,m,j);
      qisort1 (list,index,i,n);
   }
   return;
}
void Qsort(Dynamic_Real &list,Integer &index[],Integer number)
//--------------------------------------------------------
// sort routine
//--------------------------------------------------------
{
   Integer j;

   // initialise the index
   for ( j=1 ; j<=number ; ++j ) index[j]=j;

   qisort1 (list,index,1,number);

   return;
}

#ifndef get_item_dynamic_text_included
#define get_item_dynamic_text_included


#ifndef USE_MASHY_LIB_GET_ITEM_CALLS
Text get_item(Dynamic_Text &list,Integer pos)
//--------------------------------------------------------
//--------------------------------------------------------
{
  Text t;
  if(Get_item(list,pos,t) == 0) return(t);

// eek programming error :-(

  return("");
}
#endif
#endif

void partit (Dynamic_Text &list, Integer &index[], Integer &i, Integer &j, Integer left, Integer right )
//--------------------------------------------------------
// auxiliary routine used by sort routine
//--------------------------------------------------------
{
   Integer temp;
   Integer ii;
   Integer jj;
   Integer pivot;

   ii = left;
   jj = right;
   pivot = index [(ii+jj)/2];

   while ( ii <= jj )
   {
      while ( get_item(list,index[ii]) < get_item(list,pivot)     ) ++ii;
      while ( get_item(list,pivot)     < get_item(list,index[jj]) ) --jj;
      if ( ii <= jj )
      {
         temp      = index[ii];
         index[ii] = index[jj];
         index[jj] = temp;
         ++ii;
         --jj;
      }
   }

   i = ii;
   j = jj;

   return;
}
void qisort1 (Dynamic_Text &list, Integer &index[], Integer m, Integer n)
//--------------------------------------------------------
// auxiliary routine used by sort routine
//--------------------------------------------------------
{
   Integer i;
   Integer j;

   if ( m < n )
   {
      partit  (list,index,i,j,m,n);
      qisort1 (list,index,m,j);
      qisort1 (list,index,i,n);
   }
   return;
}
void Qsort(Dynamic_Text &list,Integer &index[],Integer number)
//--------------------------------------------------------
// sort routine
//--------------------------------------------------------
{
   Integer j;

   // initialise the index
   for ( j=1 ; j<=number ; ++j ) index[j]=j;

   qisort1 (list,index,1,number);

   return;
}

#endif

