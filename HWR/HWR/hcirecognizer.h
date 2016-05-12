#ifndef HCIRECOGNIZER_H
#define HCIRECOGNIZER_H

#include "abstractrecognizer.h"

class HCIRecognizer : public AbstractRecognizer
{
	Q_OBJECT

public:
	HCIRecognizer(QObject *parent = nullptr);
	~HCIRecognizer();


	private slots:
	void onInit(QHash<QString, QString> options);
	void onRecognize(STROKES strokes);


private:
	bool CheckAndUpdateAuth();
	QVector<short> pack(STROKES strokes);


	int m_nSessionId;
};

#endif // HCIRECOGNIZER_H
