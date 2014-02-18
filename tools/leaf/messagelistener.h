#ifndef MESSAGELISTENER_H
#define MESSAGELISTENER_H

#include <QObject>
class QUdpSocket;

struct PacketAllocationInfo
{
	quint32       iAddr; // intptr_t, seed 32bits <> qt 64bits...
	quint32       iLine;
	quint32       iFrame;
	quint64       iTime;
	char          strCall[256];
	char          strFile[256];
	char          strFunc[256];
	bool          bFreed;
	quint64       iLifetime;
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
		void onLeafPrintLog(const QString &msg);
		void onLeafPrintError(const QString &msg);
		void onLeafPrintDebug(const QString &msg);
		void onAllocation(const PacketAllocationInfo *msg);
		void onFree(const PacketFreeInfo *msg);

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
