#ifndef GOOGLERECOGNIZER_H
#define GOOGLERECOGNIZER_H

#include "abstractrecognizer.h"

class GoogleRecognizer : public AbstractRecognizer
{
	Q_OBJECT

public:
	GoogleRecognizer(QObject *parent);
	~GoogleRecognizer();


	virtual bool init(QHash<QString, QString> options);

	virtual QStringList recognize(STROKES strokes);

private:

	QString pack();
	
};

#endif // GOOGLERECOGNIZER_H
