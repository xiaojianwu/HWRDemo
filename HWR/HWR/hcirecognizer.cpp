#include "hcirecognizer.h"

#include <QDebug>
#include <QTime>
#include <string>

#include <hci_sys.h>

#include <hci_hwr.h>

#include <time.h>



#pragma comment(lib, "hci_sys.lib")
#pragma comment(lib, "hci_hwr.lib")

HCIRecognizer::HCIRecognizer(QObject *parent)
	: AbstractRecognizer(parent)
{
	qRegisterMetaType<QHash<QString, QString>>("QHash<QString, QString>");
	qRegisterMetaType<STROKES>("STROKES");
	connect(this, SIGNAL(sigInit(QHash<QString, QString>)), this, SLOT(onInit(QHash<QString, QString>)));
	connect(this, SIGNAL(sigRecognize(STROKES)), this, SLOT(onRecognize(STROKES)));
}

HCIRecognizer::~HCIRecognizer()
{
	HCI_ERR_CODE err_code = HCI_ERR_NONE;
	
	hci_hwr_session_stop(m_nSessionId);
	qDebug() << "hci_hwr_session_stop success\n";

	//HWR����ʼ��
	hci_hwr_release();


	//SYS����ʼ��
	err_code = hci_release();
	qDebug() << "hci_release\n";
}

bool HCIRecognizer::CheckAndUpdateAuth()
{
	//��ȡ����ʱ��
	int64 expire_time;
	int64 current_time = (int64)time(NULL);
	HCI_ERR_CODE err_code = hci_get_auth_expire_time(&expire_time);
	if (err_code == HCI_ERR_NONE)
	{
		//��ȡ�ɹ����ж��Ƿ����
		if (expire_time > current_time)
		{
			//û�й���
			qDebug() << "auth can use continue\n";
			return true;
		}
	}

	//��ȡ����ʱ��ʧ�ܻ��Ѿ�����
	//�ֶ����ø�����Ȩ
	//HCI_ERR_CODE err_code;
	err_code = hci_check_auth();
	if (err_code == HCI_ERR_NONE)
	{
		//���³ɹ�
		qDebug() << "check auth success \n";
		return true;
	}
	else
	{
		//����ʧ��
		qDebug() << QString("check auth return (%1:%2)\n").arg(err_code).arg(hci_get_error_info(err_code));
		return false;
	}
}


void HCIRecognizer::onInit(QHash<QString, QString> options)
{
	std::string appkey = "ed5d5407"; //  options["appKey"].toStdString();
	std::string developerKey = "5ef9e5b01c5f6c7490777ca25469a793"; // options["developerKey"].toStdString();
	std::string cloudUrl = "test.api.hcicloud.com:8888"; // options["cloudUrl"].toStdString();
	std::string authpath = "d:\\hcitest\\auth"; // options["authpath"].toStdString();
	std::string logfilepath = "d:\\hcitest\\log";// options["logfilepath"].toStdString();


	std::string capkey = "hwr.cloud.freewrite";// options["capkey"].toStdString(); //��Ҫ���е���������

	std::string datapath = "d:\\hcitest\\data";// options["datapath"].toStdString();


											   //SYS��ʼ��
	HCI_ERR_CODE err_code = HCI_ERR_NONE;
	//���ô�����"�ֶ�=ֵ"����ʽ������һ���ַ���������ֶ�֮����','�������ֶ������ִ�Сд��
	std::string init_config = "";
	init_config += "appKey=" + appkey;              //����Ӧ�����
	init_config += ",developerKey=" + developerKey; //���ƿ�������Կ
	init_config += ",cloudUrl=" + cloudUrl;         //�����Ʒ���Ľӿڵ�ַ
	init_config += ",authpath=" + authpath;         //��Ȩ�ļ�����·������֤��д
	init_config += ",logfilepath=" + logfilepath;   //��־��·��
	init_config += ",loglevel=5";								     //��־�ĵȼ�
	init_config += ",logfilesize=512";								 //��־�ļ��Ĵ�С
																	 //��������ʹ��Ĭ��ֵ��������ӣ���������ÿ��Բο������ֲ�
	err_code = hci_init(init_config.c_str());
	if (err_code != HCI_ERR_NONE)
	{
		QString msg = QString("hci_init return (%1:%2)\n").arg(err_code).arg(hci_get_error_info(err_code));

		qDebug() << msg;
		emit error(msg);
		return;
	}


	//�����Ȩ����������������Ȩ
	if (!CheckAndUpdateAuth())
	{
		QString msg = "CheckAndUpdateAuth failed\n";
		hci_release();
		qDebug() << msg;
		emit error(msg);
		return;
	}

	//ASR��ʼ��
	std::string hwr_config = "";
	hwr_config += "initcapkeys=" + capkey;        //������initcapkeys�������Ҫʹ�õ�������������ߵ�һ��ʶ���Ч��
												  // ���������';'���� 
	hwr_config += ",datapath=" + datapath;        //����Ǳ����������˴�ָ����������������Դ
	err_code = hci_hwr_init(hwr_config.c_str());
	if (err_code != HCI_ERR_NONE)
	{
		QString msg = QString("hci_hwr_init return (%1:%2) \n").arg(err_code).arg(hci_get_error_info(err_code));

		qDebug() << msg;
		emit error(msg);
		return;
	}
	qDebug() << "hci_hwr_init success \n";


	//session start
	m_nSessionId = -1;
	std::string session_config = "capkey=" + capkey;
	qDebug() << QString("hci_hwr_session_start config[ %1 ]\n").arg(session_config.c_str());
	err_code = hci_hwr_session_start(session_config.c_str(), &m_nSessionId);
	if (err_code != HCI_ERR_NONE)
	{
		QString msg = QString("hci_hwr_session_start return (%1:%2)\n").arg(err_code).arg(hci_get_error_info(err_code));

		qDebug() << msg;
		emit error(msg);

		return;
	}
	qDebug() << "hci_hwr_session_start success\n";
}

void HCIRecognizer::onRecognize(STROKES strokes)
{
	QVector<short> shortData = pack(strokes);

	qDebug() << "strokes =" << shortData;

	QStringList resultList;


	HCI_ERR_CODE err_code = HCI_ERR_NONE;

	//hwr recog
	HWR_RECOG_RESULT hwrRecogResult;
	std::string recog_config = "";

	QTime time;
	time.start();

	err_code = hci_hwr_recog(m_nSessionId, shortData.data(), shortData.size() * sizeof(short), recog_config.c_str(), &hwrRecogResult);
	//err_code = hci_hwr_recog(nSessionId, g_sGestureStrokeData, sizeof(g_sGestureStrokeData), recog_config.c_str(), &hwrRecogResult); //����ʶ��

	qDebug() << QString("Recog time: %1\n").arg(time.elapsed());

	if (err_code == HCI_ERR_NONE)
	{
		qDebug() << "hci_hwr_recog success\n";

		for (int index = 0; index < (int)hwrRecogResult.uiResultItemCount; ++index)
		{
			HWR_RECOG_RESULT_ITEM& item = hwrRecogResult.psResultItemList[index];
			resultList << QString::fromUtf8(item.pszResult);
		}
		hci_hwr_free_recog_result(&hwrRecogResult);

		emit recognizeResult(resultList);
	}
	else
	{
		qDebug() << QString("hci_hwr_recog return (%1:%2)\n").arg(err_code).arg(hci_get_error_info(err_code));
	}
}

QVector<short> HCIRecognizer::pack(STROKES strokes)
{
	QVector<short> data;
	for each (STROKE stroke in strokes)
	{
		for (int i = 0; i < stroke[0].size() - 1; i++)
		{
			data << stroke[0].at(i) << stroke[1].at(i);
		}
		data << -1 << 0;
	}

	data << -1 << -1;	

	return data;
}