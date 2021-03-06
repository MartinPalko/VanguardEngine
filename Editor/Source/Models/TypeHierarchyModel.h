#pragma once
#include "Editor_Common.h"

#include <QAbstractItemModel>

class TypeHierarchyModel : public QAbstractItemModel
{
	Q_OBJECT;

	Vanguard::Type* root;

public:
	explicit TypeHierarchyModel(Vanguard::Type* aRoot, QObject *parent = 0);
	~TypeHierarchyModel();

	QVariant data(const QModelIndex &index, int role) const Q_DECL_OVERRIDE;
	Qt::ItemFlags flags(const QModelIndex &index) const Q_DECL_OVERRIDE;
	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const Q_DECL_OVERRIDE;
	QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
	QModelIndex parent(const QModelIndex &index) const Q_DECL_OVERRIDE;
	int rowCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;
	int columnCount(const QModelIndex &parent = QModelIndex()) const Q_DECL_OVERRIDE;

};