#include "messagelistener.h"
#include <QtNetwork>

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
		p.baData.resize(pSocket->pendingDatagramSize());
		pSocket->readDatagram(p.baData.data(), p.baData.size());

		//emit onLog(QString("Leaf Message: %1 %2").arg(p.iPacketId).arg(p.baData.data()));
		switch (p.iPacketId)
		{
			case 1:
				emit onLeafPrintLog(QString("%1").arg(p.baData.data()));
			break;

			case 2:
				emit onLeafPrintError(QString("%1").arg(p.baData.data()));
			break;

			case 3:
				emit onLeafPrintDebug(QString("%1").arg(p.baData.data()));
			break;

			case 100:
			{
				PacketAllocationInfo *pi = nullptr;
				pi = static_cast<PacketAllocationInfo *>((void *)p.baData.data());
/*
				emit onLog(QString("Received allocation: %1, %2, %3, %4, %5, %6")
							.arg(pi->iAddr)
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

			case 101:
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
