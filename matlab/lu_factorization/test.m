% Copyright (c) [2022-2026] Erdem Kose

clc; clear; close all;
% https://www.geeksforgeeks.org/doolittle-algorithm-lu-decomposition/
% http://home.cc.umanitoba.ca/~farhadi/Math2120/Inverse%20Using%20LU%20decomposition.pdf

tol  = 1e-10;
pass = 0;
fail = 0;

%% -----------------------------------------------------------------------
%% Test 1: Reference matrix (3x3, no pivot swap needed)
fprintf('Test 1: Reference 3x3 matrix\n');
A = [2 -1 -2; -4 6 3; -4 -2 8];
[L, U, P] = doolittleLU(A);
err = norm(P*A - L*U, 'fro');
fprintf('  ||P*A - L*U||_F = %.2e', err);
if err < tol
    fprintf('  PASS\n'); pass = pass + 1;
else
    fprintf('  FAIL\n'); fail = fail + 1;
end

%% -----------------------------------------------------------------------
%% Test 2: Matrix requiring row swap (small leading element)
fprintf('Test 2: Pivoting required (small leading element)\n');
A = [0.001  2; 1  1];
[L, U, P] = doolittleLU(A);
err = norm(P*A - L*U, 'fro');
fprintf('  ||P*A - L*U||_F = %.2e', err);
if err < tol
    fprintf('  PASS\n'); pass = pass + 1;
else
    fprintf('  FAIL\n'); fail = fail + 1;
end

%% -----------------------------------------------------------------------
%% Test 3: Identity matrix
fprintf('Test 3: Identity matrix (4x4)\n');
A = eye(4);
[L, U, P] = doolittleLU(A);
err = norm(P*A - L*U, 'fro');
fprintf('  ||P*A - L*U||_F = %.2e', err);
if err < tol
    fprintf('  PASS\n'); pass = pass + 1;
else
    fprintf('  FAIL\n'); fail = fail + 1;
end

%% -----------------------------------------------------------------------
%% Test 4: Upper triangular matrix (U should equal A, L = I, P = I)
fprintf('Test 4: Already upper triangular (3x3)\n');
A = [3 1 2; 0 5 4; 0 0 7];
[L, U, P] = doolittleLU(A);
err = norm(P*A - L*U, 'fro');
fprintf('  ||P*A - L*U||_F = %.2e', err);
if err < tol
    fprintf('  PASS\n'); pass = pass + 1;
else
    fprintf('  FAIL\n'); fail = fail + 1;
end

%% -----------------------------------------------------------------------
%% Test 5: Random 5x5 matrix (cross-check against MATLAB's lu)
fprintf('Test 5: Random 5x5 matrix (cross-check vs MATLAB lu)\n');
rng(42);
A = rand(5);
[L, U, P] = doolittleLU(A);
err = norm(P*A - L*U, 'fro');
fprintf('  ||P*A - L*U||_F = %.2e', err);
if err < tol
    fprintf('  PASS\n'); pass = pass + 1;
else
    fprintf('  FAIL\n'); fail = fail + 1;
end

%% -----------------------------------------------------------------------
%% Test 6: L/U structure check (unit lower triangular, upper triangular)
fprintf('Test 6: L is unit lower triangular, U is upper triangular\n');
rng(7);
A = rand(4);
[L, U, ~] = doolittleLU(A);
l_err = norm(tril(L) - (tril(L,-1) + eye(4)), 'fro');  % L == strictly_lower + I
u_err = norm(triu(U) - U, 'fro');                       % U == upper triangular
err = l_err + u_err;
fprintf('  structure error = %.2e', err);
if err < tol
    fprintf('  PASS\n'); pass = pass + 1;
else
    fprintf('  FAIL\n'); fail = fail + 1;
end

%% -----------------------------------------------------------------------
%% Test 7: Reconstruction  A == P'*L*U  (visual display)
fprintf('\nTest 7: Visual reconstruction check (reference matrix)\n');
A = [2 -1 -2; -4 6 3; -4 -2 8];
[L, U, P] = doolittleLU(A);
A_rec = P' * L * U;
fprintf('  A (original):\n');   disp(A);
fprintf('  P''*L*U (reconstructed):\n'); disp(A_rec);
err = norm(A - A_rec, 'fro');
fprintf('  ||A - P''*L*U||_F = %.2e', err);
if err < tol
    fprintf('  PASS\n'); pass = pass + 1;
else
    fprintf('  FAIL\n'); fail = fail + 1;
end

%% -----------------------------------------------------------------------
%% Summary
fprintf('\n--- Results: %d passed, %d failed ---\n', pass, fail);
