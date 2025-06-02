clc; clear; close all;
% https://www.geeksforgeeks.org/doolittle-algorithm-lu-decomposition/#:~:text=Doolittle's%20method%20provides%20an%20alternative,of%20L%20and%20U%20explicitly.
% http://home.cc.umanitoba.ca/~farhadi/Math2120/Inverse%20Using%20LU%20decomposition.pdf

%% Input Matrix
A=[2 -1 -2;
    -4 6 3;
    -4 -2 8];

%% Matlab LU
[L_1,U_1,P_1]=lu(A);

%% doolitle LU
[L_2,U_2,P_2]=doolittleLU(A);

A2=P_2'*L_2*U_2;

%% Show
disp(A)
disp(A2)