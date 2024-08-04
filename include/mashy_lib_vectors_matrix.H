/*  - 12d Model Macro Manual, Page 42 Assignment and Operators -
2.6.3 Binary Arithmetic Operators for Vectors and Matrices
The binary arithmetic operators for vectors and matrices are
+          addition
-          subtraction
*          multiplication of matrices
*          dot product of vectors
^          cross product of two vectors
where the following combinations are allowed

Vector2 + Vector2 = Vector2         Vector2 - Vector2 = Vector2
Vector3 + Vector3 = Vector3         Vector3 - Vector3 = Vector3
Vector4 + Vector4 = Vector4         Vector4 - Vector4 = Vector4
Real * Vector2 = Vector2      Vector2 * Real = Vector2             Vector2 / Real= Vector2
Real * Vector3 = Vector3      Vector3 * Real = Vector3             Vector3 / Real= Vector2
Real * Vector4 = Vector4      Vector4 * Real = Vector4             Vector4 / Real= Vector4
Vector2 * Vector2 = Real               * is the dot product between the two vectors
Vector3 * Vector3 = Real               * is the dot product between the two vectors
Vector4 * Vector4 = Real               * is the dot product between the two vectors
Vector2 ^ Vector2 = Vector3        ^ is the cross product between the two Vector2 vectors
                                                     Note: to form this cross product, the Vector2’s are turned into 
                                                     Vector3’s by adding the third dimension with value 0.
Vector3 ^ Vector3 = Vector3         ^ is the cross product between the two Vector3 vectors
Matrix3 + Matrix3 = Matrix3         Matrix3 - Matrix3 = Matrix3         Matrix3 * Matrix3 = Matrix3
Matrix4 + Matrix4 = Matrix4         Matrix4 - Matrix4 = Matrix4         Matrix4 * Matrix4 = Matrix4
Real * Matrix3 = Matrix3       Matrix3 * Real = Matrix3              Matrix3 / Real= Matrix3
Real * Matrix4 = Matrix4       Matrix4 * Real = Matrix4              Matrix4 / Real= Matrix4
Vector3 * Matrix3 = Vector3           Note that the Vector3 is treated as a row vector.
Matrix3 * Vector3 = Vector3           Note that the Vector3 is treated as a column vector.
Vector4 * Matrix4 = Vector4           Note that the Vector4 is treated as a row vector.
Matrix4 * Vector4 = Vector4           Note that the Vector4 is treated as a column vector.
A vector of dimension 2, 3 or 4 can be cast to a vector of a higher or a lower dimension.
If casting to a dimension of one higher, the new component is set by default to 1.0.
For example a Vector2 represented by (x,y) is cast to a Vector3 (x,y,1).
When casting to a dimension of one lower, the vector is homogenized and the last component 
(which has the value 1) is dropped. 
For example, a Vector4 represented by (x,y,z,w) is cast to a Vector3 as (x/w,y/w,z/w).
So for example
Vector2 * Matrix3 = Vector3             requires Vector2 say (x,y) to be cast to a Vector3 so that this
                                                         make sense and the operation is defined as (x,y,1)*Matrix3
*/

#ifndef mashy_lib_vectors_matrix_H
#define mashy_lib_vectors_matrix_H

void matmul(Vector4 v[], Integer size, Matrix4 m){
    for(Integer i=1;i<=size;i++){   v[i] = m * v[i];    }   // matrix * v
}

void matmul(Vector3 v[], Integer size, Matrix4 m){
    for(Integer i=1;i<=size;i++){   v[i] = m * v[i];    }   // matrix * v
}

void scale_vector(Vector4 v[], Integer size, Real scaler){
    Vector3 v3; Set_vector(v3,scaler,scaler,scaler);
    Matrix4 m_scale = Get_scaling_matrix(v3);
    matmul( v, size, m_scale);
}

void translate_vector(Vector4 v[], Integer size, Real x, Real y, Real z){
    Vector3 v3; Set_vector(v3,x,y,z);
    Matrix4 m_tran = Get_translation_matrix(v3);
    matmul( v, size, m_tran);
}

void rotate_vector(Vector4 v[], Integer size, Real x, Real y, Real z, Real angle){// TODO - does 12d normalise the axis vector? - tested and yes it does.
    Vector3 v3; Set_vector(v3,x,y,z);
    Matrix4 m_rot = Get_rotation_matrix(v3,angle);
    matmul( v,size,m_rot);
}

void perspective_vector(Vector4 v[], Integer size, Real d){
    Matrix4 m_per = Get_perspective_matrix(d);
    matmul( v,size,m_per);
}

Matrix4 get_projection_matrix(Real angle_of_view,  Real near,  Real far ) { 
    Matrix4 m;  Set_matrix_zero(m);
    // set the basic projection matrix
    Real scale = 1.0 / Tan(angle_of_view * 0.5 * Pi() / 180.0); 
    Set_matrix(m, 1, 1, scale );  // scale the x coordinates of the projected point 
    Set_matrix(m, 2, 2, scale );  // scale the y coordinates of the projected point 
    Set_matrix(m, 3, 3, (-far) / (far - near) );  // used to remap z to [0,1] 
    Set_matrix(m, 4, 3, (-far) * near / (far - near) );  // used to remap z [0,1] 
    Set_matrix(m, 3, 4, -1.0);  // set w = -z 
    Set_matrix(m, 4, 4, 0.0); 
    return m;
} 



#ifndef V_FMT
#define V_FMT "%11.2f"  // printf fmt helper ->   https://alvinalexander.com/programming/printf-format-cheat-sheet/
#endif

Text to_text(Vector2 v){
    Real x,y;
    Get_vector(v, x, y);
    return To_text(x,V_FMT) + "," + To_text(y,V_FMT);
}


Text to_text(Vector3 v){
    Real x,y,z;
    Get_vector(v, x, y, z);
    return To_text(x,V_FMT) + "," + To_text(y,V_FMT) + "," + To_text(z,V_FMT);
}

Text to_text(Vector4 v){
    Real x,y,z,w;
    Get_vector(v, x, y, z, w);
    return To_text(x,V_FMT) + "," + To_text(y,V_FMT) + "," + To_text(z,V_FMT) + "," + To_text(w,V_FMT);
}

void print(Vector2 v){  Print(to_text(v)+"\n"); }
void print(Vector3 v){  Print(to_text(v)+"\n"); }
void print(Vector4 v){  Print(to_text(v)+"\n"); }

void print(Vector4 v[], Integer s){
    Text t;
    for(Integer i=1;i<=s;i++){
        t+= To_text(i) + ": " + to_text(v[i]) + "\n";
    }
    Print("\n"+t);
}

void print(Matrix4 m){
    Text t;
    for(Integer i=1;i<=4;i++){
        Vector4 v;
        Get_matrix_row(m,i,v);
        t+= to_text(v)+"\n";
    }
    Print(t);
}

void print(Matrix3 m){
    Text t;
    for(Integer i=1;i<=3;i++){
        Vector3 v;
        Get_matrix_row(m,i,v);
        t+= to_text(v)+"\n";
    }
    Print(t);
}

void test_mashy_lib_vectors_matrix(){

    Clear_console();

    Vector4 v4;
    Set_vector(v4, 1.0,1.0,1.0,1.0); // original coords, x,y,z,1

    Vector3 v3;
    Set_vector(v3, 5.0,10.0,-5.0); // translation vector... dx,dy,dz
    Matrix4 t_m4 = Get_translation_matrix(v3);  // get the matrix
    Print("Position\n");
    print(v4);
    Print("translation_vector\n");
    print(v3);
    Print("translation_matrix\n");
    print(t_m4);
    v4 = t_m4 * v4;   // this is how to translate. Matrix * vector
    Print("Position after translation\n");
    print(v4);
    Print();

    Set_vector(v4, 1.0,11.0,1.0,1.0); // original coords, x,y,z,1 // RESET
    // Vector3 s_v3;
    Set_vector(v3,5.0,2.0,2.0);
    Matrix4 s_m4 = Get_scaling_matrix(v3);  // get the matrix

    Print("Position\n");
    print(v4);
    Print("scaling_vector\n");
    print(v3);
    Print("scaling_matrix\n");
    print(s_m4);
     v4 = s_m4 * v4; // this is how to scale. Matrix * vector
    Print("Position after scale\n");
    print(v4);

    Set_vector(v4, 1.0,1.0,1.0,1.0); // original coords, x,y,z,1 // RESET

    Vector4 a[1];   a[1]    = v4;
    Print();Print("Translate\n");
    translate_vector(a,1,1,2,3);
    print(a[1]);
    Print();Print("Scale\n");
    a[1] = v4;
    scale_vector(a,1,50.0);
    print(a[1]);
    Print();Print("Rotate 1\n");
    a[1] = v4;
    rotate_vector(a,1,0,0,1,Pi());
    print(a[1]);
    Print();Print("Rotate 2\n");
    a[1] = v4;
    rotate_vector(a,1,10,10,10,Pi());
    print(a[1]);
    Print();Print("Rotate 3\n");
    a[1] = v4;
    rotate_vector(a,1,1,1,1,Pi());
    print(a[1]);
    Print();Print("Rotate 4\n");
    a[1] = v4;
    rotate_vector(a,1,10,0,-10,Pi());
    print(a[1]);

    Print();Print("Persepective\n");
    // a[1] = v4;
    perspective_vector(a,1,50);
    print(a[1]);

    Print();Print("Persepective\n");
    a[1] = v4;
    perspective_vector(a,1,50);
    print(a[1]);

    Matrix4 p_m4 = get_projection_matrix(45,1,10);
    Print();Print("Projection matrix\n");
    print(p_m4);

    p_m4 = get_projection_matrix(90,1,10);
    Print();Print("Projection matrix\n");
    print(p_m4);

    p_m4 = get_projection_matrix(180,1,10);
    Print();Print("Projection matrix\n");
    print(p_m4);

    p_m4 = get_projection_matrix(90,1,2);
    Print();Print("Projection matrix\n");
    print(p_m4);

    p_m4 = get_projection_matrix(90,100,200);
    Print();Print("Projection matrix\n");
    print(p_m4);

    p_m4 = get_projection_matrix(90,-100,200);
    Print();Print("Projection matrix\n");
    print(p_m4);

p_m4 = get_projection_matrix(90,100,-200);
    Print();Print("Projection matrix\n");
    print(p_m4);

    // casting - cant cast 2 levels
    // Vector4 cv4; Set_vector(cv4,10,10,10,1);
    // Vector2 cv2;
    // cv2 = cv4;
    // print(cv4);
    // print(cv2);

}

#endif