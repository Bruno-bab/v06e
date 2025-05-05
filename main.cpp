#include "main.h"
#include "rc.h"
#include <bitset>

int main_dialog::idd() const {
	return IDD_DIALOG;
}
bool main_dialog::on_init_dialog() {
	set_text(IDC_EDIT1, text);
	return true;
}
bool main_dialog::on_ok() {
	text = get_text(IDC_EDIT1);
	return true;
}

main_window::main_window() {
	ZeroMemory(&lf, sizeof(lf));
	_tcscpy_s(lf.lfFaceName, _T("Arial"));
	HDC hdc = ::GetDC(0);
	lf.lfHeight = -15 * ::GetDeviceCaps(hdc, LOGPIXELSY) / 72;
	cr = RGB(0, 0, 0);
	::ReleaseDC(0, hdc);
}

bool get_font(HWND hw, LOGFONT& logf, COLORREF& cr) {
	LOGFONT lf{ logf };
	CHOOSEFONT cf{ sizeof cf, hw, 0, &lf, 0, CF_EFFECTS | CF_INITTOLOGFONTSTRUCT, cr };
	if (ChooseFont(&cf)) {
		logf = lf;
		cr = cf.rgbColors;
		return true;
	}
	return false;

}

void main_window::on_paint(HDC hdc) {
	if (text.empty())
		return;
	RECT r;
	GetClientRect(*this, &r);
	int x = r.right / 17;
	int y = r.bottom / text.size();
	HFONT hf = (HFONT)SelectObject(hdc, CreateFontIndirect(&lf));
	SetTextColor(hdc, cr);

	for (int i = 0; i < text.size(); i++)
	{
		std::bitset<16> b(text[i]);
		for (int j = 0; j < 16; j++)
		{
			RECT bit_r = {j * x, i * y, (j + 1) * x, (i + 1) * y };
			if (!b.test(15 - j))
				FillRect(hdc, &bit_r, (HBRUSH)GetStockObject(BLACK_BRUSH));
		}
		r = { 16 * x, i * y, 17 * x, (i + 1) * y };
		DrawText(hdc, &text[i], 1, &r, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	}
	DeleteObject((HFONT)SelectObject(hdc, hf));
}

void main_window::on_command(int id) {
	switch(id){
		case ID_FONT:
			get_font(*this, lf, cr);
			break;
		case ID_TEXT:
		{
			main_dialog d;
			d.text = text;
			if (d.do_modal(0, *this) == IDOK)
				text = d.text;
			break;
		}
		case ID_EXIT:
			::DestroyWindow(*this);
			break;
	}
	InvalidateRect(*this, NULL, true);
}

void main_window::on_destroy(){
	::PostQuitMessage(0);
}

int WINAPI WinMain(HINSTANCE hi, HINSTANCE, LPSTR, int)
{
	vsite::nwp::application app;
	main_window wnd;
	wnd.create(0, WS_OVERLAPPEDWINDOW | WS_VISIBLE, _T("NWP"), (UINT_PTR)LoadMenu(hi, MAKEINTRESOURCE(IDM_MAIN)));
	return app.run();
}
