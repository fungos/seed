#include "memorymodel.h"

#include <QStandardItemModel>

// http://qt-project.org/doc/qt-4.8/model-view-programming.html

class AllocationLessThan
{
public:
	inline AllocationLessThan() {}
	inline bool operator()(const QPair<QVariant, int> &l, const QPair<QVariant, int> &r) const
	{
		return l.first < r.first;
	}
};

class AllocationGreaterThan
{
public:
	inline AllocationGreaterThan() {}
	inline bool operator()(const QPair<QVariant, int> &l, const QPair<QVariant, int> &r) const
	{
		return r.first < l.first;
	}
};

AllocationItem::AllocationItem(AllocationData *data)
{
	pData = data;
}

AllocationItem::~AllocationItem()
{
	delete pData;
	pData = nullptr;
}

AllocationData *AllocationItem::getData() const
{
	return pData;
}

QVariant AllocationItem::data(int column, int role) const
{
	if (!pData)
		return QVariant();

	switch (role)
	{
		case Qt::TextColorRole:
		{
			if (pData->bFreed)
				return QVariant::fromValue(QColor(Qt::red));
		}
		break;

		case Qt::DisplayRole:
		{
			switch (column)
			{
				case 1: return pData->iTime;
				case 2: return pData->iLifetime;
				case 3: return pData->iFrame;
				case 4: return pData->iAddr;
				case 5: return QString("%1:%2").arg(pData->sFile).arg(pData->iLine);
				case 6: return pData->sFunction;
				case 7: return pData->sCall;
				case 8: return pData->iLine;

				case 0: 
				default: return QVariant();
			}
		}
	}

	return QVariant();
}

MemoryModel::MemoryModel(QObject *parent)
	: QAbstractTableModel(parent)
{
	vHeader << tr("id");
	vHeader << tr("Time");
	vHeader << tr("Lifetime");
	vHeader << tr("Frame");
	vHeader << tr("Address");
	vHeader << tr("File");
	vHeader << tr("Function");
	vHeader << tr("Call");
	vHeader << tr("Line");
}

MemoryModel::~MemoryModel()
{
	qDeleteAll(vItems);
}

int MemoryModel::columnCount(const QModelIndex &) const
{
	return vHeader.length();
}

int MemoryModel::rowCount(const QModelIndex &) const
{
	return vItems.length();
}

bool MemoryModel::insertRows(int, int, const QModelIndex &parent)
{
	auto l = vItems.length();
	beginInsertRows(parent, l, l);

	endInsertRows();
	return true;
}

QVariant MemoryModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
		return vHeader[section];

	return QVariant();
}

QVariant MemoryModel::data(const QModelIndex &index, int role) const
{
	if (!index.isValid())
		return QVariant();

	auto col = index.column();
	auto row = index.row();

	if (col > vHeader.length())
		return QVariant();

	if (row > vItems.length())
		return QVariant();

	auto item = vItems[row];
	return item->data(col, role);
}

void MemoryModel::sort(int column, Qt::SortOrder order)
{
	if (column >= columnCount())
		return;

	QVector<QPair<QVariant, int> > sortable;
	sortable.reserve(vItems.length());
	for (int row = 0; row < vItems.length(); ++row)
	{
		QVariant value = vItems[row]->data(column, Qt::DisplayRole);
		sortable.append(QPair<QVariant, int>(value, row));
	}

	if (order == Qt::AscendingOrder)
	{
		AllocationLessThan lt;
		qStableSort(sortable.begin(), sortable.end(), lt);
	}
	else
	{
		AllocationGreaterThan gt;
		qStableSort(sortable.begin(), sortable.end(), gt);
	}

	QModelIndexList changedPersistentIndexesFrom, changedPersistentIndexesTo;
	for (int i = 0; i < vItems.length(); ++i)
	{
		int r = sortable.at(i).second;
		for (int c = 0; c < columnCount(); ++c)
		{
			QPersistentModelIndex from(createIndex(r, c));
			QPersistentModelIndex to(createIndex(i, c));
			changedPersistentIndexesFrom.append(from);
			changedPersistentIndexesTo.append(to);
		}
	}

	changePersistentIndexList(changedPersistentIndexesFrom, changedPersistentIndexesTo);
}

void MemoryModel::alloc(const PacketAllocationInfo *packet)
{
	auto data = new AllocationData();
	data->iFrame = packet->iFrame;
	data->iAddr = packet->iAddr;
	data->sCall = QString(packet->strCall);
	data->sFile = QString(packet->strFile);
	data->sFunction = QString(packet->strFunc);
	data->iLine = packet->iLine;
	data->iTime = packet->iTime;
	data->iLifetime = packet->iLifetime;
	data->bFreed = false;

	auto item = new AllocationItem(data);
	vItems.push_back(item);

	insertRow(0);
}

void MemoryModel::free(const PacketFreeInfo *msg)
{
	foreach (auto obj, vItems)
	{
		auto data = obj->getData();
		if (data->iAddr == msg->iAddr && !data->bFreed)
		{
			data->bFreed = true;
			data->iLifetime = msg->iTime - data->iLifetime;
		}
	}
}
