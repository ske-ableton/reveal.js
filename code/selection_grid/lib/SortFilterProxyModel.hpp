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

#pragma once

#include <QtCore/QSortFilterProxyModel>
#include <QtQml/QJSValue>

/**
 * A proxy model with hooks for sorting and filtering.
 *
 * Set the sortsLessThan property of this class to a QML function to implement
 * the QSortFilterProxyModel::lessThan method.
 *
 * Set the acceptRow property of this class to a QML function to implement
 * the QSortFilterProxyModel::filterAcceptsRow method.
 *
 * @code
 *   SomeModel {
 *     id: baseModel
 *   }
 *   SortFilterProxyModel {
 *     id: sorter
 *     sourceModel: baseModel
 *
 *     sortsLessThan: function(leftRow, rightRow) {
 *       var leftIdx = sourceModel.index(leftRow, 0);
 *       var leftStartTime = sourceModel.data(leftIdx, StartTimeRole);
 *
 *       var rightIdx = sourceModel.index(rightRow, 0);
 *       var rightStartTime = sourceModel.data(rightIdx, StartTimeRole);
 *
 *       return leftStartTime < rightStartTime;
 *     }
 *   }
 *   SortFilterProxyModel {
 *     id: filter
 *     sourceModel: sorter
 *
 *     property real filterMinValue: 100
 *     onFilterMinValueChanged: filter.invalidateFilter();
 *
 *     acceptsRow: function(row) {
 *       var idx = sourceModel.index(row, 0);
 *       var startTime = sourceModel.data(idx, StartTimeRole);
 *
 *       return startTime >= filterMinValue;
 *     }
 *   }
 *   ListView {
 *     id: view
 *     model: filter
 *   }
 * @endcode
 *
 * Note that if a javascript function depends on state external to the function, it is
 * necessary to call the invalidate() or invalidateFilter() function of
 * QSortFilterProxyModel.
 * This is similar to the requirement when implementing a QSortFilterProxyModel in
 * C++ - clients need to be notified of changes using QAbstractItemModel signals.
 *
 */
class SortFilterProxyModel : public QSortFilterProxyModel
{
  Q_OBJECT
  Q_PROPERTY(QJSValue sortsLessThan READ sortsLessThan WRITE setSortsLessThan NOTIFY
               sortsLessThanChanged)
  Q_PROPERTY(Qt::SortOrder sortOrder READ sortOrder WRITE setSortOrder NOTIFY
               sortOrderChanged)
  Q_PROPERTY(QJSValue acceptsRow READ acceptsRow WRITE setAcceptsRow NOTIFY
               acceptsRowChanged)
public:
  SortFilterProxyModel(QObject* pParent = nullptr);

  QJSValue sortsLessThan() const;
  void setSortsLessThan(const QJSValue& sortsLessThan);

  Qt::SortOrder sortOrder() const;
  void setSortOrder(Qt::SortOrder sortOrder);

  QJSValue acceptsRow() const;
  void setAcceptsRow(const QJSValue& acceptsRow);

  bool lessThan(const QModelIndex& left, const QModelIndex& right) const override;
  bool filterAcceptsRow(int source_row, const QModelIndex& source_parent) const override;

  Q_INVOKABLE void invalidateFilter()
  {
    QSortFilterProxyModel::invalidateFilter();
  }

Q_SIGNALS:
  void sortsLessThanChanged();
  void sortOrderChanged();
  void acceptsRowChanged();

private:
  mutable QJSValue mAcceptsRow;
  mutable QJSValue mSortsLessThan;
  Qt::SortOrder mSortOrder;
};
