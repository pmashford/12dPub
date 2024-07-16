#ifndef mashy_lib_tins_h_included
#define mashy_lib_tins_h_included

#include "mashy_lib_tins_triangles.H"

#include "mashy_lib_text_functions.H"
#include "mashy_lib_files.H"
#include "mashy_lib_geometry.H"
#include "mashy_lib_debug.H"
#include "mashy_lib_create_super_text.H"
#include "mashy_lib_text_functions.H"
#include "mashy_lib_system_calls.H"
#include "mashy_lib_get_user_input.H"
#include "mashy_lib_element_functions.H"


Text get_tin_prefix_env(){
	Text t = Text_justify(Getenv("MODEL_FOR_TIN_PREFIX_4D"));
	if(t == "")t="tin *";
	return (t);
}


Text get_tin_model_name_using_envvar(Text tin_name){
	Text t = get_tin_prefix_env();
	t = find_replace_repeat(t, "**", "*");
	Integer pos = Find_text(t,"*");
	if(pos){
		t=find_replace(t,"*",tin_name);
	}else{
		t+=tin_name;
	}
	Text_justify(t);
	return (t);
}


Integer get_extent(Tin &tin[], Integer ntins, Real &min_x, Real &max_x, Real &min_y, Real &max_y){
	Integer ok=0;
	for(Integer i=1;i<=ntins;i++){
		Real thisminx,thisminy,thismaxx,thismaxy;
		ok+= Get_extent_y(tin[i],thisminy,thismaxy);
		ok+= Get_extent_x(tin[i],thisminx,thismaxx);

		if(i==1){
			min_x=thisminx;
			min_y=thisminy;
			max_x=thismaxx;
			max_y=thismaxy;
		}else{
			if(thisminx < min_x)min_x=thisminx;
			if(thisminy < min_y)min_y=thisminy;
			if(thismaxx > max_x)max_x=thismaxx;
			if(thismaxy > max_y)max_y=thismaxy;
		}
	}
	return ok;
}

Integer is_shared(Tin &tin){
//the uid of a shared tin begins with {... use this as a test to see if a tin has been shared in
	Uid uid;
	Get_id(tin,uid);
	Integer c;
	Get_char(To_text(uid),1,c);
	if(c=='{')	return (1);
	return (0);
}

//1 means the tin is assumed to be shared
//0 means the tin is not shared
//if *.tin exists then the tin was created in the project. If *.tin does not exist then assume it is shared - could check Get_temporary_project_directory() for the tin name - if it is in here it is definetely shared into the project
Integer is_shared_in(Tin &tin){
	Text tin_name;
	Get_name(tin,tin_name);
	tin_name = find_replace(tin_name," ","_");
	print("processed tin name",tin_name);
	Integer ierr = File_exists(get_dot_project_folder()+"\\"+tin_name+".tin");
	ierr = !ierr;
	return ierr;
}

Integer get_project_tin_models(Dynamic_Text &tin_names, Dynamic_Text &model_names){
//made some changes but never checked
	Integer count;
	Dynamic_Text project_models;
	Get_project_models(project_models);
	Integer num_models; Get_number_of_items(project_models,num_models);
	for(Integer i = 1 ; i <= num_models ; i++ ){
		Text model_name;	Get_item(project_models,i,model_name);
		//p("Checking model <"+model_name+"> for tins");
		Model model = Get_model(model_name);
		Dynamic_Element elts;
		Append(model,elts);		// <=made some changes but never checked... no QA done
		Integer num_elts;
		Get_number_of_items(elts,num_elts);
		for(Integer j = 1 ; j <= num_elts ; j++){
			Element elt;	Get_item(elts,j,elt);
			Text type;	Get_type(elt,type);
			if(type == "Tin"){
				Text tin_name;	Get_name(elt,tin_name);
				Append(tin_name,tin_names);
				Append(model_name,model_names);
				count++;
			}
			Null(elt);
		}
		Null(model);
	}
	Null(project_models);
	return count;
}

#define TIN_NOT_IN_ANY_MODELS 10

// this should be used when info is required for multiple tins - the database is only checked once for all tins - this process is SLOW
Integer get_tin_models(Text tin_name, Dynamic_Text &tin_list, Dynamic_Text &tin_model_list, Dynamic_Text &matched_model_list){
	// return none 0 if the tin tin_name does not exist
	Null(matched_model_list);
	Integer items;
	Get_number_of_items(tin_list,items);
	Integer count=0;
	for(Integer i=1;i<=items;i++){
		Text t;
		Get_item(tin_list,i,t);
		if(t == tin_name){
			Append(t,matched_model_list);
			count++;
		}
	}
	if(count)return 0;	// if there were models found then
	return TIN_NOT_IN_ANY_MODELS;
}

Integer get_tin_models(Text tin_name, Dynamic_Text &matched_model_list){
	Dynamic_Text tin_list, tin_model_list;
	get_project_tin_models(tin_list,tin_model_list);
	return get_tin_models(tin_name, tin_list, tin_model_list, matched_model_list);
}

Dynamic_Text get_tins(Text match){
	Dynamic_Text project_tins;
	Get_project_tins(project_tins);
	Dynamic_Text tins;
	match_name(project_tins,match,tins);
	Null(project_tins);
	return tins;
}

Dynamic_Text get_super_tins(Text match){
	Dynamic_Text project_tins;
	Get_project_tins(project_tins);
	Dynamic_Text tins,super_tins;
	Integer items;
	Get_number_of_items(project_tins,items);
	for(Integer i=1;i<=items;i++){
		Text tin_name;
		Get_item(project_tins,i,tin_name);
		Tin tin = Get_tin(tin_name);
		Text type;
		Get_type(tin,type);
		if(type=="SuperTin") Append(tin_name,super_tins);
	}
	match_name(super_tins,match,tins);
	Null(project_tins);Null(super_tins);
	return tins;
}

Dynamic_Text get_tins_only(Text match){
	Dynamic_Text project_tins;
	Get_project_tins(project_tins);
	Dynamic_Text tins,normal_tins;
	Integer items;
	Get_number_of_items(project_tins,items);
	for(Integer i=1;i<=items;i++){
		Text tin_name;
		Get_item(project_tins,i,tin_name);
		Tin tin = Get_tin(tin_name);
		Text type;
		Get_type(tin,type);
		if(type=="Tin") Append(tin_name,normal_tins);
	}
	match_name(normal_tins,match,tins);
	Null(project_tins);Null(normal_tins);
	return tins;
}


Real area_3d(Real x1,Real y1,Real z1,Real x2,Real y2,Real z2,Real x3,Real y3,Real z3) {
//get the 3d area of a triangle
	Real t1 = (y2-y1)*(z3-z1)-(z2-z1)*(y3-y1), t2 = (z2-z1)*(x3-x1)-(x2-x1)*(z3-z1), t3 = (x2-x1)*(y3-y1)-(y2-y1)*(x3-x1);
	return 0.5*Sqrt(t1*t1 + t2*t2 + t3*t3);
}

Real area_2d(Real x1, Real y1, Real x2, Real y2, Real x3, Real y3){
	return Absolute( ( (x2 - x1) * (y3 - y1) ) - ( (x3 - x1) * (y2 - y1) ) ) / 2.0;
	//return Absolute( (x1 * (y2 - y3)) + (x2 * (y3 - y1)) + (x3 * (y1 - y2)) ) / 2.0;
}

Real get_area(Tin &tin, Integer triangle){
	Integer point[3], n[3];
	Real x[3], y[3], z[3];
	Integer ierr =Tin_get_triangle(tin, triangle, point[1], point[2], point[3], n[1], n[2], n[3], x[1], y[1], z[1], x[2], y[2], z[2], x[3], y[3], z[3]);
	return area_2d(x[1],y[1],x[2],y[2],x[3],y[3]);
}

Integer tin_get_centroid(Tin &tin, Integer triangle, Real &xf, Real &yf, Real &zf){
// find the centroid of a triangle
	Integer point[3], n[3];
	Real x[3], y[3], z[3];
	Integer ierr =Tin_get_triangle(tin, triangle, point[1], point[2], point[3], n[1], n[2], n[3], x[1], y[1], z[1], x[2], y[2], z[2], x[3], y[3], z[3]);
	xf = (x[1] + x[2] + x[3]) / 3;
	yf = (y[1] + y[2] + y[3]) / 3;
	zf = (z[1] + z[2] + z[3]) / 3;
	return ierr;
}

Integer tin_get_normal_vector(Tin &tin, Integer triangle, Real &nx, Real &ny, Real &nz){
	Integer point[3], n[3];
	Real x[3], y[3], z[3];
	Integer ierr =Tin_get_triangle(tin, triangle, point[1], point[2], point[3], n[1], n[2], n[3], x[1], y[1], z[1], x[2], y[2], z[2], x[3], y[3], z[3]);
	//This is the normal vector, nx,ny,nz
	nx = (y[2] - y[1])*(z[3] - z[1]) - (y[3] - y[1])*(z[2] - z[1]);
	ny = (z[2] - z[1])*(x[3] - x[1]) - (z[3] - z[1])*(x[2] - x[1]);
	nz = (x[2] - x[1])*(y[3] - y[1]) - (x[3] - x[1])*(y[2] - y[1]);
	if(nz < 0.0){ // the vector must point up
		nx*=(-1.0);
		ny*=(-1.0);
		nz*=(-1.0);
	}

	return(ierr);
}

Integer triangle_get_normal_vector(Real x[], Real y[], Real z[], Integer npts_is_3, Real &nx, Real &ny, Real &nz){
	if(npts_is_3 < 3){
		return 1;
	}
	//This is the normal vector, nx,ny,nz
	nx = (y[2] - y[1])*(z[3] - z[1]) - (y[3] - y[1])*(z[2] - z[1]);
	ny = (z[2] - z[1])*(x[3] - x[1]) - (z[3] - z[1])*(x[2] - x[1]);
	nz = (x[2] - x[1])*(y[3] - y[1]) - (x[3] - x[1])*(y[2] - y[1]);
	if(nz < 0.0){ // the vector must point up
		nx*=(-1.0);
		ny*=(-1.0);
		nz*=(-1.0);
	}

	return 0;
}

Integer tin_get_normal_vector(Tin &tin, Integer triangle, Vector3 &vector){
	Integer point[3], n[3];
	Real x[3], y[3], z[3];
	Integer ierr =Tin_get_triangle(tin, triangle, point[1], point[2], point[3], n[1], n[2], n[3], x[1], y[1], z[1], x[2], y[2], z[2], x[3], y[3], z[3]);
	//This is the normal vector, nx,ny,nz
	Real nx = (y[2] - y[1])*(z[3] - z[1]) - (y[3] - y[1])*(z[2] - z[1]);
	Real ny = (z[2] - z[1])*(x[3] - x[1]) - (z[3] - z[1])*(x[2] - x[1]);
	Real nz = (x[2] - x[1])*(y[3] - y[1]) - (x[3] - x[1])*(y[2] - y[1]);
	if(nz < 0.0){ // the vector must point up
		nx*=(-1.0);
		ny*=(-1.0);
		nz*=(-1.0);
	}
	Set_vector(vector,nx,ny,nz);
	return(ierr);
}

Integer tin_get_slope(Tin tin, Integer triangle, Real &slope){ //percent
	Real nx,ny,nz;
	Integer ierr = tin_get_normal_vector(tin, triangle, nx, ny, nz);
	//now calc slope
	slope = Pow( ( Pow(nx,2) + Pow(ny,2) ) , 0.5 ) / nz;
	return ierr;
}


Integer triangle_get_slope(Real x[], Real y[], Real z[], Integer npts_is_3, Real &slope_decimal_percent, Real &slope_1_in_x){ //percent
	if(npts_is_3 < 3){
		return 2;
	}
	Real nx,ny,nz;
	Integer ierr = triangle_get_normal_vector(x,y,z,npts_is_3, nx, ny, nz);

	slope_1_in_x = nz / Sqrt( (nx * nx) + (ny * ny) );
	slope_decimal_percent = 1.0 / slope_1_in_x; //(where 1 == 100%, 0.5 == 50% etc...)

	return ierr;
}

Integer tin_get_slope_aspect(Tin &tin, Integer triangle, Real &slope, Real &aspect){
// todo: more effiecient method in raindrop macro
	Real nx,ny,nz;
	Integer ierr = tin_get_normal_vector(tin, triangle, nx, ny, nz);
	//now calc slope
	slope = Pow( ( Pow(nx,2) + Pow(ny,2) ) , 0.5 ) / nz;
	Real D = Atan(ny/nx);
	//Print(To_text(D,3));Print();
	Real T = D * 57.296;
	if(slope!=0){
		if(nx==0){
			if(ny < 0){
				aspect = 180;
				//aspect = Pi();
			}else{
				aspect = 360;
				//aspect = 2*Pi();
			}
		}else if(nx> 0){
			aspect = 90 - T;
			//aspect = Half_pi() - D;
		}else{
			aspect = 270 - T;
			//aspect = 1.5*Pi() - D;
		}
	}else{
		aspect = -1; //dead flat
	}
	return ierr;
}

Integer is_valid(Tin &tin, Integer triangle){
// if inside ==2 then this is a valid triangle
	Integer inside;
	Tin_get_triangle_inside(tin,triangle,inside);	// 2 is valid and not null(ed)
	if(inside==2)	return 1;
	return 0;
}

Integer tin_get_aspect(Tin tin, Integer triangle, Real &aspect){
	Real x,y,z;
	Integer ierr = tin_get_centroid(tin,triangle,x,y,z);
	return Tin_aspect(tin,x,y,aspect);
}

Real tin_get_slope2(Tin tin, Integer triangle, Real &slope){
	Real x,y,z;
	Integer ierr = tin_get_centroid(tin,triangle,x,y,z);
	return Tin_slope(tin,x,y,slope); //in radians
}

Real get_slope2(Tin &tin, Integer triangle){// this appears to match 12d's get_slope call, however I think it is wrong in some cases!
	Vector3 v1;
	tin_get_normal_vector(tin, triangle, v1);
	return (get_slope(v1));
}

Integer sum_tin_area_and_slope(Tin &tin, Real &sum_tri_area_times_slope, Real &sum_tri_area, Integer &count){ //in radians
	Integer ntri=0;
	Tin_number_of_triangles(tin,ntri);
	count = 0;
	sum_tri_area_times_slope = 0.0;
	sum_tri_area = 0.0;
	for(Integer i=1;i<=ntri;i++){
		Integer inside;
		if(!Tin_get_triangle_inside(tin,i,inside)){
			if(inside == 2){
				Real this_area = get_area(tin,i);
				Real this_slope;
				tin_get_slope( tin, i, this_slope);

				//Print( "slope" + To_text(this_slope,3)+"\n" );
				//Print("slope"+ To_text( tin_get_slope2(tin,i),3)+"\n");
				//sum_tri_area_times_slope += ( (get_slope2(tin,i)) * this_area);
				sum_tri_area_times_slope += ( this_slope * this_area);
				sum_tri_area += this_area;
				count++;
			}

		}
	}
	return 0;
}

Integer calc_tin_average_slope(Tin &tin, Real &average_slope, Real &sum_tri_area){ //
	//Real sum_tri_area_times_slope,sum_tri_area;
	//Integer count;
	//sum_tin_area_and_slope(tin, sum_tri_area_times_slope, sum_tri_area, count);
	Real sum_tri_area_times_slope;//, sum_tri_area;
	Integer count;
	sum_tin_area_and_slope(tin, sum_tri_area_times_slope, sum_tri_area, count);

	p("sum_tri_area_times_slope",sum_tri_area_times_slope);
	p("sum_tri_area",sum_tri_area);
	p("count",count);
	average_slope = sum_tri_area_times_slope / sum_tri_area;
	p("average_slope ",average_slope);
	//average_slope = radians_to_slope_percent(average_slope);
	//p("average_slope 2. ",average_slope);
	return 0;
}

Integer is_triangle_vectors_within_tolerance(Tin tin, Integer t1, Integer t2, Real tolerance){
	Vector3 v1,v2;
	tin_get_normal_vector(tin, t1, v1);
	tin_get_normal_vector(tin, t2, v2);
	Real angle = get_degrees_between_vectors(v1,v2);
	Print("ang diff"+To_text(angle,3));Print();
	if(Absolute(angle) <= tolerance )return(1);
	return (0);
}


Integer tin_triangle_match_2_points(Tin &tin, Integer &triangle, Integer &p1, Integer &p2){
// check the triangle and work out if both of the point numbers match points on the triangle
// do p1 and p2 match any 2 of the points on the triangle
// NOTE! return 1 passes, 0 fails
	Integer pf1,pf2,pf3;
	Tin_get_triangle_points(tin,triangle,pf1,pf2,pf3);
	if(p1 == pf1 || p1 == pf2 || p1 == pf3){
		if(p2 == pf1 || p2 == pf2 || p2 == pf3){
			return 1;//success
		}
	}
	return 0; //fail
}


Integer tin_get_flow_line_from_tin_point(Tin tin, Integer point, Real &xf, Real &yf, Real &zf, Integer &triangle_or_point){
// from a point number, find all adjacent triangles, and search for a flow line
//  the flow line may be at the opposite edge of a triangle, or it may be along a valley
//  if it's on the opposite edge return the triangle number
//  if it;s along a valley return the next tin point number

	Integer max_triangles;
	Tin_get_triangles_about_point(tin,point,max_triangles);

	//Tin_get_triangles_about_point(Tin tin,Integer triangle,Integer max_triangles,Integer &no_triangles,Integer triangles[],Integer points[],Integer status[]);
/*
Integer Tin_get_triangles_about_point(Tin tin,Integer triangle,Integer &no_triangles);
Integer Tin_get_triangles_about_point(Tin tin,Integer triangle,Integer max_triangles,Integer &no_triangles,Integer triangles[],Integer points[],Integer status[]);
*/
	return 0;
}

Integer tin_get_triangle_points(Tin tin,Integer triangle,Integer &pt_nums[]){
// array size should be 3 => pt_nums[3]
	Integer p1,p2,p3;
	return Tin_get_triangle_points( tin, triangle,pt_nums[1],pt_nums[2],pt_nums[3]);
	//Integer ierr = Tin_get_triangle_points( tin, triangle,pt_nums[1],pt_nums[2],pt_nums[3]);
	//pt_nums[1]=p1;pt_nums[2]=p2;pt_nums[3]=p3;
	//return ierr;
}
Integer tin_get_triangle_point_data(Tin tin, Integer triangle,Integer &pt_nums[],Real &pt_x[], Real &pt_y[], Real &pt_z[]){
//get the pt_num x y z for the 3 points of the triangle
	tin_get_triangle_points(tin, triangle,pt_nums); // get the point numbers for this triangle
	for(Integer i = 1 ; i <= 3 ; i++ ){ // for each point number
		Tin_get_point(tin,pt_nums[i],pt_x[i],pt_y[i],pt_z[i]);
	}

	return 0;
}

Integer tin_get_lowest_triangle_point_index(Real z[]){
	// z is size of 3, what is the lowest index, should be 1 , 2 or 3.... 
	Integer lowest_z_idx = 1;
	Real lowest_z = z[1];
	for(Integer i=2;i<=3;i++){
		if(z[i]<lowest_z){
			lowest_z = z[i];
			lowest_z_idx =1;
		}
	}
	return lowest_z_idx;
}

Integer tin_get_triangle(Tin &tin, Integer triangle, Integer pnt[], Real x[], Real y[], Real z[]){
	Integer ierr = 0;
	ierr+=Tin_get_triangle_points(tin,triangle,pnt[1],pnt[2],pnt[3]);
	ierr+=Tin_get_point(tin,pnt[1],x[1],y[1],z[1]);
	ierr+=Tin_get_point(tin,pnt[2],x[2],y[2],z[2]);
	ierr+=Tin_get_point(tin,pnt[3],x[3],y[3],z[3]);
	return ierr;
}

void tin_get_lowest_triangle_point(Tin &tin, Integer triangle, Integer &pf, Real &xf, Real &yf, Real &zf){
	// pf is the point index, xf,yf,zf is the coords of the lowest point
	Real x[3],y[3],z[3];
	Integer pnt[3];
	tin_get_triangle(tin, triangle, pnt, x, y, z);
	Integer idx = tin_get_lowest_triangle_point_index(z);
	pf = pnt[idx];
	xf = x[idx];
	yf = y[idx];
	zf = z[idx];
}

Integer tin_get_triangle_lowest_point(Tin tin, Integer triangle){
// return the lowest point number on this triangle
// -ve means there are flat parts of the triangle
// -1 = pt1 to pt2;  -2 = pt2 to pt3;  -3 = pt3 to pt1;  <-3 flat (because it's subtracting all the eq sides)

	Integer pt_nums[3];
	Real pt_x[3], pt_y[3], pt_z[3];

	tin_get_triangle_point_data(tin, triangle, pt_nums,pt_x, pt_y, pt_z);
	Real z;
	Integer pnt;
	Integer ret_val = 0;
	for(Integer i = 1 ; i <= 3 ; i++ ){ // for each point number
		if(i == 1){
			z = pt_z[i];
			pnt = pt_nums[i];
		}else{
			if(pt_z[i] < z){
				z = pt_z[i];
				pnt = pt_nums[i];
			}else if(pt_z[i] == z){
				ret_val -= i;
			}

		}
	}
	if(ret_val != 0)return ret_val;
	return pnt;
}

Integer tin_get_triangle_lowest_point_next_steepest_point(Tin tin, Integer triangle, Integer mode, Integer &pnt){
//find the lowest point on the triangle, then go through all the non nulled points and find the steepest grade
// mode 1 is highest grade (up)
// mode -1 is lowest grade (down)
// neg return value is bad
// be careful
// + the flattest (down grade) could possibly be a +ve grade
// + multiple grades could be the same (2 grades on 1 triangle may be eq) (if more than 2 grades are eq than the are flat)
	Integer lowest_point = tin_get_triangle_lowest_point( tin,  triangle);
	if(lowest_point < 0)return lowest_point;

	Integer no_triangles;
	Tin_get_triangles_about_point(tin,lowest_point,no_triangles);  //number of triangles about this point
	//print("number of triangles about point <"+To_text(tri_pts[i]) + "> : <" + To_text(no_triangles)+">");

	Integer max_triangles = no_triangles; //number of triangles = the maximum number of triangle
	Integer triangles[max_triangles],points[max_triangles],status[max_triangles];
	Integer ierr = Tin_get_triangles_about_point( tin, lowest_point, max_triangles, no_triangles, triangles, points, status); //get some info about all the triangles from that point// also get some info about the next point along the found triangle
	Real grades[max_triangles],lengths[max_triangles];

	Real x,y,z,grade;
	Tin_get_point(tin,lowest_point,x,y,z);

	for(Integer j = 1 ; j <= max_triangles ; j++){
		if(status[j] == 1)continue;//null point, skip it
		Real x2,y2,z2;
		lengths[j] = get_distance( x, y, x2, y2);
		grades[j]  = get_grade(lengths[j],z,z2);
		if(j == 1){
			grade = grades[j];
			pnt = points[j];
		}else{
			if(mode < 0){
				if(grades[j] < grade){
					grade = grades[j];
					pnt = points[j];
				}
			}else{
				if(grades[j] > grade){
					grade = grades[j];
					pnt = points[j];
				}
			}
		}
		//print(To_text(j) + " Trianlges <" + To_text(triangles[j]) + "> Points <" + To_text(points[j]) + "> Status <"+ To_text(status[j])+ ">");
	}
	return 0;
}

Integer tin_get_triangles_match_2_points(Tin tin, Integer match1, Integer match2, Integer triangle_match[]){
// return the count# of triangles which match
	Integer no_triangles;
	Tin_get_triangles_about_point(tin,match1,no_triangles);  //get all the triangle numbers which match point number match1

	Integer max_triangles = no_triangles; //number of triangles = the maximum number of triangle
	Integer triangles[max_triangles],points[max_triangles],status[max_triangles];
	Integer ierr = Tin_get_triangles_about_point( tin, match1, max_triangles, no_triangles, triangles, points, status); //get some info about all the triangles from that point// also get some info about the next point along the found triangle

	Integer num_trianlges = 0;
	for(Integer i = 1 ; i <= max_triangles ; i++){
		if(tin_triangle_match_2_points(tin, triangles[i], match1, match2)){
			num_trianlges++;
			triangle_match[num_trianlges] = triangles[i];
		}
	}
	return num_trianlges;
}

Integer is_valley(Tin tin, Integer tr1, Integer tr2){
// 2 triangles from a valley if the direction of tri2 intersects a projection of the common side

return 0;
}

Integer debug_tin(Tin tin , Integer triangle){
	print("DEBUGGING TIN");
	Integer tri_pts[3];
	tin_get_triangle_points(tin, triangle,tri_pts); // get the point numbers for this triangle

	for(Integer i = 1 ; i <= 3 ; i++ ){ // for each point number
		Integer no_triangles;
		Tin_get_triangles_about_point(tin,tri_pts[i],no_triangles);  //number of triangles about this point
		print("number of triangles about point <"+To_text(tri_pts[i]) + "> : <" + To_text(no_triangles)+">");

		Integer max_triangles = no_triangles; //number of triangles = the maximum number of triangle
		Integer triangles[max_triangles],points[max_triangles],status[max_triangles];
		Integer ierr = Tin_get_triangles_about_point( tin, tri_pts[i], max_triangles, no_triangles, triangles, points, status); //get some info about all the triangles from that point// also get some info about the next point along the found triangle
		for(Integer j = 1 ; j <= max_triangles ; j++){
			print(To_text(j) + " Trianlges <" + To_text(triangles[j]) + "> Points <" + To_text(points[j]) + "> Status <"+ To_text(status[j])+ ">");
		}
	}
	print(To_text(lowest_point = tin_get_triangle_lowest_point( tin, triangle)));



	return 0;
}

Integer debug_tin(Tin tin, Real x, Real y){
// get some info about the triangle at this coordt
	Integer triangle;
	if(Tin_get_triangle_from_point(tin, x, y,triangle)	!= 0)return -1;
	debug_tin(tin,triangle);
	return triangle;
}

Integer debug_tin_label_info(Tin tin, Text model_name){
	Integer npts;
	Tin_number_of_points(tin,npts);
	for(Integer i = 1 ; i <= npts ; i++ ){
		Real x, y, z;
		Tin_get_point(tin,i,x,y,z);
		Element elt = create_super_text(x, y, z, To_text(i));
		Calc_extent(elt);
		Set_colour(elt,3);
		Set_model(elt,Get_model_create(model_name));

	}
	Integer ntri;
	Tin_number_of_triangles(tin, ntri);
	for(i = 1 ; i<= ntri ; i ++){
		Integer p1,p2,p3;
		Tin_get_triangle_points(tin,i,p1,p2,p3);
		Real x, y, z;
		tin_get_centroid(tin, i, x, y, z);
		Element elt = create_super_text(x, y, z, To_text(i));
		Calc_extent(elt);
		Set_colour(elt,1);
		Set_model(elt,Get_model_create(model_name));
	}
	return 0;
}

Integer tin_get_triangle_edge_point_at_aspect(Tin tin, Real xc, Real yc, Real &height, Real &xf, Real &yf, Real &zf){
// give an xy pos and work out where the intersection of the aspect is with that triangle
//return the triangle that intersects?
	Real aspect;
	Integer triangle;
	if(Tin_get_triangle_from_point(tin, xc, yc,triangle)	!= 0)	return -1;	// what triange is xc,yc at?
	if(tin_get_aspect(tin,triangle,aspect)			!= 0)			return -2;	// what is it's aspect?

	Bearing_to_angle(aspect,aspect);
	Tin_height(tin,xc,yc,height);	// height at the point coming in

	Real dist = 999.9;
	Point pnt1,pnt2;
	Set_x(pnt1,xc);Set_y(pnt1,yc);
	Locate_point(pnt1,aspect,dist,pnt2);
	Real x2 = Get_x(pnt2);	Real y2 = Get_y(pnt2);

	Integer point[3], n[3];			Real x[3], y[3], z[3];
	Integer ierr =Tin_get_triangle(tin, triangle, point[1], point[2], point[3], n[1], n[2], n[3], x[1], y[1], z[1], x[2], y[2], z[2], x[3], y[3], z[3]);

	Integer side = 0;
	for(Integer i = 1 ; i <= 3 ; i++){
		Real xr,yr;
		Integer ii = i+1;
		if(ii == 4)ii = 1;
		Integer int_type = get_segments_intersection(xc,yc,x2,y2,x[i],y[i],x[ii],y[ii],xr,yr);
		if(int_type == 0){
			side = i;
			xf = xr;	//this is the intersection point with the edge of triangle
			yf = yr;
			Tin_height(tin,xf,yf,zf);
			break;
		}
	}
	if(side == 0){
		//need to run a diferent method
		return -3;
	}

	//now need to find the which edge intersected
	// p1 and p2 are the points on the side of the triangle where the intersection was found
	Integer p1,p2;
	p1 = point[side];
	side++; if(side==4)side=1;
	p2 = point[side];

	Integer neighbour = 0;
	for (i = 1 ; i <= 3 ; i++){
		if(tin_triangle_match_2_points(tin, n[i], p1, p2)){
			return(n[i]);//return the neighbour item number
			break;
		}
	}
	return -999;

}

Integer is_null(Tin &tin){
//check to see if the handle of a tin is null
// 0 not null, non zero is null
	Text tin_name;
	return Get_name(tin,tin_name);
}

Tin prompt_for_project_tin(){
	Tin tin;
	Dynamic_Text project_tins;	Get_project_tins(project_tins);
	Integer items;		Get_number_of_items(project_tins,items);
	if(items < 1){
		Null(tin);
		return tin;
	}
	Integer i_tins=0,i_supertins=0;
	Dynamic_Text tins,supertins;
	for(Integer i = 1 ; i <= items ; i++){
		Text tin_name;	Get_item(project_tins,i,tin_name);
		Tin tinf = Get_tin(tin_name);
		Text type;	Get_type(tinf,type);
		if(type == "Tin"){
			i_tins++;
			Append(tin_name,tins);
		}else{
			i_supertins++;
			Append(tin_name,supertins);
		}
	}
	Null(project_tins);
	if(i_tins > 0){
		Append("-Tins-",project_tins);
		for(i = 1 ; i <= i_tins ; i++){
			Text tin_name;	Get_item(tins,i,tin_name);
			Append(tin_name,project_tins);
		}
	}

	if(i_supertins > 0){
		Append("-Super Tins-",project_tins);
		for(i = 1 ; i <= i_supertins ; i++){
			Text tin_name;	Get_item(supertins,i,tin_name);
			Append(tin_name,project_tins);
		}
	}

	Text tin_name = create_button_menu(" ",project_tins);
	if(tin_name == ""){
		return tin;
	}else{
		tin = Get_tin(tin_name);
	}
	return tin;
}

Element create_element(Dynamic_Real &xdim, Dynamic_Real &ydim){
	Element e;
	Integer items;
	Get_number_of_items(xdim,items);
	if(items<1)return e;
	Real x[items],y[items],z[items],r[items]; Integer f[items];
	for(Integer i=1;i<=items;i++){
		Get_item(xdim,i,x[i]);
		Get_item(ydim,i,y[i]);
		z[i] = 0.0;
		r[i] = 0.0;
		f[i] = 0;
	}
	e = Create_super(1,x,y,z,r,f,items);
	Calc_extent(e);
	return e;
}


Integer get_section(Tin &tin, Real x1, Real y1, Real x2, Real y2, Real start_ch, Element &out){
	Integer size = 2;
	Real x[size],y[size],z[size],r[size];	Integer f[size];
	x[1] = x1;	y[1] = y1;	z[1] = 0.0;	r[1] = 0.0;	f[1] = 0;
	x[2] = x2;	y[2] = y2;	z[2] = 0.0;	r[2] = 0.0;	f[2] = 0;
	Integer flag = Att_ZCoord_Value;

	Element e = Create_super(flag,x,y,z,r,f,size);	Calc_extent(e);
	Set_chainage(e,start_ch);

	Dynamic_Element result;
	Drape(tin,e,result);

	Integer colour;
	Get_colour(tin,colour);

	Integer items;
	Get_number_of_items(result,items);

	Dynamic_Real chainages,heights;
	Integer count=0;

	for(Integer i=1;i<=items;i++){
		Element elt;
		Get_item(result,i,elt);
		Integer points;
		Get_points(elt,points);
		for(Integer j=1;j<=points;j++){
			Real xd,yd,zd;
			Get_data(elt,j,xd,yd,zd);
			count++;
			Set_item(heights,count,zd);
			Real xf,yf,zf,chf,dirf,off;
			Drop_point(e,xd,yd,zd,xf,yf,zf,chf,dirf,off);
			Set_item(chainages,count,chf);
		}
	}
	out = create_element(chainages,heights);
	Set_colour(out,colour);

	Element_delete(e);
	elements_delete_slow_method(result);

	return 0;
}



// This will null triangles outside the list of elements
/*Say you've got 10 polys and want do an inclusive outside null (n=10)

for n=1 null outside
for n >= 2 reset inside
*/
Integer null_triangles_outside(Tin tin, Dynamic_Element &delt){
	Integer ok = 0;
	Integer items;
	Get_number_of_items(delt,items);
	for(Integer i=1;i<=items;i++){
		Element e;
		Get_item(delt,i,e);
		if(i==1){
			ok+=Null_triangles(tin,e,1);		//null outside 1st poly
		}else{
			ok+=Reset_null_triangles(tin,e,0);	//reset inside remaining polys
		}
	}
	return ok;
}


Integer get_tin_colour(Tin &tin, Integer &colour){
	Integer ntri;
	Tin_number_of_triangles(tin,ntri);
	for(Integer i=1;i<=ntri;i++){
		Integer this_colour;
		Tin_get_triangle_colour(tin,i,this_colour);
		if(this_colour == 0){
			Real xf,yf,zf;
			tin_get_centroid(tin, i, xf, yf, zf);

			if(!Tin_colour(tin,xf,yf,this_colour)){
				if(this_colour != 0){
					colour = this_colour;
					return 0;
				}
			}

		}
	}
	return 1;
}

#endif