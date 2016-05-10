#ifndef ABSTRACTRECOGNIZER_H
#define ABSTRACTRECOGNIZER_H

#include <QObject>

#include <QPainterPath>

#include <QHash>

class AbstractRecognizer : public QObject
{
	Q_OBJECT

public:
	AbstractRecognizer(QObject *parent);
	~AbstractRecognizer();

	virtual bool init(QHash<QString, QString> options) = 0;

	virtual QStringList recognize(QList<QPainterPath*>& strokes) = 0;

private:
	
};

#endif // ABSTRACTRECOGNIZER_H
