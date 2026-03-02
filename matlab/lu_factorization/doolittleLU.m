% Copyright (c) [2022-2026] Erdem Kose

function [L, U, P] = doolittleLU(A)
    [N, M] = size(A);
    L = zeros(N, M);
    U = zeros(N, M);
    P = eye(N, M);

    for k = 1:N
        % Partial pivoting: find row >= k with largest |A(i,k)|
        [~, r] = max(abs(A(k:N, k)));
        r = r + k - 1;

        if r ~= k
            A([k, r], :)     = A([r, k], :);
            P([k, r], :)     = P([r, k], :);
            L([k, r], 1:k-1) = L([r, k], 1:k-1);
        end

        % Compute row k of U
        for j = k:N
            tmp = 0;
            for s = 1:k-1
                tmp = tmp + L(k, s) * U(s, j);
            end
            U(k, j) = A(k, j) - tmp;
        end

        % Diagonal of L is 1 (Doolittle convention)
        L(k, k) = 1;

        % Compute column k of L
        for i = k+1:N
            tmp = 0;
            for s = 1:k-1
                tmp = tmp + L(i, s) * U(s, k);
            end
            L(i, k) = (A(i, k) - tmp) / U(k, k);
        end
    end
end
