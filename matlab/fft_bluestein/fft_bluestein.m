% https://ccrma.stanford.edu/~jos/st/Bluestein_s_FFT_Algorithm.html

function X=fft_bluestein(x,inv)
    N=size(x,2);

    n=-(N-1):(N-1);

    trig_param=(2*pi/N).*0.5.*n.*n;
    if inv~=0
        trig_param=-trig_param;
    end
        
    wq=exp(1i*trig_param);
    xq=x./wq(N:end);
    
    X=conv(wq,xq,'valid')./wq(N:end);

    if inv~=0
        X=X./N;
    end
end
