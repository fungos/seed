#include "memorysortfiltermodel.h"

#include <QDebug>

MemorySortFilterModel::MemorySortFilterModel(QObject *parent)
	: QSortFilterProxyModel(parent)
	, iMinFrame(0)
	, iMaxFrame(0)
{
}

bool MemorySortFilterModel::filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const
{
	QModelIndex index1 = sourceModel()->index(sourceRow, 1, sourceParent);
	QModelIndex index2 = sourceModel()->index(sourceRow, 2, sourceParent);
	QModelIndex index3 = sourceModel()->index(sourceRow, 3, sourceParent);
	QModelIndex index4 = sourceModel()->index(sourceRow, 4, sourceParent);
	QModelIndex index5 = sourceModel()->index(sourceRow, 5, sourceParent);
	QModelIndex index6 = sourceModel()->index(sourceRow, 6, sourceParent);
	QModelIndex index7 = sourceModel()->index(sourceRow, 7, sourceParent);
	QModelIndex index8 = sourceModel()->index(sourceRow, 8, sourceParent);
/*
	auto s1 = sourceModel()->data(index1).toString();
	auto s2 = sourceModel()->data(index2).toString();
	auto s3 = sourceModel()->data(index3).toString();
	auto s4 = sourceModel()->data(index4).toString();
	auto s5 = sourceModel()->data(index5).toString();
	auto s6 = sourceModel()->data(index6).toString();
	auto s7 = sourceModel()->data(index7).toString();
	auto s8 = sourceModel()->data(index8).toString();

	auto r1 = s1.contains(filterRegExp());
	auto r2 = s2.contains(filterRegExp());
	auto r3 = s3.contains(filterRegExp());
	auto r4 = s4.contains(filterRegExp());
	auto r5 = s5.contains(filterRegExp());
	auto r6 = s6.contains(filterRegExp());
	auto r7 = s7.contains(filterRegExp());
	auto r8 = s8.contains(filterRegExp());
*/
	auto str = (
				sourceModel()->data(index1).toString().contains(filterRegExp()) ||
				sourceModel()->data(index2).toString().contains(filterRegExp()) ||
				sourceModel()->data(index3).toString().contains(filterRegExp()) ||
				sourceModel()->data(index4).toString().contains(filterRegExp()) ||
				sourceModel()->data(index5).toString().contains(filterRegExp()) ||
				sourceModel()->data(index6).toString().contains(filterRegExp()) ||
				sourceModel()->data(index7).toString().contains(filterRegExp()) ||
				sourceModel()->data(index8).toString().contains(filterRegExp())
			);
	auto frame = frameInRange(sourceModel()->data(index3).toUInt());
	auto addr = addrInRange(sourceModel()->data(index4).toUInt());

	return str && frame && addr;
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
