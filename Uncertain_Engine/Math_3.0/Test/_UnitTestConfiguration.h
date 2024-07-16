//-----------------------------------------------------------------------------
// Copyright 2022, Ed Keenan, all rights reserved.
//-----------------------------------------------------------------------------  

#ifndef UNIT_TEST_CONFIGURATION_H
#define UNIT_TEST_CONFIGURATION_H

#define eq_float(A,B)	Util::isEqual(A,B, MATH_TOLERANCE) 

#define QTEST(A,B) { bool Quat_Check = (eq_float(A[x], B[x]) && \
                           eq_float(A[y], B[y]) && \
                           eq_float(A[z], B[z]) && \
                           eq_float(A[w], B[w])) || \
				          (eq_float(-A[x], B[x]) && \
                           eq_float(-A[y], B[y]) && \
                           eq_float(-A[z], B[z]) && \
                           eq_float(-A[w], B[w])) ; \
                     CHECK( Quat_Check == true ); }

// Make sure all are enabled for final submission...
//      I grade whatever is set in this file - Keenan

#define QuatApp_SlerpDataArray                  true
#define QuatApp_SlerpDataArray0                 true
#define QuatApp_SlerpDataArray1                 true
#define QuatApp_SlerpData                       true

#define Vec3App_Lerp                            true
#define Vec3App_Lerp2                           true
#define Vec3App_Lerp3                           true
#define Vec3App_Lerp4                           true

#define Vec3App_LERP_Array                      true
#define Vec3App_LERP_Array0                     true
#define Vec3App_LERP_Array1                     true

#define Combo_Junkfood                          true                                   
#define Combo_VectCrossAddSub                   true                            
#define Combo_VectCrossAddSubMatrixMult         true                  
#define Combo_VectCrossAddSubMatrixMultCross    true             
#define Combo_VectCrossCrossNorm                true                         
#define Combo_VectLine                          true      

#define Mat3_Accessor_Get                       true                                
#define Mat3_Accessor_Get_Const                 true                          
#define Mat3_Accessor_Set                       true                                
#define Mat3_AddEqual_Mat3                      true                             
#define Mat3_Add_Mat3                           true                                  
#define Mat3_AssignmentOperator                 true                          
#define Mat3_Bracket_Get                        true                                 
#define Mat3_Bracket_Get_Const                  true                           
#define Mat3_Bracket_Set                        true                                 
#define Mat3_Copy_Constructor                   true                            
#define Mat3_Default_Constructor                true                         
#define Mat3_Destructor_Constuctor              true                       
#define Mat3_Det                                true  
#define Mat3_GetInverse                         true                                  
#define Mat3_GetTranspose                       true                                
#define Mat3_GET_ROW_0                          true                                   
#define Mat3_GET_ROW_1                          true                                   
#define Mat3_GET_ROW_2                          true                                                                    
#define Mat3_Inverse                            true                                     
#define Mat3_isEqual_false                      true                               
#define Mat3_MultEqual_Mat3                     true                              
#define Mat3_Mult_Mat3                          true                          
#define Mat3_Mult_Scale                         true                                
#define Mat3_Scale_Equals                       true                                
#define Mat3_Scale_Mat3                         true                                
#define Mat3_SET_Identity                       true                                                            
#define Mat3_SET_ROW_0                          true                                   
#define Mat3_SET_ROW_1                          true                                   
#define Mat3_SET_ROW_2                          true                                                                                                 
#define Mat3_SET_Vectors                        true                                 
#define Mat3_SET_Zero                           true
#define Mat3_Special_isEqual_false2             true                      
#define Mat3_Special_isEqual_true               true                        
#define Mat3_Special_isIdentity_false           true                    
#define Mat3_Special_isIdentity_true            true                     
#define Mat3_Special_Matrix_Identity            true                                       
#define Mat3_Special_Matrix_Zero                true                                                        
#define Mat3_SubEqual_Mat3                      true                             
#define Mat3_Sub_Mat3                           true                                  
#define Mat3_Transpose                          true                                                                      
#define Mat3_UnaryMinus                         true                                
#define Mat3_UnaryPlus                          true                                 
#define Mat3_Unary_equal_Minus                  true                         
#define Mat3_Unary_equal_Plus                   true   
#define Mat3_Vector_Constructor                 true   

#define Mat4_Accessor_Get                       true                                
#define Mat4_Accessor_Get_Const                 true                          
#define Mat4_Accessor_Set                       true                                
#define Mat4_AddEqual_Mat4                      true                             
#define Mat4_Add_Mat4                           true                                  
#define Mat4_AssignmentOperator                 true                          
#define Mat4_Bracket_Get                        true                                 
#define Mat4_Bracket_Get_Const                  true                           
#define Mat4_Bracket_Set                        true                                 
#define Mat4_Copy_Constructor                   true                            
#define Mat4_Default_Constructor                true                         
#define Mat4_Destructor_Constuctor              true                       
#define Mat4_Det                                true                                         
#define Mat4_GetInverse                         true                                  
#define Mat4_GetTranspose                       true                                
#define Mat4_GET_ROW_0                          true                                   
#define Mat4_GET_ROW_1                          true                                   
#define Mat4_GET_ROW_2                          true                                   
#define Mat4_GET_ROW_3                          true                                   
#define Mat4_Inverse                            true                                     
#define Mat4_isEqual_false                      true                                                           
#define Mat4_MultEqual_Mat4                     true     
#define Mat4_MultEqual_Quat                     true     
#define Mat4_Mult_Mat4                          true     
#define Mat4_Mult_Quat                          true  
#define Mat4_Mult_Scale                         true                                
#define Mat4_RotAxisAngle                       true                                
#define Mat4_RotInverseOrient                   true                            
#define Mat4_RotOrient                          true                                   
#define Mat4_Scale_Equals                       true                                
#define Mat4_Scale_Mat4                         true                                
#define Mat4_SET_Identity                       true                                
#define Mat4_SET_MatrixRotAxisAngle             true                      
#define Mat4_SET_MatrixRotInverseOrient         true                  
#define Mat4_SET_MatrixRotOrient                true                         
#define Mat4_SET_RotX                           true                                    
#define Mat4_SET_RotXYZ                         true                                  
#define Mat4_SET_RotY                           true                                    
#define Mat4_SET_RotZ                           true                                    
#define Mat4_SET_ROW_0                          true                                   
#define Mat4_SET_ROW_1                          true                                   
#define Mat4_SET_ROW_2                          true                                   
#define Mat4_SET_ROW_3                          true                                   
#define Mat4_SET_SCALE                          true                                   
#define Mat4_SET_SCALE_Vect                     true                              
#define Mat4_SET_Trans                          true                                   
#define Mat4_SET_Trans_Vect                     true                              
#define Mat4_SET_Vectors                        true                                 
#define Mat4_SET_Zero                           true                                    
#define Mat4_Special_isEqual_false2             true                      
#define Mat4_Special_isEqual_true               true                        
#define Mat4_Special_isIdentity_false           true                    
#define Mat4_Special_isIdentity_true            true                     
#define Mat4_Special_Matrix_Identity            true                     
#define Mat4_Special_Matrix_SCALE               true                        
#define Mat4_Special_Matrix_SCALE_Vect          true                   
#define Mat4_Special_Matrix_Trans               true                        
#define Mat4_Special_Matrix_Trans_Vect          true                   
#define Mat4_Special_Matrix_Zero                true                         
#define Mat4_Special_RotX                       true                                
#define Mat4_Special_RotXYZ_Constructor         true                  
#define Mat4_Special_RotY                       true                                
#define Mat4_Special_RotZ                       true                                
#define Mat4_SubEqual_Mat4                      true                             
#define Mat4_Sub_Mat4                           true                                  
#define Mat4_Transpose                          true                                   
#define Mat4_UnaryMinus                         true                                
#define Mat4_UnaryPlus                          true                                 
#define Mat4_Unary_equal_Minus                  true                         
#define Mat4_Unary_equal_Plus                   true   
#define Mat4_Vector_Constructor                 true    

#define SpeedTestA                              true                                      
#define SpeedTestB                              true                                      
#define SpeedTestC                              true                                      
#define SpeedTestD                              true                                      
#define SpeedTestE                              true                                      
#define SpeedTestF                              true                                      
#define SpeedTestG                              true                                     
#define SpeedTestH                              true   
									            
#define Trig_ACos                               true                                        
#define Trig_ASin                               true                                        
#define Trig_ATan                               true                                        
#define Trig_ATan2                              true                                       
#define Trig_Cos                                true                                         
#define Trig_CosSin                             true                                      
#define Trig_InvSqrt                            true                                     
#define Trig_Sin                                true                                         
#define Trig_Sqrt                               true                                        
#define Trig_Tan                                true    

#define Quat_Get_Accessors                      true
#define Quat_Get_Accessors_const                true
#define Quat_Set_Accessors                      true
#define Quat_Set_Accessors_get                  true
#define Quat_AddSubMultDiv                       true  //
#define Quat_PlusOperator                        true
#define Quat_QuatPlusQuat                       true
#define Quat_QuatPlusEqualQuat                  true
#define Quat_QuatPlusConstant                   true
#define Quat_QuatPlusEqualConstant              true
#define Quat_ConstantPlusQuat                   true
#define Quat_ConstantPlusQuatPlusConstant       true
#define Quat_QuatPlusConstPlusQuat              true
#define Quat_QuatMinusQuat                      true
#define Quat_QuatMinusEqualQuat                 true
#define Quat_QuatMinusConstant                  true
#define Quat_QuatMinusEqualConst                true
#define Quat_ConstantMinusQuat                  true
#define Quat_ConstantMinusQuatMinusConstant     true
#define Quat_QuatMinusConstantMinusQuat         true
#define Quat_MinusOperator                      true
#define Quat_QuatTimesQuat                      true
#define Quat_QuatTimeEqualQuat                  true
#define Quat_QuatTimeConst                      true
#define Quat_QuatTimesEqualConst                true
#define Quat_ConstTimesQuat                     true
#define Quat_QuatTimesMatrix                    true
#define Quat_MatrixTimesQuat                    true
#define Quat_QuatTimesEqualMatrix               true

#define Quat_QuatDivQuat                        true
#define Quat_QuatDivEqualQuat                   true
#define Quat_QuatDivConst                       true
#define Quat_QuatDivEqualConst                  true
#define Quat_ConstDivQuat                       true
#define Quat_EqualOperator                      true
#define Quat_vectTimesQuat                      true
#define Quat_vectTimesEqualQuat                 true
#define Quat_vectTimesQuat2                     true
#define Quat_vectTimesEqualQuat2                true
#define Quat_Boolean                             true
#define Quat_IsEqual                             true  //
#define Quat_IsEqual1                            true  //
#define Quat_IsNegEqual                          true  //
#define Quat_IsNegEqual2                         true  //
#define Quat_IsEquivalent                        true
#define Quat_IsEquivalent2                       true
#define Quat_IsEquivalent3                       true
#define Quat_IsConjugateEqual                    true
#define Quat_IsConjugateEqual2                   true
#define Quat_IsIdentity                          true
#define Quat_IsIdentity2                         true
#define Quat_IsNormalized                        true
#define Quat_IsNormalized2                       true
#define Quat_IsZero                              true
#define Quat_IsZero2                             true
#define Quat_Constructor                         true
#define Quat_Default_Constructor                true
#define Quat_Constructor_Set_Every_Component    true
#define Quat_Copy_Constructor                   true
#define Quat_Vect_Comp_Real_Comp_Constructor    true
#define Quat_From_Matrix_Constructor            true
#define Quat_IDENTITY_Constructor               true
#define Quat_ROTXYZ_Constructor                 true
#define Quat_SET_ROTXYZ_Constructor             true
#define Quat_X_PI2_Constructor                  true
#define Quat_X_PI_Constructor                   true
#define Quat_Y_PI2_Constructor                  true
#define Quat_Y_PI_Constructor                   true
#define Quat_Z_PI2_Constructor                  true
#define Quat_Z_PI_Constructor                   true
#define Quat_X_Constructor                      true
#define Quat_Y_Constructor                      true
#define Quat_Z_Constructor                      true
#define Quat_XY_Constructor                     true
#define Quat_XZ_Constructor                     true
#define Quat_YX_Constructor                     true
#define Quat_YZ_Constructor                     true
#define Quat_ZX_Constructor                     true
#define Quat_ZY_Constructor                     true
#define Quat_XYZ_Constructor                    true
#define Quat_XZY_Constructor                    true
#define Quat_YXZ_Constructor                    true
#define Quat_YZX_Constructor                    true
#define Quat_ZXY_Constructor                    true
#define Quat_ZYX_Constructor                    true
#define Quat_AXIS_ANGLE_Constructor             true
#define Quat_ROT_ORIENT_Constructor             true
#define Quat_ROT_INVERSE_ORIENT_Constructor     true
#define Quat_Destructor                         true
#define Quat_Functions                           true
#define Quat_GetConjugate                        true
#define Quat_ConjugateInPlace                    true
#define Quat_ConjugateByValue                    true
#define Quat_TransposeToDest                     true
#define Quat_TransposeByValue                    true
#define Quat_TransposeInplace                    true
#define Quat_Mag                                 true
#define Quat_MagSquared                          true
#define Quat_InvMag                              true
#define Quat_Norm                                true
#define Quat_GetNorm                             true
#define Quat_GetInverse                          true
#define Quat_Inverse                             true
#define Quat_Inverse_3                           true
#define Quat_Inverse_Transpose                   true
#define Quat_Lqcvq_Lqvqc_1                       true
#define Quat_Dot                                 true
#define Quat_Assignment_Matrix                   true
#define Quat_Proxy                               true
#define Quat_BracketX_get                        true
#define Quat_BracketX_assign                     true
#define Quat_BracketX_plusEqual                  true
#define Quat_BracketX_negEqual                   true
#define Quat_BracketX_multEqual                  true
#define Quat_BracketX_divEqual                   true
#define Quat_BracketY_get                        true
#define Quat_BracketY_assign                     true
#define Quat_BracketY_plusEqual                  true
#define Quat_BracketY_negEqual                   true
#define Quat_BracketY_multEqual                  true
#define Quat_BracketY_divEqual                   true
#define Quat_BracketZ_get                        true
#define Quat_BracketZ_assign                     true
#define Quat_BracketZ_plusEqual                  true
#define Quat_BracketZ_negEqual                   true
#define Quat_BracketZ_multEqual                  true
#define Quat_BracketZ_divEqual                   true
#define Quat_BracketW_get                        true
#define Quat_BracketW_assign                     true
#define Quat_BracketW_plusEqual                  true
#define Quat_BracketW_negEqual                   true
#define Quat_BracketW_multEqual                  true
#define Quat_BracketW_divEqual                   true

#define Quat_BracketX_assignment_ConstQuat_Bracket       true
#define Quat_BracketY_assignment_ConstQuat_Bracket       true
#define Quat_BracketZ_assignment_ConstQuat_Bracket       true
#define Quat_BracketW_assignment_ConstQuat_Bracket       true
#define Quat_BracketX_assignment_Quat_Bracket            true
#define Quat_BracketY_assignment_Quat_Bracket            true
#define Quat_BracketZ_assignment_Quat_Bracket            true
#define Quat_BracketW_assignment_Quat_Bracket            true
#define Quat_BracketW_assignment_Quat_Bracket_uninit     true
#define Quat_BracketX_Assignment_Pointers               true
#define Quat_BracketY_Assignment_Pointers               true
#define Quat_BracketZ_Assignment_Pointers               true
#define Quat_BracketW_Assignment_Pointers               true
#define Quat_BracketX_Assignment_Pointers_Const         true
#define Quat_BracketY_Assignment_Pointers_Const         true
#define Quat_BracketZ_Assignment_Pointers_Const         true
#define Quat_BracketW_Assignment_Pointers_Const         true
#define Quat_BracketOperator_plus                       true
#define Quat_BracketOperator_plus_const                 true
#define Quat_BracketOperator_minus_const                true
#define Quat_BracketOperator_minus                      true
#define Quat_BracketX_Assignment_Operator_plus_Const    true
#define Quat_BracketY_Assignment_Operator_plus_Const    true
#define Quat_BracketZ_Assignment_Operator_plus_Const    true
#define Quat_BracketW_Assignment_Operator_plus_Const    true
#define Quat_BracketX_Assignment_Operator_plus          true
#define Quat_BracketY_Assignment_Operator_plus          true
#define Quat_BracketZ_Assignment_Operator_plus          true
#define Quat_BracketW_Assignment_Operator_plus          true
#define Quat_BracketX_Assignment_Operator_Minus_Const   true
#define Quat_BracketY_Assignment_Operator_Minus_Const   true
#define Quat_BracketZ_Assignment_Operator_Minus_Const   true
#define Quat_BracketW_Assignment_Operator_Minus_Const   true
#define Quat_BracketX_Assignment_Operator_Minus         true
#define Quat_BracketY_Assignment_Operator_Minus         true
#define Quat_BracketZ_Assignment_Operator_Minus         true
#define Quat_BracketW_Assignment_Operator_Minus         true
#define Quat_BracketX_Equal_Operator_true               true
#define Quat_BracketX_Equal_Operator_false              true
#define Quat_BracketY_Equal_Operator_true               true
#define Quat_BracketY_Equal_Operator_false              true
#define Quat_BracketZ_Equal_Operator_true               true
#define Quat_BracketZ_Equal_Operator_false              true
#define Quat_BracketW_Equal_Operator_true               true
#define Quat_BracketW_Equal_Operator_false              true
#define Quat_BracketX_notEqual_Operator_false           true
#define Quat_BracketX_notEqual_Operator_true            true
#define Quat_BracketY_notEqual_Operator_false           true
#define Quat_BracketY_notEqual_Operator_true            true
#define Quat_BracketZ_notEqual_Operator_false           true
#define Quat_BracketZ_notEqual_Operator_true            true
#define Quat_BracketW_notEqual_Operator_false           true
#define Quat_BracketW_notEqual_Operator_true            true
#define Quat_BracketX_LessThan_Operator_false           true
#define Quat_BracketY_LessThan_Operator_false           true
#define Quat_BracketZ_LessThan_Operator_false           true
#define Quat_BracketW_LessThan_Operator_false           true
#define Quat_BracketX_LessThan_Operator_true            true
#define Quat_BracketY_LessThan_Operator_true            true
#define Quat_BracketZ_LessThan_Operator_true            true
#define Quat_BracketW_LessThan_Operator_true            true
#define Quat_BracketX_LessThanEqual_Operator_true_eq    true
#define Quat_BracketY_LessThanEqual_Operator_true_eq    true
#define Quat_BracketZ_LessThanEqual_Operator_true_eq    true
#define Quat_BracketW_LessThanEqual_Operator_true_eq    true
#define Quat_BracketX_LessThanEqual_Operator_false      true
#define Quat_BracketY_LessThanEqual_Operator_false      true
#define Quat_BracketZ_LessThanEqual_Operator_false      true
#define Quat_BracketW_LessThanEqual_Operator_false      true
#define Quat_BracketX_LessThanEqual_Operator_true       true
#define Quat_BracketY_LessThanEqual_Operator_true       true
#define Quat_BracketZ_LessThanEqual_Operator_true       true
#define Quat_BracketW_LessThanEqual_Operator_true       true
#define Quat_BracketX_GreaterThan_Operator_false        true
#define Quat_BracketX_GreaterThan_Operator_true         true
#define Quat_BracketY_GreaterThan_Operator_false        true
#define Quat_BracketY_GreaterThan_Operator_true         true
#define Quat_BracketZ_GreaterThan_Operator_false        true
#define Quat_BracketZ_GreaterThan_Operator_true         true
#define Quat_BracketW_GreaterThan_Operator_false        true
#define Quat_BracketW_GreaterThan_Operator_true         true
#define Quat_BracketX_GreaterThanEq_Operator_true_eq    true
#define Quat_BracketY_GreaterThanEq_Operator_true_eq    true
#define Quat_BracketZ_GreaterThanEq_Operator_true_eq    true
#define Quat_BracketW_GreaterThanEq_Operator_true_eq    true
#define Quat_BracketX_GreaterThanEqual_Operator_false   true
#define Quat_BracketY_GreaterThanEqual_Operator_false   true
#define Quat_BracketZ_GreaterThanEqual_Operator_false   true
#define Quat_BracketW_GreaterThanEqual_Operator_false   true
#define Quat_BracketX_GreaterThanEqual_Operator_true    true
#define Quat_BracketY_GreaterThanEqual_Operator_true    true
#define Quat_BracketZ_GreaterThanEqual_Operator_true    true
#define Quat_BracketW_GreaterThanEqual_Operator_true    true

#define Quat_Set_everyElement                   true
#define Quat_Vect_set_3_elements                true
#define Quat_Set_Vect_Real                      true
#define Quat_Set_Quat_From_Matrix               true
#define Quat_Set_IDENTITY                       true
#define Quat_Set_ZERO                           true
#define Quat_Set_ROT_X_PI2                      true
#define Quat_Set_ROT_X_PI                       true
#define Quat_Set_ROT_Y_PI2                      true
#define Quat_Set_ROT_Y_PI                       true
#define Quat_Set_ROT_Z_PI2                      true
#define Quat_Set_ROT_Z_PI                       true
#define Quat_Set_ROT_X                          true
#define Quat_Set_ROT_Y                          true
#define Quat_Set_ROT_Z                          true
#define Quat_XY_set                             true
#define Quat_XZ_set                             true
#define Quat_YX_set                             true
#define Quat_YZ_set                             true
#define Quat_ZX_set                             true
#define Quat_ZY_set                             true
#define Quat_XYZ_set                            true
#define Quat_XZY_set                            true
#define Quat_YXZ_set                            true
#define Quat_YZX_set                            true
#define Quat_ZXY_set                            true
#define Quat_ZYX_set                            true
#define Quat_AXIS_ANGLE_set                     true
#define Quat_ROT_ORIENT_set                     true
#define Quat_ROT_INVERSE_ORIENT_set             true
#define Quat_Set_Each_ELEMENT                   true
#define Quat_Set_From_VECT                      true
#define Quat_Get_Vect_elements                  true
#define Quat_Get_Vect                           true
#define Quat_Get_Angle_of_rotation              true
#define Quat_Get_Axis_of_rotation               true
#define Quat_Get_bracket1                       true
#define Quat_Get_bracket2                       true
#define Quat_Get_bracket3                       true
#define Quat_Get_bracket4                       true



#define Vec3_Add_Vec3                           true                                    
#define Vec3_Assignment_Operator                true     
#define Vec3_Assignment_Operator_Vec4           true
#define Vec3_ConstantCheck                      true                               
#define Vec3_Constructor_Copy                   true                            
#define Vec3_Constructor_Default                true                         
#define Vec3_Constructor_Individual             true                       
#define Vec3_Constructor_Vec4                   true                             
#define Vec3_Cross                              true                                       
#define Vec3_Destructor                         true                                  
#define Vec3_Dot                                true                                         
#define Vec3_Get_Accessors                      true                               
#define Vec3_Get_Accessors_const                true                         
#define Vec3_Get_Angle                          true                                   
#define Vec3_Get_Brackets                       true                                
#define Vec3_Get_Brackets_const                 true                          
#define Vec3_Get_Normalize                      true                               
#define Vec3_IsEqualFalse                       true                                
#define Vec3_IsEqualFalse_default               true                        
#define Vec3_IsEqualTrue                        true                                 
#define Vec3_IsEqualTrue_default                true                         
#define Vec3_IsZeroFalse                        true                                 
#define Vec3_IsZeroFalse_default                true                         
#define Vec3_IsZeroTrue_default                 true                          
#define Vec3_IsZeroTrue_True                    true                             
#define Vec3_Len                                true                                         
#define Vec3_MultEqual_Mat3                     true                              
#define Vec3_MultEqual_Mat4                     true                              
#define Vec3_Mult_Mat3                          true                                   
#define Vec3_Mult_Mat4                          true                                   
#define Vec3_Mult_Scale                         true                                  
#define Vec3_Normalize                          true                                   
#define Vec3_PlusEqual_Vect                     true                              
#define Vec3_Proxy                              true                                       
#define Vec3_SampleTest                         true                                  
#define Vec3_ScaleEquals                        true                                 
#define Vec3_Scale_Vect                         true                                  
#define Vec3_Set_3_Elements                     true                              
#define Vec3_Set_Accessors                      true                               
#define Vec3_Set_Accessors_get                  true                           
#define Vec3_Set_Brackets                       true                                
#define Vec3_Set_Brackets_get                   true                            
#define Vec3_Set_Vec3                           true                                    
#define Vec3_Set_Vec4                           true                                    
#define Vec3_SubEqual_Vect                      true                               
#define Vec3_Sub_Vect                           true                                    
#define Vec3_Unary_equal_Minus                  true                           
#define Vec3_Unary_equal_Plus                   true                            
#define Vec3_Unary_Minus                        true                                 
#define Vec3_Unary_Plus                         true    
									            
#define Vec4_Add_Vec4                           true                                    
#define Vec4_Assignment_Operator                true                         
#define Vec4_ConstantCheck                      true                               
#define Vec4_Constructor_Copy                   true  
#define Vec4_Constructor_Default                true                            
#define Vec4_Constructor_Individual             true                      
#define Vec4_Constructor_Vec3                   true
#define Vec4_Destructor                         true                                  
#define Vec4_Dot                                true                                         
#define Vec4_Get_Accessors                      true                               
#define Vec4_Get_Accessors_const                true                         
#define Vec4_Get_Brackets                       true                                
#define Vec4_Get_Brackets_const                 true                          
#define Vec4_Get_Normalize                      true                               
#define Vec4_IsEqualFalse                       true                                
#define Vec4_IsEqualFalse_default               true                        
#define Vec4_IsEqualTrue                        true                                 
#define Vec4_IsEqualTrue_default                true                         
#define Vec4_IsZeroFalse                        true                                 
#define Vec4_IsZeroFalse_default                true                         
#define Vec4_IsZeroTrue_default                 true                          
#define Vec4_IsZeroTrue_True                    true                             
#define Vec4_Len                                true                                         
#define Vec4_MultEqual_Mat4                     true                              
#define Vec4_Mult_Mat4                          true                                   
#define Vec4_Mult_Scale                         true                                  
#define Vec4_Normalize                          true                                   
#define Vec4_PlusEqual_Vec4                     true                              
#define Vec4_Proxy                              true                                       
#define Vec4_SampleTest                         true                                  
#define Vec4_ScaleEquals                        true                                 
#define Vec4_Scale_Vec4                         true                                  
#define Vec4_Set_4_Elements                     true                              
#define Vec4_Set_Accessors                      true                               
#define Vec4_Set_Accessors_Get                  true                           
#define Vec4_Set_Brackets                       true                                
#define Vec4_Set_Brackets_get                   true                            
#define Vec4_Set_Vec3                           true                                    
#define Vec4_Set_Vec4                           true                                    
#define Vec4_SubEqual_Vec4                      true                               
#define Vec4_Sub_Vec4                           true                                    
#define Vec4_Unary_equal_Minus                  true                           
#define Vec4_Unary_equal_Plus                   true                            
#define Vec4_Unary_Minus                        true                                 
#define Vec4_Unary_Plus                         true                                  

		     



// ---  End of File ---------------





#endif

// ---  End of File ---------------
