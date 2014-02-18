#ifndef MEMORYMODEL_H
#define MEMORYMODEL_H

#include "messagelistener.h"
#include <QStandardItemModel>

class QStandardItemModel;
class QAbstractItemModel;

enum Roles
{
	AllocationPointerRole = Qt::UserRole
};

struct AllocationData
{
	quint64 iTime;
	quint64 iLifetime;
	quint64 iAddr; // intptr_t, seed 32bits <> qt 64bits...
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
		void sort(int column, Qt::SortOrder order = Qt::AscendingOrder) override;

		void alloc(const PacketAllocationInfo *data);
		void free(const PacketFreeInfo *msg);

	signals:

	public slots:

	private:
		QStringList vHeader;
		QList<AllocationItem *> vItems;
};

#endif // MEMORYMODEL_H
