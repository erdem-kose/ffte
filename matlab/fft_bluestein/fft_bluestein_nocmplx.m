% https://ccrma.stanford.edu/~jos/st/Bluestein_s_FFT_Algorithm.html

function [X_real, X_imag]=fft_bluestein_nocmplx(x_real,x_imag,inv)
    N=size(x_real,2);

    n=-(N-1):(N-1);

    trig_param=(2*pi/N).*0.5.*n.*n;
    if inv~=0
        trig_param=-trig_param;
    end
    wq_real=cos(trig_param);
    wq_imag=sin(trig_param);

    xq_real=zeros(1,N);
    xq_imag=zeros(1,N);
    for i=1:N
        [xq_real(i),xq_imag(i)]=cmplx_div(x_real(i),x_imag(i),wq_real(i+N-1),wq_imag(i+N-1));
    end
    
    X_real=zeros(1,N);
    X_imag=zeros(1,N);
    for j=(N+1):(2*N)
        jj=j-N;
        for i=1:N
            [X_real_tmp,X_imag_tmp]=cmplx_mul(xq_real(i),xq_imag(i),wq_real(j-i),wq_imag(j-i));
            X_real(jj)=X_real(jj)+X_real_tmp;
            X_imag(jj)=X_imag(jj)+X_imag_tmp;
        end
        [X_real(jj),X_imag(jj)]=cmplx_div(X_real(jj),X_imag(jj),wq_real(j-1),wq_imag(j-1));
        if inv~=0
            [X_real(jj),X_imag(jj)]=cmplx_div(X_real(jj),X_imag(jj),N,0);
        end
    end
end

function [z_real,z_imag]=cmplx_mul(x_real,x_imag,y_real,y_imag)
    %https://mathworld.wolfram.com/ComplexMultiplication.html
    z_real=(x_real.*y_real) - (x_imag.*y_imag);
    z_imag=(x_imag.*y_real) + (x_real.*y_imag);
end

function [z_real,z_imag]=cmplx_div(x_real,x_imag,y_real,y_imag)
    %https://mathworld.wolfram.com/ComplexDivision.html
    const=y_real.*y_real + y_imag.*y_imag;
    z_real=((x_real.*y_real) + (x_imag.*y_imag))./const;
    z_imag=((x_imag.*y_real) - (x_real.*y_imag))./const;
end
