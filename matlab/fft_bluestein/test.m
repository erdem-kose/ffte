clear; close all; clc;

%% Signal
fx=10;
fs=200;
t=0:1/fs:1;
f=linspace(0,fs,size(t,2));
x=sin(2*pi*fx*t);

%% FFT matlab
X_matlab=fft(x);
x_matlab=ifft(X_matlab);

%% FFT Bluestein
X_bluestein=fft_bluestein(x,0);
x_bluestein=real(fft_bluestein(X_bluestein,1));

%% FFT Bluestein (No Complex Number)
[X_real, X_imag]=fft_bluestein_nocmplx(x,zeros(size(x)),0);
X_bluestein2=X_real+1i*X_imag;
x_bluestein2=fft_bluestein_nocmplx(X_real,X_imag,1);

%% FFT Comparison
subplot(3,1,1)
stem(f,abs(X_matlab),'-','LineWidth',2);
hold on;
stem(f,abs(X_bluestein),'.');
stem(f,abs(X_bluestein2),':');
legend('Matlab FFT', 'Bluestein FFT', 'Bluestein FFT (No Complex)', 'Location','best');
title('Magnitude spectrum');

subplot(3,1,2)
stem(f,angle(X_matlab),'-','LineWidth',2);
hold on;
stem(f,angle(X_bluestein),'.');
stem(f,angle(X_bluestein2),':');
legend('Matlab FFT', 'Bluestein FFT', 'Bluestein FFT (No Complex)', 'Location','best');
title('Angle spectrum');

subplot(3,1,3)
plot(f,x_matlab,'-','LineWidth',2);
hold on;
plot(f,x_bluestein,'.');
plot(f,x_bluestein2,':');
legend('Matlab iFFT', 'Bluestein iFFT', 'Bluestein iFFT (No Complex)', 'Location','best');
title('Angle spectrum');