#ifndef HCIRECOGNIZER_H
#define HCIRECOGNIZER_H

#include "abstractrecognizer.h"

class HCIRecognizer : public AbstractRecognizer
{
	Q_OBJECT

public:
	HCIRecognizer(QObject *parent);
	~HCIRecognizer();

	virtual bool init(QHash<QString, QString> options);

	virtual void recognize(STROKES strokes);


private:
	bool CheckAndUpdateAuth();
	QVector<short> pack(STROKES strokes);


	int m_nSessionId;
};

#endif // HCIRECOGNIZER_H
