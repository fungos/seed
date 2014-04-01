#ifndef MEMORYMODEL_H
#define MEMORYMODEL_H

#include "messagelistener.h"
#include <QStandardItemModel>

#define QT_THREAD_SUPPORT
#include <qmutex.h>

class QStandardItemModel;
class QAbstractItemModel;

enum Roles
{
	AllocationPointerRole = Qt::UserRole
};

struct AllocationData
{
	quint64 iAddr; // intptr_t
	quint64 iSize;
	quint64 iTime;
	quint64 iLifetime;
	quint32 iLine;
	quint32 iFrame;
	QString sCall;
	QString sFile;
	QString sFunction;
	bool	bFreed;
};

class AllocationItem
{
	public:
		explicit AllocationItem(AllocationData *data);
		~AllocationItem();

		QVariant data(int column, int role) const;
		AllocationData *getData() const;

	private:
		AllocationData *pData;
};

class MemoryModel : public QAbstractTableModel
{
	Q_OBJECT
	public:
		explicit MemoryModel(QObject *parent = 0);
		~MemoryModel();

		QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
		int columnCount(const QModelIndex &parent = QModelIndex()) const override;
		int rowCount(const QModelIndex &parent = QModelIndex()) const override;
		QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
		bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
		//void sort(int column, Qt::SortOrder order = Qt::AscendingOrder) override;

		void clear();
		void alloc(const PacketAllocationInfo *data);
		void free(const PacketFreeInfo *msg);
		void setHexadecimalAddress(bool hex);
		void update();

	signals:
		void onTotalsChanged(quint64 total, quint64 maxTotal, quint64 maxUnique);

	public slots:

	private:
		QStringList vHeader;
		QList<AllocationItem *> vItems;
		QMutex mMutex;
		quint64 iTotalSize;
		quint64 iMaxTotalSize;
		quint64 iMaxUniqueSize;
		bool bHexAddress;
};

#endif // MEMORYMODEL_H
