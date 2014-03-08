#include "messagelistener.h"
#include <QtNetwork>

enum class ePacket : quint32
{
	Log			= 1,
	Error		= 2,
	Debug		= 3,

	Start		= 4,
	Stop		= 5,

	Allocation  = 100,
	Free		= 101
};

MessageListener::MessageListener(QObject *parent)
	: QObject(parent)
	, pSocket(nullptr)
{
	pSocket = new QUdpSocket(this);
	pSocket->bind(11115, QUdpSocket::ShareAddress);

	connect(pSocket, SIGNAL(readyRead()), this, SLOT(processMessage()));
}

void MessageListener::processMessage()
{
	Packet p;

	while (pSocket->hasPendingDatagrams())
	{
		QByteArray packet;
		packet.resize(pSocket->pendingDatagramSize());
		pSocket->readDatagram(packet.data(), packet.size());

		QDataStream buf(packet);
		buf.setByteOrder(QDataStream::LittleEndian);
		buf >> p.iPacketId;
		buf >> p.iPacketSize;

		if (p.iPacketSize)
		{
			p.baData.resize(pSocket->pendingDatagramSize());
			pSocket->readDatagram(p.baData.data(), p.baData.size());

			// FIXME: Verify if the data is good and emit onLog
		}

		//emit onLog(QString("Leaf Message: %1 %2").arg(p.iPacketId).arg(p.baData.data()));
		switch (p.iPacketId)
		{
			case (int)ePacket::Log:
				emit onPrintLog(QString("%1").arg(p.baData.data()));
			break;

			case (int)ePacket::Error:
				emit onPrintError(QString("%1").arg(p.baData.data()));
			break;

			case (int)ePacket::Debug:
				emit onPrintDebug(QString("%1").arg(p.baData.data()));
			break;

			case (int)ePacket::Start:
				emit onStart();
			break;

			case (int)ePacket::Stop:
				emit onStop();
			break;

			case (int)ePacket::Allocation:
			{
				PacketAllocationInfo *pi = nullptr;
				pi = static_cast<PacketAllocationInfo *>((void *)p.baData.data());
/*
				emit onLog(QString("Received allocation: %1, %2, %3, %4, %5, %6, %7")
							.arg(pi->iAddr)
							.arg(pi->iSize)
							.arg(pi->strCall)
							.arg(pi->strFile)
							.arg(pi->strFunc)
							.arg(pi->iLine)
							.arg(pi->iFrame)
						);
*/
				emit onAllocation(pi);
			}
			break;

			case (int)ePacket::Free:
			{
				PacketFreeInfo *pi = nullptr;
				pi = static_cast<PacketFreeInfo *>((void *)p.baData.data());
/*
				emit onLog(QString("Received free: %1, %2")
							.arg(pi->iAddr)
							.arg(pi->iFrame)
						);
*/
				emit onFree(pi);
			}
			break;

			default:
			break;
		}
	}
}
