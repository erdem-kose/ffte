function [L,U,P]=doolittleLU(A)
    [N,M]=size(A);
    L=zeros(N,M);
    U=zeros(N,M);
    
    [A,P]=permutematrix(A);

    for i=1:N
        for j=1:M
            if (i<=j)
                if i==1
                    U(i,j)=A(i,j);
                else
                    tmp=0;
                    for k=1:(i-1)
                        tmp=tmp+L(i,k)*U(k,j);
                    end
                    U(i,j)=A(i,j)-tmp;
                end
                if U(i,j)==0
                    U(i,j)=U(i,j);
                end
            end
            if(i>j)
                if j==1
                    L(i,j)=A(i,j)/(U(j,j)+eps);
                else
                    tmp=0;
                    for k=1:(i-1)
                        tmp=tmp+L(i,k)*U(k,j);
                    end
                    L(i,j)=(A(i,j)-tmp)/(U(j,j)+eps);
                end
            end
            if(i==j)
                L(i,j)=1;
            end
        end
    end
end

function [A,P]=permutematrix(A)
    [N,M]=size(A);
    R=zeros(1,N);
    for i=1:N
        R(i)=find(A(i,:)~=0,1,"first");
    end
    [~,Pi]=sort(R,2,"ascend");

    P=eye(N,M);
    P=P(Pi,:);
    A=A(Pi,:);
    1;
end