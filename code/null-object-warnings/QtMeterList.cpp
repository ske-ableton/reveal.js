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

#include "QtMeterList.h"

#include "NonQtEntryPoint.h"

#include <QDebug>

QtMeterList::QtMeterList(NonQtEntryPoint* modelEntryPoint, QObject* parent)
    : QAbstractListModel(parent), mNonQtEntryPoint(modelEntryPoint)
{
    using namespace std::placeholders;
    modelEntryPoint->addRemoveObserver(std::bind(&QtMeterList::handleMeterRemoved, this, _1));
    modelEntryPoint->addResetObserver(std::bind(&QtMeterList::handleListReset, this));

    handleListReset();
}

int QtMeterList::rowCount(const QModelIndex &idx) const
{
    if (idx.isValid())
        return 0;
    return mQtMeters.size();
}

QVariant QtMeterList::data(const QModelIndex &idx, int role) const
{
    if (!idx.isValid())
        return QVariant();

    if (role == Qt::DisplayRole)
        return QString();

    if (role != Qt::UserRole)
        return QVariant();

    return QVariant::fromValue(mQtMeters.at(idx.row()).get());
}

QHash<int, QByteArray> QtMeterList::roleNames() const
{
    QHash<int, QByteArray> roleNames;
    roleNames.insert(Qt::UserRole, "meterRole");
    return roleNames;
}

void QtMeterList::handleListReset()
{
    beginResetModel();

    mQtMeters.clear();
    for(auto const& model : mNonQtEntryPoint->getMeters())
    {
        mQtMeters.emplace_back(new QtMeter(model.get()));
    }

    endResetModel();
}

void QtMeterList::handleMeterRemoved(int index)
{
    if (index >= int(mQtMeters.size())) {
        qWarning() << "Invariant violated";
        return;
    }
    beginRemoveRows(QModelIndex(), index, index);
    auto pos = mQtMeters.begin() + index;
    mQtMeters.erase(pos);
    endRemoveRows();
}
