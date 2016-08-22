/* Copyright (c) 2016 Ableton AG, Berlin

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

#include "SortFilterProxyModel.hpp"

#include <QtQml/QJSEngine>

SortFilterProxyModel::SortFilterProxyModel(QObject* pParent)
  : QSortFilterProxyModel(pParent)
  , mSortOrder(Qt::AscendingOrder)
{
}

void SortFilterProxyModel::setSortsLessThan(const QJSValue& sortsLessThan)
{
  if (!mSortsLessThan.equals(sortsLessThan))
  {
    mSortsLessThan = sortsLessThan;
    Q_ASSERT(mSortsLessThan.isCallable());
    sort(0, mSortOrder);
    Q_EMIT sortsLessThanChanged();
  }
}

QJSValue SortFilterProxyModel::sortsLessThan() const
{
  return mSortsLessThan;
}

Qt::SortOrder SortFilterProxyModel::sortOrder() const
{
  return mSortOrder;
}

void SortFilterProxyModel::setSortOrder(Qt::SortOrder sortOrder)
{
  Q_ASSERT(sortOrder == Qt::AscendingOrder || sortOrder == Qt::DescendingOrder);
  if (mSortOrder != sortOrder)
  {
    mSortOrder = sortOrder;
    sort(0, mSortOrder);
    Q_EMIT sortOrderChanged();
  }
}

void SortFilterProxyModel::setAcceptsRow(const QJSValue& acceptsRow)
{
  if (!mAcceptsRow.equals(acceptsRow))
  {
    mAcceptsRow = acceptsRow;
    Q_ASSERT(mAcceptsRow.isCallable());
    Q_EMIT acceptsRowChanged();
  }
}

QJSValue SortFilterProxyModel::acceptsRow() const
{
  return mAcceptsRow;
}

bool SortFilterProxyModel::lessThan(const QModelIndex& left,
                                    const QModelIndex& right) const
{
  if (!mSortsLessThan.isCallable())
  {
    return QSortFilterProxyModel::lessThan(left, right);
  }
  auto instance = qjsEngine(this)->newQObject(
    const_cast<SortFilterProxyModel*>(this));
  auto args = QJSValueList() << left.row() << right.row();
  return mSortsLessThan.callWithInstance(instance, args).toBool();
}

bool SortFilterProxyModel::filterAcceptsRow(int source_row,
                                            const QModelIndex& source_parent) const
{
  if (!mAcceptsRow.isCallable())
  {
    return QSortFilterProxyModel::filterAcceptsRow(source_row, source_parent);
  }
  auto instance = qjsEngine(this)->newQObject(
    const_cast<SortFilterProxyModel*>(this));
  auto args = QJSValueList() << source_row << qjsEngine(this)->toScriptValue(source_parent);
  return mAcceptsRow.callWithInstance(instance, args).toBool();
}
