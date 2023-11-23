#include <iostream>
#include <complex>
using namespace std;

int main(void)
{
	complex<float> c1(1.0f, -0.9f);
	complex<float> c2(0.2f, 17.0f);

	cout << "Sum:     " << c1 + c2 << endl;
	cout << "Product: " << c1 * c2 << endl;

	return 0;
}