#include <iostream>
#include <cstdint>
#include <algorithm>
#include <thread>

#include <conio.h>

#if defined(_WIN32_)

//https://docs.microsoft.com/en-us/windows/console/console-virtual-terminal-sequences
//http://7ujm.net/etc/esc.html

/**/
#include <windows.h>

bool SetDisplayMode() {//only after Windows10 Th2?
	HANDLE H = GetStdHandle(STD_OUTPUT_HANDLE);

	DWORD M = 0;
	if (GetConsoleMode(H, &M) == FALSE) return false;

	if (SetConsoleMode(H, M | ENABLE_VIRTUAL_TERMINAL_PROCESSING) == FALSE) return false;

	return true;
}
/**/
#else

bool SetDisplayMode() { return true; }

#endif

int Key() {
	int K = _kbhit();
	if (K) {
		return _getch();
	}

	return -1;
}

class Meter {
public:
	 bool Dec() {
		V-=Minus;
		return true;
	}
	virtual bool Set(std::intmax_t N) {
		V = N;
		return true;
	}
	bool SetMax(std::intmax_t N) {
		Max = N;
		return true;
	}
	std::intmax_t Get() {
		return V;
	}	
	std::intmax_t GetMax() {
		return Max;
	}
	bool Inc() {
		V+=Plus;
		return true;
	}

	bool SetPlus(std::intmax_t N) {
		Plus = N;
		return true;
	}
	std::intmax_t GetPlus() {
		return Plus;
	}
	bool SetMinus(std::intmax_t N) {
		Minus= N;
		return true;
	}
	std::intmax_t GetMinus() {
		return Minus;
	}
	virtual bool Update() {

		Dec();

		if (V < 0) { V = 0; }
		return true;
	}

protected:
	std::intmax_t V = 0;
	std::intmax_t Max = 50;
	std::intmax_t Plus = 1;
	std::intmax_t Minus = 1;

};
bool ClearShow() {
	std::cout << "\033[2J";
	return true;
}
bool HideCursor() {
	std::cout << "\033[?" << 25 << "h";
	return true;
}
bool ShowCursor() {
	std::cout << "\033[?" << 25 << "l";
	return true;
}
bool Locate(const std::uint_fast32_t& X, const std::int_fast32_t Y) {
	std::cout << "\033[" << Y << ";" << X << "H";
	return true;
}
int main() {
	Meter M;
	Meter S;
	int K = 0;

	int N = 0;

	SetDisplayMode();

	while (K!=27)
	{
		//Locate(1, 1);
		std::this_thread::sleep_for(std::chrono::milliseconds(800));
		std::cout << N++ << '\r'; //<< std::endl;
		K = -1;
		K = Key();
		if (K == ' ') { M.Inc(); }
		if (K == '+') { M.SetPlus(7); std::cout << "+7" << std::endl; }
		if (K == '-') { M.SetMinus(2); std::cout << "-1" << std::endl; }
		if (M.Get() > S.Get()) {
			S.Set(M.Get());
		}
		M.Update();
		S.Update();
		ClearShow();

		std::size_t i = 0;
		for (i; i < std::min(M.Get(), M.GetMax()); i++) {
			std::cout << '#';
		}
		for (i; i < std::min(S.Get(), S.GetMax()); i++) {
			std::cout << '+';
		}
	}
}