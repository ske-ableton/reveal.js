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

#include "crosshairselection.h"

CrosshairSelection::CrosshairSelection(QObject* parent)
    : QItemSelectionModel(nullptr, parent), mSelectionModel(nullptr)
{

}

void CrosshairSelection::initialize()
{
    if (mSelectionModel && mSelectionModel->model()) {
        setModel(mSelectionModel->model());
    } else {
        setModel(nullptr);
    }
}

QItemSelectionModel* CrosshairSelection::selectionModel() const
{
    return mSelectionModel;
}

void CrosshairSelection::doSelect(QItemSelection& sel, std::vector<int> rows)
{
    for (auto i = 0; i < model()->rowCount(); ++i)
    {
        auto topIdx = model()->index(i, 0);
        for (auto row : rows) {
            auto idx = model()->index(row, 0, topIdx);
            sel.select(idx, idx);
        }
    }
}

void CrosshairSelection::setSelectionModel(QItemSelectionModel* selectionModel)
{
    if (mSelectionModel != selectionModel)
    {
        if (mSelectionModel) {
            disconnect(mSelectionModel);
        }
        mSelectionModel = selectionModel;
        Q_EMIT selectionModelChanged();
        initialize();
        connect(mSelectionModel, &QItemSelectionModel::modelChanged, this, [this] {
            initialize();
        });
        if (mSelectionModel) {
            connect(mSelectionModel, &QItemSelectionModel::selectionChanged, this, [this] {
                clearSelection();
                auto targetSel = mSelectionModel->selection();
                QItemSelection sel;
                std::vector<int> rows;
                for (auto rng: targetSel) {
                    auto top = model()->sibling(0, 0, rng.topLeft());
                    auto parent = rng.parent();
                    auto lastRow = model()->rowCount(parent) - 1;
                    auto bottom = model()->index(lastRow, 0, parent);
                    sel.select(top, bottom);

                    for (auto row = rng.top(); row <= rng.bottom(); ++row)
                        rows.push_back(row);
                }
                std::sort(rows.begin(), rows.end());
                rows.erase(std::unique(rows.begin(), rows.end()), rows.end());

                doSelect(sel, rows);

                select(sel, QItemSelectionModel::Select);
            });
        }
    }
}
