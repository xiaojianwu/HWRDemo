#include "hwr.h"

#include "HWRCanvas.h"

HWR::HWR(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	connect(ui.pushButton, SIGNAL(clicked()), this, SLOT(onOne()));
	connect(ui.pushButton_2, SIGNAL(clicked()), this, SLOT(onTwo()));
}

HWR::~HWR()
{

}


void HWR::onOne()
{
	//StrokesRecognizer* w = new StrokesRecognizer;

	//HWRPaint* w = new HWRPaint;
	//w->show();


	HWRCanvas *hw = new HWRCanvas(this);
	hw->show();




}
void HWR::onTwo()
{
	//handwritor_debug* hw = new handwritor_debug;
	// hw.setGeometry(200, 200, 300, 300);
	//handwritor *hw = new handwritor(NULL, NULL, NULL, NULL, NULL);
	//hw->show();
}
