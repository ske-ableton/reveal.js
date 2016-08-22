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

#include "selectioninfo.h"

SelectionInfo::SelectionInfo(QObject* parent)
    : QObject(parent), mIsSelected(false), mIsCurrent(false)
{

}

QItemSelectionModel* SelectionInfo::selectionModel() const
{
    return mSelectionModel;
}

void SelectionInfo::setSelectionModel(QItemSelectionModel* selectionModel)
{
    if (mSelectionModel != selectionModel)
    {
        mSelectionModel = selectionModel;
        Q_EMIT selectionModelChanged();
        updateSelectedState();
        connect(mSelectionModel, &QItemSelectionModel::selectionChanged,
            this, &SelectionInfo::updateSelectedState);
        connect(mSelectionModel, &QItemSelectionModel::currentChanged,
            this, &SelectionInfo::updateCurrentState);
    }
}

QModelIndex SelectionInfo::modelIndex() const
{
    return mModelIndex;
}

void SelectionInfo::setModelIndex(const QModelIndex& modelIndex)
{
    if (mModelIndex != modelIndex)
    {
        mModelIndex = modelIndex;
        Q_EMIT modelIndexChanged();
        updateSelectedState();
    }
}

void SelectionInfo::updateSelectedState()
{
    if (!mSelectionModel || !mModelIndex.isValid())
        return;
    bool newState = mSelectionModel->isSelected(mModelIndex);
    if (mIsSelected != newState) {
        mIsSelected = newState;
        Q_EMIT isSelectedChanged();
    }
}

void SelectionInfo::updateCurrentState()
{
    if (!mSelectionModel || !mModelIndex.isValid())
        return;
    bool newState = mSelectionModel->currentIndex() == mModelIndex;
    if (mIsCurrent != newState) {
        mIsCurrent = newState;
        Q_EMIT isCurrentChanged();
    }
}

bool SelectionInfo::isSelected() const
{
    return mIsSelected;
}

bool SelectionInfo::isCurrent() const
{
    return mIsCurrent;
}
