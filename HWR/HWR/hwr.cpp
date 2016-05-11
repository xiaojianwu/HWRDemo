#include "hwr.h"

#include "HWRCanvas.h"

HWR::HWR(QWidget *parent)
	: QWidget(parent)
	, m_index(0)
{
	ui.setupUi(this);

	connect(ui.pushButton, SIGNAL(clicked()), this, SLOT(onStartHWR()));

	connect(ui.comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(onTypeChanged(int)));
}

HWR::~HWR()
{

}


void HWR::onStartHWR()
{

	HWRCanvas *hw = new HWRCanvas(m_index, this);
	hw->show();
}


void HWR::onTypeChanged(int index)
{
	m_index = index;
}
