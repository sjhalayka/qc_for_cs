#include <iostream>
#include <complex>
using namespace std;


complex<float> get_polar(const complex<float>& in)
{
	return complex<float>(abs(in), atan2f(in.imag(), in.real()));
}

complex<float> get_cartesian(const complex<float>& in)
{
	const float r = in.real();
	const float theta = in.imag();

	return complex<float>(r * cos(theta), r * sin(theta));
}


int main(void)
{
	const complex<float> c1(-2.3f, 3.0f);
	const complex<float> c2(-1.0f, -4.7f);

	cout << c1 << " " << c2 << endl;

	complex<float> p1 = get_polar(c1);
	complex<float> p2 = get_polar(c2);

	cout << p1 << " " << p2 << endl;

	cout << get_cartesian(p1) << " " << get_cartesian(p2) << endl;

	return 0;
}