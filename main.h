#include "nwpwin.h"
#include "nwpdlg.h"

class main_dialog : public vsite::nwp::dialog {
public:
	tstring text;
protected:
	int idd() const override;
	bool on_init_dialog() override;
	bool on_ok() override;
};

class main_window : public vsite::nwp::window {
public:
	tstring text;
	LOGFONT lf;
	COLORREF cr;
	main_window();
protected:
	void on_paint(HDC hdc) override;
	void on_command(int id) override;
	void on_destroy() override;
};
