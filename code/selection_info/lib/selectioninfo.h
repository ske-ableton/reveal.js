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

#include <QItemSelectionModel>
#include <QPointer>

class SelectionInfo : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QItemSelectionModel* selectionModel READ selectionModel WRITE setSelectionModel
        NOTIFY selectionModelChanged)
    Q_PROPERTY(QModelIndex modelIndex READ modelIndex WRITE setModelIndex NOTIFY modelIndexChanged)
    Q_PROPERTY(bool isSelected READ isSelected NOTIFY isSelectedChanged)
    Q_PROPERTY(bool isCurrent READ isCurrent NOTIFY isCurrentChanged)
public:
    SelectionInfo(QObject* parent = nullptr);

    QItemSelectionModel* selectionModel() const;
    void setSelectionModel(QItemSelectionModel* selectionModel);

    QModelIndex modelIndex() const;
    void setModelIndex(const QModelIndex& idx);

    bool isSelected() const;
    bool isCurrent() const;

Q_SIGNALS:
    void isSelectedChanged();
    void isCurrentChanged();
    void modelIndexChanged();
    void selectionModelChanged();

private:
    void updateSelectedState();
    void updateCurrentState();

private:
    QPointer<QItemSelectionModel> mSelectionModel;
    QPersistentModelIndex mModelIndex;
    bool mIsSelected;
    bool mIsCurrent;
};
