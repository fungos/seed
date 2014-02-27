#ifndef MESSAGELISTENER_H
#define MESSAGELISTENER_H

#include <QObject>
class QUdpSocket;

struct PacketAllocationInfo
{
	quint64       iAddr; // intptr_t
	quint64       iSize;
	quint64       iTime;
	quint64       iLifetime;
	quint32       iLine;
	quint32       iFrame;
	char          strCall[256];
	char          strFile[256];
	char          strFunc[256];
	bool          bFreed;
};
Q_DECLARE_METATYPE(PacketAllocationInfo *)

struct PacketFreeInfo
{
	quint32      iAddr;
	quint32      iFrame;
	quint64      iTime;
};
Q_DECLARE_METATYPE(PacketFreeInfo *)

class MessageListener : public QObject
{
		Q_OBJECT
	public:
		explicit MessageListener(QObject *parent = 0);

	signals:
		void onLog(const QString &msg);
		void onPrintLog(const QString &msg);
		void onPrintError(const QString &msg);
		void onPrintDebug(const QString &msg);
		void onAllocation(const PacketAllocationInfo *msg);
		void onFree(const PacketFreeInfo *msg);
		void onStart();
		void onStop();

	public slots:
		void processMessage();

	private:
		QUdpSocket *pSocket;

		struct Packet
		{
			qint32 iPacketId;
			qint32 iPacketSize;
			QByteArray baData;
		};
};

#endif // MESSAGELISTENER_H
