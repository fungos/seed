#ifndef MEMORYSORTFILTERMODEL_H
#define MEMORYSORTFILTERMODEL_H

#include <QSortFilterProxyModel>

class MemorySortFilterModel : public QSortFilterProxyModel
{
		Q_OBJECT
	public:
		explicit MemorySortFilterModel(QObject *parent = 0);

		quint32 filterMinimumFrame() const { return iMinFrame; }
		void setFilterMinimumFrame(quint32 frame);

		quint32 filterMaximumFrame() const { return iMaxFrame; }
		void setFilterMaximumFrame(quint32 frame);

		quint32 filterMinimumAddr() const { return iMinAddr; }
		void setFilterMinimumAddr(quint32 addr);

		quint32 filterMaximumAddr() const { return iMaxAddr; }
		void setFilterMaximumAddr(quint32 addr);

	protected:
		bool filterAcceptsRow(int sourceRow, const QModelIndex &sourceParent) const override;
		//bool lessThan(const QModelIndex &left, const QModelIndex &right) const override;

	private:
		bool frameInRange(quint32 frame) const;
		bool addrInRange(quint32 addr) const;

		quint32 iMinFrame;
		quint32 iMaxFrame;

		quint32 iMinAddr;
		quint32 iMaxAddr;
};

#endif // MEMORYSORTFILTERMODEL_H
