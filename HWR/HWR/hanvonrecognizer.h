#ifndef HANVONRECOGNIZER_H
#define HANVONRECOGNIZER_H

#include "abstractrecognizer.h"

#include <QNetworkAccessManager>
#include <QNetworkReply>

class HanvonRecognizer : public AbstractRecognizer
{
	Q_OBJECT

public:
	HanvonRecognizer(QObject *parent);
	~HanvonRecognizer();

	virtual bool init(QHash<QString, QString> options);

	virtual QStringList recognize(STROKES strokes);

private:

	QByteArray pack(STROKES strokes);

	void sendRequest(QByteArray data);

	private slots:
	void slotReadyRead();
	void slotError(QNetworkReply::NetworkError);
	void slotSslErrors(QList<QSslError>);

private:
	QNetworkAccessManager m_nam;
	
};

#endif // HANVONRECOGNIZER_H
