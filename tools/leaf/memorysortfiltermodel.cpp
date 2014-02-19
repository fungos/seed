#include "memorysortfiltermodel.h"

#include <QDebug>

MemorySortFilterModel::MemorySortFilterModel(QObject *parent)
	: QSortFilterProxyModel(parent)
	, iMinFrame(0)
	, iMaxFrame(0)
	, iMinAddr(0)
	, iMaxAddr(0)
	, bShowFreed(false)
{
}

bool MemorySortFilterModel::lessThan(const QModelIndex &left, const QModelIndex &right) const
{
	QVariant leftData = sourceModel()->data(left);
	QVariant rightData = sourceModel()->data(right);

	auto t = leftData.type();
	if (t == QVariant::ULongLong)
		return leftData.toULongLong() < rightData.toULongLong();

	QString leftString = leftData.toString();
	QString rightString = rightData.toString();

	return QString::localeAwareCompare(leftString, rightString) < 0;
}

bool MemorySortFilterModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{
	QModelIndex index0 = sourceModel()->index(sourceRow, 0, sourceParent);
	QModelIndex index1 = sourceModel()->index(sourceRow, 1, sourceParent);
	QModelIndex index2 = sourceModel()->index(sourceRow, 2, sourceParent);
	QModelIndex index3 = sourceModel()->index(sourceRow, 3, sourceParent);
	QModelIndex index4 = sourceModel()->index(sourceRow, 4, sourceParent);
	QModelIndex index5 = sourceModel()->index(sourceRow, 5, sourceParent);
	QModelIndex index6 = sourceModel()->index(sourceRow, 6, sourceParent);
	QModelIndex index7 = sourceModel()->index(sourceRow, 7, sourceParent);
	QModelIndex index8 = sourceModel()->index(sourceRow, 7, sourceParent);

	auto str = (
				sourceModel()->data(index0).toString().contains(filterRegExp()) ||
				sourceModel()->data(index1).toString().contains(filterRegExp()) ||
				sourceModel()->data(index2).toString().contains(filterRegExp()) ||
				sourceModel()->data(index3).toString().contains(filterRegExp()) ||
				sourceModel()->data(index4).toString().contains(filterRegExp()) ||
				sourceModel()->data(index5).toString().contains(filterRegExp()) ||
				sourceModel()->data(index6).toString().contains(filterRegExp()) ||
				sourceModel()->data(index7).toString().contains(filterRegExp())
			);

	auto frame = frameInRange(sourceModel()->data(index2).toUInt());
	auto addr = addrInRange(sourceModel()->data(index3).toUInt());
	auto v = sourceModel()->data(index8).toBool();
	auto hide = (!bShowFreed && v) || bShowFreed;

	return str && frame && addr && hide;
}

void MemorySortFilterModel::setFilterMinimumFrame(quint32 frame)
{
	iMinFrame = frame;
	invalidateFilter();
}

void MemorySortFilterModel::setFilterMaximumFrame(quint32 frame)
{
	iMaxFrame = frame;
	invalidateFilter();
}

bool MemorySortFilterModel::frameInRange(quint32 frame) const
{
	if (iMinFrame == 0 && iMaxFrame == 0)
		return true;

	if (frame > iMinFrame && frame < iMaxFrame)
		return true;

	if (iMaxFrame == 0)
		return frame > iMinFrame;

	return frame < iMaxFrame;
}

void MemorySortFilterModel::setFilterMinimumAddr(quint32 addr)
{
	iMinAddr = addr;
	invalidateFilter();
}

void MemorySortFilterModel::setFilterMaximumAddr(quint32 addr)
{
	iMaxAddr = addr;
	invalidateFilter();
}

bool MemorySortFilterModel::addrInRange(quint32 addr) const
{
	if (iMinAddr == 0 && iMaxAddr == 0)
		return true;

	if (addr > iMinAddr && addr < iMaxAddr)
		return true;

	if (iMaxAddr == 0)
		return addr > iMinAddr;

	return addr < iMaxAddr;
}

void MemorySortFilterModel::hideFreed(bool hide)
{
	bShowFreed = hide;
}